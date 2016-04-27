#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <DataGeneral/Photomultiplier.h>
#include <DataGeneral/Calibration.h>
#include <Postgres/postgres_data.h>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_SelectedRun_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    std::shared_ptr<JPetData::PhotomultipliersData> PhmData;
    std::shared_ptr<DataAccess::PQData> Source;
};

#endif // MAINWINDOW_H
