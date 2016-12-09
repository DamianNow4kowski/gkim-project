#ifndef NODE_H
#define NODE_H

#include <fstream>
#include <utility>
#include "SDL_Local.h"

class Node
{
private:
	std::pair<Uint32, Uint32> colorData;
	Node *right;
	Node *left;

public:
	Node(std::pair<Uint32, Uint32> colorData);
	Node(Node *l, Node *r);
	~Node();

	friend class Huffman;
	friend struct NodeCmp;
};

struct NodeCmp
{
	bool operator() (const Node *lhs, const Node *rhs) const
	{
		return lhs->colorData.second > rhs->colorData.second;
	}
};

#endif // ! NODE_H

