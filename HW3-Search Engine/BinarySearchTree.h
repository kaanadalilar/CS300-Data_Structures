#ifndef _BINARYSEARCHTREE_H
#define _BINARYSEARCHTREE_H
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct DocumentItem{
	string documentName;
	int count;
	
	DocumentItem(string docName, int wordCount){
		documentName = docName;
		count = wordCount;
	}
};

template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryWordNode{
public:
	Comparable word;
	vector<DocumentItem> docList;
    BinaryWordNode* left;
    BinaryWordNode* right;

	BinaryWordNode(const Comparable & theWord, const string & documentName, BinaryWordNode* lt, BinaryWordNode* rt){
		word = theWord;
		docList.push_back(DocumentItem(documentName,1));
		left = lt;
		right = rt;
	}
	friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySearchTree{
public:
	explicit BinarySearchTree(const Comparable & notFound);
	~BinarySearchTree();

	BinaryWordNode<Comparable>* getRoot() const;
	BinaryWordNode<Comparable>* findWordNode(const Comparable & x) const;
	
	void printTree(BinaryWordNode<Comparable>* t) const;
	int numberOfNodes(BinaryWordNode<Comparable>* t) const;
	void insertWord(const Comparable & x, const string & docName);
	void removeWord(const Comparable & x);
private:
	BinaryWordNode<Comparable>* root;
	const Comparable ITEM_NOT_FOUND;
	
	void insertWordPriv(const Comparable & x, const string & docName, BinaryWordNode<Comparable>* & t) const;
	void removeWordPriv(const Comparable & x, BinaryWordNode<Comparable>* & t) const;
	
	BinaryWordNode<Comparable> * findMinWord(BinaryWordNode<Comparable>* t) const;
	void makeEmpty( BinaryWordNode<Comparable>* & t ) const;
};

#endif