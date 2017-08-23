// Qt中使用QSettings类读写配置文件：https://rekols.github.io/2017/03/29/Qt%E4%B8%AD%E4%BD%BF%E7%94%A8QSettings%E7%B1%BB%E8%AF%BB%E5%86%99%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include <QStandardPaths>
#include <QDesktopWidget>
#include <QDebug>
#include <QSettings>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateTime();
    showFullScreen();    

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    timer->start(1000);
    QString userName = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0).section('/', -1);
    ui->labelUserName->setText(userName);
    // 登录设置文件：/var/lib/AccountsService/users/用户名
    QSettings *settings = new QSettings("/var/lib/AccountsService/users/" + userName, QSettings::IniFormat);
    QString avantar = settings->value("/User/Icon").toString().replace("file://","");
    //qDebug() << avantar;
    QPixmap pixmap(avantar);
    ui->labelAvantar->setPixmap(pixmap);
    QString greeterBackground = settings->value("/User/GreeterBackground").toString().replace("file://","");
    // background-image 不能拉伸，border-image 可以自动拉伸。
    QString styles = "MainWindow{ border-image: url(" + greeterBackground + ");}";
    setStyleSheet(styles);
    ui->pushButtonShutdown->move(QApplication::desktop()->width()-70,QApplication::desktop()->height()-70);
    ui->verticalWidgetTime->move(10,QApplication::desktop()->height()-130);
    ui->verticalWidgetAccount->move((QApplication::desktop()->width()-ui->verticalWidgetAccount->width())/2,(QApplication::desktop()->height()-ui->verticalWidgetAccount->height())/2);
    connect(ui->lineEditPassword,SIGNAL(textChanged(QString)),this,SLOT(passwordChange(QString)));
    connect(ui->lineEditPassword,SIGNAL(returnPressed()),this,SLOT(on_pushButtonLogin_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString STime = time.toString("hh:mm");
    QString SDate = time.toString("yyyy-MM-dd ddd");
    ui->labelTime->setText(STime);
    ui->labelDate->setText(SDate);
}

void MainWindow::on_pushButtonLogin_clicked()
{
    if(ui->lineEditPassword->text()=="1111"){
        close();
    }else{
        ui->labelTips->setText("密码错误");
    }
}

void MainWindow::on_pushButtonShutdown_clicked(){
    QProcess *proc = new QProcess;
    proc->start("systemctl poweroff");
}

void MainWindow::passwordChange(QString)
{
    ui->labelTips->setText("");
}
