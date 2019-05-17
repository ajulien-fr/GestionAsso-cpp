#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <QtWidgets>

class MessageBox : public QWidget
{
    Q_OBJECT

public:
    static void showLoginError(QString sqlerror, QWidget *parent);
    static void showDatabaseError(QString sqlerror, QWidget *parent);
};

#endif // MESSAGE_BOX_H
