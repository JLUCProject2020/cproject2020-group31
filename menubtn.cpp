#include "menubtn.h"
#include <QDebug>

MenuBtn::MenuBtn(QWidget* parent)
    :QLabel(parent)
{
    pic = new QPixmap;
    bool status = pic->load(":/icons/res/pics/menu-btn.png");
    setAlignment(Qt::AlignCenter);

    if (status)
    {
        qDebug() << "Loaded image menu-btn.png successfully." << endl;
    } else
    {
        qDebug() << "Failed to load image menu-btn.jpg." << endl;
    }
    setIndent(0);
    setMargin(0);
    setScaledContents(true);
    *pic = pic->scaled(width(), height());
    setPixmap(*pic);
//    this->setFlat(true);
    pressed = false;
}

MenuBtn::~MenuBtn()
{
    delete pic;
}

void MenuBtn::enterEvent(QEvent* e)
{
    if (pressed == false)
        setStyleSheet("background-color:rgba(255, 255, 255, 0.2);border-style:solid;border-width:2px;border-color:rgba(255, 255, 255, 0);");
}


void MenuBtn::leaveEvent(QEvent* e)
{
    if (pressed == false)
        setStyleSheet("background-color:rgba(255, 255, 255, 0);border-style:solid;border-width:2px;border-color:rgba(255, 255, 255, 0);");
}

void MenuBtn::mousePressEvent(QMouseEvent *ev)
{
    pressed = ! pressed;
    if (pressed == true)
    {
        setStyleSheet("background-color:rgba(255, 255, 255, 0.2);border-style:solid;border-width:2px;border-color:rgba(255, 255, 255, 0.7);");
    }
    emit clickMenu(pressed);
}
