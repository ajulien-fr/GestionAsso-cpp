#include "action.h"
#include "message-box.h"

ActionWidget::ActionWidget(Database *db, uint id, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
    , m_idAction(id)
    , m_stackedLayout(new QStackedLayout)
    , m_widget(new QWidget)
    , m_type(new QComboBox)
    , m_date(new QDateEdit)
    , m_depense(new QLineEdit)
    , m_recette(new QLineEdit)
    , m_remarque(new QTextEdit)
    , m_intervenantBtn(new QPushButton)
    , m_animalBtn(new QPushButton)
    , m_objetBtn(new QPushButton)
    , m_compteBtn(new QPushButton)
    , m_comptesPage(new ComptesPageWidget(m_db, m_stackedLayout))
    , m_intervenantsPage(new IntervenantsPageWidget(m_db, m_stackedLayout))
    , m_animauxPage(new AnimauxPageWidget(m_db, m_stackedLayout))
    , m_objetsPage(new ObjetsPageWidget(m_db, m_stackedLayout))
{
    QSqlQuery query;
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setMargin(20);

    QStringList items = { "manifestation", "donation", "adhesion", "subvention", "vente", "achat",
                          "adoption", "sterilisation", "soin", "placement", "entreeAnimal", "sortieAnimal" };

    if (m_idAction != 0)
    {
        query = QSqlQuery(m_db->getSqlDatabase());

        if (!query.exec(QString("SELECT * FROM Action WHERE idAction=%1").arg(m_idAction)))
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }

        query.first();
    }

    m_type->addItems(items);
    connect(m_type, SIGNAL(activated(int)), this, SLOT(m_onTypeActivated(int)));

    if (m_idAction == 0)
    {
        m_type->setCurrentIndex(-1);
        m_date->setDate(QDate::currentDate());

        m_idIntervenant = 0;
        m_idAnimal = 0;
        m_idObjet = 0;
        m_idCompte = 0;
    }

    else
    {
        m_type->setCurrentText(query.value(1).toString());
        m_date->setDate(query.value(2).toDate());
        m_depense->setText(query.value(3).toString());
        m_recette->setText(query.value(4).toString());
        m_remarque->setText(query.value(5).toString());

        m_idIntervenant = query.value(6).toUInt();
        m_idAnimal = query.value(7).toUInt();
        m_idObjet = query.value(8).toUInt();
        m_idCompte = query.value(9).toUInt();
    }

    gridLayout->addWidget(new QLabel("Type"), 0, 0);
    gridLayout->addWidget(m_type, 0, 1);

    gridLayout->addWidget(new QLabel("Date"), 1, 0);
    gridLayout->addWidget(m_date, 1, 1);

    gridLayout->addWidget(new QLabel("Dépense"), 2, 0);
    gridLayout->addWidget(m_depense, 2, 1);

    gridLayout->addWidget(new QLabel("Recette"), 3, 0);
    gridLayout->addWidget(m_recette, 3, 1);

    gridLayout->addWidget(new QLabel("Remarque"), 4, 0);
    gridLayout->addWidget(m_remarque, 4, 1);

    m_intervenantBtn->setText("Selectionner Intervenant");
    m_intervenantBtn->setEnabled(false);
    connect(m_intervenantBtn, SIGNAL(clicked()), this, SLOT(m_onIntervenantButtonClicked()));
    gridLayout->addWidget(m_intervenantBtn, 5, 0, 1, -1);

    m_animalBtn->setText("Selectionner Animal");
    m_animalBtn->setEnabled(false);
    connect(m_animalBtn, SIGNAL(clicked()), this, SLOT(m_onAnimalButtonClicked()));
    gridLayout->addWidget(m_animalBtn, 6, 0, 1, -1);

    m_objetBtn->setText("Selectionner Objet");
    m_objetBtn->setEnabled(false);
    connect(m_objetBtn, SIGNAL(clicked()), this, SLOT(m_onObjetButtonClicked()));
    gridLayout->addWidget(m_objetBtn, 7, 0, 1, -1);

    m_compteBtn->setText("Selectionner Compte");
    m_compteBtn->setEnabled(false);
    connect(m_compteBtn, SIGNAL(clicked()), this, SLOT(m_onCompteButtonClicked()));
    gridLayout->addWidget(m_compteBtn, 8, 0, 1, -1);

    if (m_idAction != 0) m_type->activated(m_type->currentIndex());

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

    gridLayout->addLayout(hLayout, 9, 0, 1, -1);

    m_widget->setLayout(gridLayout);

    m_stackedLayout->addWidget(m_widget);

    setLayout(m_stackedLayout);
}

void ActionWidget::m_onIntervenantButtonClicked()
{
    if (m_idIntervenant != 0)
    {
        int ret = QMessageBox::question(this, tr("GestionAsso"),
                                        tr("Un intervenant est déjà sélectionné.\n"
                                           "Voulez vous continuez?"),
                                        QMessageBox::Ok | QMessageBox::Cancel);

        if (ret == QMessageBox::Cancel) return;
    }

    QWidget *widget = m_createPageWidget(m_intervenantsPage);
    m_intervenantsPage->refresh();

    connect(m_selectionnerBtn, &QPushButton::clicked, this, [=] () {
        if (m_intervenantsPage->getSelectionModel()->hasSelection())
        {
            m_idIntervenant = m_intervenantsPage->getSelectionModel()->selectedRows(0).first().data().toUInt();
        }
        m_stackedLayout->removeWidget(widget);
        m_stackedLayout->setCurrentWidget(m_widget);
    });


    m_stackedLayout->addWidget(widget);
    m_stackedLayout->setCurrentWidget(widget);
}

void ActionWidget::m_onAnimalButtonClicked()
{
    if (m_idAnimal != 0)
    {
        int ret = QMessageBox::question(this, tr("GestionAsso"),
                                        tr("Un animal est déjà sélectionné.\n"
                                           "Voulez vous continuez?"),
                                        QMessageBox::Ok | QMessageBox::Cancel);

        if (ret == QMessageBox::Cancel) return;
    }

    QWidget *widget = m_createPageWidget(m_animauxPage);
    m_animauxPage->refresh();

    connect(m_selectionnerBtn, &QPushButton::clicked, this, [=] () {
        if (m_animauxPage->getSelectionModel()->hasSelection())
        {
            m_idAnimal = m_animauxPage->getSelectionModel()->selectedRows(0).first().data().toUInt();
        }
        m_stackedLayout->removeWidget(widget);
        m_stackedLayout->setCurrentWidget(m_widget);
    });

    m_stackedLayout->addWidget(widget);
    m_stackedLayout->setCurrentWidget(widget);
}

void ActionWidget::m_onObjetButtonClicked()
{
    if (m_idObjet != 0)
    {
        int ret = QMessageBox::question(this, tr("GestionAsso"),
                                        tr("Un objet est déjà sélectionné.\n"
                                           "Voulez vous continuez?"),
                                        QMessageBox::Ok | QMessageBox::Cancel);

        if (ret == QMessageBox::Cancel) return;
    }

    QWidget *widget = m_createPageWidget(m_objetsPage);
    m_objetsPage->refresh();

    connect(m_selectionnerBtn, &QPushButton::clicked, this, [=] () {
        if (m_objetsPage->getSelectionModel()->hasSelection())
        {
            m_idObjet = m_objetsPage->getSelectionModel()->selectedRows(0).first().data().toUInt();
        }
        m_stackedLayout->removeWidget(widget);
        m_stackedLayout->setCurrentWidget(m_widget);
    });

    m_stackedLayout->addWidget(widget);
    m_stackedLayout->setCurrentWidget(widget);
}

void ActionWidget::m_onCompteButtonClicked()
{
    if (m_idCompte != 0)
    {
        int ret = QMessageBox::question(this, tr("GestionAsso"),
                                        tr("Un compte est déjà sélectionné.\n"
                                           "Voulez vous continuez?"),
                                        QMessageBox::Ok | QMessageBox::Cancel);

        if (ret == QMessageBox::Cancel) return;
    }

    QWidget *widget = m_createPageWidget(m_comptesPage);
    m_comptesPage->refresh();

    connect(m_selectionnerBtn, &QPushButton::clicked, this, [=] () {
        if (m_comptesPage->getSelectionModel()->hasSelection())
        {
            m_idCompte = m_comptesPage->getSelectionModel()->selectedRows(0).first().data().toUInt();
        }
        m_stackedLayout->removeWidget(widget);
        m_stackedLayout->setCurrentWidget(m_widget);
    });

    m_stackedLayout->addWidget(widget);
    m_stackedLayout->setCurrentWidget(widget);
}

QWidget *ActionWidget::m_createPageWidget(QWidget *page)
{
    QWidget *widget = new QWidget;
    QVBoxLayout *vLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;

    vLayout->addWidget(page);

    QPushButton *annulerBtn = new QPushButton("Annuler");
    connect(annulerBtn, &QPushButton::clicked, this, [=] () {
        m_stackedLayout->removeWidget(widget);
        m_stackedLayout->setCurrentWidget(m_widget);
    });
    hLayout->addWidget(annulerBtn);

    m_selectionnerBtn = new QPushButton("Selectionner");
    hLayout->addWidget(m_selectionnerBtn);

    vLayout->addLayout(hLayout);

    widget->setLayout(vLayout);

    return widget;
}

void ActionWidget::m_onTypeActivated(int index)
{
    switch (index)
    {
    case MANIFESTATION:
        m_intervenantBtn->setEnabled(false);
        m_animalBtn->setEnabled(false);
        m_objetBtn->setEnabled(false);
        m_compteBtn->setEnabled(true);
        break;

    case DONATION:
    case VENTE:
        m_intervenantBtn->setEnabled(true);
        m_animalBtn->setEnabled(false);
        m_objetBtn->setEnabled(true);
        m_compteBtn->setEnabled(true);
        break;

    case ADHESION:
    case SUBVENTION:
        m_intervenantBtn->setEnabled(true);
        m_animalBtn->setEnabled(false);
        m_objetBtn->setEnabled(false);
        m_compteBtn->setEnabled(true);
        break;

    case ACHAT:
        m_intervenantBtn->setEnabled(false);
        m_animalBtn->setEnabled(false);
        m_objetBtn->setEnabled(true);
        m_compteBtn->setEnabled(true);
        break;

    case ADOPTION:
    case STERILISATION:
    case SOIN:
        m_intervenantBtn->setEnabled(true);
        m_animalBtn->setEnabled(true);
        m_objetBtn->setEnabled(false);
        m_compteBtn->setEnabled(true);
        break;

    case PLACEMENT:
        m_intervenantBtn->setEnabled(true);
        m_animalBtn->setEnabled(true);
        m_objetBtn->setEnabled(false);
        m_compteBtn->setEnabled(false);
        break;

    case ENTREE_ANIMAL:
    case SORTIE_ANIMAL:
        m_intervenantBtn->setEnabled(false);
        m_animalBtn->setEnabled(true);
        m_objetBtn->setEnabled(false);
        m_compteBtn->setEnabled(false);
        break;
    }
}

void ActionWidget::m_onValiderBtnClicked()
{
    bool b = true;

    switch (m_type->currentIndex())
    {
    case MANIFESTATION:
        if (m_idCompte == 0) b = false;
        break;

    case DONATION:
        if (m_idIntervenant == 0) b = false;
        break;

    case ADHESION:
    case SUBVENTION:
        if (m_idIntervenant == 0) b = false;
        if (m_idCompte == 0) b = false;
        break;

    case VENTE:
    case ACHAT:
        if (m_idObjet == 0) b = false;
        if (m_idCompte == 0) b = false;
        break;

    case ADOPTION:
    case STERILISATION:
    case SOIN:
        if (m_idIntervenant == 0) b = false;
        if (m_idAnimal == 0) b = false;
        if (m_idCompte == 0) b = false;
        break;

    case PLACEMENT:
        if (m_idIntervenant == 0) b = false;
        if (m_idAnimal == 0) b = false;
        break;

    case ENTREE_ANIMAL:
    case SORTIE_ANIMAL:
        if (m_idAnimal == 0) b = false;
        break;
    }

    if (b == false)
    {
        QMessageBox::information(this, "GestionAsso :: Information",
                                 "Vous n'avez pas entré toutes les informations nécessaires",
                                 QMessageBox::Ok);
        return;
    }

    QSqlQuery query(m_db->getSqlDatabase());

    if (m_idAction == 0)
    {
        query.prepare("INSERT INTO Action (type, date, depense, recette, remarque, "
                      "idIntervenant, idAnimal, idObjet, idCompte) "
                      "VALUES (:type, :date, :depense, :recette, :remarque, "
                      ":idIntervenant, :idAnimal, :idObjet, :idCompte)");

        query.bindValue(":type", m_type->currentText());
        query.bindValue(":date", m_date->date());
        query.bindValue(":depense", m_depense->text().toDouble());
        query.bindValue(":recette", m_recette->text().toDouble());
        query.bindValue(":remarque", m_remarque->toPlainText());
        query.bindValue(":idIntervenant", m_idIntervenant);
        query.bindValue(":idAnimal", m_idAnimal);
        query.bindValue(":idObjet", m_idObjet);
        query.bindValue(":idCompte", m_idCompte);

        if (!query.exec())
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }
    }

    else
    {
        query.prepare(QString("UPDATE Action SET type=:type, date=:date,"
                              "depense=:depense, recette=:recette,"
                              "remarque=:remarque, idIntervenant=:idIntervenant,"
                              "idAnimal=:idAnimal, idObjet=:idObjet, idCompte=:idCompte "
                              "WHERE idAction=%1").arg(m_idAction));

        query.bindValue(":type", m_type->currentText());
        query.bindValue(":date", m_date->date());
        query.bindValue(":depense", m_depense->text().toDouble());
        query.bindValue(":recette", m_recette->text().toDouble());
        query.bindValue(":remarque", m_remarque->toPlainText());
        query.bindValue(":idIntervenant", m_idIntervenant);
        query.bindValue(":idAnimal", m_idAnimal);
        query.bindValue(":idObjet", m_idObjet);
        query.bindValue(":idCompte", m_idCompte);

        if (!query.exec())
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }
    }

    emit edited();
}
