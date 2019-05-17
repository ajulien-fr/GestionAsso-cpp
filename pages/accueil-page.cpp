#include "accueil-page.h"
#include "message-box.h"

AccueilPageWidget::AccueilPageWidget(Database *db, QWidget *parent)
    : QWidget(parent)
    , m_db(db)
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    QLabel *label;

    hLayout->setAlignment(Qt::AlignCenter);
    hLayout->addStretch();

    QImage image;
    image.load(":/images/logo.png");

    label = new QLabel;
    label->setAlignment(Qt::AlignCenter);
    label->setPixmap(QPixmap::fromImage(image));
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label->setScaledContents(true);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(label);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFixedSize(450, 350);
    scrollArea->setStyleSheet("background-color: rgba(0,0,0,0)");

    hLayout->addWidget(scrollArea);
    hLayout->addSpacing(50);

    QGridLayout *layout = new QGridLayout;
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    QSettings settings;

    label = new QLabel(tr("Bienvenue ACC!"));
    label->setObjectName("title");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, 0, 0, 1, -1);

    label = new QLabel(tr("Adresse"));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, 1, 0);

    m_address = new QLineEdit;
    m_address->setFixedWidth(350);
    m_address->setPlaceholderText("localhost");
    m_address->setText(settings.value("dbAddress").toString());
    layout->addWidget(m_address, 1, 1);

    label = new QLabel(tr("Port"));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, 2, 0);

    m_port = new QLineEdit;
    m_port->setFixedWidth(350);
    m_port->setPlaceholderText("3306");
    m_port->setText(settings.value("dbPort").toString());
    layout->addWidget(m_port, 2, 1);

    label = new QLabel(tr("Nom"));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, 3, 0);

    m_name = new QLineEdit;
    m_name->setFixedWidth(350);
    m_name->setPlaceholderText("assodb");
    m_name->setText(settings.value("dbName").toString());
    layout->addWidget(m_name, 3, 1);

    label = new QLabel(tr("Utilisateur"));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, 4, 0);

    m_user = new QLineEdit;
    m_user->setFixedWidth(350);
    m_user->setPlaceholderText("asso");
    m_user->setText(settings.value("dbUser").toString());
    layout->addWidget(m_user, 4, 1);

    label = new QLabel(tr("Mot de passe"));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, 5, 0);

    m_pass = new QLineEdit;
    m_pass->setFixedWidth(350);
    m_pass->setEchoMode(QLineEdit::Password);
    connect(m_pass, SIGNAL(returnPressed()), this, SLOT(m_connectDisconnect()));
    layout->addWidget(m_pass, 5, 1);

    m_connectDisconnectBtn = new QPushButton;
    m_connectDisconnectBtn->setAutoDefault(true);
    m_connectDisconnectBtn->setText(tr("Connexion"));
    connect(m_connectDisconnectBtn, SIGNAL(clicked()), this, SLOT(m_connectDisconnect()));
    layout->addWidget(m_connectDisconnectBtn, 6, 0, 1, -1);

    hLayout->addLayout(layout);
    hLayout->addStretch();

    setLayout(hLayout);
}

void AccueilPageWidget::m_connectDisconnect()
{
    if (!m_db->isConnected())
    {
        if ((m_db->connect(m_address->text().trimmed(),
                           m_port->text().trimmed(),
                           m_name->text().trimmed(),
                           m_user->text().trimmed(),
                           m_pass->text().trimmed())))
        {
            m_connectDisconnectBtn->setText(tr("Déconnexion"));
            emit connected();
        }

        else
        {
            MessageBox::showLoginError(m_db->getSqlDatabase().lastError().text(), this);
            return;
        }
    }

    else
    {
        m_db->disconnect();
        m_connectDisconnectBtn->setText(tr("Connexion"));
        emit disconnected();
    }
}
