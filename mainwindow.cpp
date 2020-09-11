#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <fstream>
#include <string>
#include "dict.h"

extern unsigned int MODE;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->menubtn, SIGNAL(clickMenu(bool)), ui->menu, SLOT(menuShowHide(bool)));
    ui->menu->addBtn("MOJi辞書");
    ui->menu->addBtn("离线字典");
    ui->menu->item_selected(0);
    connect(ui->menu, SIGNAL(toOnlineMode()), this, SLOT(onlineMode()));
    connect(ui->menu, SIGNAL(toOfflineMode()), this, SLOT(offlineMode()));
    QPixmap logo;
    logo.load(":/icons/res/pics/logo.png");
    ui->icon->setPixmap(logo);

    //Set window style
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("MainWindow{border-radius: 5px;background-color: rgba(68,114,196,1);}");

    //Window animations
    animation = new QPropertyAnimation;
    animation->setPropertyName("geometry");
    animation->setTargetObject(this);
    animation->setEasingCurve(QEasingCurve::InExpo);
    animation->setDuration(500);

    //Set the geometry of some fixed controls.
    ui->closeBtn->setGeometry(width() - 40, 0, 30, 30);
    ui->menu->setGeometry(-ui->menu->width(), ui->menu->y(), ui->menu->width(), height());
    ui->webEngineView->load(QUrl("qrc:/pages/res/pages/welcome/welcome.html"));

    //Connect slots.
    connect(ui->searchBox->word, SIGNAL(fetchFinished()), this, SLOT(onFetchResultCallback()));
    connect(ui->closeBtn, SIGNAL(onCloseBtnClicked()), this, SLOT(onExit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFetchResultCallback()
{
    //Load HTML page when Python script finished fetching.
    qDebug() << "Loading page..." << endl;
    ui->webEngineView->setHtml(QString::fromStdString(ui->searchBox->word->rendered_html));
}

void MainWindow::onExit()
{
    //Show exiting animation.
    qDebug() << "Exiting" << endl;
    animation->setStartValue(QRect(x(), y(), width(), height()));
    animation->setEndValue(QRect(x(), -height(), width(), height()));
    connect(animation, SIGNAL(finished()), this, SLOT(close()));
    animation->start();
}

void MainWindow::offlineMode()
{
    if (MODE == 1)
    {
        return;
    }
    qDebug() << "Transforming into offline mode." << endl;
    MODE = 1;
    disconnect(ui->searchBox, SIGNAL(startSearch(QString)), ui->searchBox->search, SLOT(search(QString)));
    connect(ui->searchBox, SIGNAL(startSearch(QString)), ui->searchBox, SLOT(onOfflineSearch(QString)));

    disconnect(ui->searchBox, SIGNAL(startFetch(QString)), ui->searchBox->word, SLOT(fetch(QString)));
    connect(ui->searchBox, SIGNAL(startFetch(QString)), this, SLOT(onOfflineShow(QString)));
}
void MainWindow::onlineMode()
{
    if (MODE == 0)
    {
        return;
    }
    qDebug() << "Transforming into online mode." << endl;
    MODE = 0;
    disconnect(ui->searchBox, SIGNAL(startSearch(QString)), ui->searchBox, SLOT(onOfflineSearch(QString)));
    connect(ui->searchBox, SIGNAL(startSearch(QString)), ui->searchBox->search, SLOT(search(QString)));

    disconnect(ui->searchBox, SIGNAL(startFetch(QString)), this, SLOT(onOfflineShow(QString)));
    connect(ui->searchBox, SIGNAL(startFetch(QString)), ui->searchBox->word, SLOT(fetch(QString)));
}
void MainWindow::onOfflineShow(QString filename)
{
    qDebug() << "Loading " << filename << endl;
    ui->webEngineView->load("file:///dict/" + filename);
}
