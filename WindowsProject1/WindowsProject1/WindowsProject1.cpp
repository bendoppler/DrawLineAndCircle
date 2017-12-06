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
	Attribute(string _name=""):name(_name) {
		
		if (name == "rotate") {
			listValue.resize(1);
		}
		else if (name == "translate") {
			listValue.resize(2);
		}
		else if (name == "fill") {
			listValue.resize(3);
		}
		else if (name == "stroke-opacity") {
			listValue.resize(1);
			listValue[0] = 1;
		}
		else if (name == "stroke") {
			listValue.resize(3);
		}
		else if (name == "stroke-width") {
			listValue.resize(1);
			listValue[0] = 1;
		}
	};
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
		
		/*Transform*/
		k = this->list[i]->getsubAttri();
		if (k.size() != 0)
		{
			for (int i = 0; i < k.size(); i++)
				list.push_back(k[i]);//rotate or translate
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
	Regtangler() { 
		addSample(this);
		list.resize(10);
		list[0] = new Attribute("x");
		list[1] = new Attribute("y");
		list[2] = new Attribute("width");
		list[3] = new Attribute("height");
		list[4] = new Attribute("stroke");
		list[5] = new Attribute("stroke-width");
		list[6] = new Attribute("stroke-opacity");
		list[7] = new Attribute("fill");
		list[8] = new Attribute("fill-opacity");
		list[9] = new Attribute("transform");
	}
	string ShapeName() { return "rect"; }
	Shape* clone() { return new Regtangler; }
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

		/*Transform*/
		k = this->list[i]->getsubAttri();
		if (k.size() != 0)
		{
			list.resize(12);
			if (k[0]->getName() == "translate") {
				list[10] = new Attribute("translate");
				list[11] = new Attribute("rotate");
			}
			else {
				list[10] = new Attribute("rotate");
				list[11] = new Attribute("translate");
			}
			if (k.size() == 2) {
				list[11]->setListValue(k[1]->getlistValue());
			}
			list[10]->setListValue(k[0]->getlistValue());
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
		Color a(attri[6][0] * 255, attri[4][0], attri[4][1], attri[4][2]);
		Pen pen(a, attri[5][0]);
		graphics.DrawRectangle(&pen, attri[0][0], attri[1][0], attri[2][0], attri[3][0]);
		SolidBrush b(Color(attri[8][0] * 255, attri[7][0], attri[7][1],attri[7][2]));

		graphics.FillRectangle(&b, attri[0][0], attri[1][0], attri[2][0], attri[3][0]);
		if (list.size() == 12) {
			if (list[10]->getName() == "translate") {
				graphics.TranslateTransform(attri[10][0], attri[10][1]);
				graphics.RotateTransform(attri[11][0]);
			}
			else {
				graphics.RotateTransform(attri[10][0]);
				graphics.TranslateTransform(attri[11][0], attri[11][1]);
			}
		}
	}
};

class Circle : public Shape
{
private:
	vector <Attribute*> list;
public:
	Circle() {
		addSample(this); 
		list.resize(9);
		list[0] = new Attribute("cx");
		list[1] = new Attribute("cy");
		list[2] = new Attribute("r");
		list[3] = new Attribute("stroke");
		list[4] = new Attribute("stroke-width");
		list[5] = new Attribute("stroke-opacity");
		list[6] = new Attribute("fill");
		list[7] = new Attribute("fill-opacity");
		list[8] = new Attribute("transform");
	}
	string ShapeName() { return "circle"; }
	Shape* clone() { return new Circle; }
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

		/*Transform*/
		k = this->list[i]->getsubAttri();
		if (k.size() != 0)
		{
			list.resize(11);
			if (k[0]->getName() == "translate") {
				list[9] = new Attribute("translate");
				list[10] = new Attribute("rotate");
			}
			else {
				list[9] = new Attribute("rotate");
				list[10] = new Attribute("translate");
			}
			if (k.size() == 2) {
				list[10]->setListValue(k[1]->getlistValue());
			}
			list[10]->setListValue(k[0]->getlistValue());
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
		Color a(attri[5][0] * 255, attri[3][0], attri[3][1], attri[3][2]);
		Pen pen(a, attri[4][0]);
		graphics.DrawEllipse(&pen, attri[0][0] - attri[2][0], attri[1][0] - attri[2][0], attri[2][0] * 2, attri[2][0] * 2);
		SolidBrush b(Color(attri[7][0] * 255, attri[6][0], attri[6][1], attri[6][2]));
		graphics.FillEllipse(&b, attri[0][0] - attri[2][0], attri[1][0] - attri[2][0], attri[2][0] * 2, attri[2][0] * 2);
		if (list.size() == 11) {
			if (list[9]->getName() == "translate") {
				graphics.TranslateTransform(attri[9][0], attri[9][1]);
				graphics.RotateTransform(attri[10][0]);
			}
			else {
				graphics.RotateTransform(attri[9][0]);
				graphics.TranslateTransform(attri[10][0], attri[10][1]);
			}
		}
	}
};

class Ellipser : public Shape
{
private:
	vector <Attribute*> list;
public:
	Ellipser() { 
		addSample(this); 
		list.resize(10);
		list[0] = new Attribute("cx");
		list[1] = new Attribute("cy");
		list[2] = new Attribute("rx");
		list[3] = new Attribute("ry");
		list[4] = new Attribute("stroke");
		list[5] = new Attribute("stroke-width");
		list[6] = new Attribute("stroke-opacity");
		list[7] = new Attribute("fill");
		list[8] = new Attribute("fill-opacity");
		list[9] = new Attribute("transform");
	}
	string ShapeName() { return "ellipse"; }
	Shape* clone() { return new Ellipser; }
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

		/*Transform*/
		k = this->list[i]->getsubAttri();
		if (k.size() != 0)
		{
			list.resize(12);
			if (k[0]->getName() == "translate") {
				list[10] = new Attribute("translate");
				list[11] = new Attribute("rotate");
			}
			else {
				list[10] = new Attribute("rotate");
				list[11] = new Attribute("translate");
			}
			if (k.size() == 2) {
				list[11]->setListValue(k[1]->getlistValue());
			}
			list[10]->setListValue(k[0]->getlistValue());
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
		Color a(attri[6][0] * 255, attri[4][0], attri[4][1], attri[4][2]);
		Pen pen(a, attri[5][0]);
		graphics.DrawEllipse(&pen, attri[0][0], attri[1][0], attri[2][0]*2, attri[3][0]*2);
		SolidBrush b(Color(attri[8][0] * 255, attri[7][0], attri[7][1], attri[7][2]));

		graphics.FillEllipse(&b, attri[0][0], attri[1][0], attri[2][0]*2, attri[3][0]*2);
		if (list.size() == 12) {
			if (list[10]->getName() == "translate") {
				graphics.TranslateTransform(attri[10][0], attri[10][1]);
				graphics.RotateTransform(attri[11][0]);
			}
			else {
				graphics.RotateTransform(attri[10][0]);
				graphics.TranslateTransform(attri[11][0], attri[11][1]);
			}
		}
	}
};

class Polyliner : public Shape
{
private:
	vector <Attribute*> list;
public:
	Polyliner() { 
		addSample(this);
		list.resize(7);
		list[0] = new Attribute("points");
		list[1] = new Attribute("stroke");
		list[2] = new Attribute("stroke-width");
		list[3] = new Attribute("stroke-opacity");
		list[4] = new Attribute("fill");
		list[5] = new Attribute("fill-opacity");
		list[6] = new Attribute("transform");
	}
	string ShapeName() { return "polyline"; }
	Shape* clone() { return new Polyliner; }
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

		/*Transform*/
		k = this->list[i]->getsubAttri();
		if (k.size() != 0)
		{
			list.resize(9);
			if (k[0]->getName() == "translate") {
				list[7] = new Attribute("translate");
				list[8] = new Attribute("rotate");
			}
			else {
				list[7] = new Attribute("rotate");
				list[8] = new Attribute("translate");
			}
			if (k.size() == 2) {
				list[8]->setListValue(k[1]->getlistValue());
			}
			list[7]->setListValue(k[0]->getlistValue());
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
		vector <PointF> points;
		PointF tmp;
		for (int i = 0; i < attri[0].size(); i += 2) {
			tmp.X = attri[0][i];
			tmp.Y = attri[0][i + 1];
			points.push_back(tmp);
		}
		Color a(attri[3][0] * 255, attri[1][0], attri[1][1], attri[1][2]);
		Pen pen(a, attri[2][0]);
		GraphicsPath myPath;
		myPath.AddLines(&points[0],points.size());
		graphics.DrawPath(&pen, &myPath);
		SolidBrush b(Color(attri[5][0] * 255, attri[4][0], attri[4][1], attri[4][2]));
		graphics.FillPath(&b, &myPath);
		if (list.size() == 9) {
			if (list[7]->getName() == "translate") {
				graphics.TranslateTransform(attri[7][0], attri[7][1]);
				graphics.RotateTransform(attri[8][0]);
			}
			else {
				graphics.RotateTransform(attri[7][0]);
				graphics.TranslateTransform(attri[8][0], attri[8][1]);
			}
		}
	}
};

class Polygone : public Shape
{
private:
	vector <Attribute*> list;
public:
	Polygone() {
		addSample(this);
		addSample(this);
		list.resize(7);
		list[0] = new Attribute("points");
		list[1] = new Attribute("stroke");
		list[2] = new Attribute("stroke-width");
		list[3] = new Attribute("stroke-opacity");
		list[4] = new Attribute("fill");
		list[5] = new Attribute("fill-opacity");
		list[6] = new Attribute("transform");
	}
	string ShapeName() { return "polygon"; }
	Shape* clone() { return new Polygone; }
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

		/*Transform*/
		k = this->list[i]->getsubAttri();
		if (k.size() != 0)
		{
			list.resize(9);
			if (k[0]->getName() == "translate") {
				list[7] = new Attribute("translate");
				list[8] = new Attribute("rotate");
			}
			else {
				list[7] = new Attribute("rotate");
				list[8] = new Attribute("translate");
			}
			if (k.size() == 2) {
				list[8]->setListValue(k[1]->getlistValue());
			}
			list[7]->setListValue(k[0]->getlistValue());
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
		vector <PointF> points;
		PointF tmp;
		for (int i = 0; i < attri[0].size(); i += 2) {
			tmp.X = attri[0][i];
			tmp.Y = attri[0][i + 1];
			points.push_back(tmp);
		}
		Color a(attri[3][0] * 255, attri[1][0], attri[1][1], attri[1][2]);
		Pen pen(a, attri[2][0]);
		graphics.DrawPolygon(&pen, &points[0], points.size());
		SolidBrush b(Color(attri[5][0] * 255, attri[4][0], attri[4][1], attri[4][2]));
		graphics.FillPolygon(&b, &points[0], points.size());
		if (list.size() == 9) {
			if (list[7]->getName() == "translate") {
				graphics.TranslateTransform(attri[7][0], attri[7][1]);
				graphics.RotateTransform(attri[8][0]);
			}
			else {
				graphics.RotateTransform(attri[7][0]);
				graphics.TranslateTransform(attri[8][0], attri[8][1]);
			}
		}
	}
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
	listValue.clear();
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
