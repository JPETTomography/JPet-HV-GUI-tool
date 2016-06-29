#include "models.h"
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
ConfigsModel::ConfigsModel(const std::shared_ptr<DataAccess::IDataSource>src, const size_t setup_id)
    :QAbstractTableModel(),f_setup_id(setup_id),f_table(src,setup_id){
    for(const auto&item: f_table.SelectAll())f_cache.push_back(item);
}
int ConfigsModel::rowCount(const QModelIndex&) const {
    return f_cache.size();
}
int ConfigsModel::columnCount(const QModelIndex&) const{
    return 1;
}
QVariant ConfigsModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        switch(index.column()){
        case 0:
            return QString::fromStdString(f_cache[index.row()].description());
        }
    }
    return QVariant::Invalid;
}
QVariant ConfigsModel::headerData(int section, Qt::Orientation orientation, int role)const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case 0:
				return ("HV configuration");
		}
	}
	return QVariant();
}
const HVconfig&ConfigsModel::GetItem(const size_t i)const{return f_cache[i];}
void ConfigsModel::AddItem(const QString name){
    f_table.Add(HVconfig(name.toStdString()));
    f_cache.clear();
    for(const auto&item: f_table.SelectAll())f_cache.push_back(item);
}
void ConfigsModel::Delete(const size_t index){
    f_table.Delete(f_cache[index]);
    f_cache.clear();
    for(const auto&item: f_table.SelectAll())f_cache.push_back(item);
}



SetupsModel::SetupsModel(const JPetSetup::Frame&frame)
    :QAbstractTableModel(),f_table(frame.CreateSetupInterface()),f_hv_table(frame.DataSource()){
    for(const Setup&item: f_table.SelectAll()){
        f_cache.push_back(item);
        f_cache_hv.push_back(f_hv_table.ByID(item.highvoltage_id()));
    }
}

int SetupsModel::rowCount(const QModelIndex &parent) const{
    return f_cache.size();
}
int SetupsModel::columnCount(const QModelIndex &) const{
    return 3;
}
QVariant SetupsModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        switch(index.column()){
        case 0:
            return QString::fromStdString(f_cache[index.row()].name());
        case 1:
            return QString::fromStdString(f_cache[index.row()].description());
        case 2:
            return QString::fromStdString(f_cache_hv[index.row()].description());
        }
    }
    return QVariant::Invalid;
}
QVariant SetupsModel::headerData(int section, Qt::Orientation orientation, int role) const{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case 0:
				return ("Name");
			case 1:
				return ("Description");
			case 2:
				return ("High Voltage");
		}
	}
	return QVariant();
}

const Setup&SetupsModel::GetItem(const size_t i)const{return f_cache[i];}
const HighVoltage&SetupsModel::GetHVItem(const size_t index)const{return f_cache_hv[index];}




FramesModel::FramesModel(const std::shared_ptr<DataAccess::IDataSource>src)
    :QAbstractTableModel(),f_table(src){
    for(const auto&item:f_table.SelectAll())f_cache.push_back(item);
}
int FramesModel::rowCount(const QModelIndex&) const {
    return f_cache.size();
}
int FramesModel::columnCount(const QModelIndex&) const{
    return 1;
}
QVariant FramesModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        switch(index.column()){
        case 0:
            return QString::fromStdString(f_cache[index.row()].description());
        }
    }
    return QVariant::Invalid;
}
QVariant FramesModel::headerData(int section, Qt::Orientation orientation, int role) const{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case 0:
				return ("Frame");
		}
	}
	return QVariant();
}

shared_ptr<SetupsModel> FramesModel::SetupModel(const int index)const{return make_shared<SetupsModel>(f_cache[index]);}
const Frame&FramesModel::GetItem(const size_t i)const{return f_cache[i];}


HVTableModel::HVTableModel(const JPetSetup::HVconfig&config,
        const JPetSetup::Setup&setup,
        const JPetSetup::Frame&frame,
        const JPetSetup::HighVoltage&hvoltage,
        const std::shared_ptr<IDataSource> src,
        const std::shared_ptr<HVAdjust::IHVSetter> hardware
 ):f_hvtable(config,setup,frame,hvoltage,src,hardware){}
int HVTableModel::rowCount(const QModelIndex &) const{return f_hvtable.SlotInfo().size();}
int HVTableModel::columnCount(const QModelIndex &) const{return 7;}
QString toQString(const JPET_side side){return (side==side_left)?"LEFT":"RIGHT";}
QVariant HVTableModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        switch(index.column()){
        case 0:
		return toQString(f_hvtable.SlotInfo()[index.row()].hvpm.side());
        case 1:
		return QString::fromStdString(f_hvtable.SlotInfo()[index.row()].layer.name());
        case 2:
		return QString::fromStdString(f_hvtable.SlotInfo()[index.row()].slot.name());
        case 3:
		return QString::fromStdString(f_hvtable.SlotInfo()[index.row()].phm.name());
        case 4:
		return int(f_hvtable.SlotInfo()[index.row()].hvchannel.idx());
        case 5:
		return f_hvtable.HVConfigEntries()[index.row()].HV();
        case 6:
		return f_hvtable.SlotInfo()[index.row()].phm.max_hv();
        }
    }
    return QVariant::Invalid;
}
QVariant HVTableModel::headerData(int section, Qt::Orientation orientation, int role) const{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case 0:
				return ("Side");
			case 1:
				return ("Layer");
			case 2:
				return ("Slot");
			case 3:
				return ("PHM");
			case 4:
				return ("idx");
			case 5:
				return ("HV");
			case 6:
				return ("MAX");
		}
	}
	return QVariant();
}

bool HVTableModel::setData(const QModelIndex & index, const QVariant & value, int role){
    if (role == Qt::EditRole){
        switch(index.column()){
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
		return false;
        case 5:
		double new_hv=value.toInt();
		if(new_hv*f_hvtable.SlotInfo()[index.row()].phm.max_hv()<0)new_hv*=-1.0;
		return f_hvtable.SetHV(index.row(),new_hv);
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
    case 4:
    case 6:
        return Qt::ItemIsEnabled ;
    case 5:
        return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
    }
}
HVAdjust::HVTable&HVTableModel::Data(){
    return f_hvtable;
}
