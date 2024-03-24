#include <iostream>

// class DROP
// {
//     public:
//         virtual void func1() {
//             printf("drop::func1\n");
//         }
// 
//     public:
//         int _drop = 0;
// };

class AA
{
    public:
        virtual void func1() {
            printf("AA::func1\n");
        }

        virtual void func2() {
            printf("AA::func2\n");
        }

    public:
        int _a = 'a';
};

class BB : public AA
{
    public:
        virtual void func1() {
            printf("BB::func1\n");
        }

        virtual void func3() {
            printf("BB::func3\n");
        }
        
    public:
        int _b = 'b';
};

class CC : public AA
{
    public:
        virtual void func1() {
            printf("CC::func1\n");
        }

        virtual void func4() {
            printf("CC::func4\n");
        }
        
    public:
        int _c = 'c';
};

typedef void (*FUNC) ();

void printfVTable(uintptr_t *pVTable)
{
    printf("VTable addr: %p\n", pVTable);

    for (int i = 0; (pVTable[i] & 0xFF) != 0x32; i++) {
        printf("VTable[%d] addr: %lx\n", i, pVTable[i]);
        FUNC f = (FUNC)pVTable[i];
        f();
    }
}

int main()
{
    uintptr_t *pVTable = NULL;

//    printf("AA size: %zd, BB size: %zd, CC size %zd\n", 
//            sizeof (AA), sizeof (BB), sizeof (CC));

    // DROP d;

    printf("\n");
    AA a;
    a.func1();
    a.func2();
    pVTable = (uintptr_t *)(*(uintptr_t *)&a);
    printf("VTable[2] addr: %lx\n", pVTable[2]);
    printfVTable(pVTable);

    printf("\n");
    BB b;
    b.func1();
    b.func2();
    b.func3();
    pVTable = (uintptr_t *)(*(uintptr_t *)&b);
    printfVTable(pVTable);

    printf("\n");
    CC c;
    c.func1();
    c.func2();
    c.func4();
    pVTable = (uintptr_t *)(*(uintptr_t *)&c);
    printfVTable(pVTable);

	return 0;
}

