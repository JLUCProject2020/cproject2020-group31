#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QMainWindow>
#include "mainwindow.h"

extern MainWindow* wndMain;

class titlebar : public QLabel
{
    Q_OBJECT
    QPoint lastPos;
    bool isDrag;
public:
    titlebar(QWidget* parent);
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
};

#endif // TITLEBAR_H
