#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <QMainWindow>
#include <QItemSelection>
namespace DataAccess{
    class PQData;
}
namespace JPetSetup{
    class HVPMConnections;
    class Photomultipliers;
}
class ConfigsModel;
class FramesModel;
class SetupsModel;
class HVTableModel;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void closeEvent(QCloseEvent*);
private slots:
    void FrameSelect(const QItemSelection&, const QItemSelection&);
    void HVTableUpdate(const QItemSelection&, const QItemSelection&);
    void on_pushButton_clicked();
private:
    Ui::MainWindow *ui;
    std::shared_ptr<DataAccess::PQData> Source;
    std::shared_ptr<ConfigsModel> configs;
    std::shared_ptr<FramesModel> frames;
    std::shared_ptr<SetupsModel> setups;
    std::shared_ptr<JPetSetup::HVPMConnections>phm_conn_factory;
    std::shared_ptr<JPetSetup::Photomultipliers>phm_factory;
    std::shared_ptr<HVTableModel> table_model;
};

#endif // MAINWINDOW_H
