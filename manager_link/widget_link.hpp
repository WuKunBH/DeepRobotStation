#ifndef WIDGET_LINK_HPP
#define WIDGET_LINK_HPP

#include <QWidget>
#include <QPushButton>
#include "interface_remote_control_siyi.hpp"
class CWidgetLink : public QWidget
{
    Q_OBJECT
public:
    static CWidgetLink* Instance();
    explicit CWidgetLink(QWidget *parent = nullptr);
signals:
    // 链路参数设置
    void sigRemoteControlCmd(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE iTpye,
        float iData0=0,float iData1=0,float iData2=0,float iData3=0);
    
private:
    void initializeUiSetupLink();   // 初始化界面布局
    QWidget* createWidgetCtrlLink(); // 创建云台界面-控件
    QWidget* createWidgetShowLink(); // 创建云台界面-显示
public slots:
    void onPushButtonClickedEvent_LinkCtrl();   

private:
    QPushButton* makeupWidgetPushBtn(QString iName, QString iObject);
private:
    static CWidgetLink* _mInstance;


};

#endif // WIDGET_LINK_HPP
