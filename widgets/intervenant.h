#ifndef INTERVENANT_H
#define INTERVENANT_H

#include <QtWidgets>
#include <database.h>

class IntervenantWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IntervenantWidget(Database *db, uint id, QWidget *parent = nullptr);

private:
    Database *m_db;
    uint m_idIntervenant;

    QLineEdit *m_titre;
    QLineEdit *m_nom;
    QLineEdit *m_prenom;
    QLineEdit *m_adresse;
    QLineEdit *m_codePostal;
    QLineEdit *m_ville;
    QLineEdit *m_telephone;
    QLineEdit *m_email;
    QTextEdit *m_remarque;

private slots:
    void m_onValiderBtnClicked();

signals:
    void edited();
    void canceled();
};

#endif // INTERVENANT_H
