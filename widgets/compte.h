#ifndef COMPTE_H
#define COMPTE_H

#include <QtWidgets>
#include "database.h"

class CompteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompteWidget(Database *db, uint id, QWidget *parent = nullptr);

private:
    Database *m_db;
    uint m_idCompte;

    QDateEdit *m_date;
    QLineEdit *m_montant;
    QLineEdit *m_montantDepart;
    QTextEdit *m_remarque;

private slots:
    void m_onValiderBtnClicked();

signals:
    void edited();
    void canceled();
};

#endif // COMPTE_H
