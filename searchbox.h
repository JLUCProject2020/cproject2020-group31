#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QWidget>
#include "textbox.h"
#include "searchitem.h"
#include <QVector>
#include "pyonlinesearchapi.h"
#include <QTimer>
#include <QThread>
#include <string>


extern MOJiDictBasicInfo* pyInfo;
extern PyObject* pyModuleApi;
class SearchBox : public QWidget
{
    Q_OBJECT
    TextBox* textBox;
    QVector<SearchItem*> candidates;
    bool collapsed;
    QTimer* delay;
    QThread* searchThread;
    QThread* fetchThread;
    QPropertyAnimation* animation;
public:
    MOJiDictSearch* search;
    MOJiDictWord* word;
    explicit SearchBox(QWidget *parent = nullptr);
    void clearCandidates();
    void addCandidate(QString title, QString excerpt);
    ~SearchBox();
    void paintEvent(QPaintEvent* e) override;
signals:
    void startSearch(QString content);
    void startFetch(QString wordid);
private slots:
    void onTextChange();
    void onItemSelect(QString id);
    void onSearchResultCallback();
    void onTimerTick();
    void collapse();
    void expand();
};

#endif // SEARCHBOX_H
