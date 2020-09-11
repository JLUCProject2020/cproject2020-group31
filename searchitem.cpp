#include "searchitem.h"
#include <QDebug>

SearchItem::SearchItem(QWidget* parent, unsigned char id)
    :QLabel(parent)
{
    setAlignment(Qt::AlignVCenter);
    setIndent(0);
    setMargin(0);
    setScaledContents(true);
    this->id = id;
    setFont(QFont("思源黑体 CN Light"));
    setStyleSheet("background-color:rgba(68,114,196, 0);border-style:solid;border-width:2px;border-color:rgba(68,114,196, 0); font-size: 15px;padding: 0px 5px;");
}

void SearchItem::enterEvent(QEvent* e)
{
    setStyleSheet("background-color:rgba(68,114,196, 0.2);border-style:solid;border-width:2px;border-color:rgba(68,114,196, 0); font-size: 15px;padding: 0px 5px;");
}


void SearchItem::leaveEvent(QEvent* e)
{
    setStyleSheet("background-color:rgba(68,114,196, 0);border-style:solid;border-width:2px;border-color:rgba(68,114,196, 0); font-size: 15px;padding: 0px 5px;");
}

void SearchItem::mousePressEvent(QMouseEvent *ev)
{
    setStyleSheet("background-color:rgba(68,114,196, 0.5);border-style:solid;border-width:2px;border-color:rgba(68,114,196, 0.7); font-size: 15px;padding: 0px 5px;");
    emit clickItem(QString::number(id));
}

unsigned char SearchItem::getId()
{
    return id;
}
