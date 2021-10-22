#include <iostream>
#include "../include/rw_ini_base.h"

class rw_ini_test : public rw_ini::rw_ini_base {
public:
    explicit rw_ini_test(const std::string &ini_path) : rw_ini::rw_ini_base(ini_path) {}
    // 从rw_ini::rw_ini_base类继承到了解析结果集std::vector<section> s_list
    // 以及写入函数rw_ini::rw_ini_base::write()
    // 可以封装自己喜欢的api
};

int main() {
    // 生成示例文件
    std::ofstream fp("./config.ini", std::ios::trunc);
    fp << (std::string("; last modified 1 April 2001 by John Doe\n") +
           "[owner] ; belong me\n" +
           "name = John Doe;姓名\n" +
           "organization = Acme Products;组织名\n" +
           "[database]\n" +
           "server = 192.0.2.42\n" +
           "; use IP address in case network name resolution is not working\n" +
           "port = 143 ;143端口\n" +
           "file = \"acme payroll.dat\" ;文件名");
    fp.close();
    //读取
    rw_ini_test rw_ini("./config.ini");
    for (auto &&i: rw_ini.s_list) {
        for (auto &&j: i.line_up_explain) {
            std::cout << j << '\n';
        }
        std::cout << '[' << i.name << "]\t" << i.line_tail_explain << "\n";

        for (auto &&j: i.table) {
            for (auto &&k: j.line_up_explain) {
                std::cout << k << '\n';
            }
            std::cout << j.key << '=' << j.value << '\t' << j.line_tail_explain << '\n';
        }
        std::cout << "\n";
    }
    rw_ini.write();
    std::cout << "END" << std::endl;
    return 0;
}