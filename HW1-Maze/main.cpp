/*
CS300-Homework 1
Last Modified:30.03.2022
Author: Kaan Adalilar - (kaanadalilar/28384)
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "strutils.h" //used for seting the file names
#include "randgen.h"
#include "Stack.cpp"

using namespace std;

struct cell{
	int xCoor, yCoor; //x will be set according to the number of columns // y will be set according to the number of rows
	int myLeft, myRight, myUp, myDown; //=1 if there is a wall; =0 if there is no wall
	bool checkVisit;

	cell(){
		checkVisit = false;
		myUp = 1;
		myDown = 1;
		myLeft = 1;
		myRight = 1;
	}
};

void PathFinder(vector<vector<vector<cell>>> myAllMazes, int numOfMazes, int numOfRows, int numOfCols){
	int mazeID, mazeIndex;
	cout << "Enter a maze ID between 1 to " << numOfMazes << " inclusive to find a path: ";
	cin >> mazeID;
	mazeIndex = mazeID-1; //mazeID starts from 1 which is equivalent to the index 0 of the allMazes vector

	int entryX, entryY;
	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin >> entryX >> entryY;

	int exitX, exitY;
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> exitX >> exitY;

	vector<vector<cell>> wantedMaze(numOfRows, vector<cell>(numOfCols, cell()));
	wantedMaze = myAllMazes[mazeIndex];
	for (int j=0; j < numOfRows; j++){   
		for (int k=0; k < numOfCols; k++){   
			wantedMaze[j][k].checkVisit = false; //seting again all of the cells as unvisited
		}
	}
	Stack<cell> pathStack;
	pathStack.push(wantedMaze[entryY][entryX]); //iteration will start with the entrance cell

	while (pathStack.top().xCoor != exitX || pathStack.top().yCoor != exitY){
		vector<char> canGo;
		if(pathStack.top().myLeft == 0 && pathStack.top().xCoor-1 >= 0 && wantedMaze[pathStack.top().yCoor][pathStack.top().xCoor-1].checkVisit == false){ //can go left
			canGo.push_back('L'); //to be able to go to left cell, there has to be no wall between x. cell and (x-1). cell
		}
		if(pathStack.top().myRight == 0 && pathStack.top().xCoor+1 < numOfCols && wantedMaze[pathStack.top().yCoor][pathStack.top().xCoor+1].checkVisit == false){ //can go right
			canGo.push_back('R'); //to be able to go to right cell, there has to be no wall between x. cell and (x+1). cell
		}
		if(pathStack.top().myUp == 0 && pathStack.top().yCoor+1 < numOfRows && wantedMaze[pathStack.top().yCoor+1][pathStack.top().xCoor].checkVisit == false){ //can go up
			canGo.push_back('U'); //to be able to go to above cell, there has to be no wall between y. cell and (y+1). cell
		}
		if(pathStack.top().myDown == 0 && pathStack.top().yCoor-1 >= 0 && wantedMaze[pathStack.top().yCoor-1][pathStack.top().xCoor].checkVisit == false){ //can go down 
			canGo.push_back('D'); //to be able to go to below cell, there has to be no wall between y. cell and (y-1). cell
		}

		if(canGo.size() == 0){
			pathStack.pop();
		}
		else{
			RandGen rand;
			int randomIndex = rand.RandInt(canGo.size());
			wantedMaze[pathStack.top().yCoor][pathStack.top().xCoor].checkVisit = true;
			if (canGo[randomIndex] == 'L'){
				wantedMaze[pathStack.top().yCoor][pathStack.top().xCoor-1].checkVisit = true;
				pathStack.push(wantedMaze[pathStack.top().yCoor][pathStack.top().xCoor-1]);
			}
			else if (canGo[randomIndex] == 'R'){
				wantedMaze[pathStack.top().yCoor][pathStack.top().xCoor+1].checkVisit = true;
				pathStack.push(wantedMaze[pathStack.top().yCoor][pathStack.top().xCoor+1]);
			}
			else if (canGo[randomIndex] == 'U'){
				wantedMaze[pathStack.top().yCoor+1][pathStack.top().xCoor].checkVisit = true;
				pathStack.push(wantedMaze[pathStack.top().yCoor+1][pathStack.top().xCoor]);
			}
			else if (canGo[randomIndex] == 'D'){
				wantedMaze[pathStack.top().yCoor-1][pathStack.top().xCoor].checkVisit = true;
				pathStack.push(wantedMaze[pathStack.top().yCoor-1][pathStack.top().xCoor]);
			}
		}
	}
	ofstream pathDoc;
	string pathFile = "maze_" + itoa(mazeID) + "_path_" + itoa(entryX) + "_" + itoa(entryY) + "_" + itoa(exitX) + "_" + itoa(exitY) + ".txt";
	pathDoc.open(pathFile.c_str());
	
	//to print the stack in a proper order, first all of the elements has to be transferred into another stack
	//(first:a-b-c-d:last) -> (first:d-c-b-a:last) ->[printing] a,b,c,d
	Stack<cell> printStack;
	while(!pathStack.isEmpty()){
		cell transferCell = pathStack.topAndPop();
		printStack.push(transferCell);
	}
	while(!printStack.isEmpty()){
		cell printCell = printStack.topAndPop();
		pathDoc << printCell.xCoor  << " " << printCell.yCoor << endl; //writing all the coordinates to the .txt file
	}
}

int main(){
	int mazeCount;
	cout << "Enter the number of mazes: ";
	cin >> mazeCount;
	int rowCount, colCount;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> rowCount >> colCount;
	
	//in this program maze (matrix) is structured as a vector of vectors of structs cell)
	vector<vector<vector<cell>>> allMazes(mazeCount); //this vector keeps the all mazes together 

	int formedMazes = 0;
	while(formedMazes != mazeCount){ //will be iterated until formedMazes == mazeCount
		ofstream mazeDoc;
		string outputFile = "maze_" + itoa(formedMazes+1) + ".txt";
		mazeDoc.open(outputFile.c_str());
		Stack<cell> mazeStack; //to trace all the visited cells, cells will be pushed into the stack
		vector<vector<cell>> mazeBoard(rowCount, vector<cell>(colCount, cell()));

		//initially forming an MxN (rowCount*colCount) maze with constructing all the walls
		for (int j=0; j < rowCount; j++){  
			for (int k=0; k < colCount; k++){   
				mazeBoard[j][k].xCoor = k; //as columns become larger -> x become larger
				mazeBoard[j][k].yCoor = j; //rows of the matrix and y coordinates of the coordinate system are in opposite directions
				mazeBoard[j][k].myUp = 1;
				mazeBoard[j][k].myDown = 1;
				mazeBoard[j][k].myLeft = 1;
				mazeBoard[j][k].myRight = 1;
				mazeBoard[j][k].checkVisit = false;
			}
		}
		mazeStack.push(mazeBoard[0][0]); //iteration will start with the (0,0) cell
		mazeBoard[0][0].checkVisit = true;
		int unvisitedCount = rowCount*colCount;
		while (unvisitedCount > 0 && mazeStack.isEmpty()==false){ //iteration will continue until there is no remaining unvisited cells
			vector<char> goTo; //vector of chars will keep the available directions that can be reached from the current cell

			if(mazeStack.top().xCoor-1 >= 0 && mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor-1].checkVisit == false){
				goTo.push_back('L'); //to go to the left side xCoor has to be greater than 0 because there is no -1. column
			}
			if(mazeStack.top().xCoor+1 < colCount && mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor+1].checkVisit == false){
				goTo.push_back('R'); //to go to the right side xCoor has to be in the column range
			}
			if(mazeStack.top().yCoor+1 < rowCount && mazeBoard[mazeStack.top().yCoor+1][mazeStack.top().xCoor].checkVisit == false){
				goTo.push_back('U'); //to go to the up side yCoor has to be in the row range
			}
			if(mazeStack.top().yCoor-1 >= 0 && mazeBoard[mazeStack.top().yCoor-1][mazeStack.top().xCoor].checkVisit == false){
				goTo.push_back('D'); //to go to the down cell yCoor has to be greater than 0 because there is no -1. row
			}

			if(goTo.size() == 0){ //if there is no char in the vector;
				mazeStack.pop(); //it means that there is no available walls to knock down
			}
			else{ //choose a random wall
				RandGen rand;
				int randomIndex = rand.RandInt(goTo.size());

				if (goTo[randomIndex] == 'L'){
					mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor].myLeft = 0; //knock down the left-side wall
					mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor-1].myRight = 0; //left-side wall of x is also the right-side wall of the x-1
					mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor-1].checkVisit = true; //left cell is also visited and;
					mazeStack.push(mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor-1]); //pushed into the stack
				}
				else if (goTo[randomIndex] == 'R'){
					mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor].myRight = 0; //knock down the right-side wall
					mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor+1].myLeft = 0; //right-side wall of x is also the left-side wall of the x+1
					mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor+1].checkVisit = true; //right cell is also visited and;
					mazeStack.push(mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor+1]); //pushed into the stack
				}
				else if (goTo[randomIndex] == 'U'){
					mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor].myUp = 0; //knock down the up-side wall
					mazeBoard[mazeStack.top().yCoor+1][mazeStack.top().xCoor].myDown = 0; //down-side wall of y is also the up-side wall of the y+1
					mazeBoard[mazeStack.top().yCoor+1][mazeStack.top().xCoor].checkVisit = true; //above cell is also visited and;
					mazeStack.push(mazeBoard[mazeStack.top().yCoor+1][mazeStack.top().xCoor]); //pushed into the stack
				}
				else if (goTo[randomIndex] == 'D'){
					mazeBoard[mazeStack.top().yCoor][mazeStack.top().xCoor].myDown = 0; //knock down the down-side wall
					mazeBoard[mazeStack.top().yCoor-1][mazeStack.top().xCoor].myUp = 0; //up-side wall of y is also the down-side wall of the y-1
					mazeBoard[mazeStack.top().yCoor-1][mazeStack.top().xCoor].checkVisit = true; //below cell is also visited and;
					mazeStack.push(mazeBoard[mazeStack.top().yCoor-1][mazeStack.top().xCoor]); //pushed into the stack
				}
				unvisitedCount--;
			}
		}
		allMazes[formedMazes] = mazeBoard;
		formedMazes++;
		mazeDoc << rowCount << " " << colCount << endl; //row count and the column count will be the first line of the .txt file
		for (int j=0; j < rowCount; j++){ //writing the info in the maze cells to the .txt file
			for (int k=0; k < colCount; k++){   
				mazeDoc << "x=" << mazeBoard[j][k].xCoor << " y=" << mazeBoard[j][k].yCoor << " l=" << mazeBoard[j][k].myLeft << " r=" << mazeBoard[j][k].myRight << " u=" << mazeBoard[j][k].myUp << " d=" << mazeBoard[j][k].myDown << endl;
			}
		}
	}
	cout << "All mazes are generated." << endl << endl;
	/*MAZE GENERATION PART IS COMPLETED
	BELOW IS THE PATH FINDER PART (function called)*/

	PathFinder(allMazes, mazeCount, rowCount, colCount);

	return 0;
}