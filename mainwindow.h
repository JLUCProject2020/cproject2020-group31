#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QWebEngineSettings>
#include <QPropertyAnimation>
#include <QWebEnginePage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPropertyAnimation* animation;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFetchResultCallback();
    void onExit();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
