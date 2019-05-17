#ifndef COMPTES_PAGE_H
#define COMPTES_PAGE_H

#include <QtWidgets>
#include "database.h"
#include "table-view.h"

class ComptesPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ComptesPageWidget(Database *db, QStackedLayout *layout = nullptr, QWidget *parent = nullptr);

    void refresh();
    QItemSelectionModel *getSelectionModel();
    void exportation();

private:
    Database *m_db;
    QStackedLayout *m_stackedLayout;
    TableViewWidget *m_table;
    TableViewWidget *m_tableActions;

private slots:
    void m_contextMenu();
    void m_create();
    void m_edit();
    void m_delete();
};

#endif // COMPTES_PAGE_H
