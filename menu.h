#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include "menuitem.h"
#include <QPainter>
#include <QStyleOption>
#include <QPropertyAnimation>
#include <QEasingCurve>

class Menu : public QWidget
{
    Q_OBJECT
    QVector<MenuItem *> buttons;
    unsigned char selected;
    QPropertyAnimation* animation;
public:
    explicit Menu(QWidget *parent = nullptr);
    void addBtn(QString title);
    ~Menu();
signals:
public slots:
    void item_selected(unsigned char id);
    void menuShowHide(bool status);
    void paintEvent(QPaintEvent *event) override;
};

#endif // MENU_H
