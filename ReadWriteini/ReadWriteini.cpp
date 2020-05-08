#include "ReadWriteini.h"
#include <fstream>
#include <iostream>



/*
 * @name:构造函数
 * @describe:初始化基础数据
 * @param {char*}
 * @return:无
 */

rwini::ReadWriteini::ReadWriteini(const char *inipath){
	memcpy(iniPath, inipath, sizeof(char) * strlen(inipath));
    //是否录入数据
    bool continueread = false;
    //是否忽略本次外层循环
    bool cyclejump = false;
    std::ifstream iniFile;
    iniFile.open(iniPath, std::ios::in);
    //临时储存一行的数据
    std::string tempData;
    //临时存储section
    char tempSection[100];
    memset(tempSection, 0, sizeof(char) * 100);
    //临时存储key
    char tempKey[100];
    memset(tempKey, 0, sizeof(char) * 100);
    //临时存储value
    char tempValue[100];
    memset(tempValue, 0, sizeof(char) * 100);

    while (getline(iniFile, tempData)) {
        //首先判断获取的这一行有没有需要提取的数据，排除空行和;与#开头的行
        int tempcolumn = 0;
        for (tempcolumn = 0; tempcolumn < tempData.length(); tempcolumn++) {
            if (tempData[tempcolumn] == ' ') {
                //如果读取到的字符是' ',跳出本次判断，判断下一个字符
                continue;
            }
            else if (tempData[tempcolumn] == '\n' || tempData[tempcolumn] == ';' || tempData[tempcolumn] == '#') {
                //如果这一行，没有提取数据的需要，就让外层循环跳过这次循环
                cyclejump = true;
                break;
            }
            else {
                //如果是有效数据，就跳出判断循环，执行后边步骤
                break;
            }
        }


        if (cyclejump || tempData.length() == 0) {
            //判断是否跳出本次外层循环，当本行长度为0时，判断循环不会执行，所以在这里额外附加判断
            cyclejump = false;
            continue;
        }

        //经过上边循环的判断，接下来的字符是非空，判断是section还是key,如果是'['开头，则是section,否则是key
        if (tempData[tempcolumn] == '[') {
            continueread = false;
            int temp1 = 0;
            //获得'['与']'之间的section
            for (int j = tempcolumn + 1; j < tempData.length(); j++) {
                if (tempData[j] == ']') {
                    break;
                }
                tempSection[temp1++] = tempData[j];
            }
            //插入一个section
            iniContent->insert(std::make_pair(tempSection, std::unordered_map<std::string, std::string>()));
            //需要录入数据，因为需要录入的是本行的下一行的开始的数据，所以录入数部份将在下次外层循环生效
            continueread = true;
        }
        else  if (continueread) {
            //录入数据
            int part = tempData.find("=");
            for (int i = 0; i < part && tempData[i] != ';' && tempData[i] != '#'; i++) {
                tempKey[i] = tempData[i];
            }
            for (int i = part + 1; i < tempData.length() && tempData[i] != ';' && tempData[i] != '#'; i++) {
                tempValue[i - part - 1] = tempData[i];
            }
            (--iniContent->end())->second.insert(std::make_pair(tempKey, tempValue));
        }
        //清除临时数据
        tempData.clear();
        memset(tempSection, 0, sizeof(char) * 100);
        memset(tempKey, 0, sizeof(char) * 100);
        memset(tempValue, 0, sizeof(char) * 100);

    }
    //关闭文件
    iniFile.close();
}

rwini::ReadWriteini::~ReadWriteini()
{
    delete iniContent;
}


/*
 * @name:FindValue
 * @describe:根据section和key查找value
 * @param {const char*, const char*, char*}
 * @return:bool
 */

bool rwini::ReadWriteini::FindValue(const char* section, const char* key, char* value)
{
    value[0] = '\0';
    if (section != NULL && key != NULL) {
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator iter;
        std::unordered_map<std::string, std::string>::iterator tempiter;
        iter = iniContent->find(section);
        if (iter!=iniContent->end()) {
            tempiter = iter->second.find(key);
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
 * @param {const char*, const char*,const char*}
 * @return:bool
 */

bool rwini::ReadWriteini::SetValue(const char* section, const char* key, const char* value)
{
    if (section != NULL && key != NULL) {
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator iter;
        std::unordered_map<std::string, std::string>::iterator tempiter;
        iter = iniContent->find(section);
        if (iter != iniContent->end()) {
            tempiter = iter->second.find(key);
            if (tempiter!= iter->second.end()) {
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
bool rwini::ReadWriteini::SetKey(const char* section, const char* oldkey, const char* newkey)
{
    if (section != NULL && oldkey != NULL && newkey != NULL) {
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator iter;
        std::unordered_map<std::string, std::string>::iterator tempiter;
        iter = iniContent->find(section);
        if (iter != iniContent->end()) {
            tempiter = iter->second.find(oldkey);
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

bool rwini::ReadWriteini::InsertSection(const char* section)
{
    if (section != NULL) {
        iniContent->insert(std::make_pair(section, std::unordered_map<std::string, std::string>()));
        return true;
    }
    return false;
}


/*
 * @name:InsertKey
 * @describe:插入Key
 * @param {const char*}
 * @return:bool
 */
bool rwini::ReadWriteini::InsertKey(const char* section, const char* key, const char* value)
{
    if (section != NULL && key != NULL&& value!= NULL) {
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator iter=iniContent->find(section);
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
 * @param {const char*}
 * @return:bool
 */
bool rwini::ReadWriteini::DeleteSection(const char* section)
{
    if (section != NULL) {
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator iter = iniContent->find(section);
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
bool rwini::ReadWriteini::DeleteKey(const char* section, const char* key)
{
    if (section != NULL&& key != NULL) {
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator iter = iniContent->find(section);
        std::unordered_map<std::string, std::string>::iterator tempiter;
        if (iter != iniContent->end()) {
            tempiter = iter->second.find(key);
            if (tempiter != iter->second.end()) {
                iter->second.erase(tempiter);
                return true;
            }
        }
    }
    return false;
}

/*
 * @name:Writeini
 * @describe:写入文件
 * @param {const char*}
 * @return:bool
 */
bool rwini::ReadWriteini::Writeini()
{
    std::ofstream iniFile;
    iniFile.open(iniPath, std::ios::out | std::ios::trunc);

    for (auto i : *iniContent) {
        iniFile << "[" << i.first << "]" << std::endl;
        for (auto j : i.second) {
            iniFile << j.first << "=" << j.second << std::endl;
        }
        iniFile << std::endl;
    }
    iniFile.close();
    return true;
}

void rwini::ReadWriteini::show()
{
    for (auto i : *iniContent) {
        std::cout << i.first << "\t" << std::endl;
        for (std::unordered_map<std::string, std::string>::iterator iter = i.second.begin(); iter != i.second.end();iter++) {
            std::cout << iter->first << "\t=\t" << iter->second << std::endl;
        }
        std::cout<< std::endl;
    }
}
