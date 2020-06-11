
/*
 * @Description: 用于读取ini配置文件
 * @version:0.0.1
 * @Author: windSnowLi
 * @Date:2020/5/7
 * @LastEditors: windSnowLi
 * @LastEditTime:2020/6/11
 */

#ifndef READWRITE_H_
#define READWRITE_H_
#include <unordered_map>
//为了兼容MSVC与MingW,在此引入了cstring与string
#include <cstring>
#include <string>
#include <sstream>
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
        //将接收到的参数转化为字符串
        template <typename Type>
        std::string TypeToString(Type str);
    public:
        //构造函数,参数为ini文件完整路径
        ReadWriteini(const char* inipath);
        //析构函数
        ~ReadWriteini();
        //查找指定[section [key,value] ]
        template <typename Type1,typename Type2>
        bool FindValue(const Type1& tempsection, const Type2& tempkey, char* value);
        //设置value
        template <typename Type1, typename Type2,typename Type3>
        bool SetValue(const Type1& tempsection, const Type2& tempkey, const Type3& value);
        //设置key
        template <typename Type1, typename Type2, typename Type3>
        bool SetKey(const Type1& tempsection, const  Type2& tempoldkey, const  Type3& tempnewkey);
        //插入section
        template <typename Type1>
        bool InsertSection(const Type1& tempsection);
        //插入key
        template <typename Type1, typename Type2, typename Type3>
        bool InsertKey(const Type2& tempsection, const Type1& tempkey, const Type3& tempvalue);
        //删除section
        template <typename Type1>
        bool DeleteSection(const Type1& tempsection);
        //删除key
        template <typename Type1, typename Type2>
        bool DeleteKey(const Type1& tempsection, const Type2& tempkey);
        //写入ini文件
        bool Writeini();
    };
}


#ifndef READWRITEINI_TEMPLATE_
#define READWRITEINI_TEMPLATE_
/*
 * @name:TypeToString
 * @describe:将参数转化为字符串类型
 * @param typename Type
 * @return:string
 */
template<typename Type>
std::string rwini::ReadWriteini::TypeToString(Type str)
{
    std::stringstream tempStrStream;
    std::string goalStr;
    tempStrStream << str;
    goalStr = tempStrStream.str();
    return goalStr;
}

/*
 * @name:FindValue
 * @describe:根据section和key查找value
 * @param const Type1& tempsection, const Type2& tempkey, char* value
 * @return:bool
 */
 /*
 *2020/6/11修改，使用模板接收参数
 */
template <typename Type1, typename Type2>
bool rwini::ReadWriteini::FindValue(const Type1& tempsection, const Type2& tempkey, char* value)
{
    //将接收的参数转化为字符串
    std::string section = TypeToString(tempsection);
    std::string key = TypeToString(tempkey);
    value[0] = '\0';
    if ((!section.empty()) && (!key.empty())) {
        auto iter = iniContent->find(section);
        if (iter != iniContent->end()) {
            auto tempiter = iter->second.find(key);
            if (tempiter != iter->second.end()) {
                const char* tempStr = tempiter->second.c_str();
                memcpy(value, tempStr, sizeof(char) * strlen(tempStr));
                value[strlen(tempStr) + 1] = '\0';
                return true;
            }
        }
    }
    return false;
}

/*
 * @name:SetValue
 * @describe:根据section key设置value
 * @param const Type1& tempsection, const Type2& tempkey, const Type3& tempvalue
 * @return:bool
 */
 /*
  *2020/6/11修改，使用模板接收参数
  */
template <typename Type1, typename Type2, typename Type3>
bool rwini::ReadWriteini::SetValue(const Type1& tempsection, const Type2& tempkey, const Type3& tempvalue)
{
    //将接收的参数转化为字符串
    std::string section = TypeToString(tempsection);
    std::string key = TypeToString(tempkey);
    std::string value = TypeToString(tempvalue);
    if ((!section.empty()) && (!key.empty())) {
        auto iter = iniContent->find(section);
        if (iter != iniContent->end()) {
            auto tempiter = iter->second.find(key);
            if (tempiter != iter->second.end()) {
                tempiter->second = value;
                return true;
            }
        }
    }
    return false;
}


/*
 * @name:SetKey
 * @describe:根据section设置key
 * @param {const char*, const char*,const char*}
 * @return:bool
 */
 /*
 *2020/6/11修改，使用模板接收参数
 */
template <typename Type1, typename Type2, typename Type3>
bool rwini::ReadWriteini::SetKey(const Type1& tempsection, const  Type2& tempoldkey, const  Type3& tempnewkey)
{
    //将接收的参数转化为字符串
    std::string section = TypeToString(tempsection);
    std::string oldkey = TypeToString(tempoldkey);
    std::string newkey = TypeToString(tempnewkey);
    if ((!section.empty()) && (!oldkey.empty()) && (!newkey.empty())) {
        auto iter = iniContent->find(section);
        if (iter != iniContent->end()) {
            auto tempiter = iter->second.find(oldkey);
            if (tempiter != iter->second.end()) {
                char tempvalue[100] = { 0 };
                if (FindValue(section, oldkey, tempvalue)) {
                    iter->second.erase(tempiter);
                    iter->second.insert(std::make_pair(newkey, tempvalue));
                }
                return true;
            }
        }
    }
    return false;
}



/*
 * @name:InsertSection
 * @describe:插入section
 * @param {const char*}
 * @return:bool
 */
 /*
 *2020/6/11修改，使用模板接收参数
 */
template <typename Type1>
bool rwini::ReadWriteini::InsertSection(const Type1& tempsection)
{
    //将接收的参数转化为字符串
    std::string section = TypeToString(tempsection);
    if (!section.empty()) {
        iniContent->insert(std::make_pair(section, std::unordered_map<std::string, std::string>()));
        return true;
    }
    return false;
}


/*
 * @name:InsertKey
 * @describe:插入Key
 * @param const Type2& tempsection, const Type1& tempkey, const Type3& tempvalue
 * @return:bool
 */
 /*
 *2020/6/11修改，使用模板接收参数
 */
template <typename Type1, typename Type2, typename Type3>
bool rwini::ReadWriteini::InsertKey(const Type2& tempsection, const Type1& tempkey, const Type3& tempvalue)
{
    //将接收的参数转化为字符串
    std::string section = TypeToString(tempsection);
    std::string key = TypeToString(tempkey);
    std::string value = TypeToString(tempvalue);
    if ((!section.empty()) && (!key.empty())) {
        auto iter = iniContent->find(section);
        if (iter != iniContent->end()) {
            iter->second.insert(std::make_pair(key, value));
            return true;
        }
    }
    return false;
}



/*
 * @name:DeleteSection
 * @describe:删除section
 * @param const Type1& tempsection
 * @return:bool
 */
 /*
 *2020/6/11修改，使用模板接收参数
 */
template <typename Type1>
bool rwini::ReadWriteini::DeleteSection(const Type1& tempsection)
{
    //将接收的参数转化为字符串
    std::string section = TypeToString(tempsection);
    if (!section.empty()) {
        auto iter = iniContent->find(section);
        if (iter != iniContent->end()) {
            iniContent->erase(iter);
            return true;
        }
    }
    return false;
}



/*
 * @name:DeleteKey
 * @describe:删除Key
 * @param {const char*}
 * @return:bool
 */
 /*
 *2020/6/11修改，使用模板接收参数
 */
template <typename Type1, typename Type2>
bool rwini::ReadWriteini::DeleteKey(const Type1& tempsection, const Type2& tempkey)
{
    //将接收的参数转化为字符串
    std::string section = TypeToString(tempsection);
    std::string key = TypeToString(tempkey);
    if ((!section.empty()) && (!key.empty())) {
        auto iter = iniContent->find(section);
        if (iter != iniContent->end()) {
            auto tempiter = iter->second.find(key);
            if (tempiter != iter->second.end()) {
                iter->second.erase(tempiter);
                return true;
            }
        }
    }
    return false;
}
#endif // !READWRITEINI_TEMPLATE_
#endif // !READWRITE_H_