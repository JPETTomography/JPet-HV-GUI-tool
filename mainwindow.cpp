#include <DataGeneral/Photomultiplier.h>
#include <DataGeneral/Calibration.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
using namespace DataAccess;
using namespace Calibration;
using namespace JPetData;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    DBConfigData cfg;
    cfg.hostname="127.0.0.1";
    cfg.port="5432";
    cfg.db_name="postgres";
    cfg.username="postgres";
    cfg.password="pass";
    Source=make_shared<PQData>(cfg);
    ui->SelectedRun->setValue(1);
    on_SelectedRun_valueChanged(1);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_SelectedRun_valueChanged(int arg1){
    ui->ListOfPhm->clear();
    for(const PhotomultiplierData&item:PhotomultipliersData(Source,arg1).GetList())
        ui->ListOfPhm->addItem(QString(item.name().c_str()));
}
