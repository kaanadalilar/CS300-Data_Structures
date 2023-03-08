#ifndef _AVLTREE_H
#define _AVLTREE_H
#include <iostream>

using namespace std;

template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlItemNode{
public:
	Comparable itTitle;
	string itInfo;
	AvlItemNode* left;
	AvlItemNode* right;
	int itemsHeight;

    AvlItemNode(const Comparable & title, const string & info, AvlItemNode* lt, AvlItemNode* rt)
		: itTitle(title), itInfo(info), left(lt), right(rt){}   
	friend class AvlTree<Comparable>;
};

template <class Comparable>
class AvlSectionNode{
public:
	Comparable secTitle;
	AvlSectionNode* left;
	AvlSectionNode* right;
	AvlItemNode<Comparable>* secItems; 
	int sectionsHeight;

	AvlSectionNode(const Comparable & title, AvlSectionNode* lt, AvlSectionNode* rt,AvlItemNode<Comparable>* item )
		: secTitle(title), left(lt), right(rt), secItems(item){}
	
	friend class AvlTree<Comparable>;
	friend class AvlItemNode<Comparable>;
};

template <class Comparable>
class AvlTree{
public:
	explicit AvlTree(const Comparable & notFound);
	AvlSectionNode<Comparable>* getRoot() const;
	AvlSectionNode<Comparable>* findSecNode(const Comparable & x) const;
	int secHeight(AvlSectionNode<Comparable>* t) const;
	int itHeight(AvlItemNode<Comparable>* t ) const;
	
	void insertSection( const Comparable & x );
	void insertItem( const Comparable & x,const string & y, AvlItemNode<Comparable>* & items );
	
	void removeSection( const Comparable & x );
	void removeItem( const Comparable & x, AvlItemNode<Comparable>* & items );
	
	void rotateWithLeftChild(AvlSectionNode<Comparable>* & k2 ) const;
	void rotateWithLeftItem( AvlItemNode<Comparable>* & k2 ) const;
	
	void rotateWithRightChild(AvlSectionNode<Comparable>* &  k1) const;
	void rotateWithRightItem( AvlItemNode<Comparable>* & k1 ) const;
	
	void doubleWithLeftChild( AvlSectionNode<Comparable>* & k3 ) const;
	void doubleWithLeftItem( AvlItemNode<Comparable>* & k3 ) const;
	
	void doubleWithRightChild( AvlSectionNode<Comparable>* & k1 ) const;
	void doubleWithRightItem( AvlItemNode<Comparable>* & k1 ) const;
	
	void printSections(AvlSectionNode<Comparable>* t) const;
	void printItems(AvlItemNode<Comparable>* t) const;
	
	AvlSectionNode<Comparable>* findSecMin( AvlSectionNode<Comparable>* t ) const;
	AvlItemNode<Comparable>* findSecItem( const Comparable & x, AvlItemNode<Comparable>* t) const;
	AvlItemNode<Comparable>* findItMin( AvlItemNode<Comparable>* t ) const;
private:
	void insert(const Comparable & x, AvlSectionNode<Comparable>* & t) const;
	void insertIt( const Comparable & x,const string & y, AvlItemNode<Comparable> * & t ) const;
	void removeSec(const Comparable & x, AvlSectionNode<Comparable> * & t) const;
	void removeIt(const Comparable & x, AvlItemNode<Comparable> * & t) const;
	int max( int lhs, int rhs ) const;
	AvlSectionNode<Comparable>* root;
	const Comparable ITEM_NOT_FOUND;
};
#endif