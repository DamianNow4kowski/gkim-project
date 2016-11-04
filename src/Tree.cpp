#include "Tree.h"

template class Tree<SingleColorData>;

template<class T>
Tree<T>::Tree()
{
	this->root = nullptr;
}

template<class T>
Tree<T>::Tree(Node<T> &node)
{
	this->root = &node;
}


template<class T>
void Tree<T>::setRoot(Node<T>& node)
{
	this->root = &node;
}

template<class T>
Node<T> *Tree<T>::getRoot() const
{
	return this->root;
}

template<class T>
void Tree<T>::insert(Node<T> *parent, const T &data, const bool &left)
{
	Node<T> *p = new Node<T>(data);
	if (left)
		parent->prev = p;
	else
		parent->next = p;
}

template<class T>
void Tree<T>::insert(Node<T> *parent, Node<T> &node, const bool &left)
{
	if (left)
		parent->prev = &node;
	else
		parent->next = &node;
}

template<class T>
void Tree<T>::display_preorder(Node<T> *node, bool left, std::string bin)
{
	if (node == nullptr)
		return;
	if (left)
		bin += "0";
	else
		bin += "1";
	if (node->next == nullptr && node->prev == nullptr)
		std::cout << node->var << " " << bin << std::endl;
	display_preorder(node->prev, true, bin);
	display_preorder(node->next, false, bin);
}

