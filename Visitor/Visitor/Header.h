#pragma once
#include<iostream>
#include<string>
using namespace std;
class Ship {
public:
	virtual string getShipType() = 0;
	virtual void accept(class Visitor* v) = 0;
};
class Spaceship :public Ship {
public:
	virtual string getShipType() {
		return "Spaceship";
	}
	virtual void accept(Visitor* v); 
		

};
class ApolloSpacecraft :public Ship {
	virtual string getShipType() {
		return "ApolloSpaceCraft";
	}
	void accept(Visitor* v); 
};
class Visitor {
public:
	virtual void visit(Spaceship* ship) = 0;
	virtual void visit(ApolloSpacecraft* apollo) = 0;
};
class Asteroid :public Visitor {
public:
	void visit(Spaceship* ship) {
		cout << "Asteroid hit a spaceship\n";
	}
	void visit(ApolloSpacecraft* apollo) {
		cout << "Asteroid hit a ApolloSpaceCraft\n";
	}
};
class ExplodingAsteroid :public Visitor {
public:
	void visit(Spaceship* ship) {
		cout << "Exploding Asteroid hit a spaceship\n";
	}
	void visit(ApolloSpacecraft* apollo) {
		cout << "Exploding Asteroid hit an ApolloSpaceCraft\n";
	}
};
