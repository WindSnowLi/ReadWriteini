# rw_ini_base

使用C++标准库解析ini格式文件，只提供解析后的表和写入函数，查询、修改等功能可自行添加函数

## 类型说明

### 节点基类

```cpp
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
```

### section类

```cpp
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
```

### 键值对类

```cpp
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
```

## 封装问题

### 示例

```cpp
class rw_ini_test : public rw_ini::rw_ini_base {
public:
    explicit rw_ini_test(const std::string &ini_path) : rw_ini::rw_ini_base(ini_path) {}
    // 从rw_ini::rw_ini_base类继承到了解析结果集std::vector<section> s_list
    // 以及写入函数rw_ini::rw_ini_base::write()
    // 可以封装自己喜欢的api
};
```

### 说明

可通过 `std::vector<section> s_list` 直接查找 `section` 对象并或其所属键值对列表，可直接进行修改值操作，修改完成后可执行写入函数写入文件。
