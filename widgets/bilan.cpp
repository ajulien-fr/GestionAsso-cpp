#include "bilan.h"
#include "message-box.h"

BilanWidget::BilanWidget(Database *db, QString img, QString action, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
    , m_action(action)
    , m_labelNombre(new QLabel)
    , m_labelDerniereDate(new QLabel)
    , m_labelMontant(new QLabel)
{
    QGridLayout *layout = new QGridLayout;
    QLabel *label;
    QImage image;

    image.load(img);

    label = new QLabel();
    label->setPixmap(QPixmap::fromImage(image));
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label->setScaledContents(true);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(label);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedWidth(150);
    scrollArea->setFixedHeight(150);
    scrollArea->setStyleSheet("background-color: rgba(0,0,0,0)");

    layout->addWidget(scrollArea, 1, 0, -1, 1);

    label = new QLabel(action.toUpper());
    label->setObjectName("title");
    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(label, 0, 1);

    label = new QLabel("Nombre pour la dernière année:");
    layout->addWidget(label, 1, 1);
    m_labelNombre->setStyleSheet("font-weight: bold;");
    layout->addWidget(m_labelNombre, 1, 2);

    label = new QLabel("Dernière date:");
    layout->addWidget(label, 2, 1);
    m_labelDerniereDate->setStyleSheet("font-weight: bold;");
    layout->addWidget(m_labelDerniereDate, 2, 2);

    label = new QLabel("Montant pour la dernière année:");
    layout->addWidget(label, 3, 1);
    m_labelMontant->setStyleSheet("font-weight: bold;");
    layout->addWidget(m_labelMontant, 3, 2);

    layout->setHorizontalSpacing(20);
    setLayout(layout);
}

void BilanWidget::refresh()
{
    QSqlQuery query(m_db->getSqlDatabase());
    uint idCompte;

    if (!query.exec("SELECT * FROM Compte ORDER BY date DESC"))
    {
        MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
        return;
    }

    if (query.first())
    {
        idCompte = query.value(0).toUInt();

        if (!query.exec(QString("SELECT * FROM Action WHERE type = '%1' AND idCompte = '%2'").arg(m_action, QString::number(idCompte))))
        {
            MessageBox::showDatabaseError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }

        m_labelNombre->setText(QString::number(query.size()));

        int montant = 0;

        while (query.next()) {
            montant += (query.value(4).toDouble() - query.value(3).toDouble());
        }

        m_labelMontant->setText(QString::number(montant));

        if (query.last())
        {
            m_labelDerniereDate->setText(query.value(2).toDate().toString("dd/MM/yyyy"));
        }
    }
}
