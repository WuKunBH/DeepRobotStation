#ifndef WIDGET_GIMBAL_HPP
#define WIDGET_GIMBAL_HPP

#include <QWidget>
#include <QPushButton>
#include "interface_gimabal_siyi.hpp"
#include <QMap>
#include <QLabel>
#include <QTextEdit>
#include <QScrollArea>
class CWidgetGimbal : public QWidget
{
    Q_OBJECT
public:
    static CWidgetGimbal* Instance();

    explicit CWidgetGimbal(QWidget *parent = nullptr);
signals:
    void sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE iTpye,float iData0=0,float iData1=0,float iData2=0,float iData3=0);  // 云台控制指令
public slots:
    void onHandleGimabalStateResponse(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE iTpye,QStringList mContext);
private:
    void initializeUiSetup();   // 初始化界面布局
    QWidget* createGimbalWidgetCtrl(); // 创建云台界面-控件
    QScrollArea* createGimbalWidgetShow(); // 创建云台界面-显示
public slots:
    void onPushButtonClickedEvent_GimbalCtrl();
   

private:
    QPushButton* makeupWidgetPushBtn(QString iName, QString iObject);
private:
    static CWidgetGimbal* _mInstance;
    QMap<CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE, QLabel*> _mapLabelGimbalState; // 显示云台状态QString> map

};

#endif // WIDGET_GIMBAL_HPP
