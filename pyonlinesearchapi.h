#include <Python.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <QObject>


typedef struct {
    std::string id;
    std::string excerpt;
    std::string spell;
    std::string pron;
    std::string tone;
} WordSearchData;


class MOJiDictBasicInfo
{
public:
    std::string* search_api;
    std::string* word_fetch_api;
    std::string* appid;
    std::string* installationid;
    std::string* clientver;
    std::string* sessiontoken;
    PyObject* _dict_fetch;
    PyObject* _dict_search;
    MOJiDictBasicInfo::MOJiDictBasicInfo(
        const char* s_api,
        const char* wf_api,
        const char* a_id,
        const char* i_id,
        const char* c_ver,
        const char* s_token
    );

    PyObject* to_search_post_data();
    PyObject* to_fetch_post_data();
    ~MOJiDictBasicInfo();
};

class MOJiDictSearch: public QObject
{
    Q_OBJECT
    MOJiDictBasicInfo* info;

public:
    std::vector<std::string> word_ids;
    std::vector<WordSearchData> word_excerpts;
    MOJiDictSearch(MOJiDictBasicInfo* pinfo);
public slots:
    void search(QString content);
    void thread_wait();
signals:
    void searchFinished();
};

class MOJiDictWord: public QObject
{
    Q_OBJECT
    MOJiDictBasicInfo* info;
public:
    std::string id;
    std::string rendered_html;
    MOJiDictWord(MOJiDictBasicInfo* pinfo);
public slots:
    void fetch(QString wordid);
    void thread_wait();
signals:
    void fetchFinished();
};
