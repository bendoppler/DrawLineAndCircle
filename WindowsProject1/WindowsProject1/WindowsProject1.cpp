// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#define MAX_LOADSTRING 100
#pragma once

#include "resource.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_iterators.hpp"
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <set>
using namespace std;
using namespace rapidxml;

class Attribute
{
private:
	string name;
	string value;
	vector<float> listValue;
	vector<Attribute*> subAttri;
public:
	Attribute(string _name=""):name(_name) {};
	void setName(string names) { name = names; }
	void setValue(string values);
	void setListValue(vector<float> a) { listValue = a; }
	string getName() { return name; }
	vector<Attribute*> getsubAttri() { return subAttri; }
	vector<float> getlistValue() { return listValue; }
};

class Shape
{
	static vector<Shape*> obj;
protected:
	static void addSample(Shape* s);
public:
	static Shape* create(string name);
	virtual string ShapeName() = 0;
	virtual Shape* clone() = 0;
	virtual void input(string names, string values) {}
	virtual void output() {};
	virtual void draw(HDC) {};
};

class Line : public Shape
{
private:
	vector <Attribute*> list;
public:
	Line() { addSample(this);
	list.resize(7);
	list[0] = new Attribute("x1");
	list[1] = new Attribute("y1");
	list[2] = new Attribute("x2");
	list[3] = new Attribute("y2");
	list[4] = new Attribute("stroke");
	list[5] = new Attribute("stroke-width");
	list[6] = new Attribute("stroke-opacity");
	}
	string ShapeName() { return "line"; }
	Shape* clone() { return new Line; }
	void input(string names, string values)
	{
		vector<Attribute*> k;
		int i;
		for (i = 0; i<list.size(); i++)
			if (names == this->list[i]->getName())
			{
				this->list[i]->setValue(values);
				break;
			}

		/*x->setName(names);
		x->setValue(values);*/
		k = this->list[i]->getsubAttri();
		if (k.size() != 0)
		{
			for (int i = 0; i < k.size(); i++)
				list.push_back(k[i]);
		}
	}
	void output();
	void draw(HDC hdc) {
		Graphics graphics(hdc);
		graphics.SetSmoothingMode(SmoothingModeHighQuality);
		vector<vector<float>> attri(list.size());
		for (int i = 0; i < list.size(); ++i) {
			attri[i] = list[i]->getlistValue();
		}
		Color a(attri[6][0]*255, attri[4][0], attri[4][1], attri[4][2]);
		Pen pen(a, attri[5][0]);
		PointF a1(attri[0][0], attri[1][0]);
		PointF a2(attri[2][0], attri[3][0]);
		graphics.DrawLine(&pen, a1, a2);
	}

};

class Regtangler : public Shape
{
private:
	vector <Attribute*> list;
public:
	Regtangler() { addSample(this); }
	string ShapeName() { return "rect"; }
	Shape* clone() { return new Regtangler; }
	void input(string names, string values)
	{
		vector<Attribute*> k;
		Attribute* x = new Attribute;
		x->setName(names);
		x->setValue(values);
		k = x->getsubAttri();
		if (k.size() != 0)
		{
			for (int i = 0; i < k.size(); i++)
				list.push_back(k[i]);
		}
		else list.push_back(x);
	}
	void output();
};

class Circle : public Shape
{
private:
	vector <Attribute*> list;
public:
	Circle() { addSample(this); }
	string ShapeName() { return "circle"; }
	Shape* clone() { return new Circle; }
	void input(string names, string values)
	{
		vector<Attribute*> k;
		Attribute* x = new Attribute;
		x->setName(names);
		x->setValue(values);
		k = x->getsubAttri();
		if (k.size() != 0)
		{
			for (int i = 0; i < k.size(); i++)
				list.push_back(k[i]);
		}
		else list.push_back(x);
	}
	void output();
};

class Ellipser : public Shape
{
private:
	vector <Attribute*> list;
public:
	Ellipser() { addSample(this); }
	string ShapeName() { return "ellipse"; }
	Shape* clone() { return new Ellipser; }
	void input(string names, string values)
	{
		vector<Attribute*> k;
		Attribute* x = new Attribute;
		x->setName(names);
		x->setValue(values);
		k = x->getsubAttri();
		if (k.size() != 0)
		{
			for (int i = 0; i < k.size(); i++)
				list.push_back(k[i]);
		}
		else list.push_back(x);
	}
	void output();
};

class Polyliner : public Shape
{
private:
	vector <Attribute*> list;
public:
	Polyliner() { addSample(this); }
	string ShapeName() { return "polyline"; }
	Shape* clone() { return new Polyliner; }
	void input(string names, string values)
	{
		vector<Attribute*> k;
		Attribute* x = new Attribute;
		x->setName(names);
		x->setValue(values);
		k = x->getsubAttri();
		if (k.size() != 0)
		{
			for (int i = 0; i < k.size(); i++)
				list.push_back(k[i]);
		}
		else list.push_back(x);
	}
	void output();
};

class Polygone : public Shape
{
private:
	vector <Attribute*> list;
public:
	Polygone() { addSample(this); }
	string ShapeName() { return "polygon"; }
	Shape* clone() { return new Polygone; }
	void input(string names, string values)
	{
		vector<Attribute*> k;
		Attribute* x = new Attribute;
		x->setName(names);
		x->setValue(values);
		k = x->getsubAttri();
		if (k.size() != 0)
		{
			for (int i = 0; i < k.size(); i++)
				list.push_back(k[i]);
		}
		else list.push_back(x);
	}
	void output();
};

class ShapeList
{
private:
	vector<Shape*> sl;
public:
	ShapeList() {
	}
	void in();
	void out();
	void draw(HDC hdc) {
		for (int i = 0; i < sl.size(); ++i) {
			sl[i]->draw(hdc);
		}
	}
};


void ShapeList::in()
{
	Shape* s;
	xml_document<> doc;
	xml_node<>* root_node;

	ifstream file;
	file.open("test.svg");
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("svg");
	for (xml_node<> * node = root_node->first_node(); node; node = node->next_sibling())
	{
		xml_attribute<>* sub = node->first_attribute();
		s = Shape::create(node->name());
		while (sub != NULL)
		{
			s->input(sub->name(), sub->value());
			sub = sub->next_attribute();
		}
		sl.push_back(s);
	}
	file.close();
}

vector<Shape*> Shape::obj;

void Shape::addSample(Shape* sub)
{
	if (sub == NULL)
		return;
	int pos = obj.size();
	if (pos == 0) obj.push_back(sub);
	else
	{
		while (pos >= 0)
		{
			pos -= 1;
			if (pos == -1) break;
			Shape* tmp = obj[pos];
			if (sub->ShapeName() == tmp->ShapeName())
				break;
		}
		if (pos < 0) obj.push_back(sub);
	}
}

Shape* Shape::create(string name)
{
	if (name == "")
		return NULL;
	int pos = obj.size();
	while (pos >= 0)
	{
		pos -= 1;
		Shape* tmp = obj[pos];
		if (tmp == NULL)
			continue;
		if (name == obj[pos]->ShapeName())
			break;
	}
	if (pos >= 0) return obj[pos]->clone();
	else return NULL;

}

void Polygone::output()
{
	vector<float> x;
	for (int i = 0; i < list.size(); i++)
	{
		cout << list[i]->getName() << " ";
		x = list[i]->getlistValue();
		for (int j = 0; j < x.size(); j++)
			cout << x[j] << " ";
	}
	cout << endl;
}

void Polyliner::output()
{
	vector<float> x;
	for (int i = 0; i < list.size(); i++)
	{
		cout << list[i]->getName() << " ";
		x = list[i]->getlistValue();
		for (int j = 0; j < x.size(); j++)
			cout << x[j] << " ";
	}
	cout << endl;
}

void Ellipser::output()
{
	vector<float> x;
	for (int i = 0; i < list.size(); i++)
	{
		cout << list[i]->getName() << " ";
		x = list[i]->getlistValue();
		for (int j = 0; j < x.size(); j++)
			cout << x[j] << " ";
	}
	cout << endl;
}

void Circle::output()
{
	vector<float> x;
	for (int i = 0; i < list.size(); i++)
	{
		cout << list[i]->getName() << " ";
		x = list[i]->getlistValue();
		for (int j = 0; j < x.size(); j++)
			cout << x[j] << " ";
	}
	cout << endl;
}

void Regtangler::output()
{
	vector<float> x;
	for (int i = 0; i < list.size(); i++)
	{
		cout << list[i]->getName() << " ";
		x = list[i]->getlistValue();
		for (int j = 0; j < x.size(); j++)
			cout << x[j] << " ";
	}
	cout << endl;
}

void Line::output()
{
	vector<float> x;
	for (int i = 0; i < list.size(); i++)
	{
		cout << list[i]->getName() << " ";
		x = list[i]->getlistValue();
		for (int j = 0; j < x.size(); j++)
			cout << x[j] << " ";
	}
	cout << endl;
}



void ShapeList::out()
{
	for (int i = 0; i < sl.size(); i++)
		sl[i]->output();
	cout << endl;
}

void Attribute::setValue(string values)
{
	Attribute* k;
	value = values;
	float t;
	string c, x;
	if (name == "stroke" || name == "fill" || name == "transform" || name == "points")
	{
		for (int i = 0; i < values.length(); i++)
		{
			if (value[i] >= 97 && value[i] <= 122)  c.push_back(value[i]);
			while ((value[i] >= 48 && value[i] <= 57) || value[i] == 45)
			{
				x.push_back(value[i]);
				i++;
				if ((value[i] < 48 && value[i] != 45) || value[i] > 57)
				{
					t = stof(x);
					listValue.push_back(t);
					x = "";
				}
			}
			if (value[i] == 41 && c != "" && c != "rgb")
			{
				k = new Attribute;
				k->setName(c);
				k->setListValue(listValue);
				subAttri.push_back(k);
				c = "";
			}
		}
	}
	else
	{
		t = stof(value);
		listValue.push_back(t);
	}
}
VOID OnPaint(HDC hdc)
{
	ShapeList sl;
	Line a;
	Circle b;
	Regtangler c;
	Polygone d;
	Polyliner e;
	Ellipser f;
	sl.in();
	sl.draw(hdc);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("Getting Started"),  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		CW_USEDEFAULT,            // initial x size
		CW_USEDEFAULT,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} // WndProc
