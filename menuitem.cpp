#include "menuitem.h"

MenuItem::MenuItem(QWidget* parent, unsigned char id) :QLabel(parent)
{
    setIndent(0);
    setMargin(0);
    setScaledContents(true);
    selected = false;
    this->id = id;
    setFont(QFont("Microsoft YaHei UI", 10));
    setStyleSheet("border-style: solid; border-width: 2px;border-color: rgba(255, 255, 255, 0);background-color:rgba(255, 255, 255, 0); color: white;");
}

void MenuItem::enterEvent(QEvent* e)
{
    if (selected == false)
        setStyleSheet("border-style: solid; border-width: 2px;border-color: rgba(255, 255, 255, 0);background-color:rgba(255, 255, 255, 0.2);border-color: rgba(255, 255, 255, 0); color: white;");
}


void MenuItem::leaveEvent(QEvent* e)
{
    if (selected == false)
        setStyleSheet("border-style: solid; border-width: 2px;border-color: rgba(255, 255, 255, 0);background-color:rgba(255, 255, 255, 0);color: white;");
}


void MenuItem::mousePressEvent(QMouseEvent *ev)
{
    emit select(id);
}


void MenuItem::setSelected(bool s)
{
    selected = s;
    if (selected == false)
        setStyleSheet("border-style: solid; border-width: 2px;border-color: rgba(255, 255, 255, 0);background-color:rgba(255, 255, 255, 0);border-color: rgba(255, 255, 255, 0);color: white;");
    else
        setStyleSheet("border-style: solid; border-width: 2px;border-color: rgba(255, 255, 255, 0.7);background-color:rgba(255, 255, 255, 0.5);color: white;");
}


