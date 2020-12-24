# Read and write files in INI format using the standard library

## function

```c++
/// <summary>
/// Initializes the load file
/// </summary>
/// <param name="inipath">File path</param>
/// <returns>None</returns>
ReadWriteini(const char* inipath) noexcept(false); 

/// <summary>
/// End processing
/// </summary>
~ReadWriteini(); 

/// <summary>
/// Find the value based on section and key
/// </summary>
/// <typeparam name="Type1">Section typename</typeparam>
/// <typeparam name="Type2">Key typename</typeparam>
/// <param name="tempsection">Section</param>
/// <param name="tempkey">Key</param>
/// <param name="value">Results the address</param>
/// <returns>State of the query</returns>
template <typename Type1, typename Type2>
bool FindValue(const Type1& tempsection, const Type2& tempkey, char* value) noexcept(false); 

/// <summary>
/// Finds the specified value and returns std::string, and throws a NotFoundKey or NotFoundSection exception if no section or key is found
/// </summary>
/// <typeparam name="Type1">Section typename</typeparam>
/// <typeparam name="Type2">Key typename</typeparam>
/// <param name="tempsection">Section</param>
/// <param name="tempkey">Key</param>
/// <returns>Return search results</returns>
template <typename Type1, typename Type2>
std::string FindValue(const Type1& tempsection, const Type2& tempkey) noexcept(false); 

/// <summary>
/// Set value
/// </summary>
/// <typeparam name="Type1">Section typename </typeparam>
/// <typeparam name="Type2">Key typename</typeparam>
/// <typeparam name="Type3">Value typename</typeparam>
/// <param name="tempsection">Section</param>
/// <param name="tempkey">Key</param>
/// <param name="tempvalue">value</param>
/// <returns>Status</returns>
template <typename Type1, typename Type2, typename Type3>
bool SetValue(const Type1& tempsection, const Type2& tempkey, const Type3& value) noexcept(false); 

/// <summary>
/// Set the key under section
/// </summary>
/// <typeparam name="Type1">Section typename</typeparam>
/// <typeparam name="Type2">Old key typename</typeparam>
/// <typeparam name="Type3">New Key typename</typeparam>
/// <param name="tempsection">Section</param>
/// <param name="tempoldkey">Old key</param>
/// <param name="tempnewkey">New key</param>
/// <returns>Status</returns>
template <typename Type1, typename Type2, typename Type3>
bool SetKey(const Type1& tempsection, const Type2& tempoldkey, const Type3& tempnewkey) noexcept(false); 

/// <summary>
/// Insert Section
/// </summary>
/// <typeparam name="Type1">Section typename</typeparam>
/// <param name="tempsection">Section</param>
/// <returns>Status</returns>
template <typename Type1>
bool InsertSection(const Type1& tempsection); 

/// <summary>
/// Insert the Key under Section
/// </summary>
/// <typeparam name="Type1">Section typename </typeparam>
/// <typeparam name="Type2">Key typename</typeparam>
/// <typeparam name="Type3">Value typename</typeparam>
/// <param name="tempsection">Section</param>
/// <param name="tempkey">Key</param>
/// <param name="tempvalue">value</param>
/// <returns>status</returns>
template <typename Type1, typename Type2, typename Type3>
bool InsertKey(const Type2& tempsection, const Type1& tempkey, const Type3& tempvalue) noexcept(false); 

/// <summary>
/// If there is no parent tag, the parent tag is automatically added
/// </summary>
/// <typeparam name="Type1">Section typename</typeparam>
/// <typeparam name="Type2">Key typename</typeparam>
/// <typeparam name="Type3">Value typename</typeparam>
/// <param name="tempsection">Section</param>
/// <param name="tempkey">Key</param>
/// <param name="tempvalue">value</param>
/// <returns>Status</returns>
template <typename Type1, typename Type2, typename Type3>
bool AutoInsertKey(const Type2& tempsection, const Type1& tempkey, const Type3& tempvalue) noexcept(false); 

/// <summary>
/// Delete section
/// </summary>
/// <typeparam name="Type1">The parameter types</typeparam>
/// <param name="tempsection">The value of the parameter</param>
/// <returns>status</returns>
template <typename Type1>
bool DeleteSection(const Type1& tempsection) noexcept(false); 

/// <summary>
/// Delect Key
/// </summary>
/// <typeparam name="Type1">Section typename</typeparam>
/// <typeparam name="Type2">Key typename</typeparam>
/// <param name="tempsection">Section</param>
/// <param name="tempkey">Key</param>
/// <returns>Delete status</returns>
template <typename Type1, typename Type2>
bool DeleteKey(const Type1& tempsection, const Type2& tempkey); 

/// <summary>
/// Written to the file
/// </summary>
/// <returns>Written status</returns>
bool Writeini(); 

/// <summary>
/// To find the Section
/// </summary>
/// <param name="temp_section">Section typename</param>
/// <returns>Section table</returns>
std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>::iterator

    SeekSection(std::string temp_section) noexcept(false);

/// <summary>
/// Look up the Key under Section and return the Key table
/// </summary>
/// <param name="temp_section">Section typename</param>
/// <param name="temp_key">Key typename</param>
/// <returns>Key table</returns>
std::unordered_map<std::string, std::vector<std::string>>::iterator

    SeekKey(std::string temp_section, std::string temp_key) noexcept(false);

```

___

## exception

```c++
enum class RwExceptionType
{
    /// <summary>
    /// Base exception
    /// </summary>
    RwiniException = 0,
    /// <summary>
    /// Not found Key
    /// </summary>
    NotFoundKey = 1,
    /// <summary>
    /// Not found Section
    /// </summary>
    NotFoundSection = 2,
    /// <summary>
    /// File format error
    /// </summary>
    IniFormatError = 3,
    /// <summary>
    /// Parameter is null
    /// </summary>
    NullSectionOrKey = 4
};
```
