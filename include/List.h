#ifndef LIST_H
#define LIST_H

#include <iostream>
#include "Tree.h"

template<class T>
class List
{
private:
	Node<T> *head;
	Node<T> *tail;
	int size;

public:
	List();
	~List();
	bool empty() const;
	int getSize() const;
	void printList() const;
	void r_printList() const;
	void addFirst(const T &var);
	void addLast(const T &var);
	void insertQueue(const T &var);
	void insert(const T &var, const int &position);
	int checkPosition(const T &var);
	void remFirst();
	void remLast();
	void remove(const int &position);
	T getItem(const int &position) const;
};

#endif // !LIST_H