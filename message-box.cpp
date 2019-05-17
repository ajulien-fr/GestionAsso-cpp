#include "message-box.h"

void MessageBox::showLoginError(QString sqlerror, QWidget *parent)
{
    QMessageBox msgBox(QMessageBox::Critical, "GestionAsso :: Erreur", "Impossible de se connecter à cette base de données.", QMessageBox::Ok, parent);
    if (!sqlerror.isEmpty())msgBox.setDetailedText(sqlerror);
    msgBox.exec();
}

void MessageBox::showDatabaseError(QString sqlerror, QWidget *parent)
{
    QMessageBox msgBox(QMessageBox::Critical, "GestionAsso :: Erreur", "Il y a un problème avec la base de données.\nVeuillez la vérifier.", QMessageBox::Ok, parent);
    if (!sqlerror.isEmpty())msgBox.setDetailedText(sqlerror);
    msgBox.exec();
}
