#ifndef BILAN_H
#define BILAN_H

#include <QtWidgets>
#include "database.h"

class BilanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BilanWidget(Database *db, QString img, QString action, QWidget *parent = nullptr);

    void refresh();

private:
    Database *m_db;
    QString m_action;
    QLabel *m_labelNombre;
    QLabel *m_labelDerniereDate;
    QLabel *m_labelMontant;
};

#endif // BILAN_H
