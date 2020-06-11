﻿// 9.2_FunctionTemplateAndClassTemplate_002.cpp : This file contains the 'main' function. Program execution begins and ends there.
//课上的另一个例子。

#include <iostream>
using namespace std;

template <class T>
void outputArray(const T *t, int count)
{
	for(int i = 0; i < count; i++)
	{
		//使用模板时必须保证对应的类型有“<<”操作，或已经手动重载了“<<”操作符。
		cout << t[i] << " ";		
	}
	cout << endl;
}

int main()
{
	const int A_COUNT = 8, B_COUNT = 8, C_COUNT = 20;
	int a[A_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	double b[B_COUNT] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8 };
	char c[C_COUNT] = "Welcome to see you!";

	cout << "a array contains: " << endl;
	outputArray(a, A_COUNT);
	cout << "b array contains: " << endl;
	outputArray(b, B_COUNT);
	cout << "c array contains: " << endl;
	outputArray(c, C_COUNT);	
}
