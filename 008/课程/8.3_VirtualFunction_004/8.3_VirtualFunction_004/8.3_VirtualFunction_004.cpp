﻿// 8.3_VirtualFunction_004.cpp : This file contains the 'main' function. Program execution begins and ends there.
//这里主要讨论了为什么使用指针（或引用）可以使虚函数表现出多态性。

#include <iostream>
using namespace std;

class Base
{
    public:
    void virtual func() { cout << "Base" << endl; }
    int a = 1;
};

class Derived: public Base
{
    public:
    void func() { cout << "Derived" << endl; }
    int a = 2;
};

class A
{
    public:
    virtual void funA()
    {
        cout << "A::funA()" << endl;
        funB();
        funC();
    }
    virtual void funB() { cout << "A::funB()" << endl; }
    void funC() { cout << "A::funC()" << endl; }
};

class B: public A
{
    public:
    virtual void funA()
    {
        cout << "B::funA()" << endl;
        A::funA();
    }
    virtual void funB() { cout << "B::funB()" << endl; }
    void funC() { cout << "B::funC()" << endl; }
};

int main()
{
    Derived d;
    Base b = d;
    //0.这里在debug的时候看pb指针的构造会有点奇怪。它除了指向一个Derived对象外，还指向了一个_vfptr（和Derived对象中的那个虚指针的地址是一样）和一个整数a（指针对应类型中的数据成员a）。我比较好奇为什么会有这样的构造？
    Base *pb = &d;    
    Base &rb = d;

    //1.这里通过指针（或引用）访问和通过对象访问会得到不同的结果。
    //1.可能的原因是，对象访问函数时采用的并不是动态绑定，并不通过虚表来访问对应的函数。
    //1.可以注意到，当用派生类对象去初始化基类对象时，基类对象中的指向虚表的指针并不会发生指向上的改变。
    rb.func();
    pb->func();
    b.func();
    cout << endl;

    //2.这里有一点问题我有点不理解，如果说派生类的虚表是指向更新后的虚函数代码的话，指针访问虚函数应该只能访问到派生类的虚函数，为什么还能再访问到基类的虚函数呢？
    //2.一种可能的解释是，这里其实也没有发生动态绑定，由于作用域限定符的出现，这里其实就相当于在用对象调用func()。
    //2.但这里也还有一个问题，既然对象中只有一个虚表来确定函数代码所在的位置，而虚表又会在派生类中被指向派生类中重写的虚函数代码，那么这里不就没有办法找到基类代码所在的位置了吗？
    //2.[回答]对于上面那个问题，其实完全是多余的考虑。因为即使没有虚函数，类在内存中也并不会为代码分配专门的存储空间。用对象调用函数的静态绑定都是通过编译期间符号表来实现代码的嵌入的。
    //2.这里其实还有一个小问题，即虚表是在编译阶段就产生了的，还是在创建第一个对象时才产生？
    //2.[回答]在编译阶段就已经产生了，在运行时，对象根据类型确定虚表指针的指向。
    Derived *pd = &d;
    pd->Base::func();
    cout << endl;

    //3.这里可以看到，当指向B类型对象的A类型的指针pA调用funA()函数时，调用的是B的funA()函数。
    //3.随后，B的funA()函数调用A的funA()函数时，A的funA()函数调用的一般函数funC()是A的函数，而虚函数funB()却又是B的函数。
    //3.这里的解释应该和2类似，对于虚函数而言，当用作用域限定符时，就是限定了对象类型的调用；没有作用域限定符时，就是通过虚表进行调用。    
    //3.此时，虽然pA在调用函数funA()时，B类的funA()函数通过作用域限定符调用了A类的funA()函数，但A类的funA()函数中并没有用作用域限定符调用虚函数funB()，因此依旧认为是pA通过虚表调用了funB()。
    //3.更简单地去考虑，这种在类的成员函数中调用虚函数的做法实际上和之前提到的在全局函数中调用虚函数的做法是一样的。
    A *pA = new B;
    pA->funA();
    cout << endl;
    //3.上面提到，用作用域限定符时，就限定了对象类型的调用。然而这个限定其实很宽松，当对象调用的函数中又出现虚函数时，虚函数还是会按照动态绑定的方式去调用。（其实下面的例子和上面的情况差不多是一回事）
    //3.因此我猜测，在编译过程中，很有可能是涉及到非虚函数的就将函数代码嵌入，而涉及到虚函数的就将函数代码空出，等到运行时再做决定。
    //3.另外，还有一点值得注意的点，就是虽然说虚函数是动态绑定的，但在程序的.map符号表文件中还是可以找到它们在不同类型中的不同映射。
    //3.因此，实际编译过程确定的，可能就是在需要调用动态绑定的虚函数时，需要根据符号表中的哪一项跳转到哪一个路径。
    pA->A::funA();
    cout << endl;
    
    A *pbNull = NULL;
    //4.下面这行代码在运行到“pbNull->funB();”时，会导致程序卡死。而在运行“pbNull->funC(); ”时则不会。
    //4.原因在于，类的成员函数代码是单独存放的。一个指向空的A类指针要调用一般成员函数时，采用的就是对象调用的思路，程序会直接跳转到成员函数所在代码区域；
    //4.而当需要用指针调用虚函数时，由于指针指向的是空对象，因此是不存在指向虚表的指针，所以无法实现函数的调用。
    pbNull->funC();    
    /*
    pbNull->funB();
    */
}