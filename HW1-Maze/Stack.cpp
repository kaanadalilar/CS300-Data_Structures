#include "Stack.h"
#include <iostream>
using namespace std;

template <class Object>
Stack<Object>::Stack(){
	topOfStack = NULL;
}
template <class Object>
Stack<Object>::~Stack(){
	makeEmpty( );
}

template <class Object>
bool Stack<Object>::isEmpty() const{
	return topOfStack == NULL;
}

template <class Object>
const Object & Stack<Object>::top() const{
	if (!isEmpty())
		return topOfStack->element;
}

template <class Object>
void Stack<Object>::pop(){
	if (isEmpty())
		return;
	ListNode* oldTop = topOfStack;
	topOfStack = topOfStack->next;
	delete oldTop;
}

template <class Object>
void Stack<Object>::push(const Object & x){
    topOfStack = new ListNode(x, topOfStack);
}

template <class Object>
Object Stack<Object>::topAndPop(){
	Object topItem = top();
	pop();
	return topItem;
}

template <class Object>
void Stack<Object>::makeEmpty(){
while (!isEmpty())
	pop();
}