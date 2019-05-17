#include "main-window.h"

void MainWindow::m_createMenuBar()
{
    QMenuBar *rightBar = new QMenuBar();

    QAction *exportationAct = new QAction(tr("Exportation"));
    exportationAct->setEnabled(false);
    connect(exportationAct, &QAction::triggered, this, [=]() {
        m_exportation();
    });

    rightBar->addAction(exportationAct);

    QMenu *menuAide = new QMenu(tr("Aide"), rightBar);
    QAction *informationAct = new QAction(tr("Information"));
    connect(informationAct, &QAction::triggered, this, [=]() {
        QMessageBox::information(this, tr("GestionAsso"),
                                 tr("Auteur: ajulien - hello@ajulien.fr\n"
                                    "Version: 1.0.0\n"
                                    "License: https://www.gnu.org/licenses/gpl-3.0.en.html"),
                                 QMessageBox::Ok);
    });
    menuAide->addAction(informationAct);
    QAction *documentationAct = new QAction(tr("Documentation"));
    menuAide->addAction(documentationAct);
    QAction *donationAct = new QAction(tr("Donation"));
    menuAide->addAction(donationAct);

    rightBar->addMenu(menuAide);

    menuBar()->setCornerWidget(rightBar);

    m_actions[Accueil] = new QAction(tr("Accueil"));
    menuBar()->addAction(m_actions[Accueil]);
    connect(m_actions[Accueil], &QAction::triggered, this, [=]() {
        exportationAct->setEnabled(false);
        m_currentPage = Accueil;
        m_stackedWidget->setCurrentWidget(m_accueilPage);
    });

    m_actions[Bilans] = new QAction(tr("Bilans"));
    m_actions[Bilans]->setEnabled(false);
    menuBar()->addAction(m_actions[Bilans]);
    connect(m_actions[Bilans], &QAction::triggered, this, [=]() {
        exportationAct->setEnabled(false);
        m_currentPage = Bilans;
        m_stackedWidget->setCurrentWidget(m_bilansPage);
    });

    m_actions[Actions] = new QAction(tr("Actions"));
    m_actions[Actions]->setEnabled(false);
    menuBar()->addAction(m_actions[Actions]);
    connect(m_actions[Actions], &QAction::triggered, this, [=]() {
        exportationAct->setEnabled(true);
        m_currentPage = Actions;
        m_stackedWidget->setCurrentWidget(m_actionsPage);
    });

    m_actions[Intervenants] = new QAction(tr("Intervenants"));
    m_actions[Intervenants]->setEnabled(false);
    menuBar()->addAction(m_actions[Intervenants]);
    connect(m_actions[Intervenants], &QAction::triggered, this, [=]() {
        exportationAct->setEnabled(true);
        m_currentPage = Intervenants;
        m_stackedWidget->setCurrentWidget(m_intervenantsPage);
    });

    m_actions[Animaux] = new QAction(tr("Animaux"));
    m_actions[Animaux]->setEnabled(false);
    menuBar()->addAction(m_actions[Animaux]);
    connect(m_actions[Animaux], &QAction::triggered, this, [=]() {
        exportationAct->setEnabled(true);
        m_currentPage = Animaux;
        m_stackedWidget->setCurrentWidget(m_animauxPage);
    });

    m_actions[Objets] = new QAction(tr("Objets"));
    m_actions[Objets]->setEnabled(false);
    menuBar()->addAction(m_actions[Objets]);
    connect(m_actions[Objets], &QAction::triggered, this, [=]() {
        exportationAct->setEnabled(true);
        m_currentPage = Objets;
        m_stackedWidget->setCurrentWidget(m_objetsPage);
    });

    m_actions[Comptes] = new QAction(tr("Comptes"));
    m_actions[Comptes]->setEnabled(false);
    menuBar()->addAction(m_actions[Comptes]);
    connect(m_actions[Comptes], &QAction::triggered, this, [=]() {
        exportationAct->setEnabled(true);
        m_currentPage = Comptes;
        m_stackedWidget->setCurrentWidget(m_comptesPage);
    });
}
