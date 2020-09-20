#include <iostream>
#include "DebugAssertion.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "naming/beat/BeatInfo.h"
#include "json/JSON.h"

using namespace std;
using namespace rapidjson;

bool testRapidJsonIntroduce() {
    // 1. 把 JSON 解析至 DOM。
    const char *json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json);
    // 2. 利用 DOM 作出修改。
    Value &s = d["stars"];
    s.SetInt(s.GetInt() + 1);
    // 3. 把 DOM 转换（stringify）成 JSON。
    StringBuffer buffer;
    Writer <StringBuffer> writer(buffer);
    d.Accept(writer);
    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    Document parsedAgain;
    parsedAgain.Parse(buffer.GetString());
    Value &s2 = parsedAgain["stars"];
    int expectedtoBe11 = s2.GetInt();
    SHOULD_BE_TRUE(expectedtoBe11 == 11, "There should be 11 stars");

    return true;
}

bool testSerialize() {
    BeatInfo bi;
    bi.port = 10;
    cout << JSON::toJSONString(bi);
    return true;
}