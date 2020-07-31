#include "searchbox.h"
#include <QDebug>
#include "mainwindow.h"

extern MainWindow* wndMain;
SearchBox::SearchBox(QWidget *parent) : QWidget(parent)
{
    collapsed = true;

    textBox = new TextBox(this);
    connect(this->textBox, SIGNAL(textEdited()), this, SLOT(onTextChange()));
    connect(this->textBox, SIGNAL(collapse()), this, SLOT(collapse()));
    connect(this->textBox, SIGNAL(expand()), this, SLOT(expand()));
    textBox->setStyleSheet("font-family: \"Microsoft YaHei UI\"; font-size: 20px;");
    this->setAttribute(Qt::WA_StyledBackground,true);
    setStyleSheet("background-color: rgba(255, 255, 255, 1);border-style: solid; border-width: 2px; border-color: rgba(68,114,196,1);");

    delay = new QTimer;
    delay->setInterval(1);
    connect(delay, &QTimer::timeout, this, &SearchBox::onTimerTick);


    searchThread = new QThread;
    search = new MOJiDictSearch(pyInfo);
    search->moveToThread(searchThread);
    connect(this, SIGNAL(startSearch(QString)), search, SLOT(search(QString)));
    connect(search, SIGNAL(searchFinished()), this, SLOT(onSearchResultCallback()));
    connect(searchThread, SIGNAL(started()), search, SLOT(thread_wait()));
    searchThread->start();

    fetchThread = new QThread;
    word = new MOJiDictWord(pyInfo);
    word->moveToThread(fetchThread);
    connect(this, SIGNAL(startFetch(QString)), word, SLOT(fetch(QString)));
    connect(fetchThread, SIGNAL(started()), word, SLOT(thread_wait()));
    fetchThread->start();


    animation = new QPropertyAnimation;
    animation->setTargetObject(this);
    animation->setPropertyName("geometry");
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::InOutExpo);
}


void SearchBox::onTextChange()
{
    if (delay->isActive())
    {
        delay->stop();
    }
    delay->start();
    qDebug() << textBox->text() << endl;
}

void SearchBox::clearCandidates()
{
    for (auto itr: candidates)
    {
        disconnect(itr, SIGNAL(clickItem(QString)), this, SLOT(onItemSelect(QString)));
        delete itr;
    }
    candidates.clear();
}

void SearchBox::addCandidate(QString title, QString excerpt)
{
    SearchItem* i = new SearchItem(this, candidates.size());
    i->setText("<b>" + title + "</b><br/>" + excerpt.left(30) + "...");
    i->setGeometry(0, 40 + i->getId() * 50, width(), 50);
    connect(i, SIGNAL(clickItem(QString)), this, SLOT(onItemSelect(QString)));
    candidates.push_back(i);
}

void SearchBox::onTimerTick()
{
    delay->stop();
    emit startSearch(textBox->text());
    qDebug() << "Emitted signal." << endl;
}

void SearchBox::onSearchResultCallback()
{
    qDebug() << "Returned." << endl;
    clearCandidates();
    for (auto it : search->word_excerpts) {
        addCandidate(QString::fromStdString(it.spell), QString::fromStdString(it.excerpt));
        qDebug() << QString::fromStdString(it.spell) << " " << QString::fromStdString(it.excerpt) << endl;
    }
    expand();
    for (auto cand: candidates)
    {
        cand->show();
    }
}

void SearchBox::onItemSelect(QString id)
{
    emit collapse();
    emit startFetch(QString::fromStdString(search->word_ids[id.toInt()]));
}

void SearchBox::paintEvent(QPaintEvent *e)
{
    textBox->setGeometry(0, 0, width(), 40);
    QWidget::paintEvent(e);
}

void SearchBox::collapse()
{
    collapsed = true;
    animation->stop();
    animation->setEndValue(QRect(x(), y(), width(), 40));
    animation->setStartValue(QRect(x(), y(), width(), height()));
    animation->start();
}

void SearchBox::expand()
{
    collapsed = false;
    animation->stop();
    animation->setEndValue(QRect(x(), y(), width(), 50 * candidates.size() + 40));
    animation->setStartValue(QRect(x(), y(), width(), height()));
    animation->start();
}

SearchBox::~SearchBox()
{
    searchThread->quit();
    searchThread->wait();
}
