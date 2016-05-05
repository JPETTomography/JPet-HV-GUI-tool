#include <string>
#include <DataGeneral/data_access.h>
#include <Postgres/postgres_data.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "models.h"
using namespace std;
using namespace DataAccess;
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
    configs=make_shared<ConfigsModel>(Source);
    ui->configs->setModel(configs.get());
    frames=make_shared<FramesModel>(Source);
    ui->frames->setModel(frames.get());
    connect(ui->frames->selectionModel(), SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this,SLOT(FrameSelect(const QItemSelection &, const QItemSelection &)));
    connect(ui->frames->selectionModel(), SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this,SLOT(HVTableUpdate(const QItemSelection &, const QItemSelection &)));
    connect(ui->configs->selectionModel(), SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this,SLOT(HVTableUpdate(const QItemSelection &, const QItemSelection &)));
    connect(ui->Setups->selectionModel(), SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this,SLOT(HVTableUpdate(const QItemSelection &, const QItemSelection &)));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::FrameSelect(const QItemSelection &, const QItemSelection &){
    if(ui->frames->selectionModel()->currentIndex().isValid())
        ui->Setups->setModel(frames->SetupModel(ui->frames->selectionModel()->currentIndex().row()).get());
}
void MainWindow::HVTableUpdate(const QItemSelection &, const QItemSelection &){
    if(
            (ui->frames->selectionModel()->currentIndex().isValid())&&
            (ui->Setups->selectionModel()->currentIndex().isValid())&&
            (ui->configs->selectionModel()->currentIndex().isValid())
    ){

    }
}
