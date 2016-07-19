#ifndef CALCWINDOW_H
#define CALCWINDOW_H

#include <QMainWindow>
#include "calcclient.h"

namespace Ui {
class CalcWindow;
}

//! Класс главного окна калькулятора
class CalcWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalcWindow(QWidget *parent = 0);
    ~CalcWindow();

private slots:
    void on_pushButton1_clicked();
    void on_pushButton2_clicked();
    void on_pushButton3_clicked();
    void on_pushButton4_clicked();
    void on_pushButton5_clicked();
    void on_pushButton6_clicked();
    void on_pushButton7_clicked();
    void on_pushButton8_clicked();
    void on_pushButton9_clicked();
    void on_pushButtonPlus_clicked();
    void on_pushButtonMinus_clicked();
    void on_pushButtonMul_clicked();
    void on_pushButtonDiv_clicked();
    void on_pushButton0_clicked();
    void on_pushButtonC_clicked();
    void on_pushButtonCalc_clicked();
    void on_pushButtonConn_clicked();
    void onResult(const QString &result);
    void onCalcError(const QString &message);
    void onConnectionError(const QString &message);
    void on_pushButtonDisc_clicked();

private:
    Ui::CalcWindow *ui;
    CalcClient client;
    bool isResult;
    void onChar(char c);
};

#endif // CALCWINDOW_H
