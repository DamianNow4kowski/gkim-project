#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include "Node.h"
#include "SingleColorData.h"

template<class T>
class Tree
{
private:
	Node<T> *root;

public:
	Tree();
	Tree(Node<T> &node);
	Node<T> *getRoot() const;
	void insert(Node<T> *parent, const T &data, const bool &left);
	void insert(Node<T> *parent, Node<T> &node, const bool &left);
	void display_preorder(Node<T> *node, bool left, std::string bin);
	void setRoot(Node<T> &node);

	// Przeciazony operator strumienia wyjsciowego
	// Wypisuje obiekt Tree.
	//friend std::ostream &operator<<(std::ostream &out, Tree<T> &tree)
	//{
	//	tree.display_preorder(tree.getRoot(), true, "");
	//	return out;
	//}

	// Przeciazony operator '<'
	// Porownuje dwa obiekty klasy Tree (porownuje dwa wezly)
	// Zwraca true - gdy wartosc pierwszego korzenia jest mniejsza od druiego
	// zwraca false - w przeciwnym wypadku
	friend bool operator<(const Tree<T> &tree1, const Tree<T> &tree2)
	{
		return tree1.getRoot()->getVar() < tree2.getRoot()->getVar();
	}

	// Przeciazony operator '+'
	// Dodaje dwa obiekty klasy Tree oraz zwraca nowy obiekt Tree bedacy ich suma.
	friend Tree<T> &operator+(const Tree &tree1, const Tree &tree2)
	{
		Node<T> *n = new Node<T>(*(tree1.getRoot()) + *(tree2.getRoot()));
		Tree<T> *t = new Tree<T>(*n);
		if (tree1 < tree2)
		{
			t->insert(t->getRoot(), *tree1.getRoot(), true);
			t->insert(t->getRoot(), *tree2.getRoot(), false);
		}
		else
		{
			t->insert(t->getRoot(), *tree1.getRoot(), false);
			t->insert(t->getRoot(), *tree2.getRoot(), true);
		}
		return *t;
	}
};

class TreesCmp
{
public:
	bool operator()(const Tree<SingleColorData>* lhs, const Tree<SingleColorData>* rhs) const
	{
		return lhs->getRoot()->getVar().counter > rhs->getRoot()->getVar().counter;
	}
};

#endif // !TREE_H

