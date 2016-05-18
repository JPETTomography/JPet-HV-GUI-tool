#include <string>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
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
    connect(ui->frames->selectionModel(),  SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this,SLOT(  FrameSelect(const QItemSelection&,const QItemSelection&)));
    connect(ui->frames->selectionModel(),  SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this,SLOT(HVTableUpdate(const QItemSelection&,const QItemSelection&)));
    connect(ui->configs->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this,SLOT(HVTableUpdate(const QItemSelection&,const QItemSelection&)));
    connect(ui->Setups->selectionModel(),  SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this,SLOT(HVTableUpdate(const QItemSelection&,const QItemSelection&)));
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
            configs->GetItem(ui->configs->selectionModel()->currentIndex().row()),
            setups ->GetItem(ui->Setups->selectionModel()->currentIndex().row()),
            frames ->GetItem(ui->frames->selectionModel()->currentIndex().row()),
            Source
        );
        ui->curentconfig->setModel(table_model.get());
    }else ui->curentconfig->setModel(nullptr);
}

void MainWindow::on_pushButton_clicked(){
    Source->Commit();
}
void MainWindow::closeEvent(QCloseEvent*closer){
    if(Source->changed()){
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "J-PET HV manager",
                                        tr("You have made some changes. Apply them?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);
        if(QMessageBox::Cancel==resBtn)closer->ignore();
        else if(QMessageBox::Yes==resBtn) Source->Commit();
    }else closer->accept();
}

void MainWindow::on_pushButton_2_clicked(){
    QFile file(QFileDialog::getSaveFileName(this,"Export HV settings to file","","*.txt"));
    if(file.open(QIODevice::ReadWrite)){
            QTextStream stream(&file);
            for(const auto&item:table_model->Data())
                stream<<item.hvpm.side()<<"\t"
                     <<item.layer.name().c_str()<<"\t"
                    <<item.slot.name().c_str()<<"\t"
                  <<item.entry.HV()<<endl;
            file.close();
    }else QMessageBox::question(this,"File error","File cannot be created",QMessageBox::Ok,QMessageBox::NoButton);
}
void MainWindow::on_pushButton_3_clicked(){
    QFile file(QFileDialog::getOpenFileName(this,"Import HV settings to file","","*.txt"));
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        uint side;
        QString layer,slot;
        double hv;
        while(!(stream>>side>>layer>>slot>>hv).atEnd()){
            int index=-1;
            for(int i=0;i<table_model->Data().size();i++)
            if(
                (table_model->Data()[i].hvpm.side()==side)&&
                (table_model->Data()[i].layer.name()==layer.toStdString())&&
                (table_model->Data()[i].slot.name()==slot.toStdString())
            )
                index=i;
            if(index>=0)
                table_model->Data().SetHV(index,hv);
        }
        file.close();
        ui->curentconfig->setModel(table_model.get());
    }
}

void MainWindow::on_pushButton_4_clicked(){

}
