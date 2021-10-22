/*
MIT License

Copyright (c) 2021 WindSnowLi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef RW_INI_RW_INI_BASE_H
#define RW_INI_RW_INI_BASE_H

#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>

// 命名空间
namespace rw_ini {

    class key_value;

    class section;

    /**
     * 节点基类
     */
    class node_base {
    public:
        // 行上注释
        std::vector<std::string> line_up_explain{};
        // 行尾注释
        std::string line_tail_explain{};
    };

    /**
     * section节点类
     */
    class section : public node_base {
    public:
        // section名字
        std::string name{};
        // 所属键值表
        std::vector<key_value> table{};

        section() = default;

        explicit section(std::string name) : name(std::move(name)) {}

        bool operator==(const section &s) const {
            return name == s.name;
        }
    };

    /**
     * key_value 键值对类
     */
    class key_value : public node_base {
    public:
        // 键
        std::string key{};
        // 值
        std::string value{};

        bool operator==(const key_value &k_v) const {
            return key == k_v.key;
        }
    };

    /**
     * 解析基类
     */
    class rw_ini_base {
    private:
        // ini文件路径
        std::string ini_path{};

        // 节点上部注释缓冲区
        std::vector<std::string> line_up_explain_buff{};

        /**
         * 解析新的行
         * @param line ini文件的一行
         */
        void parse_line(const std::string &line);

        /**
         * 解析行中的section
         * @param line ini文件的一行
         * @param start 由parse_line解析到的有效的字符位置
         */
        void parse_line_section(const std::string &line, size_t start = 0);

        /**
         * 解析键值对中的key
         * @param line ini文件的一行
         * @param start 由parse_line解析到的有效的字符位置
         */
        void parse_line_key(const std::string &line, size_t start = 0);

        /**
         * 解析键值对中的value
         * @param line ini文件的一行
         * @param start 由parse_line_key解析到的有效的字符位置
         */
        void parse_line_value(const std::string &line, size_t start = 0);

        /**
         * 解析行上注释
         * @param line ini文件的一行
         * @param start 由parse_line解析到的有效的字符位置
         */
        void parse_line_up_explain(const std::string &line, size_t start = 0);

        /**
         * 解析行尾注释
         * @param line ini文件的一行
         * @param start 由parse_line_*解析到的有效的字符位置
         */
        void parse_line_tail_explain(const std::string &line, size_t start = 0);

    public:
        /**
         * 初始化ini文件路径，直接进行解析
         * @param ini_path ini文件路径
         */
        explicit rw_ini_base(const std::string &ini_path);

        // 整体内容表
        std::vector<section> s_list;

        // 文件尾部注释
        std::vector<std::string> tail_explain{};

        /**
         * 写入文件
         */
        void write();
    };

}

rw_ini::rw_ini_base::rw_ini_base(const std::string &ini_path) {
    this->ini_path = ini_path;
    std::ifstream fp(this->ini_path, std::ios::in);
    std::string line;
    while (std::getline(fp, line)) {
        this->parse_line(line);
    }
    fp.close();
    this->line_up_explain_buff.swap(this->tail_explain);
}

void rw_ini::rw_ini_base::parse_line(const std::string &line) {
    // 空行直接略过
    if (line.empty() || line[0] == '\r' || line[0] == '\n') {
        return;
    }
    // 寻找第一个有效字符位置
    size_t i = line.find_first_not_of(' ');
    // 读取到[视为该行为section行
    if (line[i] == '[') {
        this->parse_line_section(line, i);
    }
        // 读取到;或#视为该行为行上注释
    else if (line[i] == ';' || line[i] == '#') {
        this->parse_line_up_explain(line, i + 1);
    } else {
        //除此之外为键值对行
        this->parse_line_key(line, i);
    }
}

void rw_ini::rw_ini_base::parse_line_section(const std::string &line, size_t start) {
    // parse_line已读取到[，开始从下一个字符标记
    // 查找']'为section结束位置
    size_t right_index = line.find(']', start + 1);

    // 截取[]区间内的字符
    auto s = section(line.substr(start + 1, right_index - start - 1));

    // 如果行上注释不为空，则所属于当前section
    if (!this->line_up_explain_buff.empty()) {
        s.line_up_explain.swap(this->line_up_explain_buff);
    }

    // 列表里放入section
    this->s_list.emplace_back(s);

    // 非空索引
    size_t not_space_index = line.find_first_not_of(' ', right_index + 1);
    if (not_space_index != -1) {
        if (line[not_space_index] == ';' || line[not_space_index] == '#') {
            // 解析注释
            this->parse_line_tail_explain(line, not_space_index + 1);
        } else {
            // 充当注释
            this->parse_line_tail_explain(line, not_space_index);
        }
    }
}

void rw_ini::rw_ini_base::parse_line_key(const std::string &line, size_t start) {
    // 已由parse_line进行了排除前导空格操作，识别到了key的第一个字符
    // 如果行上注释不为空，则所属于当前key_value
    if (!this->line_up_explain_buff.empty()) {
        this->s_list.rbegin()->table.rbegin()->line_up_explain.swap(this->line_up_explain_buff);
    }
    // 分隔符索引
    size_t split_index = line.find('=', start + 1);

    // parse_line已识别至key的第一个有效字符，key_end_pos为key的结束位置
    size_t key_end_pos = line.find_last_not_of(' ', split_index - 1);

    // 截取开始~=之前第一个非空格的字符
    std::string key_buff = line.substr(start, key_end_pos - start + 1);

    // 创建新的key_value对象
    this->s_list.rbegin()->table.emplace_back(key_value());
    // 写入key
    this->s_list.rbegin()->table.rbegin()->key = key_buff;
    //接下来识别value
    this->parse_line_value(line, split_index + 1);
}

void rw_ini::rw_ini_base::parse_line_value(const std::string &line, size_t start) {
    // 已由parse_line_key识别到了=后一个字符
    // =后为value开始位置，value_end_pos为value结束位置标记
    size_t value_start_pos = line.find_first_not_of(' ', start), value_end_pos;

    //可能存在的注释
    size_t comment_index_1 = line.find(';', value_start_pos),
            comment_index_2 = line.find('#', value_start_pos);
    value_end_pos = comment_index_1 < comment_index_2 ? comment_index_1 : comment_index_2;
    std::string value_buff;
    if (value_end_pos == -1) {
        // 无注释
        value_buff = line.substr(value_start_pos);
    } else {
        // 有注释
        value_buff = line.substr(value_start_pos, value_end_pos - value_start_pos);
        this->parse_line_tail_explain(line, value_end_pos + 1);
    }

    // 如果自带引号，则去掉引号
    if (*value_buff.begin() == '"' && *value_buff.rbegin() == '"') {
        value_buff = value_buff.substr(1, value_buff.length() - 2);
    }
    this->s_list.rbegin()->table.rbegin()->value = value_buff;
}

void rw_ini::rw_ini_base::parse_line_up_explain(const std::string &line, size_t start) {
    // 去除前导空格
    // 行上注释直接追加至行上注释缓冲区
    this->line_up_explain_buff.emplace_back(line.substr(line.find_first_not_of(' ', start)));
}

void rw_ini::rw_ini_base::parse_line_tail_explain(const std::string &line, size_t start) {
    start = line.find_first_not_of(' ', start);
    // 如果section键值对表为空，则行尾注释属于最后一个section
    if (this->s_list.rbegin()->table.empty()) {
        this->s_list.rbegin()->line_tail_explain = line.substr(start);
    } else {
        // 否则属于最后一个键值对
        this->s_list.rbegin()->table.rbegin()->line_tail_explain = line.substr(start);
    }
}

void rw_ini::rw_ini_base::write() {
    std::stringstream ss;
    for (auto &&i : this->s_list) {
        for (auto &&j : i.line_up_explain) {
            ss << "# " << j << '\n';
        }
        ss << '[' << i.name << ']';
        if (!i.line_tail_explain.empty()) {
            ss << "\t # " << i.line_tail_explain;
        }
        ss << '\n';
        for (auto &&j : i.table) {
            for (auto &&k : j.line_up_explain) {
                ss << "# " << k << '\n';
            }
            ss << j.key << " = " << j.value;
            if (!j.line_tail_explain.empty()) {
                ss << "\t # " << j.line_tail_explain;
            }
            ss << '\n';
        }
        ss << "\n";
    }
    for (auto &&i:this->tail_explain) {
        ss << "# " << i << '\n';
    }
    std::ofstream fp(this->ini_path, std::ios::trunc);
    fp << ss.str();
    fp.close();
}

#endif //RW_INI_RW_INI_BASE_H
