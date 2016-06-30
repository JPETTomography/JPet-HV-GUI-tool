#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <QMainWindow>
#include <QItemSelection>
namespace HVAdjust {
    class IHVSetter;
}
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
    void FrameSelect();
    void SetupSelect();
    void HVTableUpdate();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_timer_update();
    
private:
    Ui::MainWindow *ui;
    std::shared_ptr<DataAccess::PQData> Source;
    std::shared_ptr<HVAdjust::IHVSetter> hardware;
    std::shared_ptr<ConfigsModel> configs;
    std::shared_ptr<FramesModel> frames;
    std::shared_ptr<SetupsModel> setups;
    std::shared_ptr<JPetSetup::HVPMConnections>phm_conn_factory;
    std::shared_ptr<JPetSetup::Photomultipliers>phm_factory;
    std::shared_ptr<HVTableModel> table_model;
};

#endif // MAINWINDOW_H
