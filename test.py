from readmdict import MDX, MDD
import struct
import os, sys
import base64,json

def file_error():
        print("\033[31mError: Target MDict file not found!\033[0m\nUsage:\n\033[35mpython \033[33mmdict_extractor.py \033[36m[mdd file]\033[0m\nor\n\033[35mpython \033[33mmdict_extractor.py \033[36m[mdx file]\033[0m")
        os.system('pause')
        exit(1)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        file_error()
    if not os.path.exists("pages"):
        os.mkdir("pages")
    if sys.argv[1].split('.')[-1] == "mdd":
        mdd_file = MDD(sys.argv[1])
        print("正在提取媒体文件...")
        for mdd_item in mdd_file.items():
            path = mdd_item[0].decode()[1:]
            data = mdd_item[1]
            if os.path.exists(os.path.join("pages", path)):
                print(f"[x] {path} 已存在！")
                continue
            else:
                print(f"[ ] {path}", end='\r')
                path_tuple = path.split('\\')
                path_str = str(os.path.join("pages", *path_tuple[:-1])) if len(path_tuple) > 1 else ''
                if path_str != '' and not os.path.exists(path_str):
                    os.mkdir(path_str)
                src_file = open(os.path.join("pages", path), 'wb')
                src_file.write(data)
                src_file.flush()
                src_file.close()
                print("[v]")
    elif sys.argv[1].split('.')[-1] == "mdx":
        mdx_file = MDX(sys.argv[1], encoding='utf-8')
        index_info = {}
        print("正在提取词典页面文件...")
        for mdx_item in mdx_file.items():
            item_name = base64.urlsafe_b64encode(mdx_item[0]).decode() + '.html'
            item_content = mdx_item[1].decode()
            page_path = os.path.join("pages", item_name)
            if os.path.exists(page_path):
                print(f"[x] {page_path} 已存在！")
            else:
                print(f"[ ] {page_path}", end='\r')
                if "@@@LINK=" in item_content:
                    link = item_content[8:]
                    item_content = "<a href=\"{}\">{}</a>".format(base64.urlsafe_b64encode(link.encode()).decode(), link)
                else:
                    index_info.update({mdx_item[0].decode():item_name})
                    page_file = open(page_path, 'w', encoding='utf-8')
                    page_file.write(item_content)
                    page_file.flush()
                    page_file.close()
                print("[v]")
        print("正在创建索引...")
        index_json = open(os.path.join("pages", "index.json"), 'w', encoding='utf-8')
        json.dump(index_info, index_json)
        print("成功创建索引")
    else:
        file_error()

# mdx_file = MDX("test2.mdx", encoding='utf-8')
# print(mdx_file.header)
# for item in mdx_file.items():
#     print(item[0].decode('utf-8') + '\t\t\t\t' + item[1].decode('utf-8'))
