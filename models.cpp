#include "models.h"
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
ConfigsModel::ConfigsModel(const std::shared_ptr<DataAccess::IDataSource>src)
    :QAbstractTableModel(),f_table(src){
    for(const auto&item: f_table.GetList())f_cache.push_back(item);
}
int ConfigsModel::rowCount(const QModelIndex&) const {
    return f_cache.size();
}
int ConfigsModel::columnCount(const QModelIndex&) const{
    return 2;
}
QVariant ConfigsModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        switch(index.column()){
        case 0:
               return int(f_cache[index.row()].id());
        case 1:
            return QString::fromStdString(f_cache[index.row()].description());
        }
    }
    return QVariant::Invalid;
}
const HVconfig&ConfigsModel::GetItem(const size_t i)const{return f_cache[i];}


SetupsModel::SetupsModel(const JPetSetup::Frame&src)
    :QAbstractTableModel(),f_table(src.CreateSetupFactory()){
    for(const auto&item: f_table.GetList())f_cache.push_back(item);
}

int SetupsModel::rowCount(const QModelIndex &parent) const{
    return f_cache.size();
}
int SetupsModel::columnCount(const QModelIndex &parent) const{
    return 3;
}
QVariant SetupsModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        switch(index.column()){
        case 0:
               return int(f_cache[index.row()].id());
        case 1:
            return QString::fromStdString(f_cache[index.row()].name());
        case 2:
            return QString::fromStdString(f_cache[index.row()].description());
        }
    }
    return QVariant::Invalid;
}
const Setup&SetupsModel::GetItem(const size_t i)const{return f_cache[i];}



FramesModel::FramesModel(const std::shared_ptr<DataAccess::IDataSource>src)
    :QAbstractTableModel(),f_table(src){
    for(const auto&item:f_table.GetList())f_cache.push_back(item);
}
int FramesModel::rowCount(const QModelIndex&) const {
    return f_cache.size();
}
int FramesModel::columnCount(const QModelIndex&) const{
    return 2;
}
QVariant FramesModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        switch(index.column()){
        case 0:
               return int(f_cache[index.row()].id());
        case 1:
            return QString::fromStdString(f_cache[index.row()].description());
        }
    }
    return QVariant::Invalid;
}
shared_ptr<SetupsModel> FramesModel::SetupModel(const int index)const{return make_shared<SetupsModel>(f_cache[index]);}
const Frame&FramesModel::GetItem(const size_t i)const{return f_cache[i];}


HVTableModel::HVTableModel(const JPetSetup::HVconfig&config,
        const JPetSetup::Setup&setup,
        const JPetSetup::Frame&frame,
        const std::shared_ptr<IDataSource> src
 ):f_hvtable(config,setup,frame,src){}
int HVTableModel::rowCount(const QModelIndex &) const{return f_hvtable.size();}
int HVTableModel::columnCount(const QModelIndex &) const{return 5;}
QString toQString(const JPET_side side){return (side==side_left)?"LEFT":"RIGHT";}
QVariant HVTableModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        switch(index.column()){
        case 0:
            return toQString(f_hvtable[index.row()].hvpm.side());
        case 1:
            return QString::fromStdString(f_hvtable[index.row()].layer.name());
        case 2:
            return QString::fromStdString(f_hvtable[index.row()].slot.name());
        case 3:
            return QString::fromStdString(f_hvtable[index.row()].phm.name());
        case 4:
            return f_hvtable[index.row()].entry.HV();
        }
    }
    return QVariant::Invalid;
}
bool HVTableModel::setData(const QModelIndex & index, const QVariant & value, int role){
    if (role == Qt::EditRole){
        switch(index.column()){
        case 0:
        case 1:
        case 2:
        case 3:
            return false;
        case 4:
            return f_hvtable.SetHV(index.row(),value.toInt());
        }
    }
    return true;
}
Qt::ItemFlags HVTableModel::flags(const QModelIndex & index)const{
    switch(index.column()){
    case 0:
    case 1:
    case 2:
    case 3:
        return Qt::ItemIsEnabled ;
    case 4:
        return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
    }
}
