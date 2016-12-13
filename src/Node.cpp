#include "Node.h"
#include <iostream>

Node::Node(std::pair<uint32_t, uint32_t> colorData)
	: colorData(colorData), right(nullptr), left(nullptr)
{
}

Node::Node(Node * l, Node * r)
	: left(l), right(r), colorData(std::pair<uint32_t, uint32_t>(-1, l->colorData.second + r->colorData.second))
{
}

Node::~Node()
{
	delete right; 
	delete left;
}
