// #include <stdio.h>
// 
// class Value
// {
// private:
//     int mi;
// public:
//     Value(int i)
//     {
//         printf("i = %d\n", i);
//         mi = i;
//     }
//     int getI()
//     {
//         return mi;
//     }
// };
// 
// class Test
// {
// private:
//     Value m2;
//     Value m3;
//     Value m1;
// 
// public:
//     const int ci;
// 
//     Test() : m1(1), m2(2), m3(3), ci(100)
//     {
//         printf("Test::Test()\n");
//     }
// 
//     int getCI()
//     {
//         return ci;
//     }
// 
//     int setCI(int v)
//     {
//         int* p = const_cast<int*>(&ci);  // 通过指针来操作常量对象；
//         // int* p = (int*)&ci;  // 通过指针来操作常量对象；
// 
//         *p = v;
// 
//         return *p;
//     }
// };
// 
// 
// int main()
// {
//     Test t;
// 
//     printf("t.ci = %d\n", t.getCI());
// 
//     // t.setCI(10);
//     int* p = const_cast<int*>(&t.ci);  // 通过指针来操作常量对象；
//     // int* p = (int*)&ci;  // 通过指针来操作常量对象；
// 
//     *p = 20;
// 
//     printf("t.ci = %d\n", t.getCI());
// 
//     return 0;
// }

#include <iostream>

// using namespace std;

int main()
{
    int *p;
    const int ic=0;
    p = const_cast<int*>(&ic);  // 通过指针来操作常量对象；
    *p = 5;
    printf("%d\n", ic);

    return 0;
} 
