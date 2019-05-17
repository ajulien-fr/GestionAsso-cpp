#ifndef BILANS_PAGE_H
#define BILANS_PAGE_H

#include <QtWidgets>
#include "database.h"
#include "widgets/bilan.h"

class BilansPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BilansPageWidget(Database *db, QWidget *parent = nullptr);

    void refresh();

private:
    Database *m_db;
    BilanWidget *m_bilanManifestations;
    BilanWidget *m_bilanDonations;
    BilanWidget *m_bilanAdhesions;
    BilanWidget *m_bilanSubventions;
    BilanWidget *m_bilanVentes;
    BilanWidget *m_bilanAchats;
    BilanWidget *m_bilanAdoptions;
    BilanWidget *m_bilanSterilisations;
    BilanWidget *m_bilanSoins;
};

#endif // BILANS_PAGE_H
