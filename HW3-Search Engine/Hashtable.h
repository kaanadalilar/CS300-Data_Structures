#ifndef _HASHTABLE_H
#define _HASHTABLE_H
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

struct DocItem{
	string documentName;
	int count;
	
	DocItem(string docName, int wordCount){
		documentName = docName;
		count = wordCount;
	}
};

struct WordItem{
	string word;
	vector<DocItem> docList; // list of DocumentItem’s.

	WordItem(){}

	WordItem(string w){
		word = w;
	}

	WordItem(string w, string dName){
		word = w;
		docList.push_back(DocItem(dName,1));
	}

	bool operator==(const WordItem& w){
		return (word == w.word);
	}

	bool operator!=(const WordItem& w){
		return (word != w.word);
	}
	
	bool operator<(const WordItem& w){
		return (word < w.word);
	}
    
	bool operator>(const WordItem& w){
		return (word > w.word);
	}
};

template <class HashedObj>
class HashTable{
public:
	explicit HashTable(HashedObj & notFound, int size = 50);

	HashTable( const HashTable & rhs )
		: ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ), hashArray( rhs.hashArray ), currentSize( rhs.currentSize ) { }

	HashedObj & find(HashedObj & x );

	int getCount();
	int getTableSize();
	void makeEmpty();
	void insert( HashedObj & x);
	void remove( HashedObj & x);

	const HashTable & operator=(const HashTable & rhs);
	enum EntryType {ACTIVE, EMPTY, DELETED};

	int hash ( WordItem & key, int tableSize){
		int hashVal = 0;

		for (unsigned int i = 0; i < key.word.length();i++) 
			hashVal = 37 * hashVal + key.word[ i ];

		hashVal = hashVal % tableSize;

		if (hashVal < 0) 
			hashVal = hashVal + tableSize;

		return(hashVal);
	}

	bool isPrime( int n )
	{
		if ( n == 2 || n == 3 )
			return true;

		if ( n == 1 || n % 2 == 0 )
			return false;

		for ( int i = 3; i * i <= n; i += 2 )
			if ( n % i == 0 )
				return false;

		return true;
	}

	 int nextPrime( int n )
	 {
		 if ( n % 2 == 0 )
			 n++;

		 for ( ; ! isPrime( n ); n += 2 )
			 ;

		 return n;
	 }

private:
	struct HashEntry{
		HashedObj element;
		EntryType info;

		HashEntry( const HashedObj & e = HashedObj(), EntryType i = EMPTY){
			element = e;
			info = i;
		}
	};

	vector<HashEntry> hashArray;
	unsigned int currentSize;
	double loadFactor;
	HashedObj ITEM_NOT_FOUND;

	bool isActive( int currentPos );
	int findPos( HashedObj & x );
	void rehash();
};

template <class HashedObj>
HashTable<HashedObj>::HashTable( HashedObj & notFound, int size)
	: ITEM_NOT_FOUND( notFound ), hashArray( nextPrime( size ) )
{
	loadFactor = 0.6;
	makeEmpty();
}

 template <class HashedObj>
 void HashTable<HashedObj>::makeEmpty( )
 {
	 currentSize=0;
 }

 template <class HashedObj>
 int HashTable<HashedObj>::getCount()
 {
	 return currentSize;
 }

 template <class HashedObj>
 int HashTable<HashedObj>::getTableSize()
 {
	 return hashArray.size();
 }
 
 template <class HashedObj>
void HashTable<HashedObj>::insert( HashedObj & x)
{
	// Insert x as active
	int currentPos = findPos( x );
	if ( isActive( currentPos ) )
		return;
	
	hashArray[ currentPos ] = HashEntry(x, ACTIVE);

	// enlarge the hash table if necessary 
	int firstSize = hashArray.size();

	if ( ++currentSize >= hashArray.size() * loadFactor ){
		rehash();
		cout << "rehashed..." << endl;
		cout << "previous table size:" << firstSize << ", new table size:" << hashArray.size()
			<< ", current unique word count:" << currentSize << ", current load factor:" << double(currentSize)/hashArray.size() << endl;
	}
}

template <class HashedObj>
void HashTable<HashedObj>::rehash(){
	vector<HashEntry> oldArray = hashArray;
	hashArray.resize( nextPrime( 2 * oldArray.size() ) ); // Create new double-sized, empty table
	
	for (unsigned int j = 0; j < hashArray.size(); j++ )
		hashArray[ j ].info = EMPTY;

	// Copy table over
	currentSize = 0;
	for (unsigned int i = 0; i < oldArray.size(); i++ ){
		if ( oldArray[i].info == ACTIVE ){
			insert(oldArray[i].element );
		}
	}
}

template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ){
	return hashArray[ currentPos ].info == ACTIVE;
}

template <class HashedObj>
void HashTable<HashedObj>::remove( HashedObj & x )
{
	int currentPos = findPos( x );
	if ( isActive( currentPos ) )
		hashArray[ currentPos ].info = DELETED;
}

template <class HashedObj>
int HashTable<HashedObj>::findPos( HashedObj & x ){
	int collisionNum = 0;
	int arraySize = hashArray.size( );
	int currentPos = hash( x, arraySize );

	while ( hashArray[ currentPos ].info != EMPTY && hashArray[ currentPos ].element != x)
	{
		currentPos += pow(++collisionNum, 2);  //add the difference
		if ( currentPos >= hashArray.size( ) ) // perform the mod
			currentPos -= hashArray.size( ); // if necessary
	}
	return currentPos;
}

template <class HashedObj>
HashedObj & HashTable<HashedObj>::find( HashedObj & x ){
	int currentPos = findPos( x );
	if (isActive( currentPos )) 
		return hashArray[ currentPos ].element; 
	return ITEM_NOT_FOUND;
}

template <class HashedObj>
const HashTable<HashedObj> & HashTable<HashedObj>::operator=( const HashTable<HashedObj> & rhs ){
	if( this != &rhs ){
		hashArray = rhs.hashArray;
		currentSize = rhs.currentSize;
	}
	return *this;
}
#endif