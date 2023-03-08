/*
CS300-Homework 2
Last Modified:19.04.2022
Author: Kaan Adalilar - (kaanadalilar/28384)
*/
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "BinarySearchTree.cpp"
#include "AVLTree.cpp"
#pragma comment(linker, "/STACK:2000000")
//#pragma comment(linker, "HEAP:2000000")
/*In my computer, after a certain number of compilings (approximately 2-3 times), since the stack becomes full, program gives me an unhandled
exception error (stack overflow) and it could not be compiled. To solve this problem, I update the STACK: size with #pragma comment as needed.*/

using namespace std;

int main(){
	cout << "Welcome to the Notebook!" << endl << endl;
	string notebookDataFile = "data.txt";
	ifstream nbFile;
	nbFile.open(notebookDataFile.c_str());
	string fileLine, secTitle, currentTitle;
	
	BinarySearchTree<string> myNotebook("not found");
	AvlTree<string> myAvlNotebook("not found");
	long long avlTime = 0;
	long long bstTime = 0;
	int sectionCount = 0;
	
	while(getline(nbFile, fileLine)){
		string temp;
		istringstream ss(fileLine);
		ss >> temp;
		if(temp[0] != '-'){
			secTitle = temp;

			if(sectionCount == 0){
				currentTitle = temp;
			}
			else{
				cout << "Section \"" << currentTitle << "\" has been inserted into the AVL notebook." << endl;
				cout << "[AVL] elapsed time: " << avlTime << " microseconds" << endl;
				cout << "Section \"" << currentTitle << "\" has been inserted into the BST notebook." << endl;
				cout << "[BST] elapsed time: " << bstTime << " microseconds" << endl << endl;
				avlTime = 0;
				bstTime = 0;
				currentTitle = temp;
			}
			auto bstStart = chrono::high_resolution_clock::now();
			myNotebook.insertSection(secTitle);
			auto bstEnd = chrono::high_resolution_clock::now();
			long long bstTimer = (bstEnd-bstStart).count() / 1000;
			bstTime += bstTimer;
			
			auto avlStart = chrono::high_resolution_clock::now();
			myAvlNotebook.insertSection(secTitle);
			auto avlEnd = chrono::high_resolution_clock::now();
			long long avlTimer = (avlEnd-avlStart).count() / 1000;
			avlTime += avlTimer;
			sectionCount++;
		}
		else{
			string itemTitle, itemSentence;
			if(temp.substr(1, temp.length()-1).find("-") != string::npos){
				itemTitle += temp.substr(1, temp.rfind("-")-1);
				itemSentence += temp.substr(temp.rfind("-")+1,temp.length()-1) + " ";
				while(ss >> temp){
					if(temp.find("-") != string::npos){
						itemSentence += temp.substr(0, temp.rfind("-")); 
						itemSentence += temp.substr(temp.rfind("-")+1,temp.length()-1) + " ";
					}
					else
						itemSentence += temp.substr(0, temp.length()) + " ";
				}
			}
			else{
				itemTitle += temp.substr(1, temp.length()-1) + " ";
				while(ss >> temp){
					if(temp.find("-") != string::npos){
						itemTitle += temp.substr(0, temp.rfind("-"));
						itemSentence += temp.substr(temp.rfind("-")+1,temp.length()-1) + " ";
					}
					else
						itemTitle += temp.substr(0, temp.length()) + " ";
				}
			}
			auto avlStart = chrono::high_resolution_clock::now();
			myAvlNotebook.insertItem(itemTitle, itemSentence, myAvlNotebook.findSecNode(secTitle)->secItems);
			auto avlEnd = chrono::high_resolution_clock::now();
			long long avlTimer = (avlEnd-avlStart).count() / 1000;
			avlTime += avlTimer;

			auto start = chrono::high_resolution_clock::now();
			myNotebook.insertItem(itemTitle, itemSentence, myNotebook.findSecNode(secTitle)->secItems);
			auto end = chrono::high_resolution_clock::now();
			long long timer2 = (end-start).count() / 1000;
			bstTime += timer2;
		}
	}
	cout << "Section \"" << secTitle << "\" has been inserted into the AVL notebook." << endl;
	cout << "[AVL] elapsed time: " << avlTime << " microseconds" << endl;
	cout << "Section \"" << secTitle << "\" has been inserted into the BST notebook." << endl;
	cout << "[BST] elapsed time: " << bstTime << " microseconds" << endl << endl;
	//MENU
	int option;
	cout << "MENU" << endl <<
		"Please enter an input between [1 - 6]:" << endl <<
		"1- Display the sections [AVL]" << endl <<
		"2- Display the sections [BST]" << endl <<
		"3- Select a section" << endl <<
		"4- Add new section" << endl <<
		"5- Delete a section" << endl <<
		"6- Exit" << endl;
	bool programFinished = false;
	while(programFinished == false){
		cout << "Input: ";
		cin >> option;

		if(option == 1){ //display the sections from AVL
			cout << endl << "*****" << endl;
			myAvlNotebook.printSections(myAvlNotebook.getRoot());
			cout << "*****" << endl << endl;;
		}
		
		else if(option == 2){ //display the sections from BST
			cout << endl << "*****" << endl;
			myNotebook.printTree(myNotebook.getRoot());
			cout << "*****" << endl << endl;
		}
		
		else if(option == 3){ //proceed with a wanted section
			string sectionTitle;
			cout << "Enter the title of the section: ";
			cin.get();
			getline(cin, sectionTitle);
			
			AvlSectionNode<string>* avlSectionPtr = myAvlNotebook.findSecNode(sectionTitle);
			BinarySectionNode<string>* bstSectionPtr = myNotebook.findSecNode(sectionTitle);
			
			if(avlSectionPtr == NULL && bstSectionPtr == NULL){
				cout << "Invalid title!" << endl << endl;
			}
			else{
				cout << endl << "Selected section -> " << sectionTitle << endl;
				int sectionInput;
				cout << "Please enter an input between [1-7]:" << endl <<
					"1- Display the items [AVL]" << endl <<
					"2- Display the items [BST]" << endl <<
					"3- Display the information of a item" << endl <<
					"4- Add new item" << endl <<
					"5- Update the information of a item" << endl <<
					"6- Delete an item" << endl <<
					"7- Return to main menu" << endl;

				bool selectionFinished = false;
				while(selectionFinished == false){
					cout << "Input: ";
					cin >> sectionInput;
			
					if(sectionInput == 1){ //display the items of selected section from AVL
						cout << endl << "*****" << endl;
						if(avlSectionPtr != NULL){
							myAvlNotebook.printItems(avlSectionPtr->secItems);
						}
						cout << "*****" << endl << endl;
					}
				
					else if(sectionInput == 2){ //display the items of selected section from BST
						cout << endl << "*****" << endl;
						myNotebook.printItems(bstSectionPtr->secItems);
						cout << "*****" << endl << endl;
					}
				
					else if(sectionInput == 3){ //display the information of a item 
						string itemTitle;
						cout << "Enter the title of the item: ";
						cin.get();
						getline(cin, itemTitle);
						
						auto avlStart = chrono::high_resolution_clock::now();
						AvlItemNode<string>* avlCheckPtr = myAvlNotebook.findSecItem(itemTitle, avlSectionPtr->secItems); 
						auto avlEnd = chrono::high_resolution_clock::now();
						long long avlTimer = (avlEnd-avlStart).count() / 1000;

						auto start = chrono::high_resolution_clock::now();
						BinaryItemNode<string>* checkPtr = myNotebook.findSecItem(itemTitle, bstSectionPtr->secItems); 
						auto end = chrono::high_resolution_clock::now();
						long long bstTimer = (end-start).count() / 1000;
						
						if(avlCheckPtr == NULL && checkPtr == NULL){ //if does not exist
							cout << "[AVL] elapsed time: " << avlTimer << " microseconds" << endl;
							cout << "[BST] elapsed time: " << bstTimer << " microseconds" << endl;
							cout << "Invalid title!" << endl << endl;
						}
						else{
							auto avlStart = chrono::high_resolution_clock::now();
							string avlInfo = avlCheckPtr->itInfo;
							auto avlEnd = chrono::high_resolution_clock::now();
							avlTimer += (avlEnd-avlStart).count() / 1000;
							
							auto start = chrono::high_resolution_clock::now();
							string bstInfo = checkPtr->itInfo;
							auto end = chrono::high_resolution_clock::now();
							bstTimer += (end-start).count() / 1000;
							
							cout << "[AVL] elapsed time: " << avlTimer << " microseconds" << endl;
							cout << "[BST] elapsed time: " << bstTimer << " microseconds" << endl;
							cout << checkPtr->itInfo << endl << endl;
						}
					}
				
					else if(sectionInput == 4){ //add new item
						string newItemTitle, newItemInfo;
						cout << "Enter a title for the item: ";
						cin.get();
						getline(cin, newItemTitle);

						if(myNotebook.findSecItem(newItemTitle, bstSectionPtr->secItems) != NULL && myAvlNotebook.findSecItem(newItemTitle, avlSectionPtr->secItems) != NULL){
							cout << "Item \"" << newItemTitle << "\" already exists in the \"" << sectionTitle << "\"." << endl << endl;
						}
						else{
							cout << "Enter a description for the item: ";
							getline(cin, newItemInfo);
							
							auto avlStart = chrono::high_resolution_clock::now();
							myAvlNotebook.insertItem(newItemTitle, newItemInfo, avlSectionPtr->secItems);
							auto avlEnd = chrono::high_resolution_clock::now();
							long long avlTimer = (avlEnd-avlStart).count() / 1000;
							
							auto bstStart = chrono::high_resolution_clock::now();
							myNotebook.insertItem(newItemTitle, newItemInfo, bstSectionPtr->secItems); 
							auto bstEnd = chrono::high_resolution_clock::now();
							long long bstTimer = (bstEnd-bstStart).count() / 1000;
							
							cout << "[AVL] elapsed time: " << avlTimer << " microseconds" << endl;
							cout << "[BST] elapsed time: " << bstTimer << " microseconds" << endl;
							cout << "The new item \"" << newItemTitle << "\" has been inserted." << endl << endl;
						}
					}
				
					else if(sectionInput == 5){ //update item info
						string updateItemTitle;
						cout << "Enter the title of the item: ";
						cin.get();
						getline(cin, updateItemTitle);
						
						auto updateAVLstart = chrono::high_resolution_clock::now();
						AvlItemNode<string>* avlItemPtr = myAvlNotebook.findSecItem(updateItemTitle, avlSectionPtr->secItems);
						auto updateAVLend = chrono::high_resolution_clock::now();
						long long updateAVLTimer = (updateAVLend-updateAVLstart).count() / 1000;

						auto updateBSTstart = chrono::high_resolution_clock::now();
						BinaryItemNode<string>* item = myNotebook.findSecItem(updateItemTitle, bstSectionPtr->secItems);
						auto updateBSTend = chrono::high_resolution_clock::now();
						long long updateBSTTimer = (updateBSTend-updateBSTstart).count() / 1000;

						cout << "[AVL] elapsed time: " << updateAVLTimer << " microseconds" << endl;
						cout << "[BST] elapsed time: " << updateBSTTimer << " microseconds" << endl;
						if(avlItemPtr == NULL && item == NULL){ //if the item does not exist
							cout << "Item \"" << updateItemTitle << "\" does not exist in the \"" << sectionTitle << "\"." << endl << endl;
						}
						else{
							string newInfo;
							cout << "Enter the new information: ";
							getline(cin, newInfo);
							avlItemPtr->itInfo = newInfo;
							item->itInfo = newInfo;
							cout << "The content \"" << updateItemTitle << "\" has been updated." << endl << endl;
						}
					}
				
					else if(sectionInput == 6){
						string deleteItemTitle;
						cout << "Enter the title of the item: ";
						cin.get();
						getline(cin, deleteItemTitle);

						if(myNotebook.findSecItem(deleteItemTitle, bstSectionPtr->secItems) == NULL &&  myAvlNotebook.findSecItem(deleteItemTitle, avlSectionPtr->secItems) == NULL){ //if the item does not exist
							cout << "Item \"" << deleteItemTitle << "\" does not exist in the \"" << sectionTitle << "\"." << endl << endl;
						}
						else{
							auto avlStart = chrono::high_resolution_clock::now(); 
							myAvlNotebook.removeItem(deleteItemTitle, avlSectionPtr->secItems);
							auto avlEnd = chrono::high_resolution_clock::now();
							long long timerOfAVL = (avlEnd-avlStart).count() / 1000;

							auto start = chrono::high_resolution_clock::now(); 
							myNotebook.removeItem(deleteItemTitle, bstSectionPtr->secItems);
							auto end = chrono::high_resolution_clock::now();
							long long bstTime = (end-start).count() / 1000;

							cout << "[AVL] elapsed time: " << timerOfAVL << " microseconds" << endl;
							cout << "[BST] elapsed time: " << bstTime << " microseconds" << endl;
							cout << "The item \"" << deleteItemTitle << "\" has been deleted." << endl << endl;
						}
					}

					else if(sectionInput == 7){
						selectionFinished = true;
						cout << "MENU" << endl <<
							"Please enter an input between [1 - 6]:" << endl <<
							"1- Display the sections [AVL]" << endl <<
							"2- Display the sections [BST]" << endl <<
							"3- Select a section" << endl <<
							"4- Add new section" << endl <<
							"5- Delete a section" << endl <<
							"6- Exit" << endl;
					}
				}
			}
		}
		
		else if(option == 4){ //add a new section
			string newSectionTitle;
			cout << "Enter a title for the section: ";
			cin.get();
			getline(cin, newSectionTitle);
			if(myNotebook.findSecNode(newSectionTitle) != NULL && myAvlNotebook.findSecNode(newSectionTitle) != NULL){
				cout << "Section \"" << newSectionTitle << "\" already exists." << endl << endl;
			}
			else{
				myAvlNotebook.insertSection(newSectionTitle);
				myNotebook.insertSection(newSectionTitle);
				cout << "The new section \"" << newSectionTitle << "\" has been inserted." << endl << endl;
			}
		}
		
		else if(option == 5){ //delete a section
			string deleteSectionTitle;
			cout << "Enter a title of the section: ";
			cin.get();
			getline(cin, deleteSectionTitle);
			if(myNotebook.findSecNode(deleteSectionTitle) == NULL && myAvlNotebook.findSecNode(deleteSectionTitle) == NULL){ //if the title does not exist
				cout << "Section \"" << deleteSectionTitle << "\" does not exist." << endl << endl;
			}
			else{
				myAvlNotebook.removeSection(deleteSectionTitle);
				myNotebook.removeSection(deleteSectionTitle);
				cout << "The section has been deleted." << endl << endl; 
			}
		}
		
		else if(option == 6){ //terminate the program to exit
			programFinished = true;
			exit(0);
		}
	}
	return 0;
}