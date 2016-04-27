#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
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
    std::shared_ptr<DataAccess::PQData> Source;
};

#endif // MAINWINDOW_H
