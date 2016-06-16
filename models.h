#ifndef MODELS_H
#define MODELS_H
#include <vector>
#include <QAbstractTableModel>
#include <JPetData/Detectors.h>
#include <JPetData/Frames.h>
#include <JPetData/HVconfig.h>
#include <JPetData/HVSetter.h>

class ConfigsModel : public QAbstractTableModel{
    Q_OBJECT
public:
    ConfigsModel(const std::shared_ptr<DataAccess::IDataSource> src,const size_t setup_id);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    const JPetSetup::HVconfig& GetItem(const size_t index)const;
    void AddItem(const QString name);
    void Delete(const size_t index);
private:
    size_t f_setup_id;
    JPetSetup::HVconfigTable f_table;
    std::vector<JPetSetup::HVconfig> f_cache;
};


class SetupsModel : public QAbstractTableModel{
    Q_OBJECT
public:
    SetupsModel(const JPetSetup::Frame&frame);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    const JPetSetup::Setup& GetItem(const size_t index)const;
    const JPetSetup::HighVoltage& GetHVItem(const size_t index)const;
private:
    DataAccess::Factory<JPetSetup::Setup> f_table;
    JPetSetup::HighVoltageTable f_hv_table;
    std::vector<JPetSetup::Setup> f_cache;
    std::vector<JPetSetup::HighVoltage> f_cache_hv;
};

class DummyHardware: public HVAdjust::IHVSetter{
public:
    DummyHardware(){}
    virtual ~DummyHardware(){}
    virtual bool SetHV(size_t, double)override{}
    virtual double GetHV(size_t) const override{return INFINITY;}
    virtual double GetCurent(size_t) const override{return INFINITY;}
};
class FramesModel : public QAbstractTableModel{
    Q_OBJECT
public:
    FramesModel(const std::shared_ptr<DataAccess::IDataSource> src);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    const JPetSetup::Frame& GetItem(const size_t index)const;
    std::shared_ptr<SetupsModel> SetupModel(const int index)const;
private:
    JPetSetup::Frames f_table;
    std::vector<JPetSetup::Frame> f_cache;
    std::vector<std::shared_ptr<SetupsModel>> f_setup_cache;
};

class HVTableModel : public QAbstractTableModel{
    Q_OBJECT
public:
    HVTableModel(const JPetSetup::HVconfig&config,
            const JPetSetup::Setup&setup,
            const JPetSetup::Frame&frame,
            const JPetSetup::HighVoltage&hvoltage,
            const std::shared_ptr<DataAccess::IDataSource> src
    );
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const ;
    HVAdjust::HVTable&Data();
private:
    HVAdjust::HVTable f_hvtable;
};

#endif // MODELS_H
