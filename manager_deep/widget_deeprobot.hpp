#ifndef WIDGET_DEEPROBOT_HPP
#define WIDGET_DEEPROBOT_HPP

#include <QWidget>
#include <QPushButton>
#include "interface_robot_deep.hpp"
class CWidgetDeepRobot : public QWidget
{
    Q_OBJECT
public:
    static CWidgetDeepRobot* Instance();
    explicit CWidgetDeepRobot(QWidget *parent = nullptr);
signals:
    void sigRobotControlCmd(CInterfaceRobotDeep::_ENUM_ROBOT_DEEP_CONTROL_MODE iType,
                            float iData0= 1,float iData1= 1,float iData2= 1,float iData3= 1);

private:

    void initializeUiSetupRobotDeep();
    QWidget* createWidgetCtrlDeep();
    QWidget* createWidgetShowDeep();
    QPushButton* makeupWidgetPushBtn(QString iName, QString iObject);
private slots:
    void onPushButtonClickedEvent_DeepCtrl();

private:
    static CWidgetDeepRobot* _mInstance;
};

#endif // WIDGET_DEEPROBOT_HPP
