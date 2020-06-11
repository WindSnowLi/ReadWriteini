C++基于STL的ini读写类
C++ based on STL ini reading and writing classes
v0.0.1

经过测试，只要是文本，后缀ini或者是txt，对程序是不影响的。不过比较尴尬的一点，代码只考虑了对注释屏蔽，但没考虑保存注释，所以在写入时会自动删除注释。

After testing, as long as it is text, suffix INI or TXT, the program is not affected.Awkwardly, 
the code only considers masking comments, not saving them, so comments are automatically 
deleted when they are written.


v0.0.2
改变函数接收参数的方式，使用模板接收参数，优化函数在传递参数过程中的体验。删除了测试部分代码

Change the way the function receives parameters, using templates to receive parameters, 
and optimize the experience of the function in passing parameters.The code for the test section has been removed.