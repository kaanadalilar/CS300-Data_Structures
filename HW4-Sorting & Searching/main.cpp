/*
CS300-Homework 4
Last Modified: 08.06.2022
Author: Kaan Adalilar - (kaanadalilar/28384)
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "strutils.h"

using namespace std;

struct Contact{
	string name, surname, number, country;

	Contact(){}

	Contact(string n, string s, string num, string c){
		name = n;
		surname = s;
		number = num;
		country = c;
	}

	bool operator==(const Contact& c){
		return (name == c.name && surname == c.surname && number == c.number && country == c.country);
	}

	bool operator!=(const Contact& c){
		return (number != c.number);
	}
	
	bool operator<(const Contact& c){
		if(name == c.name){
			return (surname<c.surname);
		}
		else{
			return (name < c.name);
		}
	}
	
	bool operator<=(const Contact& c){
		if(name == c.name){
			return (surname<c.surname);
		}
		else{
			return (name < c.name);
		}
	}
    
	bool operator>(const Contact& c){
		if(name == c.name){
			return (surname>c.surname);
		}
		else{
			return (name > c.name);
		}
	}
};

template <class Comparable>
inline void qSwap( Comparable & obj1, Comparable & obj2 ){
	Comparable tmp = obj1;
	obj1 = obj2;
	obj2 = tmp;
}

/////////////////////////////////////////////////////////////////////////////////
/***Sort Functions***/

template <class Comparable>
void insertionSort (vector<Comparable>& a){
	int j;
	for (unsigned int p = 1;  p < a.size(); p++){ // loop over the passes
		Comparable tmp = a[p];
		for (j = p; j > 0 && tmp < a[j-1]; j--) // loop over the elements
			a[j] = a[j-1];
		a[j] = tmp;
	}
}

/////////////////////////////////////////////////////////////////////////////////

inline int leftChild(int i){
	return 2*i+1;
}

template <class Comparable>  
// for deleteMax
// a is the array, i is the position to percolate down from
// n is the logical size of the array
void percDown( vector<Comparable> & a, int i, int n ){
	int child;
	Comparable tmp;

	for (tmp=a[i]; leftChild(i) < n; i = child ){
		child = leftChild(i);
		if ( child != n-1 && a[ child  ] < a[ child+1 ] )
			child++;
		if ( a[child ] > tmp )
			a[i] = a[ child ];
		else
			break;
	} 
	a[ i ] = tmp;
}

template <class Comparable>
void heapsort(vector<Comparable> & a){
	// buildHeap
	for (int i = a.size()/2; i >=0; i--)
		percDown(a, i, a.size());

	// sort
	for (int j = a.size()-1; j >0; j--){
  	     qSwap(a[0], a[j]);    // swap max to the last pos.
		 percDown(a, 0, j); // re-form the heap
	}
}

/////////////////////////////////////////////////////////////////////////////////

template <class Comparable>
void merge( vector<Comparable> & a, vector<Comparable> & tmpArray, int leftPos, int rightPos, int rightEnd ){
	int leftEnd = rightPos - 1;
	int tmpPos = leftPos;
	int numElements = rightEnd - leftPos + 1;
	
	while ( leftPos <= leftEnd && rightPos <= rightEnd ) // Main loop
		if ( a[ leftPos ] <= a[ rightPos ] )
			tmpArray[ tmpPos++ ] = a[ leftPos++ ];
		else
			tmpArray[ tmpPos++ ] = a[ rightPos++ ];

	while ( leftPos <= leftEnd ) // Copy rest of first half
		tmpArray[ tmpPos++ ] = a[ leftPos++ ];

	while ( rightPos <= rightEnd ) // Copy rest of right half
		tmpArray[ tmpPos++ ] = a[ rightPos++ ];

	for ( int i = 0; i < numElements; i++, rightEnd-- ) // Copy tmpArray back
		a[ rightEnd ] = tmpArray[ rightEnd ];
}

template <class Comparable>
void mergeSort( vector<Comparable> & a, vector<Comparable> & tmpArray, int left, int right ){
	if ( left < right ){
		int center = ( left + right ) / 2;
		mergeSort( a, tmpArray, left, center );
		mergeSort( a, tmpArray, center + 1, right );
		merge( a, tmpArray, left, center + 1, right );
	}
}

template <class Comparable>
void mainUseMergeSort( vector<Comparable> & a ){
	vector<Comparable> tmpArray( a.size( ) );
	mergeSort( a, tmpArray, 0, a.size( ) - 1 );
}

/////////////////////////////////////////////////////////////////////////////////

template <class Comparable>
void qInsertionSort( vector<Comparable> & a, int left, int right ){
	for ( int p = left + 1; p <= right; p++ ){
		Comparable tmp = a[ p ];
		int j;

		for ( j = p; j > left && tmp < a[j-1]; j-- )
			a[j] = a[j-1];
		a[j] = tmp;
	}
}

template <class Comparable>
const Comparable & median3( vector<Comparable> & a, int left, int right ){
	int center = ( left + right ) / 2;
	if ( a[ center ] < a[ left ] )
		qSwap( a[ left ], a[ center ] );
	if ( a[ right ] < a[ left ] )
		qSwap( a[ left ], a[ right ] );
	if ( a[ right ] < a[ center ] )
		qSwap( a[ center ], a[ right ] );

	qSwap( a[ center ], a[ right - 1 ] ); // Place pivot at position right - 1
	return a[ right - 1 ];
}

template <class Comparable>
void quicksort( vector<Comparable> & a, int left, int right ){

	if ( left + 10 <= right ){
		Comparable pivot = median3( a, left, right );
		// Begin partitioning
		int i = left, j = right - 1;
		for ( ; ; )
		{
			while ( a[ ++i ] < pivot ) { }

			while ( pivot < a[ --j ] ) { }

			if ( i < j )
				qSwap( a[ i ], a[ j ] );
			else
				break;
		}
			
		qSwap( a[ i ], a[ right - 1 ] ); // Restore pivot

		quicksort( a, left, i - 1 ); // Sort small elements
		quicksort( a, i + 1, right ); // Sort large elements
	}
	else // Do an insertion sort on the subarray
		qInsertionSort( a, left, right );
}

template <class Comparable>
void mainUseQuickSort( vector<Comparable> & a ){
	quicksort( a, 0, a.size( ) - 1 );
}

/////////////////////////////////////////////////////////////////////////////////

/***Search Functions***/
vector<Contact> binarySearch( const vector<Contact> & mainVec, string queryWord){
	vector<Contact> includedContacts;
	string name, surname;
	if(queryWord.find(" ") != string::npos){ //full contact name search
		istringstream ss(queryWord);
		ss >> name >> surname;
		int low = 0; // leftmost possible entry
		int high = mainVec.size()-1; // rightmost possible entry
		int mid; // middle of current range
		while (low <= high){ 
			mid = (low + high)/2;
			if (mainVec[mid].name == name && mainVec[mid].surname == surname){ // found key, exit search
				Contact foundedContact(name, surname, mainVec[mid].number, mainVec[mid].country);
				includedContacts.push_back(foundedContact);
				return includedContacts;
			}
			else if (mainVec[mid].name < name) // key in upper half
				low = mid + 1;
			else // key in lower half
				high = mid - 1;
		}
	}
	else{ //partial contact name search
		int low = 0; // leftmost possible entry
		int high = mainVec.size()-1; // rightmost possible entry
		int mid; // middle of current range
		while (low <= high){ 
			mid = (low + high)/2;
			if (mainVec[mid].name.find(queryWord) != string::npos){
				Contact foundedContact(mainVec[mid].name, mainVec[mid].surname, mainVec[mid].number, mainVec[mid].country);
				includedContacts.push_back(foundedContact);
				int backIndex = mid;
				while(backIndex >= 1 && mainVec[backIndex-1].name.find(queryWord) != string::npos){ //check the right sublist to find more people whose name is wanted
					Contact foundedContact(mainVec[backIndex-1].name, mainVec[backIndex-1].surname, mainVec[backIndex-1].number, mainVec[backIndex-1].country);
					includedContacts.push_back(foundedContact);
					backIndex--;
				}

				unsigned int frontIndex = mid;
				while(frontIndex < mainVec.size()-1 && mainVec[frontIndex+1].name.find(queryWord) != string::npos){ //check the left sublist to find more people whose name is wanted
					Contact foundedContact(mainVec[frontIndex+1].name, mainVec[frontIndex+1].surname, mainVec[frontIndex+1].number, mainVec[frontIndex+1].country);
					includedContacts.push_back(foundedContact);
					frontIndex++;
				}
				return includedContacts;
			}
			else if (mainVec[mid].name < queryWord) // key in upper half
				low = mid + 1;
			else // key in lower half
				high = mid - 1;
		}
	}
	return includedContacts;
}

vector<Contact> sequentialSearch( const vector<Contact> & mainVec, string queryWord){
	vector<Contact> includedContacts;
	string name, surname;
	if(queryWord.find(" ") != string::npos){ //full contact name search
		istringstream ss(queryWord);
		ss >> name >> surname;
		for(unsigned int i=0; i<mainVec.size(); i++){
			if(mainVec[i].name == name && mainVec[i].surname == surname){
				Contact foundedContact(name, surname, mainVec[i].number, mainVec[i].country);
				includedContacts.push_back(foundedContact);
			}
		}
	}
	else{ //partial contact name search
		for(unsigned int i=0; i<mainVec.size(); i++){
			if(mainVec[i].name.find(queryWord) != string::npos){
				Contact foundedContact(mainVec[i].name, mainVec[i].surname, mainVec[i].number, mainVec[i].country);
				includedContacts.push_back(foundedContact);
			}
		}
	}
	return includedContacts;
}

int main(){
	string contactFile;
	cout << "Please enter the contact file name:" << endl;
	cin >> contactFile;
	
	cin.clear();
	cin.get();
	string queryWord;
	cout << "Please enter the word to be queried:" << endl;
	getline(cin, queryWord);
	
	ifstream inputFile;
	inputFile.open(contactFile.c_str());
	string fileLine;
	vector<Contact> insertionSortVector;
	vector<Contact> quickSortVector;
	vector<Contact> mergeSortVector;
	vector<Contact> heapSortVector;

	while(getline(inputFile, fileLine)){
		istringstream ss(fileLine);
		string nameEntry, surnameEntry, numberEntry, countryEntry;
		ss >> nameEntry >> surnameEntry >> numberEntry >> countryEntry;
		Contact newContact(nameEntry, surnameEntry, numberEntry, countryEntry);
		insertionSortVector.push_back(newContact);
		quickSortVector.push_back(newContact);
		mergeSortVector.push_back(newContact);
		heapSortVector.push_back(newContact);
	}
	
	cout << endl << "Sorting the vector copies" << endl << "======================================" << endl;
	
	const int k = 100;
	vector<Contact> tempForSort;
	auto quickStart = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++){
		tempForSort = quickSortVector;
		mainUseQuickSort(tempForSort);
	}
	auto quickSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - quickStart);
	mainUseQuickSort(quickSortVector);
	
	cout << "Quick Sort Time: " << quickSortTime.count() / k << " Nanoseconds" << endl;

	auto insertStart = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++){
		tempForSort = insertionSortVector;
		insertionSort(tempForSort);
	}
	auto insertionSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - insertStart);
	insertionSort(insertionSortVector);

	cout << "Insertion Sort Time: " << insertionSortTime.count() / k << " Nanoseconds" << endl;

	auto mergeStart = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++){
		tempForSort = mergeSortVector;
		mainUseMergeSort(tempForSort);
	}
	auto mergeSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mergeStart);
	mainUseMergeSort(mergeSortVector);

	cout << "Merge Sort Time: " << mergeSortTime.count() / k << " Nanoseconds" << endl;

	auto heapStart = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++){
		tempForSort = heapSortVector;
		heapsort(tempForSort);
	}
	auto heapSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - heapStart);
	heapsort(heapSortVector);

	cout << "Heap Sort Time: " << heapSortTime.count() / k << " Nanoseconds" << endl;

	cout << endl << "Searching for " << queryWord << endl << "======================================" << endl;
	
	const int j = 100;
	auto binaryStart = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < j; i++){
		vector<Contact> isBinFounded = binarySearch(insertionSortVector, queryWord);
	}
	auto binarySearchTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - binaryStart);

	auto sequentialStart = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < j; i++){
		vector<Contact> isSeqFounded = sequentialSearch(insertionSortVector, queryWord);
	}
	auto sequentialSearchTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - sequentialStart);
	
	vector<Contact> isBinFounded = binarySearch(insertionSortVector, queryWord);
	mainUseQuickSort(isBinFounded);
	if(isBinFounded.size() == 0){
		cout << UpperString(queryWord) << " does NOT exist in the dataset" << endl;
	}
	else{
		for(unsigned int i=0; i<isBinFounded.size(); i++){
			cout << UpperString(isBinFounded[i].name) << " " << UpperString(isBinFounded[i].surname)
				<< " " << isBinFounded[i].number <<  " " << isBinFounded[i].country << endl; 
		}
	}
	cout << "Binary Search Time: " << binarySearchTime.count()/j << " Nanoseconds" << endl << endl;

	cout << "Search results for Sequential Search:" << endl;
	vector<Contact> isSeqFounded = sequentialSearch(insertionSortVector, queryWord);
	if(isSeqFounded.size() == 0){
		cout << UpperString(queryWord) << " does NOT exist in the dataset" << endl;
	}
	else{
		for(unsigned int i=0; i<isSeqFounded.size(); i++){
			cout << UpperString(isSeqFounded[i].name) << " " << UpperString(isSeqFounded[i].surname) 
				<< " " << isSeqFounded[i].number <<  " " << isSeqFounded[i].country << endl; 
		}
	}
	cout << "Sequential Search Time: " << sequentialSearchTime.count()/j << " Nanoseconds" << endl << endl;

	cout << "SpeedUp between Search Algorithms" << endl << "======================================" << endl;
	if(binarySearchTime.count()/j < sequentialSearchTime.count()/j)
		cout << "(Sequential Search/ Binary Search) SpeedUp = " << (double)sequentialSearchTime.count()/binarySearchTime.count() << endl << endl;
	else
		cout << "(Binary Search/ Sequential Search) SpeedUp = " << (double)binarySearchTime.count()/sequentialSearchTime.count() << endl << endl;
	
	cout << "SpeedUps between Sorting Algorithms" << endl << "======================================" << endl;
	
	if(quickSortTime.count()/k < insertionSortTime.count()/k)
		cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << (double)insertionSortTime.count()/quickSortTime.count() << endl;
	else
		cout << "(Quick Sort/ Insertion Sort) SpeedUp = " << (double)quickSortTime.count()/insertionSortTime.count() << endl;
	if(quickSortTime.count()/k < mergeSortTime.count()/k)
		cout << "(Merge Sort/ Quick Sort) SpeedUp = " << (double)mergeSortTime.count()/quickSortTime.count() << endl;
	else
		cout << "(Quick Sort/ Merge Sort) SpeedUp = " << (double)quickSortTime.count()/mergeSortTime.count() << endl;
	if(quickSortTime.count()/k < heapSortTime.count()/k)
		cout << "(Heap Sort/ Quick Sort) SpeedUp = " << (double)heapSortTime.count()/quickSortTime.count() << endl;
	else
		cout << "(Quick Sort/ Heap Sort) SpeedUp = " << (double)quickSortTime.count()/heapSortTime.count() << endl;

	return 0;
}