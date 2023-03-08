#ifndef _BINARYSEARCHTREE_H
#define _BINARYSEARCHTREE_H
#include <iostream>

using namespace std;

template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryItemNode{
public:
	Comparable itTitle;
	string itInfo;
    BinaryItemNode* left;
    BinaryItemNode* right;

    BinaryItemNode(const Comparable & title, const string & info, BinaryItemNode* lt, BinaryItemNode* rt)
		: itTitle(title), itInfo(info), left(lt), right(rt){}   
	friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySectionNode{
public:
	Comparable secElem;
    BinarySectionNode* left;
    BinarySectionNode* right;
	BinaryItemNode<Comparable>* secItems;

	BinarySectionNode(const Comparable & theElement, BinarySectionNode* lt, BinarySectionNode* rt, BinaryItemNode<Comparable>* item)
		: secElem(theElement), left(lt), right(rt), secItems(item){}
	friend class BinarySearchTree<Comparable>;
	friend class BinaryItemNode<Comparable>;
};

template <class Comparable>
class BinarySearchTree{
public:
	explicit BinarySearchTree(const Comparable & notFound);
	~BinarySearchTree();

	BinarySectionNode<Comparable>* getRoot() const;
	BinarySectionNode<Comparable>* findSecNode(const Comparable & x) const;
	BinaryItemNode<Comparable>* findSecItem(const Comparable & x, BinaryItemNode<Comparable>* t) const;
	
	void printTree(BinarySectionNode<Comparable>* t) const;
	void printItems(BinaryItemNode<Comparable>* t) const;
	void insertItem(const Comparable & x, const string & y, BinaryItemNode<Comparable> * & t);
	void insertSection(const Comparable & x);
	void removeItem(const Comparable & x, BinaryItemNode<Comparable> * & p);
	void removeSection(const Comparable & x);
private:
	BinarySectionNode<Comparable>* root;
	const Comparable ITEM_NOT_FOUND;
	
	void insertSec(const Comparable & x, BinarySectionNode<Comparable>* & t) const;
	void insertIt(const Comparable & x, const string & y, BinaryItemNode<Comparable>* & t) const;
	
	void removeSec(const Comparable & x, BinarySectionNode<Comparable>* & t) const;
	void removeIt(const Comparable & x, BinaryItemNode<Comparable>* & t) const;
	
	BinarySectionNode<Comparable> * findSecMin(BinarySectionNode<Comparable>* t) const;
	BinaryItemNode<Comparable> * findItMin(BinaryItemNode<Comparable> *t) const;
	void makeEmpty( BinarySectionNode<Comparable>* & t ) const;
	void makeItemsEmpty( BinaryItemNode<Comparable>* & t ) const;
};

#endif