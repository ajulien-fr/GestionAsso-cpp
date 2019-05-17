#include "intervenant.h"
#include "message-box.h"

IntervenantWidget::IntervenantWidget(Database *db, uint id, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
    , m_idIntervenant(id)
    , m_titre(new QLineEdit)
    , m_nom(new QLineEdit)
    , m_prenom(new QLineEdit)
    , m_adresse(new QLineEdit)
    , m_codePostal(new QLineEdit)
    , m_ville(new QLineEdit)
    , m_telephone(new QLineEdit)
    , m_email(new QLineEdit)
    , m_remarque(new QTextEdit)
{
    QSqlQuery query;
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(20);

    if (m_idIntervenant != 0)
    {
        query = QSqlQuery(m_db->getSqlDatabase());

        if (!query.exec(QString("SELECT * FROM Intervenant WHERE idIntervenant=%1").arg(m_idIntervenant)))
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }

        query.first();
    }

    if (m_idIntervenant != 0)
    {
        m_titre->setText(query.value(1).toString());
        m_nom->setText(query.value(2).toString());
        m_prenom->setText(query.value(3).toString());
        m_adresse->setText(query.value(4).toString());
        m_codePostal->setText(query.value(5).toString());
        m_ville->setText(query.value(6).toString());
        m_telephone->setText(query.value(7).toString());
        m_email->setText(query.value(8).toString());
        m_remarque->setText(query.value(9).toString());
    }

    layout->addWidget(new QLabel("Titre"), 0, 0);
    layout->addWidget(m_titre, 0, 1);

    layout->addWidget(new QLabel("Nom"), 1, 0);
    layout->addWidget(m_nom, 1, 1);

    layout->addWidget(new QLabel("Prénom"), 2, 0);
    layout->addWidget(m_prenom, 2, 1);

    layout->addWidget(new QLabel("Adresse"), 3, 0);
    layout->addWidget(m_adresse, 3, 1);

    layout->addWidget(new QLabel("Code postal"), 4, 0);
    layout->addWidget(m_codePostal, 4, 1);

    layout->addWidget(new QLabel("Ville"), 5, 0);
    layout->addWidget(m_ville, 5, 1);

    layout->addWidget(new QLabel("Téléphone"), 6, 0);
    layout->addWidget(m_telephone, 6, 1);

    layout->addWidget(new QLabel("Email"), 7, 0);
    layout->addWidget(m_email, 7, 1);

    layout->addWidget(new QLabel("Remarque"), 8, 0);
    layout->addWidget(m_remarque, 8, 1);

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

    layout->addLayout(hLayout, 9, 0, 1, -1);

    setLayout(layout);
}

void IntervenantWidget::m_onValiderBtnClicked()
{
    QSqlQuery query(m_db->getSqlDatabase());

    if (m_idIntervenant == 0)
    {
        query.prepare("INSERT INTO Intervenant (titre, nom, prenom, adresse, codePostal, "
                      "ville, telephone, email, remarque) "
                      "VALUES (:titre, :nom, :prenom, :adresse, :codePostal, "
                      ":ville, :telephone, :email, :remarque)");

        query.bindValue(":titre", m_titre->text());
        query.bindValue(":nom", m_nom->text());
        query.bindValue(":prenom", m_prenom->text());
        query.bindValue(":adresse", m_adresse->text());
        query.bindValue(":codePostal", m_codePostal->text());
        query.bindValue(":ville", m_ville->text());
        query.bindValue(":telephone", m_telephone->text());
        query.bindValue(":email", m_email->text());
        query.bindValue(":remarque", m_remarque->toPlainText());

        if (!query.exec())
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }
    }

    else
    {
        query.prepare(QString("UPDATE Intervenant SET titre=:titre, "
                              "nom=:nom, prenom=:prenom, adresse=:adresse, codePostal=:codePostal, "
                              "ville=:ville, telephone=:telephone, email=:email, remarque=:remarque "
                              "WHERE idIntervenant=%1").arg(m_idIntervenant));

        query.bindValue(":titre", m_titre->text());
        query.bindValue(":nom", m_nom->text());
        query.bindValue(":prenom", m_prenom->text());
        query.bindValue(":adresse", m_adresse->text());
        query.bindValue(":codePostal", m_codePostal->text());
        query.bindValue(":ville", m_ville->text());
        query.bindValue(":telephone", m_telephone->text());
        query.bindValue(":email", m_email->text());
        query.bindValue(":remarque", m_remarque->toPlainText());

        if (!query.exec())
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }
    }

    emit edited();
}
