#include "resultsdialog.h"
#include "qcustomplot.h"
#include <QVector>
#include <QDebug>

ResultsDialog::ResultsDialog(QList<double> X, QList<double> Y, QList<double> aY, int n, int initN, QWidget *parent) : QDialog(parent)
{
    qDebug() << "len X" << X.length();
    this->plot = new QCustomPlot();
    plot->setMinimumSize(800, 400);
    plot->addGraph();
    plot->addGraph();
    QVector<double> vectorX = X.toVector();
    QVector<double> vectorY = Y.toVector();
    QVector<double> vectoraY = aY.toVector();
    plot->graph(0)->setData(vectorX, vectorY);
    plot->graph(1)->setData(vectorX, vectoraY);
    plot->graph(1)->setPen(QPen(QColor(255, 0, 0)));
    plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 4));
    plot->graph(0)->setScatterSkip(X.length()/60);
    plot->graph(0)->setName("Приближенное решение");
    plot->graph(1)->setName("Аналитическое решение");
    plot->setInteraction(QCP::iRangeDrag);
    plot->setInteraction(QCP::iRangeZoom);
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    plot->xAxis->setRange(X[0]-0.25, X.last()+0.25);
    plot->yAxis->setRange(Y[0]-0.25, Y.last()+0.25);

    plot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    plot->legend->setFont(legendFont);
    plot->legend->setSelectedFont(legendFont);
    plot->legend->setSelectableParts(QCPLegend::spItems);
    plot->replot();

    table = new QTableWidget();
    table->setColumnCount(4);
    table->setMinimumWidth(800);
    table->setHorizontalHeaderLabels(QStringList{"X", "Y прибл.", "Y точн.", "Погрешность"});

    QList<QPair<double, double>> outRK;
    for (int i = 0; i <= initN; i++) {
        int index = i * (n/initN);
        outRK.append(QPair<double, double>(X[index], round(Y[index] * 1000000000.0) / 1000000000.0));
    }

    QList<QPair<double, double>> outAnalytic;
    for (int i = 0; i <= initN; i++) {
        int index = i * (n/initN);
        outAnalytic.append(QPair<double, double>(X[index], round(aY[index] * 1000000000.0) / 1000000000.0));
    }

    for (int i = 0; i <= initN; i++) {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::number(outRK[i].first)));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(outRK[i].second)));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(outAnalytic[i].second)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(abs(outRK[i].second-outAnalytic[i].second))));
    }

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(plot);
    layout->addWidget(table);
    QString calculationsString;
    calculationsString.append("Вычислено ");
    calculationsString.append(QString::number(Y.length()));
    calculationsString.append(" точек у");
    QLabel *calculations = new QLabel(calculationsString);
    layout->addWidget(calculations);
    setMinimumSize(800, 600);
    setLayout(layout);
}
