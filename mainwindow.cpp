#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->menubtn, SIGNAL(clickMenu(bool)), ui->menu, SLOT(menuShowHide(bool)));
    ui->menu->addBtn("MOJi辞書");
    ui->menu->addBtn("离线字典");
    ui->menu->item_selected(0);
    QPixmap logo;
    logo.load("pics/logo.png");
    ui->icon->setPixmap(logo);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("MainWindow{border-radius: 5px;background-color: rgba(68,114,196,1);}");

    animation = new QPropertyAnimation;
    animation->setPropertyName("geometry");
    animation->setTargetObject(this);
    animation->setEasingCurve(QEasingCurve::InExpo);
    animation->setDuration(500);

    ui->closeBtn->setGeometry(width() - 40, 0, 30, 30);
    ui->menu->setGeometry(-ui->menu->width(), ui->menu->y(), ui->menu->width(), height());
    ui->webEngineView->load(QUrl("file:///pages/welcome.html"));

    connect(ui->searchBox->word, SIGNAL(fetchFinished()), this, SLOT(onFetchResultCallback()));
    connect(ui->closeBtn, SIGNAL(onCloseBtnClicked()), this, SLOT(onExit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFetchResultCallback()
{
    qDebug() << "Loading page..." << endl;
    ui->webEngineView->setHtml(QString::fromStdString(ui->searchBox->word->rendered_html));
}

void MainWindow::onExit()
{
    qDebug() << "Exiting" << endl;
    animation->setStartValue(QRect(x(), y(), width(), height()));
    animation->setEndValue(QRect(x(), -height(), width(), height()));
    connect(animation, SIGNAL(finished()), this, SLOT(close()));
    animation->start();
}
