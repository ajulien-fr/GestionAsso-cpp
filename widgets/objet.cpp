#include "objet.h"
#include "message-box.h"

ObjetWidget::ObjetWidget(Database *db, uint id, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
    , m_idObjet(id)
    , m_description(new QLineEdit)
    , m_remarque(new QTextEdit)
{
    QSqlQuery query;
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(20);

    if (m_idObjet != 0)
    {
        query = QSqlQuery(m_db->getSqlDatabase());

        if (!query.exec(QString("SELECT * FROM Objet WHERE idObjet=%1").arg(m_idObjet)))
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }

        query.first();
    }

    if (m_idObjet != 0)
    {
        m_description->setText(query.value(1).toString());
        m_remarque->setText(query.value(2).toString());
    }

    layout->addWidget(new QLabel("Description"), 0, 0);
    layout->addWidget(m_description, 0, 1);

    layout->addWidget(new QLabel("Remarque"), 1, 0);
    layout->addWidget(m_remarque, 1, 1);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setMargin(20);

    QPushButton *annulerBtn = new QPushButton("Annuler");
    connect(annulerBtn, &QPushButton::clicked, this, [=] () {
        emit canceled();
    });
    hLayout->addWidget(annulerBtn);

    QPushButton *validerBtn = new QPushButton("Valider");
    connect(validerBtn, SIGNAL(clicked()), this, SLOT(m_onValiderBtnClicked()));
    hLayout->addWidget(validerBtn);

    layout->addLayout(hLayout, 2, 0, 1, -1);

    setLayout(layout);
}

void ObjetWidget::m_onValiderBtnClicked()
{
    if (m_description->text().trimmed().length() <= 0)
    {
        QMessageBox::information(this, "GestionAsso :: Information",
                                 "Vous n'avez pas entré toutes les informations nécessaires",
                                 QMessageBox::Ok);
        return;
    }

    QSqlQuery query(m_db->getSqlDatabase());

    if (m_idObjet == 0)
    {
        query.prepare("INSERT INTO Objet (description, remarque) "
                      "VALUES (:description, :remarque)");

        query.bindValue(":description", m_description->text());
        query.bindValue(":remarque", m_remarque->toPlainText());

        if (!query.exec())
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }
    }

    else
    {
        query.prepare(QString("UPDATE Objet SET "
                              "description=:description,"
                              "remarque=:remarque "
                              "WHERE idObjet=%1").arg(m_idObjet));

        query.bindValue(":description", m_description->text());
        query.bindValue(":remarque", m_remarque->toPlainText());

        if (!query.exec())
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }
    }

    emit edited();
}
