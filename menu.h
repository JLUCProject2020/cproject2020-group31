#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include "menuitem.h"

class Menu : public QWidget
{
    Q_OBJECT
    unsigned char selected;
    QPropertyAnimation* animation;
    QVector<MenuItem *> buttons;
public:
    explicit Menu(QWidget *parent = nullptr);
    void addBtn(QString title);
    ~Menu();
signals:
    void toOnlineMode();
    void toOfflineMode();
public slots:
    void item_selected(unsigned char id);
    void menuShowHide(bool status);
    void paintEvent(QPaintEvent *event) override;
};

#endif // MENU_H
