# mojidict-qt5

[MerlynAllen](https://github.com/MerlynAllen)

## 介绍

这是一个由Qt5和Python开发的辞典程序。

- 目前支持日语和英语的查询。日语部分采用调用[MOJi辞書](https://mojidict.com/)在线API的方式，网络部分调用由Python脚本负责，也方便后期导入其他在线查询模块。离线词典部分采用MDict词典格式`*.mdx, *.mdd`。目前采用了先提取出词典文件中解释和资源文件，使用时直接访问相应界面的逻辑。C++的MDict支持还没有做。

- 运行环境 Windows x64

- 编译环境 Windows 10 Build 19041; msvc 2019 16.5.30002.166(amd64).

- Python环境 Python-3.7.6-embed-amd64

- 借助第三方开源项目

  [nlohmann/json](https://github.com/nlohmann/json) C++ json库

  [csarron/mdict-analysis](https://github.com/csarron/mdict-analysis) Python MDict解析

## 关于MDict

MDict文件格式的分析见https://bitbucket.org/xwang/mdict-analysis。

## 架构设计

本来是想做成可拓展性非常强的，最后由于GUI没有提前设计好接口而导致界面上相互的逻辑一塌糊涂，拓展的话只能硬头皮写。因为是大作业，时间不太够，C++对MDict的直接读取也没有做到，而是选择了一种比较蠢的（但是足够有效）直接利用现成的Python项目直接提取资源的方式。

## 安装运行

release中提供了zip、二进制安装包。如提示缺少dll文件请下载[Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://www.visualstudio.com/downloads/)。

下载示例词典文件[sample.mdd]() [sample.mdx]()于安装目录内。

`Win + R` 打开 `cmd`

```bash
cd [installation path]
python mdict_extractor.py sample.mdd
python mdict_extractor.py sample.mdx
```

安装目录内会自动建立dict目录，里面就是提取的资源文件。

## 编译

- 修改`MOJiDict.pro` 改为本地Python环境相应路径

  ```
  win32:CONFIG(release, debug|release): LIBS += -LC:/ProgramData/Anaconda3/libs/ -lpython37
  else:win32:CONFIG(debug, debug|release): LIBS += -LC:/ProgramData/Anaconda3/libs/ -lpython37_d
  INCLUDEPATH += C:/ProgramData/Anaconda3/include
  DEPENDPATH += C:/ProgramData/Anaconda3/include
  ```

- Qt默认的工作目录不是exe所在目录`\${build_directory}\release\` 或 `\${build_directory}\debug\`而是 `\${build_directory}\`

- 一个可以独立运行的build目录配置如下(以release为例)

  ```
  -> release
     - fonts
     - plugins
     - resourses
     - translations
     - MOJiDict.exe
     - pyonlinesearchapi.py
     - [extracted files from python-3.7.6-embed-amd64.zip]
     - python37.zip*
     - [*.dll]
     - [*.exe]
  -> styles
     - style.css
  -> dict
     - [extracted files from *.mdd and *.mdx]
  -> mdict_extractor.py
  -> readmdict.py
  -> ripemd128.py
  -> pureSalsa20.py
  -> settings.json
  ```

  从release压缩包中可获取缺失的文件。

  编译器使用msvc。

  *注：`python37.zip`中已经安装好requests库。

## 未实现

- [ ] 日语离线字典的搜索
- [ ] 单词收藏
- [ ] 单词笔记

