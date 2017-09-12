#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include <QStandardPaths>
#include <QDesktopWidget>
#include <QDebug>
#include <QSettings>
#include <QProcess>
#include <QPainter>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonShutMenu->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
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
    QPixmap pixmapa(avantar);
    QPixmap pixmap(80,80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addEllipse(0, 0, 80, 80);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 80, 80, pixmapa);
    ui->labelAvantar->setPixmap(pixmap);
    QString greeterBackground = settings->value("/User/GreeterBackground").toString().replace("file://","");
    // background-image 不能拉伸，border-image 可以自动拉伸。
    QString styles = "MainWindow{ border-image: url(" + greeterBackground + ");}";
    setStyleSheet(styles);
    ui->pushButtonShutdown->move(QApplication::desktop()->width()-130,QApplication::desktop()->height()-70);
    ui->pushButtonShutMenu->move(ui->pushButtonShutdown->x()+ui->pushButtonShutdown->width()+10,ui->pushButtonShutdown->y());
    ui->verticalWidgetTime->move(10,QApplication::desktop()->height()-130);
    ui->verticalWidgetAccount->move((QApplication::desktop()->width()-ui->verticalWidgetAccount->width())/2,(QApplication::desktop()->height()-ui->verticalWidgetAccount->height())/2);
    connect(ui->lineEditPassword,SIGNAL(textChanged(QString)),this,SLOT(passwordChange(QString)));
    connect(ui->lineEditPassword,SIGNAL(returnPressed()),this,SLOT(on_pushButtonLogin_clicked()));

    QMenu *shutmenu=new QMenu;
    QAction *logout=new QAction("注销",this);
    QAction *reboot=new QAction("重启",this);
    QAction *suspend=new QAction("待机",this);
    QAction *hibernate=new QAction("休眠",this);
    QAction *lock=new QAction("锁定",this);
    shutmenu->addAction(logout);
    shutmenu->addAction(reboot);
    shutmenu->addAction(suspend);
    shutmenu->addAction(hibernate);
    shutmenu->addAction(lock);
    ui->pushButtonShutMenu->setMenu(shutmenu);
    connect(logout,SIGNAL(triggered()),this,SLOT(logout()));
    connect(reboot,SIGNAL(triggered()),this,SLOT(reboot()));
    connect(suspend,SIGNAL(triggered()),this,SLOT(suspend()));
    connect(hibernate,SIGNAL(triggered()),this,SLOT(hibernate()));
    connect(lock,SIGNAL(triggered()),this,SLOT(lock()));
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

void MainWindow::logout()
{
    QProcess *proc = new QProcess;
    proc->start("qdbus com.deepin.SessionManager /com/deepin/SessionManager com.deepin.SessionManager.RequestLogout");
    //proc->start("systemctl restart lightdm");
}

void MainWindow::reboot()
{
    QProcess *proc = new QProcess;
    proc->start("systemctl reboot");
}

void MainWindow::suspend()
{
    QProcess *proc = new QProcess;
    proc->start("systemctl suspend");
}

void MainWindow::hibernate()
{
    QProcess *proc = new QProcess;
    proc->start("systemctl hibernate");
}

void MainWindow::lock()
{
    QProcess *proc = new QProcess;
    //proc->start("systemctl lock");
    proc->start("qdbus com.deepin.SessionManager /com/deepin/SessionManager com.deepin.SessionManager.RequestLock");
}
