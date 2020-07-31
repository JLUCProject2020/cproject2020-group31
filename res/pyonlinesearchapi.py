import json as json
import requests as requests
def search_word(searchurl, postdata, content):
    assert(content)
    postdata.update({"searchText": content})
    repl_json = requests.post(
        searchurl, data=postdata).text
    repl_dict = json.loads(repl_json)["result"]
    filteredrst_id = []
    word_abstr = []

    for i in range(len(repl_dict["words"])):
        filteredrst_id.append(repl_dict["words"][i]["objectId"])
        word_abstr.append({
            "id": repl_dict["words"][i]["objectId"],
            "excerpt": repl_dict["words"][i]["excerpt"],
            "spell": repl_dict["words"][i]["spell"],
            "tone": repl_dict["words"][i]["accent"],
            "pron": repl_dict["words"][i]["pron"]
        })
    return (filteredrst_id, word_abstr)


def fetch_word(fetchurl, postdata, wordid):
    def to_html(dictinfo):
        title, pron, tone, term_info = dictinfo
        try:
            cssfile  = open("style.css", encoding='utf-8')
            css_text = cssfile.read()
            cssfile.close()
        except:
            css_text = ""
        def get_examples(word_class_index, expl_index):
            template4 = '<div class="sample"><span>{0}</span><span><img class="read_icon" src="sound.svg"></img></span></div><div class="trans">{1}</div>'
            ret_str = ""
            if "examples" in term_info[word_class_index]["explanations"][expl_index].keys():
                for item in term_info[word_class_index]["explanations"][expl_index]["examples"]:
                    ret_str += template4.format(item["example"], item["translation"])
            return ret_str

        def get_expl(word_class_index):
            template3 = '<li><div class="explanation">{0}{1}</div></li>'
            ret_template = ""
            for i in range(len(term_info[word_class_index]["explanations"])):
                ret_template += template3.format(
                    term_info[word_class_index]["explanations"][i]["explanation"],
                    get_examples(word_class_index, i))
            return ret_template

        def get_word_classes():
            template2 = '<div class="word_class"><div class="wd_class_ind">{0}</div><ul>{1}</ul></div>'
            ret_template = ""
            for i, item in enumerate(term_info):
                ret_template += template2.format(
                    item["word-class"], get_expl(i))
            return ret_template

        template0 = '<html><head><style>{0}</style></head><body style="overflow-x: hidden; overflow-y: auto;">{1}{2}</body></html>'
        template1 = '<div id="title"><ruby class="sample1">{0}<rt>{1}</rt></ruby><span id="tone">{2}</span></div><hr />'
        return template0.format(css_text, template1.format(title, pron, tone), get_word_classes())

    postdata.update({"wordId": wordid})
    repl_json = requests.post(
        fetchurl, data=postdata).text
    repl_dict = json.loads(repl_json)["result"]
    term_info = []
    for wrd_cls in repl_dict["details"]:
        expl_list = []
        wrd_cls_id = wrd_cls["objectId"]
        for expl in repl_dict["subdetails"]:
            if expl["detailsId"] == wrd_cls_id:
                expl_exmp_list = []
                expl_id = expl["objectId"]
                if "examples" in repl_dict.keys():
                    for exmp in repl_dict["examples"]:
                        if exmp["subdetailsId"] == expl_id:
                            expl_exmp_list.append({
                                "example": exmp["title"],
                                "translation": exmp["trans"]
                            })
                expl_list.append({
                    "explanation": expl["title"],
                    "examples": expl_exmp_list
                })
        term_info.append({
            "word-class": wrd_cls["title"],
            "explanations": expl_list
        })
    title = repl_dict["word"]["spell"]
    pron = repl_dict["word"]["pron"]
    tone = repl_dict["word"]["accent"]
    html = to_html((title, pron, tone, term_info))
    return (title, pron, tone, term_info, html)

def test():
    import os
    return os.getcwd()

#///////////////////testing//////////////////////#
if __name__ == "__main__":
    # pdata = {
    #     "needWords": True,
    #     "langEnv": "zh-CN_ja",
    #     "_ApplicationId": "E62VyFVLMiW7kvbtVq3p",
    #     "_InstallationId": "5562c88b-b67a-c285-b9d1-a8360121380a",
    #     "_ClientVersion": "js2.12.0",
    #     "_SessionToken": "r:36b53b94af1a089da1656200530c36a6"
    # }
    pdata = {
        "_ApplicationId": "E62VyFVLMiW7kvbtVq3p",
        "_InstallationId": "5562c88b-b67a-c285-b9d1-a8360121380a",
        "_ClientVersion": "js2.12.0",
        "_SessionToken": "r:36b53b94af1a089da1656200530c36a6"
    }
    url = "https://api.mojidict.com/parse/functions/fetchWord_v2"
    print(fetch_word(url, pdata, "1989110285"))