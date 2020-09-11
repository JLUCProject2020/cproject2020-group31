#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "pyonlinesearchapi.h"
#include <QFontDatabase>
#include <fstream>
#include "dict.h"

using std::ifstream;

QApplication* appMain;
MainWindow* wndMain;
PyObject* pyModuleApi;
PyObject* pyFuncSearchWord;
PyObject* pyFuncFetchWord;
MOJiDictBasicInfo* pyInfo;
DictIndex* dict;

unsigned int MODE = 0;

int main(int argc, char *argv[])
{
    //Initialize Python Environment
    //Py_SetPythonHome(L"C:\\ProgramData\\Anaconda3"); //For debugging
    Py_Initialize();
    qDebug() << "Current PATH varriant: " <<QString::fromWCharArray(Py_GetPath()) << endl;

    //Python import module
    pyModuleApi = PyImport_ImportModule("pyonlinesearchapi");
    Py_XINCREF(pyModuleApi);
    if (pyModuleApi == nullptr)
    {
        qDebug() << "Error occurred while initializing Python modules." << endl;
        return -1;
    }

    //从settings.json读取配置
    ifstream glb_settings_file("settings.json", std::ios_base::in);
    if (!glb_settings_file.is_open())
    {
        std::cout << "\"settings.json\" not found!" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Loaded \"settings.json\" successfully!" << std::endl;
    }
    char* glb_settings_json;
    glb_settings_file.seekg(0, std::ios_base::end);
    int filelen = glb_settings_file.tellg();
    glb_settings_file.seekg(0);
    glb_settings_json = new char[filelen];
    glb_settings_file.read(glb_settings_json, filelen);
    glb_settings_json[filelen] = '\0';
    std::cout << glb_settings_json << std::endl;
    auto settings = json::parse(glb_settings_json);
    string webSearchApi = settings["webSearchApi"];
    string webFetchApi = settings["webFetchApi"];
    string appId = settings["appId"];
    string installationId = settings["installationId"];
    string clientVersion = settings["clientVersion"];
    string sessionToken = settings["sessionToken"];

    //Initialize global settings
    pyInfo = new MOJiDictBasicInfo(
            webSearchApi.c_str(),
            webFetchApi.c_str(),
            appId.c_str(),
            installationId.c_str(),
            clientVersion.c_str(),
            sessionToken.c_str()
        );

    //Get the pointer of Python function
    pyFuncSearchWord = PyObject_GetAttrString(pyModuleApi, "search_word");
    Py_XINCREF(pyFuncSearchWord);
    pyFuncFetchWord = PyObject_GetAttrString(pyModuleApi, "fetch_word");
    Py_XINCREF(pyFuncFetchWord);

    //加载离线字典
    dict = new DictIndex;
    dict->import_from_json("dict/index.json");

    //Qt环境初始化
    appMain = new QApplication(argc, argv);
    QFontDatabase::addApplicationFont("fonts/SourceHanSerifSC-Regular.otf");
    QFontDatabase::addApplicationFont("fonts/SourceHanSansCN-Light.otf");
    wndMain = new MainWindow;
    wndMain->show();
    int appRetVal = appMain->exec();

    //清理Python环境
    Py_Finalize();
    return appRetVal;
}
