#include "menu.h"
#include <QDebug>

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    animation = new QPropertyAnimation;
    animation->setTargetObject(this);
    animation->setPropertyName("pos");
    animation->setDuration(750);
    animation->setEasingCurve(QEasingCurve::InOutExpo);
}
void Menu::item_selected(unsigned char id)
{
    selected = id;
    for (auto itr = buttons.begin(); itr != buttons.end(); itr++)
    {
        if ((*itr)->getId() == id && (*itr)->getSelected() == false)
        {
            (*itr)->setSelected(true);
        } else
        if ((*itr)->getId() != id && (*itr)->getSelected() == true)
        {
            (*itr)->setSelected(false);
        }
    }
    if (id == 0)
    {
        emit toOnlineMode();
    }
    else
    {
        emit toOfflineMode();
    }
}
void Menu::addBtn(QString title)//向左侧目录栏添加新按钮 按下事件发出信号select(id)
{
    MenuItem* item = new MenuItem(this, buttons.size());
    buttons.push_back(item);
    item->setText(title);
    item->setMargin(0);
    item->setGeometry(0, buttons.size() * 60, width(), 50);
    item->setAlignment(Qt::AlignCenter);
    item->show();
    connect(item, SIGNAL(select(unsigned char)), this, SLOT(item_selected(unsigned char)));
}

void Menu::menuShowHide(bool status)
{
    if (status == false)
    {
        animation->stop();
        animation->setStartValue(QPoint(x(), y()));
        animation->setEndValue(QPoint(-width(), y()));
        animation->start();
    }
    else
    {
        animation->stop();
        animation->setStartValue(QPoint(x(), y()));
        animation->setEndValue(QPoint(0, y()));
        animation->start();
    }
}
void Menu::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(e);
}

Menu::~Menu()
{
    delete animation;
    for (auto itr = buttons.begin(); itr != buttons.end(); itr++)
    {
        delete *itr;
    }
}
