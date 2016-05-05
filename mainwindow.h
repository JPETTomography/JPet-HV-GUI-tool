#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <QMainWindow>
#include <QItemSelection>
namespace DataAccess{
    class IDataSource;
}
class ConfigsModel;
class FramesModel;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void FrameSelect(const QItemSelection&, const QItemSelection&);
    void HVTableUpdate(const QItemSelection&, const QItemSelection&);
private:
    Ui::MainWindow *ui;
    std::shared_ptr<DataAccess::IDataSource> Source;
    std::shared_ptr<ConfigsModel> configs;
    std::shared_ptr<FramesModel> frames;
};

#endif // MAINWINDOW_H
