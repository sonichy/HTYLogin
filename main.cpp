// 只允许运行一个程序： http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=20676637&id=86141

#include "mainwindow.h"
#include <QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSharedMemory *shareMem = new QSharedMemory(QString("SingleInstanceIdentify"));
    // if the sharedmemory has not been created, it returns false, otherwise true. But if the application exit unexpectedly, the sharedmemory will not detach. So, we try twice.
    volatile short i = 2;
    while(i--){if(shareMem->attach(QSharedMemory::ReadOnly)){shareMem->detach();}}
    if(shareMem->create(1))
    {
        MainWindow w;
        w.show();
        app.exec();
        if(shareMem->isAttached())shareMem->detach();
        delete shareMem;
    }
    return 0;
}
