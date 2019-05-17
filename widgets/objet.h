#ifndef OBJET_H
#define OBJET_H

#include <QtWidgets>
#include "database.h"

class ObjetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObjetWidget(Database *db, uint id, QWidget *parent = nullptr);

private:
    Database *m_db;
    uint m_idObjet;
    QLineEdit *m_description;
    QTextEdit *m_remarque;

private slots:
    void m_onValiderBtnClicked();

signals:
    void edited();
    void canceled();
};

#endif // OBJET_H
