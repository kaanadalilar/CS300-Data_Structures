#include "BinarySearchTree.h"
#include <iostream>
using namespace std;

template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable & notFound) 
	:ITEM_NOT_FOUND( notFound ), root( NULL )
{}

template <class Comparable>
BinarySectionNode<Comparable>* BinarySearchTree<Comparable>::getRoot() const{
	return root;
}

template <class Comparable>
void BinarySearchTree<Comparable>::printTree(BinarySectionNode<Comparable>* t) const{
	if ( t != NULL ){
		printTree( t->left );
		cout << t->secElem << endl;
		printTree( t->right );
	}
	else
		return;
}

template <class Comparable>
void BinarySearchTree<Comparable>::printItems(BinaryItemNode<Comparable>* t) const{
	if ( t != NULL ){
		printItems( t->left );
		cout << t->itTitle << endl;
		printItems( t->right );
	}
	else
		return;
}

template <class Comparable>
void BinarySearchTree<Comparable>::insertSection( const Comparable & x ){
	insertSec( x, root );
}

template <class Comparable>
void BinarySearchTree<Comparable>::insertSec( const Comparable & x, BinarySectionNode<Comparable>* & t ) const{
	if( t == NULL ) //create a new node at the right place
		t = new BinarySectionNode<Comparable>( x, NULL, NULL, NULL);
	else if( x < t->secElem )
		insertSec( x, t->left ); //insert at the left or 
	else if( t->secElem < x )
		insertSec( x, t->right ); //right subtree
	else
		return;
}

template <class Comparable>
void BinarySearchTree<Comparable>::insertItem( const Comparable & x,const string & y, BinaryItemNode<Comparable>* & items ){
	insertIt( x, y, items );
}

template <class Comparable>
void BinarySearchTree<Comparable>::insertIt( const Comparable & x,const string & y, BinaryItemNode<Comparable>* & t ) const{
	if( t == NULL ) //create a new node at the right place
		t = new BinaryItemNode<Comparable>( x, y, NULL, NULL);
	else if( x < t->itTitle )
		insertIt( x, y, t->left );  //insert at the left or 
	else if( t->itTitle < x )
		insertIt( x, y, t->right );  //right subtree
	else
		return;
}

template <class Comparable>
BinarySectionNode<Comparable>* BinarySearchTree<Comparable>::findSecNode( const Comparable & x ) const{
	BinarySectionNode<Comparable>* temp = root;
	while(temp != NULL){
		if( x < temp->secElem )
			temp = temp->left;
		else if(temp->secElem < x)
			temp = temp->right;
		else
			return temp;
	}
	return NULL;
}

template <class Comparable>
BinaryItemNode<Comparable>* BinarySearchTree<Comparable>::findSecItem( const Comparable & x, BinaryItemNode<Comparable>* t) const{
	BinaryItemNode<Comparable>* temp = t;
	while(temp != NULL){
		if( x < temp->itTitle )
			temp = temp->left;
		else if(temp->itTitle < x)
			temp = temp->right;
		else
			return temp;
	}
	return NULL;
}

template <class Comparable>
void BinarySearchTree<Comparable>::removeSection( const Comparable & x ){
	removeSec( x, root );
}

template <class Comparable>
void BinarySearchTree<Comparable>::removeSec( const Comparable & x, BinarySectionNode<Comparable>* & t ) const{
	if( t == NULL )
		return;   //Item not found; do nothing
	if( x < t->secElem )
		removeSec( x, t->left );
	else if( t->secElem < x )
		removeSec( x, t->right );
	else if( t->left != NULL && t->right != NULL ) //Two children
	{
		t->secItems = findSecMin(t->right)->secItems;
		t->secElem = findSecMin( t->right )->secElem;
		removeSec( t->secElem, t->right );
	}
	else //one or no children
	{
		BinarySectionNode<Comparable> *oldNode = t;
		t = ( t->left != NULL ) ? t->left : t->right;
		delete oldNode;
	}
}  

template <class Comparable>
BinarySectionNode<Comparable> * BinarySearchTree<Comparable>::findSecMin( BinarySectionNode<Comparable>* t ) const
{
	if( t == NULL )
		return NULL;
	if( t->left == NULL )
		return t;
	return findSecMin( t->left );
}

template <class Comparable>
BinaryItemNode<Comparable> * BinarySearchTree<Comparable>::findItMin( BinaryItemNode<Comparable>* t ) const{
	if( t == NULL )
		return NULL;
	if( t->left == NULL )
		return t;
	return findItMin( t->left );
}

template <class Comparable>
void BinarySearchTree<Comparable>::removeItem( const Comparable & x, BinaryItemNode<Comparable> * & ptr ){
	removeIt( x, ptr );
}

template <class Comparable>
void BinarySearchTree<Comparable>::removeIt( const Comparable & x, BinaryItemNode<Comparable> * & t ) const{
	if( t == NULL )
		return;   // Item not found; do nothing
	if( x < t->itTitle )
		removeIt( x, t->left );
	else if( t->itTitle < x )
		removeIt( x, t->right );
	else if( t->left != NULL && t->right != NULL ) // Two children
	{
		t->itInfo = findItMin(t)->itInfo;;
		t->itTitle = findItMin(t)->itTitle;
		removeIt( t->itTitle, t->right );
	}
	else // one or no children
	{
		BinaryItemNode<Comparable> *oldNode = t;
		t = ( t->left != NULL ) ? t->left : t->right;
		delete oldNode;
	}
}

//BELOW THREE FUNCTIONS: DESTRUCTING THE SECTIONS TREE AND ITEM OF ITS SECTIONS
template <class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree( ){
	makeEmpty( root );
}

template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( BinarySectionNode<Comparable> * & t ) const{
	BinaryItemNode<Comparable>* temp = t->secItems;
	if( t != NULL ){
		makeEmpty( t->left );
		makeEmpty( t->right );
		makeItemsEmpty(temp);
		delete t;
	}
	t = NULL;
}

template <class Comparable>
void BinarySearchTree<Comparable>::makeItemsEmpty( BinaryItemNode<Comparable> * & t ) const{
	if( t != NULL ){
		makeItemsEmpty( t->left );
		makeItemsEmpty( t->right );
		delete t;
	}
	t = NULL;
}