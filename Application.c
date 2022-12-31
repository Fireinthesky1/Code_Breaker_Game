#include "Utility.h"
#include <math.h>
#include <time.h>



//prototypes
int containsCharactersNotIntSymbolSet(char* userCode);
int characterCanStillBeCounted(char c);
int numCharactersInPosition(char* userCode);
int* initializeSymbolSet();
int* initializeKey();
char* initializeCode();
void printBoard(char* userCode);
void playAgain();
void playGame();
int checkGameOver(char* userCode);



//global variables
int numAttempts;
int turn;
int lengthOfCode;
int lengthOfSymbolSet;
int message = 0;
int *symbolSet;
int *key;
char *code;




int main()
{
	srand(time(0));

	playGame();

	return 0;
}




void initializeGame()
{
	printf("Please enter length of code (3-8): \n");
	lengthOfCode = getUserInputInt(3,8);

	clearScreen();

	printf("Please enter the number of symbols to be used in the code (4-8): \n");
	lengthOfSymbolSet = getUserInputInt(4,8);

	numAttempts = 1 + (ceil)(lengthOfCode * log2(lengthOfSymbolSet));

	turn = 1;

	symbolSet = initializeSymbolSet();

	code = initializeCode();

	key = initializeKey();

	clearScreen();
}



int* initializeKey()
{
	//allocate memory
	int* ptr = (int*)malloc(lengthOfSymbolSet * sizeof(int));

	//check if allocation was successful
	if (ptr == NULL)
	{
		printf("not enough memory");
		exit(0);
	}

	for (int i = 0; i < lengthOfSymbolSet; i++)
	{
		int currentSymbol = *(symbolSet + i);
		int numberOfOccurrences = 0;
		for (int j = 0; j < lengthOfCode; j++)
		{
			if (currentSymbol == *(code + j))
				numberOfOccurrences++;
		}
		*(ptr + i) = numberOfOccurrences;
	}

	return ptr;
}




//Allocates the memory for the array
//Initializes the array
//Returns a pointer to that array
//Need to make sure there are no duplicates in the symbol set
int* initializeSymbolSet()
{
	//allocate memory
	int *ptr = (int*)malloc(lengthOfSymbolSet * sizeof(int));

	//check if allocation was successful
	if (ptr == NULL)
	{
		printf("not enough memory");
		exit(0);
	}

	//initialize and verify uniqueness of symbol in the symbol set
	for (int i = 0; i < lengthOfSymbolSet; i++)
	{
		//set to a random askii value
		int newNumber = generateRandomNumber(33, 126);
		//check that we have a unique number
		int uniqueNumber = 0;

		while (!uniqueNumber)
		{
			for (int j = 0; j < i; j++)
			{
				//if we've found its duplicate
				if (*(ptr + j) == newNumber);
				{
					newNumber = generateRandomNumber(33, 126);
					break;
				}
			}
			//We've made it through the for loop and thus have a unique number
			uniqueNumber = 1;
		}

		*(ptr + i) = newNumber;
	}

	return ptr;
}




char* initializeCode()
{
	//allocate the memory
	char *ptr = (char*)malloc(lengthOfCode * sizeof(char));

	//check if allocation was successful
	if (ptr == NULL)
	{
		printf("not enough memory");
		exit(0);
	}

	for (int i = 0; i < lengthOfCode; i++)
	{
		//set to a random char in our symbol set
		*(ptr + i) = (char)(*(symbolSet + generateRandomNumber(0, lengthOfSymbolSet - 1)));
	}

	return ptr;
}




void takeTurn()
{
	if (turn == 1)
	{
		char* userCode = malloc(lengthOfCode * sizeof(char));

		//check if allocation was successful
		if (userCode == NULL)
		{
			printf("not enough memory");
			exit(0);
		}

		for (int i = 0; i < lengthOfCode; i++)
			*(userCode + i) = '_';

		printBoard(userCode);

		numAttempts--;
		turn++;
	}
	else
	{

		//Get the user Code
		char* userCode = getUserInputCharArray(lengthOfCode);

		//verify validity of user code
		while (containsCharactersNotIntSymbolSet(userCode))
		{
			clearScreen();
			printf("Input contains characters not in symbol set. Try again:\n\n");
			userCode = getUserInputCharArray(lengthOfCode);
		}

		//check if we've won or lost
		if (checkGameOver(userCode))
		{
			free(userCode);
			playAgain();
		}

		printBoard(userCode);

		free(userCode);

		free(key);
		key = initializeKey();
		
		numAttempts--;
		turn++;
	}
}



//boolean
int containsCharactersNotIntSymbolSet(char* userCode)
{
	for (int i = 0; i < lengthOfCode; i++)
	{
		//initially set to false
		int characterIsInSymbolSet = 0;

		for (int j = 0; j < lengthOfSymbolSet; j++)
		{
			if ((int)*(userCode + i) == *(symbolSet + j));
				characterIsInSymbolSet = 1;
		}

		//if after our check characterIsInSymolSet is still false then return
		if (!characterIsInSymbolSet)
			return 1;
	}

	return 0;
}



//boolean
int characterIsInCode(char c)
{
	for (int i = 0; i < lengthOfCode; i++)
	{
		if (c == *(code + i))
			return 1;
	}

	return 0;
}



int numCorrectCharactersInCode(char* userCode)
{
	int numCorrectCharacters = 0;

	for (int i = 0; i < lengthOfCode; i++)
	{
		char currentCharacter = *(userCode + i);
		if (characterIsInCode(currentCharacter) && characterCanStillBeCounted(currentCharacter))
			numCorrectCharacters++;	
	}

	return numCorrectCharacters;
}



int characterCanStillBeCounted(char c)
{
	//search the symbol set for the character
	int positionOfCharacterInSymbolSet = 0;
	for (int i = 0; i < lengthOfSymbolSet; i++)
	{
		if (c == (char)*(symbolSet + i))
		{
			positionOfCharacterInSymbolSet = i;
			break;
		}
	}

	if (key[positionOfCharacterInSymbolSet] > 0)
	{
		key[positionOfCharacterInSymbolSet]--;
		return 1;
	}

	return 0;
}



int numCharactersOutOfPosition(char* userCode)
{
	int numCorrectCharacters = numCorrectCharactersInCode(userCode);
	return numCorrectCharacters - numCharactersInPosition(userCode);
}



int numCharactersInPosition(char* userCode)
{
	int numCharactersInPosition = 0;
	
	for (int i = 0; i < lengthOfCode; i++)
	{
		if ((int)*(userCode + i) == (int)*(code + i))
			numCharactersInPosition++;
	}

	return numCharactersInPosition;
}



void playGame()
{
	initializeGame();

	while (numAttempts > -1)
	{
		//win is checked in takeTurn
		takeTurn();
	}
}



int checkGameOver(char* userCode)
{
	//are we out of attempts?
	if (numAttempts <= 0)
	{
		printf_s("You lose!");
		Sleep(1000);
		free(symbolSet);
		free(code);
		free(key);
		return 1;
	}

	//did we crack the code?
	for (int i = 0; i < lengthOfCode; i++)
	{
		if ((int)*(userCode + i) != (int)*(code + i))
		{
			//game is NOT over we didn't crack the code
			return 0;
		}
	}

	//game is over we cracked the code
	printf_s("Congratulations you've cracked the code!");
	Sleep(1000);
	free(symbolSet);
	free(code);
	free(key);
	return 1;
}



void playAgain()
{
	clearScreen();
	printf_s("Would you like to play again? (1 for yes, 2 for no)");
	int response = getUserInputInt(1, 2);
	if (response == 1)
	{
		clearScreen();
		playGame();
	}
	else
	{
		clearScreen();
		printf("Thanks for playing!");
		exit(0);
	}
}



//prints a formatted version of the symbol set
void printSymbolSet()
{
	printf_s("Symbol Set: ");

	for (int i = 0; i < lengthOfSymbolSet; i++)
	{
		//TESTING
		char currentCharacter = (char)*(symbolSet + i);
		printf_s("%c ",currentCharacter);
	}

	printf_s("\n");
}



//prints the num attempts left, the length of the code, the numbers of symbols in the symbol set
void printInformation()
{
	printf_s("Attempts Remaining: %d\nLength Of Code: %d\nSymbols In Symbol Set: %d\n", numAttempts, lengthOfCode, lengthOfSymbolSet);
}



//prints the current code as input by the user
//If round 1 prints underscores
void printCurrentCode(char *userCode)
{
	printf_s("Your Code: ");

	for (int i = 0; i < lengthOfCode; i++)
		printf_s("%c", *(userCode + i));

	printf_s("\n");
}



//prints the current message from the computer
//This depends on how many symbols are correct and in/out of place
void printMessage(char *userCode)
{
	int inPosition = numCharactersInPosition(userCode);
	int outPosition = numCharactersOutOfPosition(userCode);

	if (turn != 1 && inPosition <= 0 && outPosition <= 0)
		printf_s("No help.\n");

	if (inPosition > 0)
		printf_s("%d character(s) in correct position.\n", inPosition);
	if (outPosition > 0)
		printf_s("%d character(s) out of position.\n", outPosition);



	//THIS CODE IS FOR TESTING (prints the solution)
	/*
	for (int i = 0; i < lengthOfCode; i++)
	{
		printf_s("%c ", *(code + i), (int)*(code + i));
	}
	*/
}



void printBoard(char *userCode)
{
	clearScreen();
	printInformation();
	printSymbolSet();
	printCurrentCode(userCode);
	printMessage(userCode);
}