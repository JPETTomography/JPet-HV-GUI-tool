#include <string>
#include <DataGeneral/data_access.h>
#include <Postgres/postgres_data.h>
#include <JPetData/Detectors.h>
#include <JPetData/HVconfig.h>
#include <JPetData/Frames.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "models.h"
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
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
    phm_factory=make_shared<Photomultipliers>(Source);
    phm_conn_factory=make_shared<HVPMConnections>(Source);
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
    if(ui->frames->selectionModel()->currentIndex().isValid()){
        setups=frames->SetupModel(ui->frames->selectionModel()->currentIndex().row());
        ui->Setups->setModel(setups.get());
    }
}
void MainWindow::HVTableUpdate(const QItemSelection &, const QItemSelection &){
    if(
            (ui->frames->selectionModel()->currentIndex().isValid())&&
            (ui->Setups->selectionModel()->currentIndex().isValid())&&
            (ui->configs->selectionModel()->currentIndex().isValid())
    ){
        table_model=make_shared<HVTableModel>(
                    configs->GetItem(ui->configs->selectionModel()->currentIndex().row()).CreateEntriesFactory(),
                    setups->GetItem(ui->Setups->selectionModel()->currentIndex().row()),
                    frames->GetItem(ui->frames->selectionModel()->currentIndex().row()),
                    Source
                    );
        ui->curentconfig->setModel(table_model.get());
    }
}
