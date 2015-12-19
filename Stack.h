#ifndef STACK_H_
#define STACK_H_

#include <string>
#include <vector>
#include <iostream>

using namespace std;

//-- Structure for storing data in each individual Cell --//

struct dataCell
{
	string strData;
	int xCord;
	int yCord;
};

//-- Class Definiton for Stack data Structure --//

class Stack
{
private:
	vector<dataCell> mainStorage;

public:
	Stack();

	//-- Pushes an element to the top of the Stack --//
	void push(dataCell newItem);

	//-- Pops an element off the top of the Stack and return it --//
	dataCell pop();

	//-- Returns the size of the Stack --//
	int size();

	//-- Prints out the raw Data of the Stack --//
	void dumpRaw();

	//-- Clear the stack of all its elements --//
	void clear();

	//-- Check to see if the given item is present in the stack --//
	bool isPresent(dataCell item);
};



#endif