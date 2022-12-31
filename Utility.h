#pragma once
#include <windows.h>
#include <stdio.h>
#include <assert.h>



//clear the input buffer
void clearInputBuffer()
{
	char c;
	while ((c = getchar()) != '\n');
}



//returns an int as input by user between specified bounds
int getUserInputInt(int leftBound, int rightBound)
{
    //get the input
    int input;
    scanf_s("%d", &input);

    //check the validity of the input
    if (input < leftBound || input > rightBound)
    {
        clearInputBuffer();
        return getUserInputInt(leftBound, rightBound);
    }
    else
    {
        return input;
    }
}



//returns a pointer to a char array 
//with each member of the char array
//as input by user
//After this char array has been used by the game it needs to be freed
char* getUserInputCharArray(int lengthOfCode)
{
	int numCharacters = 0;
	char* input = malloc(lengthOfCode * sizeof(char));

	//check if allocation was successful
	if (input == NULL)
	{
		printf("not enough memory");
		exit(0);
	}

	clearInputBuffer();

	//this ensures no access violation
	while (numCharacters < lengthOfCode)
	{
		char currentCharacter = '0';
		scanf_s("%c", &currentCharacter, 1);

		if (currentCharacter == '\n')
		{
			free(input);
			printf("Code not long enough\n");
			return getUserInputCharArray(lengthOfCode);
		}
		else
		{
			*(input + numCharacters) = currentCharacter;
		}
		numCharacters++;
	}

	return input;
}



void clearScreen()
{
	COORD topLeft = { 0,0 };
	DWORD cCharsWritten;
	HANDLE screenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;



	//Get the number of character cells in the current buffer
	if (!GetConsoleScreenBufferInfo(screenBuffer, &csbi))
	{
		printf("error calling GetConsoleScreenBufferInfo");
		abort();
	}



	//Calculating the number of cells in the console
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;



	//Fill the entire screen with blanks
	if (!FillConsoleOutputCharacter(screenBuffer,		//Handle to console Screen Buffer
		(TCHAR)' ',										//Character to write to the buffer
		dwConSize,										//Number of cells to write
		topLeft,										//Coordinate of first cell
		&cCharsWritten))								//Recieve number of characters written
	{
		printf("error calling FillConsoleOutputCharacter");
		return;
	}



	//Get the current text attribute
	if (!GetConsoleScreenBufferInfo(screenBuffer, &csbi))
	{
		printf("error calling GetConsoleScreenBufferInfo");
		abort();
	}



	//set the buffer's attributes accordingly
	if (!FillConsoleOutputAttribute(screenBuffer,		//Handle to console Screen Buffer	
		csbi.wAttributes,								//Character attributes to use
		dwConSize,										//Number of cells to set attribute
		topLeft,										//Coordinates of the first cell
		&cCharsWritten))								//Recieve number of characters written
	{
		return;
	}



	//Put the cursor at its home coordinates
	SetConsoleCursorPosition(screenBuffer, topLeft);
}



//generate a random number between two bounds
int generateRandomNumber(int leftBound, int rightBound)
{
	return ((rand() % (rightBound - leftBound + 1)) + leftBound);
}