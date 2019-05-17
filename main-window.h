#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets>
#include "pages/accueil-page.h"
#include "pages/bilans-page.h"
#include "pages/actions-page.h"
#include "pages/intervenants-page.h"
#include "pages/animaux-page.h"
#include "pages/objets-page.h"
#include "pages/comptes-page.h"
#include "database.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    enum { Accueil, Bilans, Actions, Intervenants, Animaux, Objets, Comptes, Max };
    QAction *m_actions[Max];
    void m_createMenuBar();

    int m_currentPage;

    Database *m_db;

    QStackedWidget *m_stackedWidget;

    AccueilPageWidget *m_accueilPage;
    BilansPageWidget *m_bilansPage;
    ActionsPageWidget *m_actionsPage;
    IntervenantsPageWidget *m_intervenantsPage;
    AnimauxPageWidget *m_animauxPage;
    ObjetsPageWidget *m_objetsPage;
    ComptesPageWidget *m_comptesPage;

    void m_exportation();

private slots:
    void m_onConnected();
    void m_onDisconnected();
};

#endif // MAIN_WINDOW_H
