#ifndef TABLE_VIEW_H
#define TABLE_VIEW_H

#include <QtWidgets>
#include <QtSql>

class TableViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableViewWidget(QComboBox *columns, QWidget *parent = nullptr);

    QSqlQueryModel *getSqlQueryModel();
    QItemSelectionModel *getSelectionModel();
    void sortByColumn(int column, Qt::SortOrder order);
    QSortFilterProxyModel *getFilterModel();

private:
    QComboBox *m_columns;
    QLineEdit *m_pattern;
    QTableView *m_tableView;
    QSqlQueryModel *m_sqlModel;
    QSortFilterProxyModel *m_filterModel;

private slots:
    void m_onPatternChanged(const QString &str);
    void m_onColumnChanged(int i);

signals:
    void contextMenuRequested();
};

#endif // TABLE_VIEW_H
