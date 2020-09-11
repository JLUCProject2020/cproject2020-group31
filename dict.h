#ifndef DICT_H
#define DICT_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "json.hpp"

using std::string;
using std::vector;
using json = nlohmann::json;

struct TNode
{
    struct TNode *parent;
    struct TNode *children[26];
    char letter;
    bool has_target;
    string target_page;
};

struct kv
{
    string key;
    string value;
};

//实现了一个Trie树结构作为字典索引
class DictIndex
{
    TNode *head;
    unsigned long __index_count;

    void __recursive_del_nodes(TNode *s_node);
    bool __parse(string &str);
    string __get_path(TNode *node);              //返回某节点的路径
    bool __seek_first(TNode *&node);             //检索从node开始的所有分支的最优先分支对应末端节点
    bool __seek_next(TNode *&node, TNode *root); //寻找下一个分支
public:
    DictIndex();
    void append(string key, string value);
    vector<kv> search(string prefix);
    void import_from_json(string path);
    ~DictIndex();
};

#endif // DICT_H
