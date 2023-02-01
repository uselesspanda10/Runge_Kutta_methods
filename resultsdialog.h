#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

#include <QDialog>
#include "qcustomplot.h"

class ResultsDialog : public QDialog
{
    Q_OBJECT
public:
    ResultsDialog(QList<double> X, QList<double> Y, QList<double> aY, int n, int initN, QWidget *parent = 0);

    QCustomPlot *plot;
    QTableWidget *table;
};

#endif // RESULTSDIALOG_H
