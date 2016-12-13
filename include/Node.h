#ifndef NODE_H
#define NODE_H

#include <fstream>
#include <utility>
#include "SDL_Local.h"

class Node
{
private:
	std::pair<uint32_t, uint32_t> colorData;
	Node *right;
	Node *left;

public:
	Node(std::pair<uint32_t, uint32_t> colorData);
	//Node(const std::pair<uint32_t, uint32_t> &colorData); //it should be like this i think
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

