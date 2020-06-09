﻿// 8.5_Supplement_004_TypeId.cpp : This file contains the 'main' function. Program execution begins and ends there.
//关于typeid。

#include <iostream>
#include <typeinfo>
using namespace std;

class A0
{};
class A1: public A0
{};

class B0
{
    public:
    virtual ~B0() {}
    virtual A0* f() 
    { 
        cout << "B0::f()" << endl;
        return new A0; 
    }
};
class B1: public B0
{
    public:
    virtual A1 *f() 
    { 
        cout << "B1::f()" << endl;
        return new A1; 
    }
};

class Base
{
    public:
    virtual ~Base() {}
};

class Derived: public Base
{};

void fun(Base *b)
{    
    const type_info &info1 = typeid(b);
    const type_info &info2 = typeid(*b);
    //3.虽然b是指向多态类型的指针，但b本身作为指针并不是多态类型的。因此每次输出typeid(b)，都是Base *。
    cout << "typeid(b): " << info1.name() << endl;
    cout << "typeid(*b): " << info2.name() << endl;
    //4.typeid只会判断一个类型是否为某个具体类型，而不会把子类也包括在内。
    //4.如果要判断某个对象是否是某个类型或其子类型，还是用dynamic_cast，看返回值是否为空指针（或在引用的情况下是否报错）会更合适。
    if(info2 == typeid(Base))
    {
        cout << "A Base class !" << endl;
    }
}


int main()
{
    {
        //1.关于书上p339的“typeid所作用于的表达式具有多态类型，则表达式会被求值”的疑惑。
        A1 a1;
        A0 *pa0 = &a1;
        cout << "没有虚函数所以不计算表达式。" << endl;
        //1.输出指针与指针所指对象的类型都是A0。证明这里没有对*pa0这个表达式进行求值。
        cout << typeid(pa0).name() << endl;
        cout << typeid(*pa0).name() << endl;
        cout << endl;

        B1 b1;
        B0 *pb0 = &b1;
        cout << "有虚函数所以计算表达式。" << endl;
        //1.输出指针的类型为B0*，而指针所指向对象的类型为B1。证明这里对*pb0这个表达式求值了。
        cout << typeid(pb0).name() << endl;
        cout << typeid(*pb0).name() << endl;
        cout << endl;

        cout << "明明有虚函数，为什么不计算表达式？" << endl;
        //1.这里的输出是A0 *，且没有执行f()中的输出操作，说明这里根本没有对pb0->f()这个表达式求值。
        //1.这就很奇怪了，pb0明明是一个指向多态类型对象的指针，且之前也对其进行过求值。
        cout << typeid(pb0->f()).name() << endl;
        //1.如果说上行代码可以解释为指针本身不具有多态性，那为什么下面这行代码也没有对表达式求值？
        cout << typeid((*pb0).f()).name() << endl;
        cout << endl;

    }

    //2.书上给出的例子。
    cout << "书上的例子。" << endl;
    Base b;
    fun(&b);
    Derived d;
    fun(&d);    
}