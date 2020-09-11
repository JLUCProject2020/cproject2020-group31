#include "windowcontrol.h"
#include <QDebug>

WindowControl::WindowControl(QWidget* parent)
    :QLabel(parent)
{
    image = new QPixmap;
    bool success = image->load(":/icons/res/pics/close.png");
    if (success)
    {
        qDebug() << "Loaded image close.png successfully." << endl;
    }else
    {
        qDebug() << "Cannot find close.png." << endl;
    }
    *image = image->scaledToHeight(height());
    setAlignment(Qt::AlignCenter);
    setPixmap(*image);
    setStyleSheet("border-radius: 1px;");
    app = appMain;
}

void WindowControl::enterEvent(QEvent *e)
{
    setStyleSheet("border-radius: 2px;background-color:rgba(255, 100, 120, 1);");
}
void WindowControl::leaveEvent(QEvent* e)
{
    setStyleSheet("border-radius: 2px;background-color:rgba(255, 100, 120, 0);");
}

void WindowControl::mouseReleaseEvent(QMouseEvent *ev)
{
    emit onCloseBtnClicked();
}

void WindowControl::mousePressEvent(QMouseEvent *ev)
{
    setStyleSheet("border-radius: 2px;background-color:rgba(255, 100, 120, 0.5);");
}
