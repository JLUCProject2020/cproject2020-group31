#ifndef WINDOWCONTROL_H
#define WINDOWCONTROL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QApplication>
#include "mainwindow.h"

extern QApplication* appMain;
extern MainWindow* wndMain;
class WindowControl : public QLabel
{
    Q_OBJECT
    QPixmap* image;
public:
    QApplication* app;
    enum class btnType {
        WND_CLOSE,
        WND_MINIMIZE,
        WND_MAXIMIZE
    } btn_type;
    WindowControl(QWidget* parent);

    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
signals:
    void onCloseBtnClicked();
};

#endif // WINDOWCONTROL_H
