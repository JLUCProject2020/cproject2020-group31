#include "dict.h"

using std::string;
using std::vector;
using json = nlohmann::json;

DictIndex::DictIndex()
{
    head = new TNode;
    head->letter = 0;
    head->parent = nullptr;
    head->has_target = false;
    head->target_page = "";
    for (auto& i: head->children)
    {
        i = nullptr;
    }
}

bool DictIndex::__parse(string &str) //把大写字母转换小写，删除字母以外的任何字符
{
    string temp_str;
    for (auto i = str.begin(); i != str.end(); i++)
    {
        if (*i >= 'A' && *i <= 'Z')
        {
            temp_str.push_back(*i - 'A' + 'a');
        }
        else if (*i >= 'a' && *i <= 'z')
        {
            temp_str.push_back(*i);
        }
        else
            return 1;
    }
    str = temp_str;
    // std::cout << temp_str << std::endl;
    return 0;
}

void DictIndex::append(string key, string value) //创建字典索引路径
{
    int bit_ind = 0;
    if (__parse(key)) return;
    TNode *cur_node = head;

    while (bit_ind < (int)key.size())
    {
        TNode *&child = cur_node->children[key[bit_ind] - 'a'];
        if (child == nullptr)
        {
            child = new TNode;
            child->letter = key[bit_ind];
            child->parent = cur_node;
            child->has_target = false;
            child->target_page = "";
            for (auto& i: child->children)
            {
                i = nullptr;
            }
        }
        bit_ind++;
        cur_node = child;
    }
    cur_node->has_target = true;
    cur_node->target_page = value;
}

bool DictIndex::__seek_first(TNode *&node)
{
    TNode *cur_node = node;
node_enter:
    if (cur_node->has_target)
    {
        node = cur_node;
        return 0;
    }
    for (int i = 0; i < 26; i++)
    {
        if (cur_node->children[i] != nullptr)
        {
            cur_node = cur_node->children[i];
            goto node_enter;
        }
    }
    //26个子entry都为空
    return 1;
}

bool DictIndex::__seek_next(TNode *&node, TNode *root)
{
    char c = 'a' - 1;
    TNode *cur_node = node;
node_enter:
    if (cur_node->has_target && cur_node != node)
    {
        node = cur_node;
        return 0;
    }
back:
    for (int i = c - 'a' + 1; i < 26; i++)
    {
        if (cur_node->children[i] != nullptr)
        {
            cur_node = cur_node->children[i];
            c = 'a' - 1;
            goto node_enter;
        }
    }
    if (cur_node != root)
    {
        c = cur_node->letter;
        cur_node = cur_node->parent;
        goto back;
    }
    else
    {
        return 1;
    }
}

string DictIndex::__get_path(TNode *node)
{
    string ret_val;
    TNode *cur_node = node;
    while (true)
    {
        ret_val.push_back(cur_node->letter);
        if (cur_node->parent != head)
        {
            cur_node = cur_node->parent;
        }
        else
        {
            break;
        }
    }
    std::reverse(ret_val.begin(), ret_val.end());
    return ret_val;
}

vector<kv> DictIndex::search(string prefix)
{
    __parse(prefix);
    vector<kv> ret_val;
    int bit_ind = 0;
    TNode *cur_node = head;
    while (bit_ind < prefix.size())
    {
        TNode *&child = cur_node->children[prefix[bit_ind] - 'a'];
        if (child == nullptr)
        {
            return ret_val;
        }
        bit_ind++;
        cur_node = child;
    }
    //开始遍历所有末端节点
    TNode *root = cur_node;
    if (__seek_first(cur_node))
        return ret_val;
    kv p;
    p.key = __get_path(cur_node);
    p.value = cur_node->target_page;
    ret_val.push_back(p);
    while (!__seek_next(cur_node, root))
    {
        p.key = __get_path(cur_node);
        p.value = cur_node->target_page;
        ret_val.push_back(p);
    }
    return ret_val;
}

void DictIndex::import_from_json(string path)
{
    using std::ifstream;
    ifstream j;
    j.open(path);
    if (!j.is_open())
    {
        std::cout << path << " not found!" << std::endl;
        return;
    }
    else
    {
        std::cout << "Loaded " << path << " successfully!" << std::endl;
    }
    char *json_str;
    int file_len;
    j.seekg(0, std::ios_base::end);
    file_len = j.tellg();
    j.seekg(0);
    json_str = new char[file_len];
    j.read(json_str, file_len);
    auto hash_dict = json::parse(json_str);
    int item_count = 0;
    for (auto item = hash_dict.begin(); item != hash_dict.end(); item++)
    {
        append(item.key(), item.value());
        item_count++;
        std::cout << "Added " << item_count << " items to index.\r";
    }
    std::cout << std::endl;
}

void DictIndex::__recursive_del_nodes(TNode *s_node)
{
    TNode* cur = s_node;
    if (__seek_first(cur))
    {
        delete cur;
        return;
    }
    else
    {
        while (true)
        {
            TNode* tmp = cur;
            if (__seek_next(cur, s_node))
            {
                delete cur;
                return;
            }
            else
            {
                delete tmp;
            }
        }
    }
}

DictIndex::~DictIndex()
{
    __recursive_del_nodes(head);
}

//int main()
//{
//    DictIndex d;
//    d.import_from_json("index.json");
//    vector<kv> res = d.search("app");
//    for (auto i: res)
//    {
//        std::cout << i.key << '\t' << i.value << std::endl;
//    }
//}
