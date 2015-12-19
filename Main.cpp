#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>

#include "Stack.h"
#include "strutils.h"

using namespace std;

int main()
{
	//-- Render variables --//

	//-- Define and initialize the "world" rectangle --//
	sf::RectangleShape mainRect;
	mainRect.setPosition(0,0);
	mainRect.setSize(sf::Vector2f(20, 20));
	mainRect.setFillColor(sf::Color::Red);


	//-- Input variables --//
	string inputBuffer;
	vector<string> inputVector;

	int mazeWidth;
	int mazeHeight;

	//-- Starting points for the AI --//
	int startX;
	int startY;

	//-- Current position of the AI --//
	int currentX;
	int currentY;

	//-- Main solution stack --//
	Stack mainStack;

	//-- Temporary insertion data --//
	dataCell tempData;

	//-- Keep track of the previous "move" --//
	string lastMove;

	//-- Boolean to check if solved --//
	bool goalState = false;

	//-- Result vector, it contains the dump of the solution --//
	vector<dataCell> resVector;

	//-- Retrieve the input data --//
	cout << "Please enter the input data for the maze below : " << endl;

	//-- The first line contains the input information --//
	getline(cin,inputBuffer);

	mazeWidth = atoi(inputBuffer.substr(0,inputBuffer.find(" ")));
	mazeHeight = atoi(inputBuffer.substr(inputBuffer.find(" "),INT_MAX));

	//-- The second line defines the beginning of the maze --//
	getline(cin,inputBuffer);

	startY = atoi(inputBuffer.substr(0,inputBuffer.find(" ")));
	startX = atoi(inputBuffer.substr(inputBuffer.find(" "),INT_MAX));


	//-- Store the maze in a vector --//
	for(int i = 0; i < mazeHeight; i++)
	{
		getline(cin,inputBuffer);

		//-- Clear all the white spaces --//
		while(inputBuffer.find(" ") != string::npos)
		{ inputBuffer.erase(inputBuffer.find(" "),1); }
		StripWhite(inputBuffer);

		inputVector.push_back(inputBuffer);
	}

	/* Uncomment this section to see the contents of the stored input.
	for(int i = 0; i < inputVector.size(); i++)
	{ cout << inputVector[i] << " " << i << endl; }*/


	//-- The main render window --//
	sf::RenderWindow window(sf::VideoMode((mazeWidth)*20, (mazeHeight)*20), "CS 300 - Maze", sf::Style::Close);

	currentX = startX;
	currentY = startY;

	tempData.xCord = currentX;
	tempData.yCord = currentY;
	tempData.strData = "";
	mainStack.push(tempData);

	//-- Main render loop --//
	while (window.isOpen())
	{

		//-- The code below checks for user bound events such as keyboard, UI, mouse and joystick events --//
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//-- This Method gets the mouse position --//
		sf::Vector2i mouse = sf::Mouse::getPosition(window);

		//-- This code hides the console window, MS-WINDOWS specific --//
		//HWND hWnd = GetConsoleWindow();
		//ShowWindow( hWnd, SW_HIDE ); 


		//-- Clear and draw methods --//

		window.clear(sf::Color::Black);

		//-- This section draws the world state --//
		for(int i = 0; i < inputVector.size(); i++)
		{ 
			for(int j = 0; j < inputVector[i].length(); j++)
			{
				mainRect.setPosition(20*j,20*i);
				mainRect.setFillColor(sf::Color::Red);
				if(inputVector[i][j] == '1')
				{
					window.draw(mainRect);
				}
			}
		}

		if(mainStack.size() != 0)
		{
			//-- Add some delay --//
			Sleep(200);

			//-- Check if the current position is a goal state --//
			if(currentY + 1 >= inputVector.size() || currentX + 1 >= inputVector[0].size())
			{
				if(currentX != startX || currentY != startY)
				{
					//-- East, South goal state --//
					goalState = true;
				}
			}

			else if(currentY - 1 < 0 || currentX - 1 < 0)
			{
				if(currentX != startX || currentY != startY)
				{
					//-- West, North goal state --//
					goalState = true;
				}
			}

			//-- Goal State has not been reached, make some moves --//
			if(!goalState)
			{
				//-- Some datacells that represent the up down left and right moves, these will be used for probing --//
				dataCell tempRight;
				tempRight.xCord = currentX + 1;
				tempRight.yCord = currentY;

				dataCell tempLeft;
				tempLeft.xCord = currentX - 1;
				tempLeft.yCord = currentY;

				dataCell tempUp;
				tempUp.xCord = currentX;
				tempUp.yCord = currentY - 1;

				dataCell tempDown;
				tempDown.xCord = currentX;
				tempDown.yCord = currentY + 1;

				//-- Move Left --//
				if(currentX - 1 >= 0 && mainStack.isPresent(tempLeft) == false && inputVector[currentY][currentX - 1] != '1')
				{
					currentX--;

					//-- Update the temp data and Push --//
					tempData.xCord = currentX;
					tempData.yCord = currentY;
					tempData.strData = "";
					mainStack.push(tempData);
				}

				//-- Move Right --//
				else if(currentX + 1 < mazeWidth && mainStack.isPresent(tempRight) == false && inputVector[currentY][currentX + 1] != '1')
				{
					currentX++;

					//-- Update the temp data and Push --//
					tempData.xCord = currentX;
					tempData.yCord = currentY;
					tempData.strData = "";
					mainStack.push(tempData);
				}

				//-- Move Down --//
				else if(currentY + 1 < mazeHeight && mainStack.isPresent(tempDown) == false && inputVector[currentY + 1][currentX] != '1')
				{
					currentY++;

					//-- Update the temp data and Push --//
					tempData.xCord = currentX;
					tempData.yCord = currentY;
					tempData.strData = "";
					mainStack.push(tempData);
				}

				//-- Move Up --//
				else if(currentY - 1 >= 0 && mainStack.isPresent(tempUp) == false && inputVector[currentY - 1][currentX] != '1')
				{
					currentY--;

					//-- Update the temp data and Push --//
					tempData.xCord = currentX;
					tempData.yCord = currentY;
					tempData.strData = "";
					mainStack.push(tempData);
				}

				//-- No possible moves, back-track --//
				else
				{
					//-- Set the current position to '1' to prevent retaking the same path --//
					inputVector[currentY][currentX] = '1';
					tempData = mainStack.pop();
				}

				mainRect.setPosition(20*currentX,20*currentY);
				mainRect.setFillColor(sf::Color::Cyan);
				window.draw(mainRect);

				cout << tempData.xCord << " " << tempData.yCord << endl;
				//system("PAUSE");

			}


			else
			{
				//-- Empty out the mainStack and exit the main loop --//
				while(mainStack.size() != 0)
				{
					resVector.push_back(mainStack.pop());
				}
			}
		}


		//-- Draw the result path in this section --//
		if(resVector.size() > 1)
		{
			for(int i = 0; i < resVector.size(); i++)
			{
				mainRect.setPosition(20*resVector[i].xCord,20*resVector[i].yCord);
				mainRect.setFillColor(sf::Color::Green);
				window.draw(mainRect);
			}
		}

		window.display();

	}




	cout << endl << endl;
	cout << "Solution to the maze is : " << endl;

	for(int i = resVector.size() - 1; i >= 0; i--)
	{ cout << resVector[i].yCord << " " << resVector[i].xCord << endl; }

	system("PAUSE");
	return 0;

	//-- END --//
}