#include "objets-page.h"
#include "message-box.h"
#include "widgets/objet.h"

ObjetsPageWidget::ObjetsPageWidget(Database *db, QStackedLayout *layout, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
    , m_stackedLayout(layout)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(20);

    QLabel *label = new QLabel(tr("Objets"));
    label->setObjectName("title");
    mainLayout->addWidget(label, 0);

    QComboBox *columns = new QComboBox;
    columns->addItem("");
    columns->addItem("ID");
    columns->addItem("Description");
    columns->addItem("Remarque");

    m_table = new TableViewWidget(columns);
    connect(m_table, SIGNAL(contextMenuRequested()), this, SLOT(m_contextMenu()));

    mainLayout->addWidget(m_table, 1);

    setLayout(mainLayout);
}

void ObjetsPageWidget::m_contextMenu()
{
    QMenu menu(this);
    QItemSelectionModel *selection = m_table->getSelectionModel();

    QIcon addIcon(":/images/add.png");
    QIcon editIcon(":/images/edit.png");
    QIcon deleteIcon(":/images/delete.png");

    QAction *addAct = new QAction(addIcon, tr("Créer un objet"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(m_create()));
    menu.addAction(addAct);

    if (selection->hasSelection())
    {
        QAction *editAct = new QAction(editIcon, tr("Éditer l'objet"), this);
        connect(editAct, SIGNAL(triggered()), this, SLOT(m_edit()));
        menu.addAction(editAct);

        QAction *deleteAct = new QAction(deleteIcon, tr("Supprimer l'objet"), this);
        connect(deleteAct, SIGNAL(triggered()), this, SLOT(m_delete()));
        menu.addAction(deleteAct);
    }

    menu.exec(QCursor::pos());
}

void ObjetsPageWidget::m_create()
{
    if (m_stackedLayout == nullptr)
    {
        ObjetWidget *widget = new ObjetWidget(m_db, 0);
        QDialog *dlg = new QDialog(this);
        dlg->setLayout(widget->layout());
        dlg->setMinimumSize(800, 500);
        dlg->show();

        connect(widget, &ObjetWidget::canceled, this, [=] () {
            dlg->close();
        });

        connect(widget, &ObjetWidget::edited, this, [=] () {
            refresh();
            dlg->close();
        });
    }

    else
    {
        ObjetWidget *widget = new ObjetWidget(m_db, 0);
        m_stackedLayout->addWidget(widget);
        m_stackedLayout->setCurrentWidget(widget);

        connect(widget, &ObjetWidget::canceled, this, [=] () {
            m_stackedLayout->removeWidget(widget);
        });

        connect(widget, &ObjetWidget::edited, this, [=] () {
            refresh();
            m_stackedLayout->removeWidget(widget);
        });
    }
}

void ObjetsPageWidget::m_edit()
{
    uint id = m_table->getSelectionModel()->selectedRows(0).first().data().toUInt();

    if (m_stackedLayout == nullptr)
    {
        ObjetWidget *widget = new ObjetWidget(m_db, id);
        QDialog *dlg = new QDialog(this);
        dlg->setLayout(widget->layout());
        dlg->setMinimumSize(800, 500);
        dlg->show();

        connect(widget, &ObjetWidget::canceled, this, [=] () {
            dlg->close();
        });

        connect(widget, &ObjetWidget::edited, this, [=] () {
            refresh();
            dlg->close();
        });
    }

    else
    {
        ObjetWidget *widget = new ObjetWidget(m_db, id);
        m_stackedLayout->addWidget(widget);
        m_stackedLayout->setCurrentWidget(widget);

        connect(widget, &ObjetWidget::canceled, this, [=] () {
            m_stackedLayout->setCurrentWidget(this);
            m_stackedLayout->removeWidget(widget);
        });

        connect(widget, &ObjetWidget::edited, this, [=] () {
            refresh();
            m_stackedLayout->setCurrentWidget(this);
            m_stackedLayout->removeWidget(widget);
        });
    }
}

void ObjetsPageWidget::m_delete()
{
    QSqlQuery query = QSqlQuery(m_db->getSqlDatabase());
    uint id = m_table->getSelectionModel()->selectedRows(0).first().data().toUInt();

    if (!query.exec(QString("SELECT * FROM Action WHERE idObjet=%1").arg(QString::number(id))))
    {
        MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
        return;
    }

    if (query.size() > 0)
    {
        QMessageBox::information(this, tr("GestionAsso :: Information"),
                                 tr("Il éxiste des actions avec cet objet.\n"
                                    "Vous devez d'abord supprimer ces actions."),
                                 QMessageBox::Ok);

        return;
    }

    if (!query.exec(QString("DELETE FROM Objet WHERE idObjet=%1").arg(QString::number(id))))
    {
        MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
        return;
    }

    refresh();
}

void ObjetsPageWidget::refresh()
{
    QSqlQueryModel *sqlQueryModel = m_table->getSqlQueryModel();

    sqlQueryModel->setQuery("SELECT * FROM Objet", m_db->getSqlDatabase());

    if (sqlQueryModel->lastError().isValid())
    {
        MessageBox::showDatabaseError(sqlQueryModel->lastError().text(), this);
        return;
    }

    sqlQueryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    sqlQueryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Description"));
    sqlQueryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Remarque"));
}

QItemSelectionModel *ObjetsPageWidget::getSelectionModel()
{
    return m_table->getSelectionModel();
}

void ObjetsPageWidget::exportation()
{
    QString textData;
    int rows = m_table->getFilterModel()->rowCount();
    int columns = m_table->getFilterModel()->columnCount();

    textData = "ID, Description, Remarque\n";

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
