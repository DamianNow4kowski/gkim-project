#ifndef NODE_H
#define NODE_H

#include <fstream>

template<typename T>
class List;

template<typename T>
class Tree;

template<class T>
class Node
{
private:
	Node<T> *next;
	Node<T> *prev;
	T var;

public:
	Node();
	Node(const T &var);
	void setVar(const T &var);
	T getVar() const;

	friend std::ostream& operator<<(std::ostream &op, Node<T> &node)
	{
		op << node.var;
		return op;
	}

	friend Node<T>& operator+(Node<T> node1, Node<T> node2)
	{
		Node<T>*n = new Node<T>(node1.var + node2.var);
		return *n;
	}

	friend class List<T>;
	friend class Tree<T>;
};

template<class T>
Node<T>::Node()
{
	this->next = nullptr;
	this->prev = nullptr;
}

template<class T>
Node<T>::Node(const T &var)
{
	this->var = var;
	this->next = nullptr;
	this->prev = nullptr;
}

template<class T>
void Node<T>::setVar(const T &var)
{
	this->var = var;
}

template<class T>
T Node<T>::getVar() const
{
	return this->var;
}

#endif // ! NODE_H

