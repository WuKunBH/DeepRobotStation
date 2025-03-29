#ifndef WIDGET_OVERLAY_HPP
#define WIDGET_OVERLAY_HPP

#include <QWidget>

class QLabel;

class CWidgetOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetOverlay(QWidget *parent = nullptr);
    void setText(const QString& text); // 设置显示文本

private:
    QLabel* _label = nullptr; // 用于显示文本的标签

signals:
};

#endif // WIDGET_OVERLAY_HPP
