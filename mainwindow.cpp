#include <string>
#include <iostream>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <JPetData/data_access.h>
#include <JPetData/Detectors.h>
#include <JPetData/HVconfig.h>
#include <JPetData/Frames.h>
#include <JPetData/HVSetter.h>
#include <Postgres/postgres_data.h>
#include <HV/CAEN.h>
#include <config-read.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "models.h"
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
using namespace HVAdjust;
using namespace Hardware;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    string cstr1=connstr_config::DB();
    string cstr2=connstr_config::HV();
    Source=make_shared<PQData>(cstr1);
    if(cstr2!="")hardware=make_shared<CAEN>(cstr2);
    else hardware=make_shared<DummyHV>();
    phm_factory=make_shared<Photomultipliers>(Source);
    phm_conn_factory=make_shared<HVPMConnections>(Source);
    frames=make_shared<FramesModel>(Source);
    ui->frames->setModel(frames.get());
    connect(ui->frames->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this,SLOT(  FrameSelect()));
    ui->frames->setColumnWidth(0,350);
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
    ui->Setups->setColumnWidth(0,100);
    ui->Setups->setColumnWidth(1,100);
    ui->Setups->setColumnWidth(2,150);
    SetupSelect();
}
void MainWindow::SetupSelect(){
    if((setups)&&(ui->Setups->selectionModel()->currentIndex().isValid())){
        configs=make_shared<ConfigsModel>(Source,setups ->GetItem(ui->Setups->selectionModel()->currentIndex().row()).id());
        ui->configs->setModel(configs.get());
    }else ui->configs->setModel(nullptr);
    ui->configs->setColumnWidth(0,350);
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
            Source,hardware
        );
        ui->curentconfig->setModel(table_model.get());
    }else ui->curentconfig->setModel(nullptr);
    ui->curentconfig->setColumnWidth(0,50);
    ui->curentconfig->setColumnWidth(1,60);
    ui->curentconfig->setColumnWidth(2,50);
    ui->curentconfig->setColumnWidth(3,60);
    ui->curentconfig->setColumnWidth(4,80);
    ui->curentconfig->setColumnWidth(5,50);
    ui->curentconfig->setColumnWidth(6,80);
    ui->curentconfig->setColumnWidth(7,80);
    on_timer_update();
}

void MainWindow::on_pushButton_clicked(){
	if(Source->changed()){
		Source->Commit();
		QMessageBox::question( this, "J-PET HV manager",tr("Changes have been commited to the database"),QMessageBox::Ok);
	}else{
		QMessageBox::question( this, "J-PET HV manager",tr("There are no changes to commit"),QMessageBox::Ok);
	}
}
void MainWindow::closeEvent(QCloseEvent*closer){
    if(Source->changed()){
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "J-PET HV manager",
                                        tr("You have made some changes. Apply them?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);
        if(QMessageBox::Cancel==resBtn) closer->ignore();
        else{
		if(QMessageBox::Yes==resBtn)on_pushButton_clicked();
		else QMessageBox::question( this, "J-PET HV manager",tr("Changes have been discarded"),QMessageBox::Ok);
	}
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
    }else QMessageBox::question(this,"File error","File cannot be opened",QMessageBox::Ok,QMessageBox::NoButton);
}
void MainWindow::on_pushButton_8_clicked(){
	if(
		(frames)&&(setups)&&(configs)&&
		(ui->frames->selectionModel()->currentIndex().isValid())&&
		(ui->Setups->selectionModel()->currentIndex().isValid())&&
		(ui->configs->selectionModel()->currentIndex().isValid())
	){
		table_model->Data().read_hardware();
		for(size_t i=0;i<table_model->Data().SlotInfo().size();i++){
			double hv=table_model->Data().HardwareHV()[i];
			table_model->Data().SetHV(i,hv);
		}
		table_model->Data().read_hardware();
	}
}

void MainWindow::on_pushButton_6_clicked(){
	if(
		(frames)&&(setups)&&(configs)&&
		(ui->frames->selectionModel()->currentIndex().isValid())&&
		(ui->Setups->selectionModel()->currentIndex().isValid())&&
		(ui->configs->selectionModel()->currentIndex().isValid())
	){
		table_model->Data().SynchroHardwarewithDB();
		table_model->Data().read_hardware();
	}
}
void MainWindow::on_pushButton_7_clicked(){
	if(
		(frames)&&(setups)&&(configs)&&
		(ui->frames->selectionModel()->currentIndex().isValid())&&
		(ui->Setups->selectionModel()->currentIndex().isValid())&&
		(ui->configs->selectionModel()->currentIndex().isValid())
	){
		table_model->Data().SwitchOffHardware();
		table_model->Data().read_hardware();
	}
}
void MainWindow::on_timer_update(){
	if(
		(frames)&&(setups)&&(configs)&&
		(ui->frames->selectionModel()->currentIndex().isValid())&&
		(ui->Setups->selectionModel()->currentIndex().isValid())&&
		(ui->configs->selectionModel()->currentIndex().isValid())
	){
		table_model->Data().read_hardware();
		QTimer::singleShot(1000,this,SLOT(on_timer_update()));
	}
}
