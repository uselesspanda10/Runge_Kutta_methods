#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "math.h"
#include <iomanip>
#include <QList>
#include <QDebug>
#include "resultsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double f(double x, double y) {
    return y*(2*y*log(x)-1)/x;
//    return 2*x;
}

double analytic(double x, double c) {
    return 1/(c*x+2*log(x)+2);
//    return x*x+c;
}

double c(double x, double y) {
//    return y-x*x;
    return (-2*y*log(x)-2*y+1)/(x*y);
}

static double h = 0.1;

void MainWindow::RK(int l, int r, double y0, bool customPrecision)
{
//    int l = 1;
//    int r = 2;
//    double y0 = 0.333333333;
//    double eps = 0.000001;
    double eps = 0;
    if (customPrecision) eps = ui->epsSpinBox->value();

    QList<double> X;
    QList<double> Y;
    Y.append(y0);
    QList<double> Yprev;
    QList<double> Yanalytic;
    int initN = (r-l)/h;
    int n;

    while (true) {
        n = (r-l)/h;
        int j = 0;
        while (l+h*j <= r) {
            X.append(l+h*j);
            j++;
        }

        for (int i = 1; i <= n; i++) {
            double x = X[i];
            double y = Y[i-1];
            double k1 = f(x, y);
            double k2 = f(x + h/2, y + h*k1/2);
            double k3 = f(x + h/2, y + h*k2/2);
            double k4 = f(x + h, y + h*k3);
            double yi = y + h*(k1 + 2*k2 + 2*k3 + k4)/6;
            Y.append(yi);
        }
        if (!customPrecision) break;
        if (Yprev.length() != 0) {
            double R = abs(Y.last()-Yprev.last())/15;
            if (R < eps) break;
        }

        Yprev = Y;
        X.clear();
        Y.clear();
        Y.append(y0);
        h = h/2;
    }

    double c0 = c(l, y0);
    std::cout << "c: " << c0 << std::endl;
    for (int i = 0; i < X.length(); i++) Yanalytic.append(analytic(X[i], c0));

//    for (int i = 0; i < outRK.length(); i++) {
//        std::cout << std::setprecision(9) << outRK[i].first << ' ' << outRK[i].second << '\n';
//    }
//    std::cout << "\n";
//    for (int i = 0; i < outAnalytic.length(); i++) {
//        std::cout << std::setprecision(9) << outAnalytic[i].first << ' ' << outAnalytic[i].second << '\n';
//    }
//    std::cout << std::fixed;
//    std::cout << "\n";
//    for (int i = 0; i < outRK.length(); i++) {
//        std::cout << outAnalytic[i].second << '-' << outRK[i].second << '=';
//        std::cout << std::setprecision(9) << abs(outAnalytic[i].second-outRK[i].second) << '\n';
//    }
    qDebug() << "len analytic: " << Yanalytic.length();
    qDebug() << "len approx.: " << Y.length();
    ResultsDialog dialog(X, Y, Yanalytic, n, initN);
    dialog.exec();
    h = 0.1;
}

void MainWindow::on_calculateButton_clicked()
{
    int l = ui->leftInput->value();
    int r = ui->rightInput->value();
    double y0 = ui->y0Input->value();
    if (l > r || l <= 0) {
        QMessageBox::warning(this, "Ошибка", "Границы заданы некорректно");
        return;
    }
    bool customPrecision = ui->gridCheckbox->isChecked();
    if (customPrecision && ui->epsSpinBox->value() <= 0.0) {
        QMessageBox::warning(this, "Ошибка", "Точность задана некорректно");
        return;
    }
    RK(l, r, y0, customPrecision);
}

void MainWindow::on_gridCheckbox_stateChanged(int arg1)
{
    ui->epsSpinBox->setEnabled(arg1);
}
