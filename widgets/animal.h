#ifndef ANIMAL_H
#define ANIMAL_H

#include <QtWidgets>
#include "database.h"

class AnimalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnimalWidget(Database *db, uint id, QWidget *parent = nullptr);

private:
    Database *m_db;
    uint m_idAnimal;

    QComboBox *m_type;
    QLineEdit *m_identification;
    QLineEdit *m_nom;
    QLineEdit *m_description;
    QTextEdit *m_remarque;

private slots:
    void m_onValiderBtnClicked();

signals:
    void edited();
    void canceled();
};

#endif // ANIMAL_H
