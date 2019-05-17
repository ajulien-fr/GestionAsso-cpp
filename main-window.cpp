#include "main-window.h"
#include "table-view.h"
#include "table-view.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_db(new Database)
    , m_stackedWidget(new QStackedWidget)
    , m_accueilPage(new AccueilPageWidget(m_db))
    , m_bilansPage(new BilansPageWidget(m_db))
    , m_actionsPage(new ActionsPageWidget(m_db))
    , m_intervenantsPage(new IntervenantsPageWidget(m_db))
    , m_animauxPage(new AnimauxPageWidget(m_db))
    , m_objetsPage(new ObjetsPageWidget(m_db))
    , m_comptesPage(new ComptesPageWidget(m_db))
{
    m_createMenuBar();

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m_stackedWidget);

    setCentralWidget(scrollArea);

    m_stackedWidget->addWidget(m_accueilPage);
    m_stackedWidget->addWidget(m_actionsPage);
    m_stackedWidget->addWidget(m_intervenantsPage);
    m_stackedWidget->addWidget(m_animauxPage);
    m_stackedWidget->addWidget(m_objetsPage);
    m_stackedWidget->addWidget(m_comptesPage);
    m_stackedWidget->addWidget(m_bilansPage);

    connect(m_accueilPage, SIGNAL(connected()), this,
            SLOT(m_onConnected()));
    connect(m_accueilPage, SIGNAL(disconnected()), this,
            SLOT(m_onDisconnected()));

    connect(m_actionsPage, &ActionsPageWidget::refreshAll, this, [=] () {
        m_bilansPage->refresh();
        m_actionsPage->refresh();
        m_intervenantsPage->refresh();
        m_animauxPage->refresh();
        m_objetsPage->refresh();
        m_comptesPage->refresh();
    });

    m_currentPage = Accueil;
    m_stackedWidget->setCurrentWidget(m_accueilPage);
}

MainWindow::~MainWindow()
{
}

void MainWindow::m_onConnected()
{
    m_bilansPage->refresh();
    m_actionsPage->refresh();    
    m_intervenantsPage->refresh();
    m_animauxPage->refresh();
    m_objetsPage->refresh();
    m_comptesPage->refresh();

    for (int i = Bilans; i < Max; i++) m_actions[i]->setEnabled(true);
}

void MainWindow::m_onDisconnected()
{
    for (int i = Bilans; i < Max; i++) m_actions[i]->setEnabled(false);
}

void MainWindow::m_exportation()
{
    switch (m_currentPage)
    {
    case Actions:
        m_actionsPage->exportation();
        break;

    case Intervenants:
        m_intervenantsPage->exportation();
        break;

    case Animaux:
        m_animauxPage->exportation();
        break;

    case Objets:
        m_objetsPage->exportation();
        break;

    case Comptes:
        m_comptesPage->exportation();
        break;
    };
}
