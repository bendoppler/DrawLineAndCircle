#include"Header.h"
int main() {
	Ship *apollo = new ApolloSpacecraft;
	Visitor *v=new ExplodingAsteroid;
	apollo->accept(v);
	return 0;
}