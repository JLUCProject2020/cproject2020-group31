#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "pyonlinesearchapi.h"
#include <QFontDatabase>

QApplication* appMain;
MainWindow* wndMain;
PyObject* pyModuleApi;
PyObject* pyFuncSearchWord;
PyObject* pyFuncFetchWord;
MOJiDictBasicInfo* pyInfo;

int main(int argc, char *argv[])
{
    //Initialize Python Environment
//    Py_SetPythonHome(L"C:\\ProgramData\\Anaconda3");
    Py_Initialize();
    qDebug() << "Current PATH varriant: " <<QString::fromWCharArray(Py_GetPath()) << endl;
    pyModuleApi = PyImport_ImportModule("pyonlinesearchapi");
    Py_XINCREF(pyModuleApi);
    if (pyModuleApi == nullptr)
    {
        qDebug() << "Error occurred while initializing Python modules." << endl;
        return -1;
    }
    pyInfo = new MOJiDictBasicInfo(
            "https://api.mojidict.com/parse/functions/search_v3",
            "https://api.mojidict.com/parse/functions/fetchWord_v2",
            "E62VyFVLMiW7kvbtVq3p",
            "5562c88b-b67a-c285-b9d1-a8360121380a",
            "js2.12.0",
            "r:36b53b94af1a089da1656200530c36a6"
        );

    pyFuncSearchWord = PyObject_GetAttrString(pyModuleApi, "search_word");
    Py_XINCREF(pyFuncSearchWord);
    pyFuncFetchWord = PyObject_GetAttrString(pyModuleApi, "fetch_word");
    Py_XINCREF(pyFuncFetchWord);

    //Initialize Qt Application
    appMain = new QApplication(argc, argv);
    QFontDatabase::addApplicationFont("fonts/SourceHanSerifSC-Regular.otf");
    QFontDatabase::addApplicationFont("fonts/SourceHanSansCN-Light.otf");
    wndMain = new MainWindow;
    wndMain->show();
    int appRetVal = appMain->exec();


    Py_Finalize();
    return appRetVal;
}
