#include "Node.h"
#include <iostream>

Node::Node(std::pair<Uint32, Uint32> colorData)
	: colorData(colorData), right(nullptr), left(nullptr)
{
}

Node::Node(Node * l, Node * r)
	: left(l), right(r), colorData(std::pair<Uint32, Uint32>(-1, l->colorData.second + r->colorData.second))
{
}

Node::~Node()
{
	delete right; 
	delete left;
}
