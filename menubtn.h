#ifndef MENUBTN_H
#define MENUBTN_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>


class MenuBtn: public QLabel
{
    Q_OBJECT
private:
    bool pressed;
    QPixmap* pic;
public:
    MenuBtn(QWidget* parent);
    ~MenuBtn();
    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void mousePressEvent(QMouseEvent *ev) override;
signals:
    void clickMenu(bool status);
};

#endif // MENUBTN_H
