#include "titlebar.h"
#include <QDebug>

titlebar::titlebar(QWidget* parent)
    :QLabel(parent)
{
    setStyleSheet("border-radius: 5px;");
    setAttribute(Qt::WA_StyledBackground, true);
}

void titlebar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDrag = true;
        lastPos = event->pos();
        QWidget::mousePressEvent(event);
    }
}

void titlebar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDrag = false;
        QWidget::mouseReleaseEvent(event);
    }
}

void titlebar::mouseMoveEvent(QMouseEvent *event)  //移动窗体距离为鼠标移动距离就可以做到鼠标始终对应窗体上某一点
{
    if(isDrag)
    {
        QPoint relaPos(QCursor::pos() - lastPos);
        wndMain->move(relaPos);
    }
    QWidget::mouseMoveEvent(event);
}
