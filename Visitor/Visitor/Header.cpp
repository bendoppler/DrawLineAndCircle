#include "Header.h"

void Spaceship::accept(Visitor * v)
{
	v->visit(this);
}

void ApolloSpacecraft::accept(Visitor * v)
{
	v->visit(this);
}
