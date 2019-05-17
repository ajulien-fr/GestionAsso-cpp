#ifndef ACTION_H
#define ACTION_H

#include <QtWidgets>
#include "database.h"
#include "pages/comptes-page.h"
#include "pages/intervenants-page.h"
#include "pages/animaux-page.h"
#include "pages/objets-page.h"

class ActionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionWidget(Database *db, uint id, QWidget *parent = nullptr);

private:
    enum { MANIFESTATION, DONATION, ADHESION, SUBVENTION, VENTE, ACHAT, ADOPTION, STERILISATION, SOIN, PLACEMENT, ENTREE_ANIMAL, SORTIE_ANIMAL };

    Database *m_db;
    uint m_idAction;

    QStackedLayout *m_stackedLayout;

    QWidget *m_widget;
    QComboBox *m_type;
    QDateEdit *m_date;
    QLineEdit *m_depense;
    QLineEdit *m_recette;
    QTextEdit *m_remarque;
    QPushButton *m_intervenantBtn;
    QPushButton *m_animalBtn;
    QPushButton *m_objetBtn;
    QPushButton *m_compteBtn;
    QPushButton *m_selectionnerBtn;

    QWidget *m_createPageWidget(QWidget *page);

    ComptesPageWidget *m_comptesPage;
    IntervenantsPageWidget *m_intervenantsPage;
    AnimauxPageWidget *m_animauxPage;
    ObjetsPageWidget *m_objetsPage;

    uint m_idIntervenant;
    uint m_idAnimal;
    uint m_idObjet;
    uint m_idCompte;

private slots:
    void m_onIntervenantButtonClicked();
    void m_onAnimalButtonClicked();
    void m_onObjetButtonClicked();
    void m_onCompteButtonClicked();
    void m_onTypeActivated(int index);
    void m_onValiderBtnClicked();

signals:
    void edited();
    void canceled();
};

#endif // ACTION_H
