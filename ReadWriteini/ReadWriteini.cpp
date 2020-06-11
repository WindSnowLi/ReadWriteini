#include "ReadWriteini.h"
#include <fstream>
#include <iostream>


/*
 * @name:构造函数
 * @describe:初始化基础数据
 * @param {char*}
 * @return:无
 */

rwini::ReadWriteini::ReadWriteini(const char* inipath) {
    memset(iniPath, '\0', sizeof(char) * MAX_PATH);
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
    char tempBufferSection[100];
    memset(tempSection, '\0', sizeof(char) * 100);
    //临时存储key
    char tempKey[100];
    memset(tempKey, '\0', sizeof(char) * 100);
    //临时存储value
    char tempValue[100];
    memset(tempValue, '\0', sizeof(char) * 100);
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
            memset(tempBufferSection, '\0', sizeof(char) * 100);
            memcpy(tempBufferSection, tempSection, sizeof(char) * 100);
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
            auto iter = iniContent->find(tempBufferSection);
            if (iter != iniContent->end()) {
                iter->second.insert(std::make_pair(tempKey, tempValue));
            }
            //(--iniContent->end())->second.insert(std::make_pair(tempKey, tempValue));
        }
        //清除临时数据
        tempData.clear();
        memset(tempSection, '\0', sizeof(char) * 100);
        memset(tempKey, '\0', sizeof(char) * 100);
        memset(tempValue, '\0', sizeof(char) * 100);

    }
    //关闭文件
    iniFile.close();
}

rwini::ReadWriteini::~ReadWriteini()
{
    delete iniContent;
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