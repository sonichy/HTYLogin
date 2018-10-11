#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;

private slots:
    void updateTime();
    void on_pushButtonLogin_clicked();
    void on_pushButtonShutdown_clicked();
    void passwordChange(QString);
    void logout();
    void reboot();
    void suspend();
    //void hibernate();
    void lock();

};

#endif // MAINWINDOW_H
