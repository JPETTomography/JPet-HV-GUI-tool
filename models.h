#ifndef MODELS_H
#define MODELS_H
#include <vector>
#include <QAbstractTableModel>
#include <JPetData/Detectors.h>
#include <JPetData/Frames.h>
#include <JPetData/HVconfig.h>

class ConfigsModel : public QAbstractTableModel{
    Q_OBJECT
public:
    ConfigsModel(const std::shared_ptr<DataAccess::IDataSource> src);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    const JPetSetup::HVconfig& GetItem(const size_t index)const;
private:
    JPetSetup::HVconfigTable f_table;
    std::vector<JPetSetup::HVconfig> f_cache;
};


class SetupsModel : public QAbstractTableModel{
    Q_OBJECT
public:
    SetupsModel(const JPetSetup::Frame&src);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    const JPetSetup::Setup& GetItem(const size_t index)const;
private:
    DataAccess::Factory<JPetSetup::Setup> f_table;
    std::vector<JPetSetup::Setup> f_cache;
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
    HVTableModel(const DataAccess::Factory<JPetSetup::HVconfigEntry>&config,
            const JPetSetup::Setup&setup,
            const JPetSetup::Frame&frame,
            const std::shared_ptr<DataAccess::IDataSource> src
    );
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const ;
private:
    DataAccess::Factory<JPetSetup::HVconfigEntry> f_entries;
    JPetSetup::Setup f_setup;
    JPetSetup::Frame f_frame;
    JPetSetup::HVPMConnections f_phms;
    JPetSetup::Photomultipliers f_photomultipliers;
    std::vector<JPetSetup::Layer> f_layers;
    std::vector<JPetSetup::Slot> f_slots;
    std::vector<JPetSetup::HVPMConnection> f_connections;
    std::vector<JPetSetup::HVconfigEntry> f_entries_cache;
};

#endif // MODELS_H
