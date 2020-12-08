#include <iostream>
#include <string>
#include <fstream>
//#include "Giggle.h"
using namespace std;
const int maximum = 1000000001;
struct Treap {//сама структура
	int y;//поле кучи
	int value;//хранимые данные
	int count;//колличество элементов в дереве
	int min;//минимум в дереве(в узле дерева)
	Treap* left;//ссылка на левое поддерево
	Treap* right;//ссылка на правое поддерево
};
typedef Treap* PTreap;

int Min(int a, int b) {//вспомогательная функция для поиска минимума
	if (a < b)
		return a;
	else
		return b;
}
void Recalc(PTreap t) {//функция для пересчета минимума и колличества элементов в дереве
	if (t == nullptr)
		return;
	int a, b;
	int minA, minB;
	if (t->left) {
		a = t->left->count;
		minA = t->left->min;
	}
	else {
		a = 0;
		minA = maximum;
	}
	if (t->right) {
		minB = t->right->min;
		b = t->right->count;
	}
	else {
		b = 0;
		minB = maximum;
	}
	t->count = a + b + 1;
	t->min = Min(Min(t->value, minA), minB);
}
void Show(ofstream& a, PTreap t) {//показывает дерево использую для отладки
	if (t == nullptr)
		return;
	Show(a, t->left);
	int X;
	if (t->left == 0)
		X = 0;
	else
		X = t->left->count;
	a << t->value << "(" << t->min << "," << t->y << "," << t->count << "," << "|" << X << "|"")" << " ";
	Show(a, t->right);
}
void ReadStr(string st, int& n1, int& n2, char& k) {//функция для получения информации о считываемой строке
	k = st[0];
	string s1 = "";
	string s2 = "";
	int f = 2;
	while (st[f] != ' ')
		s1 = s1 + st[f++];
	f++;
	while (f < st.length())
		s2 = s2 + st[f++];
	n1 = stoi(s1);
	n2 = stoi(s2);
}
PTreap Merge(PTreap t1, PTreap t2) {//слияне двух деревьев, таких что все элементы одного больше всех элементов другого
	if (t1 == nullptr)
		return t2;
	if (t2 == nullptr)
		return t1;
	if (t1->y > t2->y) {//проверяем у кого y больше и в зависимости от этого запускаем рекурсивнй merge
		t1->right = Merge(t1->right, t2);
		Recalc(t1);
		return t1;
	}
	else {
		t2->left = Merge(t1, t2->left);
		Recalc(t2);
		return t2;
	}
	//на каждом шаге пересчитываем деревья
}
void Split(PTreap t, PTreap& t1, PTreap& t2, int key) {//разрезаем Treap на 2 дерева с условием для merge
	if (t == nullptr) {
		t1 = nullptr;
		t2 = nullptr;
		return;
	}
	int g;//неявный ключ
	if (t->left == nullptr) {
		g = 0;
	}
	else
		g = t->left->count;
	if (g < key) {//проверяем ключ и действуем в зависимости от результата
		PTreap a, b;
		Split(t->right, a, b, key - g - 1);
		t->right = a;
		t1 = t;
		Recalc(t1);
		t2 = b;
		Recalc(t2);
	}
	else {
		PTreap a, b;
		//
		Split(t->left, a, b, key);
		t->left = b;
		t2 = t;
		Recalc(t2);
		t1 = a;
		Recalc(t1);
	}
	//на каждом шаге пересчитываем деревья
}
void Add(PTreap& t, int pos, int v) {//функция добавления в дерево
	PTreap N = new Treap;
	N->value = v;
	N->y = rand();
	N->left = nullptr;
	N->right = nullptr;
	N->count = 1;
	N->min = v;
	PTreap a, b;
	Split(t, a, b, pos);//посплитили
	t = Merge(Merge(a, N), b);//слили 3 дерева, вставив элемент
}

int FindMin(PTreap t, int l, int r) {//ищем минимум  на отрезке
	PTreap a, b, c, d;
	Split(t, a, b, l);//вырезали b
	Split(b, c, d, r - l + 1);//скорректировали b, разрезав его на с и d. c совпадает с отрезком
	int k = c->min;//смотрим минимум
	b = Merge(c, d);//мерджим обратно
	t = Merge(a, b);
	return k;
}
int main()
{
	int n;
	ifstream in("input.txt");
	ofstream out("output.txt");
	string st;
	in >> n;
	in.ignore();
	if (n == 0)
		return 0;
	int num1, num2;
	char token;
	getline(in, st);
	ReadStr(st, num1, num2, token);
	PTreap t = new Treap;//создаем первый элемент
	t->y = rand();
	t->left = nullptr;
	t->right = nullptr;
	t->count = 1;
	t->value = num2;
	t->min = num2;
	for (int i = 1; i < n; i++) {
		getline(in, st);
		ReadStr(st, num1, num2, token);
		if (token == '+') {
			Add(t, num1, num2);//добавляем
			/*Show(out, t);
			out << endl;*/
		}
		else {
			out << FindMin(t, --num1, --num2) << endl; //ищем минимуми и выводим
		}
	}
}