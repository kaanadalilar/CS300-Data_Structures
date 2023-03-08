#ifndef _STACK_H
#define _STACK_H
#include <iostream>

using namespace std;

template <class Object>
class Stack{
public:
	Stack();
	~Stack();

	bool isEmpty() const;
	void makeEmpty();

	void pop();
	void push(const Object &);
	Object topAndPop();
	const Object & top() const;

private:
	struct ListNode{
		Object element;
        ListNode* next;
		ListNode(const Object& theElement, ListNode* n = NULL)
			:element(theElement), next(n){}
	};
	ListNode *topOfStack;  //list itself is the stack
}; 
#endif