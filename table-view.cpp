#include "table-view.h"

TableViewWidget::TableViewWidget(QComboBox *columns, QWidget *parent)
    : QWidget(parent)
    , m_columns(columns)
    , m_pattern(new QLineEdit)
    , m_tableView(new QTableView)
    , m_sqlModel(new QSqlQueryModel)
    , m_filterModel(new QSortFilterProxyModel)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;

    mainLayout->setSpacing(20);
    mainLayout->setMargin(0);

    hLayout->setSpacing(20);
    hLayout->addWidget(new QLabel("Filtre"));
    hLayout->addWidget(m_pattern);
    connect(m_pattern, SIGNAL(textChanged(const QString &)), this, SLOT(m_onPatternChanged(const QString &)));

    hLayout->addWidget(new QLabel("Sur la colonne"));
    hLayout->addWidget(m_columns);
    connect(m_columns, SIGNAL(currentIndexChanged(int)), this, SLOT(m_onColumnChanged(int)));

    mainLayout->addLayout(hLayout);

    m_filterModel->setSourceModel(m_sqlModel);

    m_tableView->setModel(m_filterModel);
    m_tableView->setSortingEnabled(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_tableView, &QTableView::customContextMenuRequested, [=] () {
        emit contextMenuRequested();
    });

    mainLayout->addWidget(m_tableView);

    setLayout(mainLayout);
}

void TableViewWidget::m_onPatternChanged(const QString &str)
{
    QRegExp regExp(str, Qt::CaseInsensitive, QRegExp::RegExp);

    m_filterModel->setFilterRegExp(regExp);

    if (m_columns->currentIndex() == 0) m_filterModel->setFilterKeyColumn(-1);
    else m_filterModel->setFilterKeyColumn(m_columns->currentIndex() - 1);
}

void TableViewWidget::m_onColumnChanged(int i)
{
    if (i == 0) m_filterModel->setFilterKeyColumn(-1);
    else m_filterModel->setFilterKeyColumn(i - 1); // la colonne à i = 0 n'éxiste pas
}

QItemSelectionModel *TableViewWidget::getSelectionModel()
{
    return m_tableView->selectionModel();
}

QSqlQueryModel *TableViewWidget::getSqlQueryModel()
{
    return m_sqlModel;
}

void TableViewWidget::sortByColumn(int column, Qt::SortOrder order)
{
    m_tableView->sortByColumn(column, order);
}

QSortFilterProxyModel *TableViewWidget::getFilterModel()
{
    return m_filterModel;
}
