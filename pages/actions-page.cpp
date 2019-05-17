#include "actions-page.h"
#include "message-box.h"
#include "widgets/action.h"

ActionsPageWidget::ActionsPageWidget(Database *db, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
    , m_text(new QTextEdit)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(20);

    QLabel *label = new QLabel(tr("Actions"));
    label->setObjectName("title");
    mainLayout->addWidget(label, 0);

    QCheckBox *checkBox = new QCheckBox(tr("Afficher uniquement la dernière année"));
    connect(checkBox, &QCheckBox::stateChanged, this, &ActionsPageWidget::m_onCheckBoxChanged);
    mainLayout->addWidget(checkBox);

    QComboBox *columns = new QComboBox;
    columns->addItem("");
    columns->addItem("ID");
    columns->addItem("Type");
    columns->addItem("Date");
    columns->addItem("Dépense");
    columns->addItem("Recette");
    columns->addItem("Remarque");
    columns->addItem("Intervenant ID");
    columns->addItem("Animal ID");
    columns->addItem("Objet ID");
    columns->addItem("Compte ID");

    m_table = new TableViewWidget(columns);
    connect(m_table, SIGNAL(contextMenuRequested()), this, SLOT(m_contextMenu()));
    connect(m_table->getSelectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            SLOT(m_onSelectionChanged(const QItemSelection &, const QItemSelection &))
            );

    m_text->setReadOnly(true);
    m_text->document()->setDefaultStyleSheet(qApp->styleSheet());

    QSplitter *splitter = new QSplitter;
    splitter->addWidget(m_table);
    splitter->addWidget(m_text);

    mainLayout->addWidget(splitter, 1);

    setLayout(mainLayout);
}

void ActionsPageWidget::m_contextMenu()
{
    QMenu menu(this);
    QItemSelectionModel *selection = m_table->getSelectionModel();

    QIcon addIcon(":/images/add.png");
    QIcon editIcon(":/images/edit.png");
    QIcon deleteIcon(":/images/delete.png");

    QAction *addAct = new QAction(addIcon, tr("Créer une action"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(m_create()));
    menu.addAction(addAct);

    if (selection->hasSelection())
    {
        QAction *editAct = new QAction(editIcon, tr("Éditer l'action"), this);
        connect(editAct, SIGNAL(triggered()), this, SLOT(m_edit()));
        menu.addAction(editAct);

        QAction *deleteAct = new QAction(deleteIcon, tr("Supprimer l'action"), this);
        connect(deleteAct, SIGNAL(triggered()), this, SLOT(m_delete()));
        menu.addAction(deleteAct);
    }

    menu.exec(QCursor::pos());
}

void ActionsPageWidget::m_create()
{
    ActionWidget *widget = new ActionWidget(m_db, 0);
    QDialog *dlg = new QDialog(this);
    dlg->setLayout(widget->layout());
    dlg->setMinimumSize(800, 500);
    dlg->show();

    connect(widget, &ActionWidget::canceled, this, [=] () {
        dlg->close();
    });

    connect(widget, &ActionWidget::edited, this, [=] () {
        emit refreshAll();
        dlg->close();
    });
}

void ActionsPageWidget::m_edit()
{
    // on chope l'ID de l'action
    uint id = m_table->getSelectionModel()->selectedRows().first().data().toUInt();

    ActionWidget *widget = new ActionWidget(m_db, id);
    QDialog *dlg = new QDialog(this);
    dlg->setLayout(widget->layout());
    dlg->setMinimumSize(800, 500);
    dlg->show();

    connect(widget, &ActionWidget::canceled, this, [=] () {
        dlg->close();
    });

    connect(widget, &ActionWidget::edited, this, [=] () {
        emit refreshAll();
        dlg->close();
    });
}

void ActionsPageWidget::m_delete()
{
    QSqlQuery query = QSqlQuery(m_db->getSqlDatabase());
    uint id = m_table->getSelectionModel()->selectedRows(0).first().data().toUInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "GestionAsso :: Question", "Voulez vous vraiment supprimer cette action ?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) return;

    if (!query.exec(QString("DELETE FROM Action WHERE idAction = '%1'").arg(QString::number(id))))
    {
        MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
        return;
    }

    emit refreshAll();
}

void ActionsPageWidget::refresh()
{
    QSqlQueryModel *sqlQueryModel = m_table->getSqlQueryModel();

    sqlQueryModel->setQuery("SELECT * FROM Action", m_db->getSqlDatabase());

    if (sqlQueryModel->lastError().isValid())
    {
        MessageBox::showDatabaseError(sqlQueryModel->lastError().text(), this);
        return;
    }

    sqlQueryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    sqlQueryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    sqlQueryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
    sqlQueryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Dépense"));
    sqlQueryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Recette"));
    sqlQueryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Remarque"));
    sqlQueryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Intervenant ID"));
    sqlQueryModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Animal ID"));
    sqlQueryModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Objet ID"));
    sqlQueryModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Compte ID"));
}

void ActionsPageWidget::m_onCheckBoxChanged(int state)
{
    if (state == Qt::Checked)
    {
        QSqlQuery query = QSqlQuery(m_db->getSqlDatabase());

        if (!query.exec("SELECT * FROM Compte ORDER BY date DESC"))
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }

        query.first();

        uint id = query.value(0).toUInt();

        QSqlQueryModel *sqlQueryModel = m_table->getSqlQueryModel();

        sqlQueryModel->setQuery(QString("SELECT * FROM Action WHERE idCompte=%1").arg(id), m_db->getSqlDatabase());

        if (sqlQueryModel->lastError().isValid())
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }

        sqlQueryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        sqlQueryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
        sqlQueryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
        sqlQueryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Dépense"));
        sqlQueryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Recette"));
        sqlQueryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Remarque"));
        sqlQueryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Intervenant ID"));
        sqlQueryModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Animal ID"));
        sqlQueryModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Objet ID"));
        sqlQueryModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Compte ID"));
    }

    else refresh();
}

void ActionsPageWidget::m_onSelectionChanged(const QItemSelection &, const QItemSelection &)
{
    QSqlQuery query = QSqlQuery(m_db->getSqlDatabase());

    uint idCompte = 0;
    uint idIntervenant = 0;
    uint idAnimal = 0;
    uint idObjet = 0;

    QString html;

    if (m_table->getSelectionModel()->hasSelection())
    {
        uint id = m_table->getSelectionModel()->selectedRows(0).first().data().toUInt();

        if (!query.exec(QString("SELECT * FROM Action WHERE idAction = '%1'").arg(id)))
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }

        query.first();

        idIntervenant = query.value(6).toUInt();
        idAnimal = query.value(7).toUInt();
        idObjet = query.value(8).toUInt();
        idCompte = query.value(9).toUInt();

        html = QString("<p>L'action</p>");
        html += "<ul>";
        html += QString("<li>Type     : <b>%1</b></li>").arg(query.value(1).toString());
        html += QString("<li>Date     : <b>%1</b></li>").arg(query.value(2).toDate().toString());
        html += QString("<li>Dépense  : <b>%1</b>€</li>").arg(QString::number(query.value(3).toDouble()));
        html += QString("<li>Recette  : <b>%1</b>€</li>").arg(QString::number(query.value(4).toDouble()));
        html += QString("<li>Remarque : <b>%1</b></li>").arg(query.value(5).toString());
        html += "</ul>";

        if (idCompte != 0)
        {
            if (!query.exec(QString("SELECT * FROM Compte WHERE idCompte = '%1'").arg(idCompte)))
            {
                MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
                return;
            }

            query.first();

            html += QString("<p>Le compte</p>");
            html += "<ul>";
            html += QString("<li>Date              : <b>%1</b></li>").arg(query.value(1).toDate().toString());
            html += QString("<li>Montant           : <b>%1</b>€</li>").arg(QString::number(query.value(2).toDouble()));
            html += QString("<li>Montant de départ : <b>%1</b>€</li>").arg(QString::number(query.value(3).toDouble()));
            html += QString("<li>Remarque          : <b>%1</b></li>").arg(query.value(4).toString());
            html += "</ul>";
        }

        if (idIntervenant != 0)
        {
            if (!query.exec(QString("SELECT * FROM Intervenant WHERE idIntervenant = '%1'").arg(idIntervenant)))
            {
                MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
                return;
            }

            query.first();

            html += QString("<p>L'intervenant</p>");
            html += "<ul>";
            html += QString("<li>Titre       : <b>%1</b></li>").arg(query.value(1).toString());
            html += QString("<li>Nom         : <b>%1</b></li>").arg(query.value(2).toString());
            html += QString("<li>Prenom      : <b>%1</b></li>").arg(query.value(3).toString());
            html += QString("<li>Adresse     : <b>%1</b></li>").arg(query.value(4).toString());
            html += QString("<li>Code Postal : <b>%1</b></li>").arg(query.value(5).toString());
            html += QString("<li>Ville       : <b>%1</b></li>").arg(query.value(6).toString());
            html += QString("<li>Téléphone   : <b>%1</b></li>").arg(query.value(7).toString());
            html += QString("<li>Email       : <b>%1</b></li>").arg(query.value(8).toString());
            html += QString("<li>Remarque    : <b>%1</b></li>").arg(query.value(9).toString());
            html += "</ul>";
        }

        if (idAnimal != 0)
        {
            if (!query.exec(QString("SELECT * FROM Animal WHERE idAnimal = '%1'").arg(idAnimal)))
            {
                MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
                return;
            }

            query.first();

            html += QString("<p>L'animal</p>");
            html += "<ul>";
            html += QString("<li>Type           : <b>%1</b></li>").arg(query.value(1).toString());
            html += QString("<li>Identification : <b>%1</b></li>").arg(query.value(2).toString());
            html += QString("<li>Nom            : <b>%1</b></li>").arg(query.value(3).toString());
            html += QString("<li>Description    : <b>%1</b></li>").arg(query.value(4).toString());
            html += QString("<li>Remarque       : <b>%1</b></li>").arg(query.value(5).toString());
            html += "</ul>";
        }

        if (idObjet != 0)
        {
            if (!query.exec(QString("SELECT * FROM Objet WHERE idObjet = '%1'").arg(idObjet)))
            {
                MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
                return;
            }

            query.first();

            html += QString("<p>L'objet</p>");
            html += "<ul>";
            html += QString("<li>Description : <b>%1</b></li>").arg(query.value(1).toString());
            html += QString("<li>Remarque    : <b>%1</b></li>").arg(query.value(2).toString());
            html += "</ul>";
        }

        m_text->setHtml(html);
    }
}

void ActionsPageWidget::exportation()
{
    QString textData;
    int rows = m_table->getFilterModel()->rowCount();
    int columns = m_table->getFilterModel()->columnCount();

    textData = "ID, Type, Date, Dépense, Recette, Remarque, ID Intervenant, ID Animal, ID Objet, ID Compte\n";

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

            textData += m_table->getFilterModel()->data(m_table->getFilterModel()->index(i,j)).toString();
            textData += ", ";
        }
        textData += "\n";
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Fichier"), "", "*.csv");

    if (fileName.isEmpty()) return;

    QFile csvFile(fileName);

    if (csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        QTextStream out(&csvFile);
        out << textData;

        csvFile.close();

        QDesktopServices::openUrl(fileName);
    }
}
