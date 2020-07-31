#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>


class SearchItem: public QLabel
{
    Q_OBJECT
private:
    unsigned char id;
public:
    SearchItem(QWidget* parent, unsigned char id);
    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void mousePressEvent(QMouseEvent *ev) override;
    unsigned char getId();
signals:
    void clickItem(QString id);
};


#endif // SEARCHITEM_H
