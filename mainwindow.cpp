#include <string>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <JPetData/data_access.h>
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
    Source=make_shared<PQData>("host=127.0.0.1 dbname=postgres user=postgres password=pass");
    phm_factory=make_shared<Photomultipliers>(Source);
    phm_conn_factory=make_shared<HVPMConnections>(Source);
    frames=make_shared<FramesModel>(Source);
    ui->frames->setModel(frames.get());
    connect(ui->frames->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this,SLOT(  FrameSelect()));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::FrameSelect(){
    if((frames)&&ui->frames->selectionModel()->currentIndex().isValid()){
        setups=frames->SetupModel(ui->frames->selectionModel()->currentIndex().row());
        ui->Setups->setModel(setups.get());
    }else ui->Setups->setModel(nullptr);
    if(ui->Setups->selectionModel())
        connect(ui->Setups->selectionModel(),  SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this,SLOT(  SetupSelect()));
    SetupSelect();
}
void MainWindow::SetupSelect(){
    if((setups)&&(ui->Setups->selectionModel()->currentIndex().isValid())){
        configs=make_shared<ConfigsModel>(Source,setups ->GetItem(ui->Setups->selectionModel()->currentIndex().row()).id());
        ui->configs->setModel(configs.get());
    }else ui->configs->setModel(nullptr);
    if(ui->configs->selectionModel())
        connect(ui->configs->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this,SLOT(HVTableUpdate()));
    HVTableUpdate();
}
void MainWindow::on_pushButton_4_clicked(){
    if((frames)&&(setups)&&(configs)){
        if(ui->new_config_name->text()==""){
            QMessageBox::question(this,"File error","Config must have name",QMessageBox::Ok,QMessageBox::NoButton);
        }else{
            configs->AddItem(ui->new_config_name->text());
            ui->new_config_name->setText("");
            SetupSelect();
        }
    }else QMessageBox::question(this,"File error","Please select setup",QMessageBox::Ok,QMessageBox::NoButton);
}
void MainWindow::on_pushButton_5_clicked(){
    if(
            (frames)&&(setups)&&(configs)&&
            (ui->frames->selectionModel()->currentIndex().isValid())&&
            (ui->Setups->selectionModel()->currentIndex().isValid())&&
            (ui->configs->selectionModel()->currentIndex().isValid())
    ){
            table_model->Data().clear();
            configs->Delete(ui->configs->selectionModel()->currentIndex().row());
            SetupSelect();
    }
}

void MainWindow::HVTableUpdate(){
    if(
            (frames)&&(setups)&&(configs)&&
            (ui->frames->selectionModel()->currentIndex().isValid())&&
            (ui->Setups->selectionModel()->currentIndex().isValid())&&
            (ui->configs->selectionModel()->currentIndex().isValid())
    ){
        table_model=make_shared<HVTableModel>(
            configs->GetItem(ui->configs->selectionModel()->currentIndex().row()),
            setups ->GetItem(ui->Setups->selectionModel()->currentIndex().row()),
            frames ->GetItem(ui->frames->selectionModel()->currentIndex().row()),
            setups ->GetHVItem(ui->Setups->selectionModel()->currentIndex().row()),
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
            for(size_t i=0;i<table_model->Data().SlotInfo().size();i++)
                stream<<table_model->Data().SlotInfo()[i].hvpm.side()<<"\t"
                     <<table_model->Data().SlotInfo()[i].layer.name().c_str()<<"\t"
                    <<table_model->Data().SlotInfo()[i].slot.name().c_str()<<"\t"
                  <<table_model->Data().HVConfigEntries()[i].HV()<<endl;
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
            for(int i=0;i<table_model->Data().SlotInfo().size();i++)
            if(
                (table_model->Data().SlotInfo()[i].hvpm.side()==side)&&
                (table_model->Data().SlotInfo()[i].layer.name()==layer.toStdString())&&
                (table_model->Data().SlotInfo()[i].slot.name()==slot.toStdString())
            )
                index=i;
            if(index>=0)
                table_model->Data().SetHV(index,hv);
        }
        file.close();
        ui->curentconfig->setModel(table_model.get());
    }else QMessageBox::question(this,"File error","File cannot be opened",QMessageBox::Ok,QMessageBox::NoButton);
}

void MainWindow::on_pushButton_6_clicked(){
    if(
            (frames)&&(setups)&&(configs)&&
            (ui->frames->selectionModel()->currentIndex().isValid())&&
            (ui->Setups->selectionModel()->currentIndex().isValid())&&
            (ui->configs->selectionModel()->currentIndex().isValid())
    )
        table_model->Data().SynchroHardwarewithDB();
}
