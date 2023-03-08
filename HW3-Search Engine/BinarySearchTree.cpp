#include "BinarySearchTree.h"
#include <iostream>
using namespace std;

template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable & notFound) 
	:ITEM_NOT_FOUND( notFound ), root( NULL )
{}

template <class Comparable>
BinaryWordNode<Comparable>* BinarySearchTree<Comparable>::getRoot() const{
	return root;
}

template <class Comparable>
void BinarySearchTree<Comparable>::printTree(BinaryWordNode<Comparable>* t) const{
	if ( t != NULL ){
		printTree( t->left );
		cout << t->word << endl;
		printTree( t->right );
	}
	else
		return;
}

template <class Comparable>
int BinarySearchTree<Comparable>::numberOfNodes(BinaryWordNode<Comparable>* t) const{
	static int count = 0;
	if ( t != NULL ){
		numberOfNodes( t->left );
		count++;
		numberOfNodes( t->right );
	}
	else
		return 0;
	return count;
}

template <class Comparable>
void BinarySearchTree<Comparable>::insertWord( const Comparable & x, const string & docName){
	insertWordPriv( x, docName, root );
}

template <class Comparable>
void BinarySearchTree<Comparable>::insertWordPriv( const Comparable & x, const string & docName, BinaryWordNode<Comparable>* & t ) const{
	if( t == NULL ) //create a new node at the right place
		t = new BinaryWordNode<Comparable>( x, docName, NULL, NULL);
	else if( x < t->word )
		insertWordPriv( x, docName, t->left ); //insert at the left or 
	else if( t->word < x )
		insertWordPriv( x, docName, t->right ); //right subtree
	else
		return;
}

template <class Comparable>
BinaryWordNode<Comparable>* BinarySearchTree<Comparable>::findWordNode( const Comparable & x ) const{
	BinaryWordNode<Comparable>* temp = root;
	while(temp != NULL){
		if( x < temp->word )
			temp = temp->left;
		else if(temp->word < x)
			temp = temp->right;
		else
			return temp;
	}
	return NULL;
}

template <class Comparable>
void BinarySearchTree<Comparable>::removeWord( const Comparable & x ){
	removeWordPriv( x, root );
}

template <class Comparable>
void BinarySearchTree<Comparable>::removeWordPriv( const Comparable & x, BinaryWordNode<Comparable>* & t ) const{
	if( t == NULL )
		return;   //Item not found; do nothing
	if( x < t->word )
		removeWordPriv( x, t->left );
	else if( t->word < x )
		removeWordPriv( x, t->right );
	else if( t->left != NULL && t->right != NULL ) //Two children
	{
		t->word = findMinWord( t->right )->word;
		removeWordPriv( t->word, t->right );
	}
	else //one or no children
	{
		BinaryWordNode<Comparable> *oldNode = t;
		t = ( t->left != NULL ) ? t->left : t->right;
		delete oldNode;
	}
}  

template <class Comparable>
BinaryWordNode<Comparable> * BinarySearchTree<Comparable>::findMinWord( BinaryWordNode<Comparable>* t ) const{
	if( t == NULL )
		return NULL;
	if( t->left == NULL )
		return t;
	return findMinWord( t->left );
}

//BELOW THREE FUNCTIONS: DESTRUCTING THE SECTIONS TREE AND ITEM OF ITS SECTIONS
template <class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree( ){
	makeEmpty( root );
}

template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( BinaryWordNode<Comparable> * & t ) const{
	if( t != NULL ){
		makeEmpty( t->left );
		makeEmpty( t->right );
		delete t;
	}
	t = NULL;
}