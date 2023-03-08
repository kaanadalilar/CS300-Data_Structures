/*
CS300-Homework 3
Last Modified: 15.05.2022
Author: Kaan Adalilar - (kaanadalilar/28384)
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "strutils.h"
#include "BinarySearchTree.cpp"
#include "Hashtable.h"

//#pragma comment(linker, "/STACK:2000000")
//#pragma comment(linker, "HEAP:2000000")
/*In my computer, after a certain number of compilings (approximately 2-3 times), since the stack becomes full, program gives me an unhandled
exception error (stack overflow) and it could not be compiled. To solve this problem, I update the STACK: size with #pragma comment as needed.*/

using namespace std;

void coutBSTResults(BinarySearchTree<string>&, vector<string>&, vector<string>&);
void coutHSTResults(HashTable<WordItem>&, WordItem&, vector<string>&, vector<string>&);
vector<string> queryAnalysisForBST(BinarySearchTree<string>&, vector<string>&, vector<string>&);
vector<WordItem> queryAnalysisForHST(HashTable<WordItem>&, WordItem& notFound, vector<string>&, vector<string>&);

int main(){
	int numOfFiles;
	cout << "Enter the number of input files: ";
	cin >> numOfFiles;
	vector<string> allFiles; // keeps the file names
	BinarySearchTree<string> myBST("not found");
	WordItem notFound("notFound","notFound");
	HashTable<WordItem> myHST(notFound); //HashTable element is WordItem // takes the WordItem

	for(int i=1; i<=numOfFiles; i++){
		string nameOfFile;
		cout << "Enter " << i << ". file name: ";
		cin >> nameOfFile;
		allFiles.push_back(nameOfFile);
	}
	for(int itr=0; itr<numOfFiles; itr++){
		string fileName = allFiles[itr];
		ifstream inputFile;
		inputFile.open(fileName.c_str());
		string fileLine;

		while(getline(inputFile, fileLine)){
			string entry;
			istringstream ss(fileLine);
			while(ss >> entry){
				bool fullyAlphabetic = true;
				for(unsigned int i=0; i<entry.length(); i++){ // check whether the entry includes non-alphabetical characters or not
					if(!isalpha(entry[i])){
						fullyAlphabetic = false;
						i = entry.length(); // after finding that the entry includes of non-alphabetical characters, break the loop
					}
				}
				string myWord;

				if(fullyAlphabetic){
					myWord = entry;
					ToLower(myWord);
					BinaryWordNode<string>* isExist = myBST.findWordNode(myWord);
					WordItem hstWord(myWord, fileName);
					WordItem& isExistInHST = myHST.find(hstWord);

					bool isDocExistInBST = false;
					bool isDocExistInHST = false;
					if( isExist != NULL && isExistInHST != notFound){ // if the word already exists; then increase the number
						for(unsigned int k=0; k<isExist->docList.size(); k++){
							if(isExist->docList[k].documentName == fileName){
								isExist->docList[k].count++;
								isDocExistInBST = true;
							}
						}
						for(unsigned int k=0; k<isExistInHST.docList.size(); k++){
							if(isExistInHST.docList[k].documentName == fileName){
								isExistInHST.docList[k].count++;
								isDocExistInHST = true;
							}
						}
						if(isDocExistInBST == false && isDocExistInHST == false){
							isExist->docList.push_back(DocumentItem(fileName,1));
							isExistInHST.docList.push_back(DocItem(fileName,1));
						}
					}
					else{
						myBST.insertWord(myWord, fileName);
						myHST.insert(hstWord);
					}
				}
				else{
					string temp;
					for(unsigned int k=0; k<entry.length(); k++){
						if(isalpha(entry[k])){
							temp += entry[k];
						}
						else{
							temp = "";
						}
						if(temp != "" && (!isalpha(entry[k+1]) || entry[k+1] == ' ')){
							myWord = temp;
							ToLower(myWord);
							BinaryWordNode<string>* isExist = myBST.findWordNode(myWord);
							WordItem hstWord(myWord, fileName);
							WordItem& isExistInHST = myHST.find(hstWord);

							bool isDocExistInBST = false;
							bool isDocExistInHST = false;
							if( isExist != NULL && isExistInHST != notFound ){ // if the word already exists; then increase the number
								for(unsigned int k=0; k<isExist->docList.size(); k++){
									if(isExist->docList[k].documentName == fileName){
										isExist->docList[k].count++;
										isDocExistInBST = true;
									}
								}
								for(unsigned int k=0; k<isExistInHST.docList.size(); k++){
									if(isExistInHST.docList[k].documentName == fileName){
										isExistInHST.docList[k].count++;
										isDocExistInHST = true;
									}
								}
								if(isDocExistInBST == false && isDocExistInHST == false){
									isExist->docList.push_back(DocumentItem(fileName,1));
									isExistInHST.docList.push_back(DocItem(fileName,1));
								}
							}
							else{
								myBST.insertWord(myWord, fileName);
								WordItem hstWord(myWord, fileName);
								myHST.insert(hstWord);
							}
						}
					}
				}
			}
		}
	}
	int wordCount = myBST.numberOfNodes(myBST.getRoot());

	cout << endl << endl << "After preprocessing, the unique word count is " << wordCount 
		<< ". Current load ratio is " << double(myHST.getCount())/myHST.getTableSize() << endl;

	cin.clear();
	cin.get();
	string query;
	cout << "Enter queried words in one line: ";
	getline(cin, query);
	
	vector<string> querriedWords;
	string queryWord;
	istringstream ss(query);
	while(ss >> queryWord){
		bool fullyAlphabetic = true;
		for(unsigned int i=0; i<queryWord.length(); i++){ // check whether the entry includes non-alphabetical characters or not
			if(!isalpha(queryWord[i])){
				fullyAlphabetic = false;
				i = queryWord.length(); // after finding that the entry includes of non-alphabetical characters, break the loop
			}
		}
		string searchWord;
		if(fullyAlphabetic){
		searchWord = queryWord;
			ToLower(searchWord);
			querriedWords.push_back(searchWord);
		}
		else{
			string temp;
			for(unsigned int k=0; k<queryWord.length(); k++){
				if(isalpha(queryWord[k])){
					temp += queryWord[k];
				}
				else{
					temp = "";
				}
				if(temp != "" && (!isalpha(queryWord[k+1]) || queryWord[k+1] == ' ')){
					searchWord = temp;
					querriedWords.push_back(searchWord);
				}
			}
		}
	}

	coutHSTResults(myHST, notFound, querriedWords, allFiles);
	coutBSTResults(myBST, querriedWords, allFiles);

	int k = 1000;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++){
		queryAnalysisForBST(myBST, querriedWords, allFiles);
	}
	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	cout << "\nTime: " << BSTTime.count() / k << "\n";
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++){
		queryAnalysisForHST(myHST, notFound, querriedWords, allFiles);
	}
	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - start);
	cout << "\nTime: " << HTTime.count() / k << "\n";
	cout << "Speed Up: " << (double)BSTTime.count()/HTTime.count() << endl;

	return 0;
}

void coutHSTResults(HashTable<WordItem>& funcHST, WordItem& notFound, vector<string>& words, vector<string>& files){
	vector<WordItem> resultsVector = queryAnalysisForHST(funcHST, notFound, words, files);
	if(resultsVector.size()==0){
		cout << "No document contains the given query" << endl;
	}
	else{
		for (unsigned int j = 0; j < files.size(); j++){
			bool willCout = false;
			for (unsigned int i = 0; i < resultsVector.size(); i++){
				willCout = false;
				WordItem temp = resultsVector[i];
				WordItem& temp2 = funcHST.find(temp);
				for(unsigned int k=0; k < temp2.docList.size(); k++){
					if(files[j] == temp2.docList[k].documentName){
						willCout = true;
					}
				}
			}
			if(willCout){
				cout << "in Document " << files[j];
				for (unsigned int i = 0; i < resultsVector.size(); i++){
					WordItem temp = resultsVector[i];
					WordItem& temp2 = funcHST.find(temp);
					for(unsigned int k=0; k < temp2.docList.size(); k++){
						if(files[j] == temp2.docList[k].documentName){
							cout << ", " << temp.word << " found " << temp2.docList[k].count << " times";
						}
					}
				}
				if(willCout){
					cout << "." << endl;
				}
			}
		}
	}
}

vector<WordItem> queryAnalysisForHST(HashTable<WordItem>& funcHST, WordItem& notFound, vector<string>& words, vector<string>& files){
	vector<WordItem> resultingVector;
	int coutNum = 0;
	for (unsigned int i = 0; i < words.size(); i++){
		WordItem hstWord(words[i]);
		WordItem& isExistInHST = funcHST.find(hstWord);
		if(isExistInHST == notFound){
			vector<WordItem> empty;
			return empty;
		}
		else{
			resultingVector.push_back(hstWord);
		}
	}
	if(resultingVector.size() != 0){
		for (unsigned int j = 0; j < files.size(); j++){
			bool willCout = false;
			for (unsigned int i = 0; i < resultingVector.size(); i++){
				willCout = false;
				WordItem temp = resultingVector[i];
				WordItem& temp2 = funcHST.find(temp);
				for(unsigned int k=0; k < temp2.docList.size(); k++){
					if(files[j] == temp2.docList[k].documentName){
						willCout = true;
					}
				}
			}
			if(willCout){
				for (unsigned int i = 0; i < resultingVector.size(); i++){
					WordItem temp = resultingVector[i];
					WordItem& temp2 = funcHST.find(temp);
					for(unsigned int k=0; k < temp2.docList.size(); k++){
						if(files[j] == temp2.docList[k].documentName){
							bool willFinish = true;
						}
					}
				}
			}
		}
	}
	return resultingVector;
}

void coutBSTResults(BinarySearchTree<string>& funcBST, vector<string>& words, vector<string>& files){
	vector<string> resultsVector = queryAnalysisForBST(funcBST, words, files);
	if(resultsVector.size()==0){
		cout << "No document contains the given query" << endl;
	}
	else{
		for (unsigned int j = 0; j < files.size(); j++){
			bool willCout = false;
			for (unsigned int i = 0; i < resultsVector.size(); i++){
				willCout = false;
				string bstWord = words[i];
				BinaryWordNode<string>* isExistInBST = funcBST.findWordNode(bstWord);
				for(unsigned int k=0; k < isExistInBST->docList.size(); k++){
					if(files[j] == isExistInBST->docList[k].documentName){
						willCout = true;
					}
				}
			}
			if(willCout){
				cout << "in Document " << files[j];
				for (unsigned int i = 0; i < resultsVector.size(); i++){
					string bstWord = words[i];
					BinaryWordNode<string>* isExistInBST = funcBST.findWordNode(bstWord);
					for(unsigned int k=0; k < isExistInBST->docList.size(); k++){
						if(files[j] == isExistInBST->docList[k].documentName){
							cout << ", " << bstWord << " found " << isExistInBST->docList[k].count << " times";
						}
					}
				}
				if(willCout){
					cout << "." << endl;
				}
			}
		}
	}
}

vector<string> queryAnalysisForBST(BinarySearchTree<string>& funcBST, vector<string>& words, vector<string>& files){
	vector<string> resultingVector;
	int coutNum = 0;
	for (unsigned int i = 0; i < words.size(); i++){
		string bstWord = words[i];
		BinaryWordNode<string>* isExistInBST = funcBST.findWordNode(bstWord);
		if(isExistInBST == NULL){
			vector<string> empty;
			return empty;
		}
		else{
			resultingVector.push_back(bstWord);
		}
	}
	if(resultingVector.size() != 0){
		for (unsigned int j = 0; j < files.size(); j++){
			bool willCout = false;
			for (unsigned int i = 0; i < resultingVector.size(); i++){
				willCout = false;
				string bstWord = words[i];
				BinaryWordNode<string>* isExistInBST = funcBST.findWordNode(bstWord);
				for(unsigned int k=0; k < isExistInBST->docList.size(); k++){
					if(files[j] == isExistInBST->docList[k].documentName){
						willCout = true;
					}
				}
			}
			if(willCout == true){
				for (unsigned int i = 0; i < resultingVector.size(); i++){
					string bstWord = words[i];
					BinaryWordNode<string>* isExistInBST = funcBST.findWordNode(bstWord);
					for(unsigned int k=0; k < isExistInBST->docList.size(); k++){
						if(files[j] == isExistInBST->docList[k].documentName){
							bool willFinish = true;
						}
					}
				}
			}
		}
	}
	return resultingVector;
}