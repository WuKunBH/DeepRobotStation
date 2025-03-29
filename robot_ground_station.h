#ifndef ROBOT_GROUND_STATION_H
#define ROBOT_GROUND_STATION_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QTouchEvent>
class CRobotGroundStation : public QMainWindow
{
    Q_OBJECT

public:
    CRobotGroundStation(QWidget *parent = nullptr);
    ~CRobotGroundStation();
private:
    // 设置覆盖层
    void setupOverlay();
protected:
    void showEvent(QShowEvent *event) override;
    bool event(QEvent *event) override;

private slots:
    void onPushButtonClickedEvent();
private:
    void initializeUiSetup();
    void setupAndroidFullscreen();
    bool mIsInitialized;
    QWidget* initializeRobotCtrl(); // 机器人控制指令
    QPushButton* makeupWidgetPushBtn(QString iName, QString iObject);
    void createSignalConnectToSlots();
private:
    QStackedWidget* _mStackedCenter = nullptr;
    
};
#endif // ROBOT_GROUND_STATION_H
