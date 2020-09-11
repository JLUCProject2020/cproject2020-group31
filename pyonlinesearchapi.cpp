#include "pyonlinesearchapi.h"
#include <iostream>
#include <QDebug>
#include <QCoreApplication>

// using namespace Py;
using namespace std;
extern PyObject* pyModuleApi;
extern PyObject* pyFuncSearchWord;
extern PyObject* pyFuncFetchWord;


MOJiDictBasicInfo::MOJiDictBasicInfo(
    const char* s_api,
    const char* wf_api,
    const char* a_id,
    const char* i_id,
    const char* c_ver,
    const char* s_token
)
{
    search_api = new string(s_api);
    word_fetch_api = new string(wf_api);
    appid = new string(a_id);
    installationid = new string(i_id);
    clientver = new string(c_ver);
    if (s_token == nullptr)
    {
        sessiontoken = nullptr;
    }
    else
    {
        sessiontoken = new string(s_token);
    }

    _dict_search = PyDict_New();
    _dict_fetch = PyDict_New();

    PyDict_SetItemString(_dict_search, "needWords", PyBool_FromLong(1));
    PyDict_SetItemString(_dict_search, "langEnv", PyUnicode_FromString("zh-CN_ja"));
    PyDict_SetItemString(_dict_search, "_ApplicationId", PyUnicode_FromString(appid->c_str()));
    PyDict_SetItemString(_dict_search, "_InstallationId", PyUnicode_FromString(installationid->c_str()));
    PyDict_SetItemString(_dict_search, "_ClientVersion", PyUnicode_FromString(clientver->c_str()));
    if (sessiontoken != nullptr)
    {
        PyDict_SetItemString(_dict_search, "_SessionToken", PyUnicode_FromString(sessiontoken->c_str()));
    }

    PyDict_SetItemString(_dict_fetch, "_ApplicationId", PyUnicode_FromString(appid->c_str()));
    PyDict_SetItemString(_dict_fetch, "_InstallationId", PyUnicode_FromString(installationid->c_str()));
    PyDict_SetItemString(_dict_fetch, "_ClientVersion", PyUnicode_FromString(clientver->c_str()));
    if (sessiontoken != nullptr)
    {
        PyDict_SetItemString(_dict_fetch, "_SessionToken", PyUnicode_FromString(sessiontoken->c_str()));
    }
}

PyObject* MOJiDictBasicInfo::to_search_post_data()
{
    return _dict_search;
}

PyObject* MOJiDictBasicInfo::to_fetch_post_data()
{
    return _dict_fetch;
}

MOJiDictBasicInfo::~MOJiDictBasicInfo()
{
    delete search_api;
    delete word_fetch_api;
    delete appid;
    delete installationid;
    delete clientver;
    delete sessiontoken;
    Py_XDECREF(_dict_fetch);
    Py_XDECREF(_dict_search);
}

MOJiDictSearch::MOJiDictSearch(MOJiDictBasicInfo* pinfo)
{
    info = pinfo;
}

void MOJiDictSearch::search(QString content)
{
    qDebug() << "Received signal." << endl;
    word_ids.clear();
    word_excerpts.clear();
    if (pyFuncSearchWord == NULL)
    {
        qDebug() << "Error occurred when parsing Python function." << endl;
        exit(-1);
    }

    PyObject* pyTupleReturn = PyObject_CallFunction(pyFuncSearchWord, "sOs", info->search_api->c_str(), info->to_search_post_data(), content.toStdString().c_str());
    if (pyTupleReturn == NULL)
    {
        qDebug() << "Error occurred when executing python module." << endl;
        exit(-1);
    };

    PyObject* pyListWords = PyTuple_GetItem(pyTupleReturn, 0);
    Py_XINCREF(pyListWords);
    PyObject* pyListDictWordsExcerpt = PyTuple_GetItem(pyTupleReturn, 1);
    Py_XINCREF(pyListDictWordsExcerpt);
    PyObject* pyDictWordsExcerpt;
    string id, excerpt, spell, tone, pron;
    for (int i = 0; i < PyList_Size(pyListWords); i++)
    {
        word_ids.push_back(string(PyUnicode_AsUTF8(PyList_GetItem(pyListWords, i))));
        pyDictWordsExcerpt = PyList_GetItem(pyListDictWordsExcerpt, i);
        Py_XINCREF(pyDictWordsExcerpt);
        id = PyUnicode_AsUTF8(PyDict_GetItemString(pyDictWordsExcerpt, "id"));
        excerpt = PyUnicode_AsUTF8(PyDict_GetItemString(pyDictWordsExcerpt, "excerpt"));
        spell = PyUnicode_AsUTF8(PyDict_GetItemString(pyDictWordsExcerpt, "spell"));
        tone = PyUnicode_AsUTF8(PyDict_GetItemString(pyDictWordsExcerpt, "tone"));
        pron = PyUnicode_AsUTF8(PyDict_GetItemString(pyDictWordsExcerpt, "pron"));
        WordSearchData data;
        data.id = id;
        data.pron = pron;
        data.spell = spell;
        data.tone = tone;
        data.excerpt = excerpt;
        word_excerpts.push_back(data);
    }
    emit searchFinished();
    qDebug() << "Search finished." << endl;
    Py_XDECREF(pyTupleReturn);
    Py_XDECREF(pyDictWordsExcerpt);
    Py_XDECREF(pyListDictWordsExcerpt);
    Py_XDECREF(pyListWords);
}

void MOJiDictSearch::thread_wait()  //线程常驻
{
    while(1)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}


MOJiDictWord::MOJiDictWord(MOJiDictBasicInfo* pinfo)
{
    info = pinfo;
}

void MOJiDictWord::fetch(QString wordid)
{
    qDebug() << "Received signal." << endl;
    id = wordid.toStdString();
    qDebug() << wordid << endl;
    qDebug() << pyFuncFetchWord << endl;
    PyObject* pyTupleReturn = PyObject_CallFunction(pyFuncFetchWord, "sOs", info->word_fetch_api->c_str(), info->to_fetch_post_data(), id.c_str());
    if (pyTupleReturn == NULL)
    {
        cout << "Error occurred when excuting python module." << endl;
        exit(-1);
    }

    //这里应该还有处理其他返回内容的代码，暂时忽略

    rendered_html = PyUnicode_AsUTF8(PyTuple_GetItem(pyTupleReturn, 4));
    emit fetchFinished();
    qDebug() << "Fetch finished." << endl;
    Py_XDECREF(pyTupleReturn);
}

void MOJiDictWord::thread_wait()  //线程常驻
{
    while(1)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

