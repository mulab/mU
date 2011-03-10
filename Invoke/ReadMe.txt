类似于.NET P/Invoke,J/Invoke,JNA,Python/ctype等，直接调用平台的动态链接库，自动转换平台API基本数据类型(ctype,win32 dword等)、结构体等。需要用到libffi。

动态链接库导出函数名不能重载，因此不需要用反射机制去确定重载，但没有反射机制意味着函数参数类型（压栈顺序？）未知，需要手动添加C头文件声明或本语言类似等价声明，如Cython/cdef，Java/JNA函数原型声明（用于反射查找），.NET @external）等。
