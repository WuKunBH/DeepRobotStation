#include "robot_ground_station.h"

#include <QApplication>
#include <QFile>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/qss/QSS/style.qss"); // 使用资源系统加载文件
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString style(file.readAll());
        a.setStyleSheet(style);
        qDebug()<<"---------- 加载样式表成功 --------------";
    }
    CRobotGroundStation w;
    w.show();
    return a.exec();
}
