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
    for(const auto&item:f_cache)f_setup_cache.push_back(make_shared<SetupsModel>(item));
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
shared_ptr<SetupsModel> FramesModel::SetupModel(const int index){return f_setup_cache[index];}
const Frame&FramesModel::GetItem(const size_t i)const{return f_cache[i];}
