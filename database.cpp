#include "database.h"

Database::Database(QObject *parent)
    : QObject(parent)
{
    m_sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    m_connected = false;
}

Database::~Database()
{
}

bool Database::connect(QString address, QString port,
                       QString name, QString user, QString pass)
{
    QSettings settings;

    m_sqlDatabase.setHostName(address);
    m_sqlDatabase.setPort(port.toInt());
    m_sqlDatabase.setDatabaseName(name);
    m_sqlDatabase.setUserName(user);
    m_sqlDatabase.setPassword(pass);

    m_connected = m_sqlDatabase.open();

    if (m_connected == true)
    {
        settings.setValue("dbAddress", address);
        settings.setValue("dbPort", port);
        settings.setValue("dbName", name);
        settings.setValue("dbUser", user);
        settings.sync();
    }

    return m_connected;
}

void Database::disconnect()
{
    m_sqlDatabase.close();
    QSqlDatabase::removeDatabase("QMYSQL");
    m_connected = false;
}

bool Database::isConnected()
{
    return m_connected;
}

QSqlDatabase Database::getSqlDatabase()
{
    return m_sqlDatabase;
}
