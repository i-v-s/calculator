#include <QTime>
#include <QMessageBox>
#include "calcwindow.h"
#include "ui_calcwindow.h"

CalcWindow::CalcWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalcWindow),
    isResult(false)
{
    ui->setupUi(this);
    connect(&client, SIGNAL(onResult(QString)), this, SLOT(onResult(QString)));
    connect(&client, SIGNAL(onCalcError(QString)), this, SLOT(onCalcError(QString)));
    connect(&client, SIGNAL(onSocketError(QString)), this, SLOT(onConnectionError(QString)));
}

CalcWindow::~CalcWindow()
{
    delete ui;
}

void CalcWindow::onChar(char c)
{
    QString prev = ui->lineEdit->text();
    if(prev == "0" || prev == "ERROR" || (isResult && c >= '0' && c <= '9')) prev = "";
    isResult = false;
    ui->lineEdit->setText(prev + c);
}

void CalcWindow::on_pushButton0_clicked() { onChar('0'); }
void CalcWindow::on_pushButton1_clicked() { onChar('1'); }
void CalcWindow::on_pushButton2_clicked() { onChar('2'); }
void CalcWindow::on_pushButton3_clicked() { onChar('3'); }
void CalcWindow::on_pushButton4_clicked() { onChar('4'); }
void CalcWindow::on_pushButton5_clicked() { onChar('5'); }
void CalcWindow::on_pushButton6_clicked() { onChar('6'); }
void CalcWindow::on_pushButton7_clicked() { onChar('7'); }
void CalcWindow::on_pushButton8_clicked() { onChar('8'); }
void CalcWindow::on_pushButton9_clicked() { onChar('9'); }

void CalcWindow::on_pushButtonPlus_clicked()
{
    onChar('+');
}

void CalcWindow::on_pushButtonMinus_clicked()
{
    onChar('-');
}

void CalcWindow::on_pushButtonMul_clicked()
{
    onChar('*');
}

void CalcWindow::on_pushButtonDiv_clicked()
{
    onChar('/');
}

void CalcWindow::on_pushButtonC_clicked()
{
    ui->lineEdit->setText("0");
}

void CalcWindow::on_pushButtonCalc_clicked()
{
    QString expr = ui->lineEdit->text();
    client.sendExpression(expr);
    int row = ui->logTable->rowCount();
    ui->logTable->setRowCount(row + 1);
    ui->logTable->setItem(row, 0, new QTableWidgetItem(QTime::currentTime().toString()));
    ui->logTable->setItem(row, 1, new QTableWidgetItem(client.peer()));
    ui->logTable->setItem(row, 2, new QTableWidgetItem(expr));
}

void CalcWindow::on_pushButtonConn_clicked()
{
    client.connectRemote(ui->lineEditHost->text(), ui->spinBoxPort->value());
    ui->pushButtonCalc->setEnabled(true);
    ui->pushButtonConn->setEnabled(false);
    ui->pushButtonDisc->setEnabled(true);
}

void CalcWindow::onResult(const QString &result)
{
    ui->lineEdit->setText(result);
    isResult = true;
    int row = ui->logTable->rowCount() - 1;
    ui->logTable->setItem(row, 0, new QTableWidgetItem(QTime::currentTime().toString()));
    ui->logTable->setItem(row, 3, new QTableWidgetItem(result));
}

void CalcWindow::onCalcError(const QString &message)
{
    ui->lineEdit->setText("ERROR");
    int row = ui->logTable->rowCount() - 1;
    ui->logTable->setItem(row, 0, new QTableWidgetItem(QTime::currentTime().toString()));
    ui->logTable->setItem(row, 3, new QTableWidgetItem(message));
}

void CalcWindow::onConnectionError(const QString &message)
{
    QMessageBox::warning(this, "Ошибка соединения", message);
    ui->pushButtonDisc->setEnabled(false);
    ui->pushButtonCalc->setEnabled(false);
    ui->pushButtonConn->setEnabled(true);
}

void CalcWindow::on_pushButtonDisc_clicked()
{
    client.disconnect();
    ui->pushButtonDisc->setEnabled(false);
    ui->pushButtonCalc->setEnabled(false);
    ui->pushButtonConn->setEnabled(true);
}
