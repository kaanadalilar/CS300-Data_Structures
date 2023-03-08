#include "AVLTree.h"
#include <iostream>
using namespace std;

template <class Comparable>
AvlTree<Comparable>::AvlTree(const Comparable & notFound) 
	:ITEM_NOT_FOUND(notFound), root(NULL)
{}

template <class Comparable>
AvlSectionNode<Comparable>* AvlTree<Comparable>::getRoot() const{
	return root;
}

template <class Comparable>
int AvlTree<Comparable>::secHeight(AvlSectionNode<Comparable>* t) const{
	if (t == NULL)
		return -1;
	return t->sectionsHeight;
}

template <class Comparable>
int AvlTree<Comparable>::itHeight(AvlItemNode<Comparable>* t) const{
	if (t == NULL)
		return -1;
	return t->itemsHeight;
}

template <class Comparable>
void AvlTree<Comparable>::insertSection( const Comparable & x ){
	insert( x, root );
}

 template <class Comparable>
 void AvlTree<Comparable>::insert( const Comparable & x, AvlSectionNode<Comparable> * & t ) const{
	 if ( t == NULL )
		 t = new AvlSectionNode<Comparable>( x, NULL, NULL, NULL);
	 
	 else if ( x < t->secTitle ){ // X should be inserted to the left tree!
		 insert( x, t->left );
           
		 // Check if the left tree is out of balance (left subtree grew in height!)
		 if ( secHeight( t->left ) - secHeight( t->right ) == 2 )
			 if ( x < t->left->secTitle )  // X was inserted to the left-left subtree!
				 rotateWithLeftChild( t );
			 else			     // X was inserted to the left-right subtree!
				 doubleWithLeftChild( t );
	 }
	 else if( t->secTitle < x ){ // Otherwise X is inserted to the right subtree
		 insert( x, t->right );
		 if ( secHeight( t->right ) - secHeight( t->left ) == 2 )
			 // height of the right subtree increased
				 if ( t->right->secTitle < x ) // X was inserted to right-right subtree
					 rotateWithRightChild( t );
				 else // X was inserted to right-left subtree
					 doubleWithRightChild( t );
	 }
	 else
		 ;  // Duplicate; do nothing

	 // update the height the node
	 t->sectionsHeight = max( secHeight( t->left ), secHeight( t->right ) ) + 1;
 }

template <class Comparable>
void AvlTree<Comparable>::insertItem( const Comparable & x,const string & y, AvlItemNode<Comparable> * & items ){
	insertIt( x, y, items );
}

template <class Comparable>
void AvlTree<Comparable>::insertIt( const Comparable & x,const string & y, AvlItemNode<Comparable> * & t ) const{
	if( t == NULL ) //  create a new node at the right place
		t = new AvlItemNode<Comparable>( x, y, NULL, NULL);
	else if ( x < t->itTitle ) {// X should be inserted to the left tree!
		insertIt( x, y, t->left );
		 if ( itHeight( t->left ) - itHeight( t->right ) == 2 ) // Check if the left tree is out of balance (left subtree grew in height!)
			 if ( x < t->left->itTitle )  // X was inserted to the left-left subtree!
				 rotateWithLeftItem( t );
			 else			     // X was inserted to the left-right subtree!
				 doubleWithLeftItem( t );
	 }
	 else if( t->itTitle < x )
	 {    // Otherwise X is inserted to the right subtree
		 insertIt( x, y, t->right );
		 if ( itHeight( t->right ) - itHeight( t->left ) == 2 )
			 // height of the right subtree increased
				 if ( t->right->itTitle < x ) // X was inserted to right-right subtree
					 rotateWithRightItem( t );
				 else // X was inserted to right-left subtree
					 doubleWithRightItem( t );
	 }
	 else
		 ;  // Duplicate; do nothing

	 // update the height the node
	 t->itemsHeight = max( itHeight( t->left ), itHeight( t->right ) ) + 1;
}

template <class Comparable>
int AvlTree<Comparable>::max( int lhs, int rhs ) const
{
      if (lhs > rhs)
	      return lhs;
      return rhs;            
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild( AvlSectionNode<Comparable> *&  k2 ) const
{
	AvlSectionNode<Comparable> *k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->sectionsHeight = max( secHeight( k2->left ), secHeight( k2->right ) ) + 1;
	k1->sectionsHeight = max( secHeight( k1->left ), k2->sectionsHeight ) + 1;
	k2 = k1;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftItem( AvlItemNode<Comparable> *&  k2 ) const
{
	AvlItemNode<Comparable> *k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->itemsHeight = max( itHeight( k2->left ), itHeight( k2->right ) ) + 1;
	k1->itemsHeight = max( itHeight( k1->left ), k2->itemsHeight ) + 1;
	k2 = k1;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild( AvlSectionNode<Comparable> *&  k1 ) const{
	AvlSectionNode<Comparable> *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->sectionsHeight = max( secHeight( k1->left ), secHeight( k1->right ) ) + 1;
	k2->sectionsHeight = max( secHeight( k2->right ), k1->sectionsHeight ) + 1;
	k1 = k2;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithRightItem( AvlItemNode<Comparable> *&  k1 ) const{
	AvlItemNode<Comparable> *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->itemsHeight = max( itHeight( k1->left ), itHeight( k1->right ) ) + 1;
	k2->itemsHeight = max( itHeight( k2->right ), k1->itemsHeight ) + 1;
	k1 = k2;
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild( AvlSectionNode<Comparable> *&  k3 ) const
{
	rotateWithRightChild( k3->left );
	rotateWithLeftChild( k3 );
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftItem( AvlItemNode<Comparable> *&  k3 ) const
{
	rotateWithRightItem( k3->left );
	rotateWithLeftItem( k3 );
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild( AvlSectionNode<Comparable> *&  k1 ) const
{
	rotateWithLeftChild( k1->right );
	rotateWithRightChild( k1 );
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithRightItem( AvlItemNode<Comparable> *&  k1 ) const
{
	rotateWithLeftItem( k1->right );
	rotateWithRightItem( k1 );
}

template <class Comparable>
AvlSectionNode<Comparable>* AvlTree<Comparable>::findSecNode( const Comparable & x ) const{
	AvlSectionNode<Comparable>* temp = root;
	while(temp != NULL){
		if( x < temp->secTitle )
			temp = temp->left;
		else if(temp->secTitle < x)
			temp = temp->right;
		else
			return temp;
	}
	return NULL;
}

template <class Comparable>
void AvlTree<Comparable>::printSections(AvlSectionNode<Comparable>* t) const{
	if ( t != NULL ){
		printSections( t->left );
		cout << t->secTitle << endl;
		printSections( t->right );
	}
	else
		return;
}

template <class Comparable>
void AvlTree<Comparable>::printItems(AvlItemNode<Comparable>* t) const{
	if ( t != NULL ){
		printItems( t->left );
		cout << t->itTitle << endl;
		printItems( t->right );
	}
	else
		return;
}

template <class Comparable>
AvlSectionNode<Comparable> * AvlTree<Comparable>::findSecMin( AvlSectionNode<Comparable>* t ) const
{
	if( t == NULL )
		return NULL;
	if( t->left == NULL )
		return t;
	return findSecMin( t->left );
}

template <class Comparable>
AvlItemNode<Comparable> * AvlTree<Comparable>::findItMin( AvlItemNode<Comparable>* t ) const{
	if( t == NULL )
		return NULL;
	if( t->left == NULL )
		return t;
	return findItMin( t->left );
}

template <class Comparable>
AvlItemNode<Comparable>* AvlTree<Comparable>::findSecItem( const Comparable & x, AvlItemNode<Comparable>* t) const{
	AvlItemNode<Comparable>* temp = t;
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
void AvlTree<Comparable>::removeSection( const Comparable & x ){
	removeSec(x, root);
}

template <class Comparable>
void AvlTree<Comparable>::removeSec(const Comparable & x, AvlSectionNode<Comparable>* & t) const{
	if( t == NULL )
		return; // Item not found; do nothing
	if( x < t->secTitle ){
		removeSec( x, t->left );
		if ( secHeight( t->left ) - secHeight( t->right ) == 2 ){
			if ( x < t->left->secTitle )  
				rotateWithLeftChild( t );
			else			    
				doubleWithLeftChild( t );
		}
	}
	else if( t->secTitle < x ){
		removeSec( x, t->right );
		if ( secHeight( t->right ) - secHeight( t->left ) == 2 ){
				if ( t->right->secTitle < x ) 
					rotateWithRightChild( t );
				else 
					doubleWithRightChild( t );
		}
	}
	else if( t->left != NULL && t->right != NULL ) // Two children
	{
		t->secItems = findSecMin(t->right)->secItems;
		t->secTitle = findSecMin( t->right )->secTitle;
		removeSec( t->secTitle, t->right );
	}
	else // one or no children
	{
		AvlSectionNode<Comparable> *oldNode = t;
		t = ( t->left != NULL ) ? t->left : t->right;
		delete oldNode;
	}
}

template <class Comparable>
void AvlTree<Comparable>::removeItem( const Comparable & x, AvlItemNode<Comparable>* & items ){
	removeIt(x, items);
}

template <class Comparable>
void AvlTree<Comparable>::removeIt(const Comparable & x, AvlItemNode<Comparable>* & t) const{
	if( t == NULL )
		return; // Item not found; do nothing
	if( x < t->itTitle ){
		removeIt( x, t->left );
		if ( itHeight( t->left ) - itHeight( t->right ) == 2 ){
			if ( x < t->left->itTitle )  
				rotateWithLeftItem( t );
			else			    
				doubleWithLeftItem( t );
		}
	}
	else if( t->itTitle < x ){
		removeIt( x, t->right );
		if ( itHeight( t->right ) - itHeight( t->left ) == 2 ){
				if ( t->right->itTitle < x ) 
					rotateWithRightItem( t );
				else 
					doubleWithRightItem( t );
		}
	}
	else if( t->left != NULL && t->right != NULL ) // Two children
	{
		t->itInfo = findItMin(t->right)->itInfo;
		t->itTitle = findItMin( t->right )->itTitle;
		removeIt( t->itTitle, t->right );
	}
	else // one or no children
	{
		AvlItemNode<Comparable> *oldNode = t;
		t = ( t->left != NULL ) ? t->left : t->right;
		delete oldNode;
	}
}