#include "widget_overlay.hpp"
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
CWidgetOverlay::CWidgetOverlay(QWidget *parent)
    : QWidget{parent}
{
    _label = new QLabel(this);
    // 设置半透明背景和白色文字
    // _label->setStyleSheet("QLabel { color: white; font-size: 16px; padding: 10px; }");
    _label->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(_label,Qt::AlignLeft);

    layout->addStretch();
    this->setLayout(layout);
}

void CWidgetOverlay::setText(const QString& text)
{
    if (_label) {
        _label->setText(text);
    }
}
