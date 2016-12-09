#ifndef NODE_H
#define NODE_H

#include <fstream>
#include <utility>
#include "SDL_Local.h"

class Node
{
public:
	std::pair<Uint32, Uint32> colorData;
	Node *right;
	Node *left;

	Node(std::pair<Uint32, Uint32> colorData)
		: colorData(colorData), right(nullptr), left(nullptr) {}

	Node(Node *l, Node *r)
		: left(l), right(r)
	{
		std::pair<Uint32, Uint32> clrData;
		clrData.first = -1;
		clrData.second = l->colorData.second + r->colorData.second;
		colorData = clrData;
	}

	~Node() 
		{ delete right; delete left; }
};

struct NodeCmp
{
	bool operator() (const Node *lhs, const Node *rhs) const
	{
		return lhs->colorData.second > rhs->colorData.second;
	}
};

#endif // ! NODE_H

