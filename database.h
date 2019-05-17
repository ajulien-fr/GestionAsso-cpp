#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool connect(QString address, QString port, QString name, QString user, QString pass);
    void disconnect();
    bool isConnected();

    QSqlDatabase getSqlDatabase();

private:
    QSqlDatabase m_sqlDatabase;
    bool m_connected;
};

#endif // DATABASE_H
