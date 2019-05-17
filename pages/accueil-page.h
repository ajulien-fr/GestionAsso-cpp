#ifndef ACCUEIL_PAGE_H
#define ACCUEIL_PAGE_H

#include <QtWidgets>
#include <database.h>

class AccueilPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AccueilPageWidget(Database *db, QWidget *parent = nullptr);

private:
    Database *m_db;

    QLineEdit *m_address;
    QLineEdit *m_port;
    QLineEdit *m_name;
    QLineEdit *m_user;
    QLineEdit *m_pass;
    QPushButton *m_connectDisconnectBtn;

private slots:
    void m_connectDisconnect();

signals:
    void connected();
    void disconnected();
};

#endif // ACCUEIL_PAGE_H
