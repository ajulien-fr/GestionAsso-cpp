#include "compte.h"
#include "message-box.h"

CompteWidget::CompteWidget(Database *db, uint id, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
    , m_idCompte(id)
    , m_date(new QDateEdit)
    , m_montant(new QLineEdit)
    , m_montantDepart(new QLineEdit)
    , m_remarque(new QTextEdit)
{
    QSqlQuery query;
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(20);

    if (m_idCompte != 0)
    {
        query = QSqlQuery(m_db->getSqlDatabase());

        if (!query.exec(QString("SELECT * FROM Compte WHERE idCompte=%1").arg(m_idCompte)))
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }

        query.first();
    }

    if (m_idCompte == 0) m_date->setDate(QDate::currentDate());
    else m_date->setDate(query.value(1).toDate());

    if (m_idCompte != 0)
    {
        m_montant->setText(query.value(2).toString());
        m_montantDepart->setText(query.value(3).toString());
        m_remarque->setText(query.value(4).toString());
    }

    layout->addWidget(new QLabel("Date"), 0, 0);
    layout->addWidget(m_date, 0, 1);

    layout->addWidget(new QLabel("Montant de départ"), 1, 0);
    layout->addWidget(m_montantDepart, 1, 1);

    if (m_idCompte != 0)
    {
        layout->addWidget(new QLabel("Montant"), 2, 0);
        layout->addWidget(m_montant, 2, 1);

        layout->addWidget(new QLabel("Remarque"), 3, 0);
        layout->addWidget(m_remarque, 3, 1);

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

        layout->addLayout(hLayout, 4, 0, 1, -1);
    }

    else
    {
        layout->addWidget(new QLabel("Remarque"), 2, 0);
        layout->addWidget(m_remarque, 2, 1);

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

        layout->addLayout(hLayout, 3, 0, 1, -1);
    }

    setLayout(layout);
}

void CompteWidget::m_onValiderBtnClicked()
{
    bool b = true;

    if (m_idCompte != 0)
    {
        if (m_montant->text().trimmed().length() <= 0) b = false;
    }

    if (m_montantDepart->text().trimmed().length() <= 0) b = false;

    if (b == false)
    {
        QMessageBox::information(this, "GestionAsso :: Information",
                                 "Vous n'avez pas entré toutes les informations nécessaires",
                                 QMessageBox::Ok);
        return;
    }

    QSqlQuery query(m_db->getSqlDatabase());

    if (m_idCompte == 0)
    {
        query.prepare("INSERT INTO Compte (date, montant, montantDepart, remarque) "
                      "VALUES (:date, :montantDepart, :montantDepart, :remarque)");

        query.bindValue(":date", m_date->date());
        query.bindValue(":montantDepart", m_montantDepart->text().toDouble());
        query.bindValue(":remarque", m_remarque->toPlainText());

        if (!query.exec())
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }
    }

    else
    {
        query.prepare(QString("UPDATE Compte SET "
                              "date=:date,"
                              "montant=:montant,"
                              "montantDepart=:montantDepart,"
                              "remarque=:remarque "
                              "WHERE idCompte=%1").arg(m_idCompte));

        query.bindValue(":date", m_date->date());
        query.bindValue(":montant", m_montant->text().toDouble());
        query.bindValue(":montantDepart", m_montantDepart->text().toDouble());
        query.bindValue(":remarque", m_remarque->toPlainText());

        if (!query.exec())
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }
    }

    emit edited();
}
