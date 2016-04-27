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
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_SelectedRun_valueChanged(int arg1){
    PhmData=make_shared<PhotomultipliersData>(Source,arg1);
    ui->ListOfPhm->clear();

}
