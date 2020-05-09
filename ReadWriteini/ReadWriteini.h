
/*
 * @Description: 用于读取ini配置文件
 * @version:0.0.1
 * @Author: windSnowLi
 * @Date:2020/5/7
 * @LastEditors: windSnowLi
 * @LastEditTime:2020/5/9
 */

#ifndef READWRITE_H_
#define READWRITE_H_
#include <unordered_map>
#include <string>
#ifndef MAX_PATH
#define MAX_PATH 260
#endif // !MAX_PATH
namespace rwini {


    class ReadWriteini {
    private:
        //配置文件路径全称
        char iniPath[MAX_PATH];
        //[section [key,value] ]
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>* iniContent = new std::unordered_map<std::string, std::unordered_map<std::string, std::string>>();
    public:
        //构造函数,参数为ini文件完整路径
        ReadWriteini(const char* inipath);
        //析构函数
        ~ReadWriteini();
        //查找指定[section [key,value] ]
        bool FindValue(const char* section, const char* key, char* value);
        //设置value
        bool SetValue(const char* section, const char* key, const char* value);
        //设置key
        bool SetKey(const char* section, const char* oldkey, const char* newkey);
        //插入section
        bool InsertSection(const char* section);
        //插入key
        bool InsertKey(const char* section, const char* key, const char* value);
        //删除section
        bool DeleteSection(const char* section);
        //删除key
        bool DeleteKey(const char* section, const char* key);
        //写入ini文件
        bool Writeini();
        //测试输出
        void show();
    };
}
#endif // !READWRITE_H_