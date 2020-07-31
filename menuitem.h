#ifndef MENUITEM_H
#define MENUITEM_H

#include <QWidget>
#include <QLabel>

class MenuItem : public QLabel
{
    Q_OBJECT
    bool selected;
public:
    unsigned char id;
    MenuItem(QWidget* parent, unsigned char id);
    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void mousePressEvent(QMouseEvent *ev) override;
    unsigned char getId()
    {
        return id;
    }
    bool getSelected()
    {
        return selected;
    }
    void setSelected(bool s);
signals:
    void select(unsigned char id);
};

#endif // MENUITEM_H
