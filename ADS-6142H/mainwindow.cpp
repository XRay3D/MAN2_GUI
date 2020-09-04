#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hw/hwi.h"
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_pbPing_clicked();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pbPing_clicked()
{
    HWI::osc->ping();
    ui->lineEdit->setText(HWI::osc->conected);
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << "\nCH1:SQUAresum" << HWI::osc->wrRdData(":MEASUrement:CH1:SQUAresum?");
    qDebug() << "CH1:AVERage" << HWI::osc->wrRdData(":MEASUrement:CH1:AVERage?");
    qDebug() << "CH2:SQUAresum" << HWI::osc->wrRdData(":MEASUrement:CH2:SQUAresum?");
    qDebug() << "CH2:AVERage" << HWI::osc->wrRdData(":MEASUrement:CH2:AVERage?");

    qDebug() << "\nCH1:PKPK" << HWI::osc->wrRdData(":MEASUrement:CH1:PKPK?");
    qDebug() << "CH1:VBASe" << HWI::osc->wrRdData(":MEASUrement:CH1:VBASe?");
    qDebug() << "CH2:PKPK" << HWI::osc->wrRdData(":MEASUrement:CH2:PKPK?");
    qDebug() << "CH2:VBASe" << HWI::osc->wrRdData(":MEASUrement:CH2:VBASe?");

    //    "MAX|MIN|PKPK|VTOP|VBASe|VAMP|AVERage"
    //    "|SQUAresum|CYCRms|CURSorrms|OVERShoot"
    //    "|PREShoot|PERiod|FREQuency|RTime|FTime"
    //    "|PWIDth|NWIDth|PDUTy|NDUTy|SCREenduty"
    //    "|FRR|FRF|FFR|FFF|LRR|LRF|LFR|LFF|RDELay"
    //    "|FDELay|RPHAse|PPULsenum|NPULsenum"
    //    "|RISEedgenum|FALLedgenum|AREA|CYCLearea"
    //    "|HARDfrequency";
}
