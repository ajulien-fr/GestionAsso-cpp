#include "bilans-page.h"

BilansPageWidget::BilansPageWidget(Database *db, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
    , m_bilanManifestations(new BilanWidget(db, ":/images/manifestations.svg", "manifestation"))
    , m_bilanDonations(new BilanWidget(db, ":/images/donations.svg", "donation"))
    , m_bilanAdhesions(new BilanWidget(db, ":/images/adhesions.svg", "adhesion"))
    , m_bilanSubventions(new BilanWidget(db, ":/images/subventions.svg", "subvention"))
    , m_bilanVentes(new BilanWidget(db, ":/images/ventes.svg", "vente"))
    , m_bilanAchats(new BilanWidget(db, ":/images/achats.svg", "achat"))
    , m_bilanAdoptions(new BilanWidget(db, ":/images/adoptions.svg", "adoption"))
    , m_bilanSterilisations(new BilanWidget(db, ":/images/sterilisations.svg", "sterilisation"))
    , m_bilanSoins(new BilanWidget(db, ":/images/soins.svg", "soin"))
{
    QGridLayout *layout = new QGridLayout;

    layout->setSpacing(80);

    layout->addWidget(m_bilanAdhesions, 0, 0, Qt::AlignCenter);

    layout->addWidget(m_bilanSubventions, 0, 1, Qt::AlignCenter);

    layout->addWidget(m_bilanDonations, 0, 2, Qt::AlignCenter);

    layout->addWidget(m_bilanManifestations, 1, 0, Qt::AlignCenter);

    layout->addWidget(m_bilanVentes, 1, 1, Qt::AlignCenter);

    layout->addWidget(m_bilanAchats, 1, 2, Qt::AlignCenter);

    layout->addWidget(m_bilanAdoptions, 2, 0, Qt::AlignCenter);

    layout->addWidget(m_bilanSterilisations, 2, 1, Qt::AlignCenter);

    layout->addWidget(m_bilanSoins, 2, 2, Qt::AlignCenter);

    layout->setAlignment(Qt::AlignCenter);

    setLayout(layout);
}

void BilansPageWidget::refresh()
{
    m_bilanManifestations->refresh();
    m_bilanDonations->refresh();
    m_bilanAdhesions->refresh();
    m_bilanSubventions->refresh();
    m_bilanVentes->refresh();
    m_bilanAchats->refresh();
    m_bilanAdoptions->refresh();
    m_bilanSterilisations->refresh();
    m_bilanSoins->refresh();
}
