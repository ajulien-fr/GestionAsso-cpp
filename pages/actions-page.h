#ifndef ACTIONS_PAGE_H
#define ACTIONS_PAGE_H

#include <QtWidgets>
#include "database.h"
#include "table-view.h"

class ActionsPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionsPageWidget(Database *db, QWidget *parent = nullptr);

    void refresh();
    void exportation();

private:
    Database *m_db;
    QStackedWidget *m_parentStackedWidget;
    TableViewWidget *m_table;
    QTextEdit *m_text;

private slots:
    void m_contextMenu();
    void m_create();
    void m_edit();
    void m_delete();
    void m_onCheckBoxChanged(int state);
    void m_onSelectionChanged(const QItemSelection &, const QItemSelection &);

signals:
    void refreshAll();
};

#endif // ACTIONS_PAGE_H
