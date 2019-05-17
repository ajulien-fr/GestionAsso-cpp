#include "animal.h"
#include "message-box.h"

AnimalWidget::AnimalWidget(Database *db, uint id, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
    , m_idAnimal(id)
    , m_type(new QComboBox)
    , m_identification(new QLineEdit)
    , m_nom(new QLineEdit)
    , m_description(new QLineEdit)
    , m_remarque(new QTextEdit)
{
    QSqlQuery query;
    QGridLayout *layout = new QGridLayout;
    layout->setMargin(20);

    QStringList items = { "Chat", "Chien", "Autre" };

    if (m_idAnimal != 0)
    {
        query = QSqlQuery(m_db->getSqlDatabase());

        if (!query.exec(QString("SELECT * FROM Animal WHERE idAnimal=%1").arg(m_idAnimal)))
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }

        query.first();
    }

    m_type->addItems(items);

    if (m_idAnimal == 0) m_type->setCurrentIndex(-1);
    else m_type->setCurrentText(query.value(1).toString());

    if (m_idAnimal != 0)
    {
        m_identification->setText(query.value(2).toString());
        m_nom->setText(query.value(3).toString());
        m_description->setText(query.value(4).toString());
        m_remarque->setText(query.value(5).toString());
    }

    layout->addWidget(new QLabel("Type"), 0, 0);
    layout->addWidget(m_type, 0, 1);

    layout->addWidget(new QLabel("Identification"), 1, 0);
    layout->addWidget(m_identification, 1, 1);

    layout->addWidget(new QLabel("Nom"), 2, 0);
    layout->addWidget(m_nom, 2, 1);

    layout->addWidget(new QLabel("Description"), 3, 0);
    layout->addWidget(m_description, 3, 1);

    layout->addWidget(new QLabel("Remarque"), 4, 0);
    layout->addWidget(m_remarque, 4, 1);

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

    layout->addLayout(hLayout, 5, 0, 1, -1);

    setLayout(layout);
}

void AnimalWidget::m_onValiderBtnClicked()
{
    bool b = true;

    if (m_type->currentIndex() == -1) b = false;

    if (m_nom->text().trimmed().length() <= 0) b = false;

    if (m_description->text().trimmed().length() <= 0) b = false;

    if (b == false)
    {
        QMessageBox::information(this, "GestionAsso :: Information",
                                 "Vous n'avez pas entré toutes les informations nécessaires",
                                 QMessageBox::Ok);
        return;
    }

    QSqlQuery query(m_db->getSqlDatabase());

    if (m_idAnimal == 0)
    {
        query.prepare("INSERT INTO Animal (type, identification, nom, description, remarque) "
                      "VALUES (:type, :identification, :nom, :description, :remarque)");

        query.bindValue(":type", m_type->currentText());
        query.bindValue(":identification", m_identification->text());
        query.bindValue(":nom", m_nom->text());
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
        query.prepare(QString("UPDATE Animal SET "
                              "type=:type, identification=:identification,"
                              "nom=:nom, description=:description,"
                              "remarque=:remarque "
                              "WHERE idAnimal=%1").arg(m_idAnimal));

        query.bindValue(":type", m_type->currentText());
        query.bindValue(":identification", m_identification->text());
        query.bindValue(":nom", m_nom->text());
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
