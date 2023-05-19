#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

// json序列化实例1
void func1() {
    json js;
    // json 有点像 map表 键值对 key-value 是无序的映射容易 链式哈希表
    js["msg_type"] = 2;
    js["from"] = "Zhang San";
    js["to"] = "Li Si";
    js["msg"] = "Hello, what are you doing now?";

    string sendBuf = js.dump(); // json数据对象 -> json字符串 （序列化）
    // cout << js << endl;
    cout << sendBuf.c_str() << endl;
}

// json序列化实例2
void func2() {
    json js;
    // 添加 数组
    js["id1"] = {1, 2, 3, 4, 5};
    js["id2"] = {1, 2};
    // 添加 key-value
    js["name1"] = "Zhang San";
    js["name2"] = {"Zhang San", "Li Si"};
    // 添加 对象
    js["msg1"]["Zhang San"] = "Hello World";
    js["msg1"]["Li Si"] = "Hello China";
    js["msg3"]["Zhang San"]["Hello"] = "World";
    js["msg3"]["Li Si"]["Hello"] = "China";
    // 上面两句 等同于 下面一句 一次性添加 数组对象
    js["msg2"] = {{"Zhang San", "Hello World"}, {"Li Si", "Hello China"}, {"Zhao Wu", "Hello Zhejiang"}};
    js["msg4"] = {{"Zhang San", "Hello", "World"}, {"Li Si", "Hello", "China"}};
    js["msg5"] = {{"Zhang San", {"Hello", "World"}}, {"Li Si", {"Hello", "China"}}};

    cout << js << endl;
}

// json序列化实例3 可以序列化 容器
void func3() {
    json js;

    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;

    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;

    string sendBuf = js.dump(); // json数据对象 -> json字符串 （序列化）
    // cout << js << endl;
    cout << sendBuf.c_str() << endl;
}


string func4() {
    json js;
    // json 有点像 map表 键值对 key-value 是无序的映射容易 链式哈希表
    js["msg_type"] = 2;
    js["from"] = "Zhang San";
    js["to"] = "Li Si";
    js["msg"] = "Hello, what are you doing now?";

    string sendBuf = js.dump();
    // cout << js << endl;
    // cout << sendBuf.c_str() << endl;
    return sendBuf;
}

string func5() {
    json js;
    // 添加 数组
    js["id"] = {1, 2, 3, 4, 5};
    // 添加 key-value
    js["name"] = "Zhang San";
    // 添加 对象
    js["msg1"]["Zhang San"] = "Hello World";
    js["msg1"]["Li Si"] = "Hello China";
    // 上面两句 等同于 下面一句 一次性添加 数组对象
    js["msg2"] = {{"Zhang San", "Hello World"}, {"Li Si", "Hello China"}};

    string sendBuf = js.dump();
    return sendBuf;
}

string func6() {
    json js;

    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;

    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;

    string sendBuf = js.dump(); // json数据对象 -> json字符串 （序列化）
    // cout << js << endl;
    // cout << sendBuf.c_str() << endl;
    return sendBuf;
}

int main() {
    // func1();
    // func2();
    // func3();

    string recvBuf1 = func4();
    cout << recvBuf1 << endl;
    // 数据的反序列化  json字符串 -> json数据对象 （反序列化） （看作容器 方便访问）
    json jsbuf1 = json::parse(recvBuf1);
    cout << jsbuf1 << endl;
    cout << jsbuf1["msg_type"] << endl;
    cout << jsbuf1["from"] << endl;
    cout << jsbuf1["to"] << endl;
    cout << jsbuf1["msg"] << endl;

    string recvBuf2 = func5();
    cout << recvBuf2 << endl;
    json jsbuf2 = json::parse(recvBuf2);
    cout << jsbuf2 << endl;
    cout << jsbuf2["id"] << endl;
    auto arr = jsbuf2["id"];
    cout << arr[2] << endl;
    cout << jsbuf2["name"] << endl;
    cout << jsbuf2["msg1"] << endl;
    cout << jsbuf2["msg2"] << endl;
    auto msgjs = jsbuf2["msg1"];
    cout << msgjs["Zhang San"] << endl;
    cout << msgjs["Li Si"] << endl;

    string recvBuf3 = func6();
    cout << recvBuf3 << endl;
    json jsbuf3 = json::parse(recvBuf3);
    cout << jsbuf3 << endl;
    cout << jsbuf3["list"] << endl;
    cout << jsbuf3["path"] << endl;

    // json对象里面的数组容器 可以直接放入vector容器中
    vector<int> vec = jsbuf3["list"];
    for (int& v : vec) {
        cout << v << " ";
    }
    cout << endl;

    map<int, string> myMap = jsbuf3["path"];
    for (auto& m : myMap) {
        cout << m.first << " " << m.second << endl;
    }
    cout << endl;

    return 0;
}