#include "List.h"

template class List<Tree<SingleColorData>>;

template<class T>
List<T>::List()
{
	this->head = nullptr;
	this->tail = nullptr;
	this->size = 0;
}

template<class T>
List<T>::~List()
{
	Node<T> *p = this->head;
	while (p != nullptr)
	{
		this->head = this->head->next;
		delete p;
		p = this->head;
	}
}

template<class T>
void List<T>::printList() const
{
	Node<T> *p = this->head;
	while (p != nullptr)
	{
		std::cout << p->var << " ";
		p = p->next;
	}
}

template<class T>
void List<T>::r_printList() const
{
	Node<T> *p = this->tail;
	while (p != nullptr)
	{
		std::cout << p->var << " ";
		p = p->prev;
	}
}

template<class T>
void List<T>::addFirst(const T &var)
{
	Node<T> *p = new Node<T>(var);
	if (this->head == nullptr)
		this->head = this->tail = p;
	else
	{
		p->next = this->head;
		this->head->prev = p;
		this->head = p;
	}
	this->size++;
}

template<class T>
void List<T>::addLast(const T &var)
{
	Node<T> *p = new Node<T>(var);
	if (this->tail == nullptr)
		this->tail = this->head = p;
	else
	{
		p->prev = this->tail;
		this->tail->next = p;
		this->tail = p;
	}
	this->size++;
}

template<class T>
void List<T>::insertQueue(const T &var)
{
	int position = checkPosition(var);
	if (position <= 0)
	{
		this->addFirst(var);
		return;
	}
	else if (position >= this->getSize())
	{
		this->addLast(var);
		return;
	}

	Node<T> *p = this->head;
	Node<T> *node = new Node<T>(var);
	for (int i = 0; i < position - 1; i++)
		p = p->next;

	node->next = p->next;
	p->next->prev = node;
	p->next = node;
	node->prev = p;
	this->size++;
}

template<class T>
int List<T>::checkPosition(const T &var)
{
	int position = 0;
	Node<T> *p = this->head;
	while (p != nullptr && p->var < var)
	{
		p = p->next;
		position++;
	}
	return position;
}

template<class T>
int List<T>::getSize() const
{
	return this->size;
}

template<class T>
bool List<T>::empty() const
{
	return !(this->size > 0);
}

template<class T>
void List<T>::remFirst()
{
	if (!this->size)
		return;
	Node<T> *p = this->head;
	this->head->next->prev = nullptr;
	this->head = this->head->next;
	this->size--;
	delete p;
}

template<class T>
void List<T>::remLast()
{
	if (!this->size)
		return;
	Node<T>*p = this->tail;
	this->tail->prev->next = nullptr;
	this->tail = this->tail->prev;
	this->size--;
	delete p;
}

template<class T>
T List<T>::getItem(const int &position) const
{
	if (position > this->size)
		return this->tail->var;
	else if (position < 0)
		return this->head->var;

	Node<T> *p = this->head;
	for (int i = 0; i < position; i++)
		p = p->next;
	return p->var;
}

template<class T>
void List<T>::insert(const T& var, const int &position)
{
	Node<T> *p = this->head;
	Node<T> *node = new Node<T>(var);
	for (int i = 0; i < position - 1; i++)
		p = p->next;

	node->next = p->next;
	p->next->prev = node;
	p->next = node;
	node->prev = p;
	this->size++;
}

template<class T>
void List<T>::remove(const int &position)
{
	if (position <= 0)
	{
		this->remFirst();
		return;
	}
	else if (position >= this->size - 1)
	{
		this->remLast();
		return;
	}

	Node<T> *p = this->head;
	for (int i = 0; i < position - 1; i++)
		p = p->next;

	Node<T> *tmp = p->next;
	p->next = tmp->next;
	p->next->prev = p;

	delete tmp;
}
