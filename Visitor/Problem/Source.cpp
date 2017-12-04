#include<iostream>
#include<string>
using namespace std;
class Ship {
public:
	virtual string getShipType() = 0;
};
class Spaceship :public Ship {
public:
	virtual string getShipType() {
		return "Spaceship";
	}
};
class ApolloSpacecraft :public Ship {
	virtual string getShipType() {
		return "ApolloSpaceCraft";
	}
};
class Collision {
public:
	virtual void collideWith(Ship* ship) = 0;
};
class Asteroid:public Collision {
	virtual void collideWith(Ship *ship) {
		cout << "Asteroid hit a spaceship\n";
	}
};
class ExplodingAsteroid :public Collision {
	virtual void collideWith(Ship *ship) {
		cout << "Exploding Asteroid hit a spaceship\n";
	}
	virtual void collideWith(ApolloSpacecraft *ship) {
		cout << "Exploding Asteroid hit a ApolloSpaceCraft\n";
	}
};

int main() {
	Collision *asteroid = new ExplodingAsteroid;
	Ship *ship = new ApolloSpacecraft;
	asteroid->collideWith(ship);
}