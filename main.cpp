//-----------------------------------------------------------------------------//                    
//                          TEXT FILE ANALYSER - 2018                          //
//-----------------------------------------------------------------------------//

//                Source file:      main.cpp
//                Code version:     v0.4                  
//                Revision date:    18/01/2018 - 06.52pm 
//                Programmer:       Janitha Gamage
//                Contact:          jay.dgamage@gmail.com

// The following console program analyses the content of a text file and reports the number of characters,
// integers and fractional numbers as well as the total count of numbers (integers and fractional)
// contained within the file.

/****************************************************
*            Text File Analyser v0.4                *
*           Last Modified - 18/01/2018              *
****************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

/****************************************************
*            CONSTANTS FOR PREPROCESSOR             *
****************************************************/
// States in CountNumbers function:
#define NumberCanStart 01												// State 01
#define IntNumberStarted 02												// State 02
#define FracNumberStarted 03											// State 03
#define NoNumber 04														// State 04

// MainMenu Options:
// ASCII value of the digits are used to identify user input
#define FindChar '1'
#define FindInt '2'
#define FindFractional '3'
#define FindTotalNum '4'
#define FindAll '5'
#define EnterNameOfFile '6'
#define Help '7'

// Character Types:
#define WhiteSpace 10
#define Digit 11
#define Sign 12
#define Other 13
#define EndOfFile 14
#define DecimalPoint 15

// Types of text printed by Print function:
#define IntroandMainMenu 100
#define Guidelines 200
#define Result 300

using namespace std;

/****************************************************
*            GLOBAL VARIABLES / CLASSES             *
****************************************************/
int CharCount;															// Stores total number of characters
int IntCount;															// Stores total number of integers
int FractionCount;														// Stores total number of fractions
char SelectedOption;													// Holds option selected by user at main menu
ifstream TextFile;														// Used for opening text file
string FileName="Text.txt";												// Stores name of text file given by user

/***************************************************
*                   FUNCTIONS                      *
***************************************************/
int ClassifyNextChar();													// Reads and classifies next character from text file and counts number of characters
void Print(int TextToPrint);											// Prints different types of text based on input parameter: TextToPrint
void CountNumbers();													// Counts the number of integers and fractional numbers in text file
void OpenTextFile();													// Opens text file, displays error if file doesn't exist

int main()
{
	char AnotherOperation('y');											// Stores character that determines if user would like to perform another operation or analyse another file

	do {																// Keep running the program if the user wants to analyse more files
		
		// The program will return HERE at the end if the user selects to analyse another file or perfrom another operation
		// Hence the counting variables are set to zero for another calculation
		CharCount = 0;
		IntCount = 0;
		FractionCount = 0;							
		system("cls");													// Clear screen
		
		system("color 17");												// Set background color to blue and text color to white	

		Print(IntroandMainMenu);										// Print introduction and main menu

		
		do{
			cout << "Enter number of operation you wish to perform (1-7).\n";
			cin >> SelectedOption;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');		// Discard all characters up to and including '\n' remaining in input buffer (cin stream)
			
			if (SelectedOption == Help)									// If user selected Help, print Guidelines
				Print(Guidelines);
		
		} while ((SelectedOption < FindChar) || (SelectedOption > EnterNameOfFile));	// Only accept user input between '1' and '6' inclusive

		OpenTextFile();													// Open text file
		CountNumbers();													// Count numbers in text file
		Print(Result);													// Print results
		TextFile.close();												// Close text file as user may want to analyse another file


		cout << "\n---------------------------------------------------------------------------\n\n"
			 << "Would you like to perform another operation or analyse another file? (Y\\N)\n"
			 << "NOTE: This will clear any results displayed above.\n\n"
			 << "Enter Y to proceed or any other key to exit.\n";
		cin >> AnotherOperation;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');				// Discard all characters up to and including "\n" remaining in input buffer (cin stream)

	} while (AnotherOperation == 'y' || AnotherOperation == 'Y');			// Keep running the program as long as user input is 'Y' or 'y'
	return 0;																// else end program  
}

/***************************************************
*           TEXT FILE OPENING FUNCTION             *
***************************************************/
void OpenTextFile()
{
	if (SelectedOption == EnterNameOfFile)									// If user selected to enter name of text file
	{													
		cout << "Please enter the name of the text file with the \".txt\" extension.\n";
		getline(cin, FileName);												// Extract line of user input and store in FileName
	}
	TextFile.open(FileName);												// Open text file with string stored in FileName
																			// If the user has not entered a name, "Text.txt" will be opened by default
																			
	while (TextFile.fail())													// Display error message and beep once if file doesn't exist
	{
		cout << "\nThe text file with name: \"" << FileName << "\" was not found.\n\a"
			 << "Please make sure the file is in the same directory (folder) as the program.\n"
			 << "Please enter the name of the text file with the \".txt\" extension.\n\n";
		getline(cin, FileName);												// Extract line of user input and store in FileName					
		TextFile.open(FileName);											// Open text file with string stored in FileName
	}
	return;
}

/***************************************************
*            NUMBER COUNTING FUNCTION              *
***************************************************/
void CountNumbers()
{
	int State = NumberCanStart;												// Used to specify the current state, initialized to start from: State 01-NumberCanStart	
	bool DigitPresent;														// Stores flag whether a digit exists in the current set of characters being read

	while (true)															// Infinite while loop to keep counting numbers
	{																		
		switch (State)														// Switch to state in "State" variable
		{
			// State 01	- NumberCanStart
			case NumberCanStart:																		
				DigitPresent = false;										// Set initial condition (i.e. no digit is present)
				switch (ClassifyNextChar())									// Read and classify next character
				{
					case Digit:												// If a Digit is found, set DigitPresent to true and switch to State 02-IntNumberStarted
						DigitPresent = true;																
					case Sign:												// If a Sign is found, switch to State 02-IntNumberStarted
						State = IntNumberStarted;																 
					case WhiteSpace:										// If a WhiteSpace is found, read next character
						continue;
					case Other:												// If an Other character is found, switch to State 04-NoNumber
						State = NoNumber;
						continue;
					case DecimalPoint:										// If a DecimalPoint is found, switch to State 03-FracNumberStarted
						State = FracNumberStarted;
						continue;
					case EndOfFile:											// If EndOfFile is reached, return 
						return;														
				}

			// State 02 - IntNumberStarted
			case IntNumberStarted:												
				switch (ClassifyNextChar())									// Read and classify next character
				{
					case WhiteSpace:										// If a WhiteSpace is found,
						if (DigitPresent)									// increment Integer count only if digit is present
							IntCount++;
						State = NumberCanStart;								// and switch back to State 01-NumberCanStart
						continue;
					case Digit:
						DigitPresent = true;								// If a Digit is found, set DigitPresent to true and read next character
						continue;
					case Sign:												// If a Sign or
					case Other:												// Other character is found, switch to State 04-NoNumber
						State = NoNumber;
						continue;
					case DecimalPoint:										// If a DecimalPoint is found, switch to State 03-FracNumberStarted					
						State = FracNumberStarted;
						continue;
					case EndOfFile:											// If EndOfFile is reached,
						if (DigitPresent)									// increment Integer count only if digit is present and return
							IntCount++;
						return;												
				}

			// State 03 - FracNumberStarted
			case FracNumberStarted:																			
				switch (ClassifyNextChar())									// Read and classify next character
				{
					case WhiteSpace:										// If a WhiteSpace is found,
						if (DigitPresent)									// increment Fraction count only if digit is present
							FractionCount++;
						State = NumberCanStart;								// and switch back to State 01-NumberCanStart
						continue;
					case EndOfFile:											// If EndOfFile is reached,
						if (DigitPresent)									// increment Fraction count only if digit is present and return
							FractionCount++;
						return;												
					case Digit:												// If a Digit is found, set DigitPresent to true and read next character
						DigitPresent = true;
						continue;
					default:												// If a Sign, Other character or DecimalPoint is found, switch to State 04-NoNumber
						State = NoNumber;
						continue;
				}

			// State 04 - NoNumber
			case NoNumber:																					
				switch (ClassifyNextChar())									// Read and classify next character
				{
					case EndOfFile:											// If EndOfFile is reached, return
						return;	
					case WhiteSpace:										// If a WhiteSpace is found, switch to State 01-NumberCanStart
						State = NumberCanStart;
					default:												// If a Sign, DecimalPoint, Other character or Digit is found,
						continue;											// remain in same state and read next character
			}
				}
	}
}

/***************************************************
*    READ AND CLASSIFY NEXT CHARACTER FUNCTION     *
***************************************************/
int ClassifyNextChar()
{
	char C;																	// Used to read next character from text file
	TextFile.read(&C, 1);													// Read next character from text file
	if (!TextFile.good())													// Check if EndOfFile is reached, if so return EndOfFile
		return EndOfFile;
	CharCount++;															// Increment character count as a character has been read
	if (C == ' ' || C == '\t' || C == '\n')									// If a Whitespace is read, return WhiteSpace
		return WhiteSpace;
	if (isdigit(C))															// If a Digit is read, return Digit 
		return Digit;
	if (C == '+' || C == '-')												// If a Sign is read, return Sign 
		return Sign;																	
	if (C == '.')															// If a DecimalPoint is read, return DecimalPoint 
		return DecimalPoint;
	return Other;															// Else return Other
}	

/***************************************************
*            TEXT PRINTING FUNCTION                *
***************************************************/
void Print(int TextToPrint)
{
	switch (TextToPrint)													// Decide which text to print based on input parameter: "TextToPrint"
	{
		case IntroandMainMenu:
			cout << "------------------------------------------------------------------------------------------\n"
				 << "-                               TEXT FILE ANALYSER v0.4                                  -\n"
				 << "------------------------------------------------------------------------------------------\n\n"
				 << "This program analyses a text file selected by the user and displays the results.\n"
				 << "By default, if a filename is not provided, the program will analyse \"Text.txt\" text file.\n\n"
				 << "------------------------------------------------------------------------------------------\n"
				 << "- [1] Find no. of characters                                                             -\n"
				 << "- [2] Find no. of integers                                                               -\n"
				 << "- [3] Find no. of fractional numbers                                                     -\n"
				 << "- [4] Find total count of numbers (integers and fractional)                              -\n"
				 << "- [5] Perform ALL of the above functions                                                 -\n"
				 << "- [6] Enter name of another text file                                                    -\n"
				 << "- [7] Help                                                                               -\n"
				 << "------------------------------------------------------------------------------------------\n\n";
				break;

		case Guidelines:
			cout << "------------------------------------------------------------------------------------------\n"
				 << "- Help:                                                                                  -\n"
			 	 << "------------------------------------------------------------------------------------------\n"
				 << "- [1] Displays the number of characters in the file.                                     -\n"
				 << "- [2] Displays the number of integers the file contains.                                 -\n"
				 << "- [3] Displays the number of fractional numbers the file contains.                       -\n"
				 << "- [4] Displays the total count of numbers(integers and fractional) in the file.          -\n"
				 << "- [5] Displays number of characters, integers, fractional numbers and the total count of -\n"
				 << "-     numbers in the file.                                                               -\n"
				 << "- [6] Allows to enter the name of the text file you want to analyse.                     -\n"
				 << "-     Make sure you enter the extension \".txt\" after the name of the file.               -\n"
				 << "- [7] Shows a summary of what action each of the options perform.                        -\n"
				 << "-                                                                                        -\n"
				 << "- [*] Options 1 to 5:                                                                    -\n"
				 << "- These options will analyse the text file to determine the different types of           -\n"
				 << "- characters within the file.                                                            -\n"
				 << "-                                                                                        -\n"
				 << "- Please refer the user manual for additional guidance.                                  -\n"
				 << "------------------------------------------------------------------------------------------\n\n";
				break;

		case Result:
			// setw(n) sets field width for output operation which allows the results to be displayed in tabular format

			cout << "\n---------------------------------------------------------------------------"
				 << "\n- Filename                     : " << setw(40) << FileName           << " -"
				 << "\n---------------------------------------------------------------------------";

			switch (SelectedOption)											// Decide which result to print based on option selected by user at main menu
			{
				case FindChar:			cout << "\n- Number of characters         : " << setw(40) << CharCount                << " -"; break;
				case FindInt:			cout << "\n- Number of integers           : " << setw(40) << IntCount                 << " -"; break;
				case FindFractional:	cout << "\n- Number of fractional numbers : " << setw(40) << FractionCount            << " -"; break;
				case FindTotalNum:		cout << "\n- Total numbers                : " << setw(40) << IntCount + FractionCount << " -"; break;

				default:													
										cout << "\n- Number of characters         : " << setw(40) << CharCount                << " -"
											 << "\n- Number of integers           : " << setw(40) << IntCount                 << " -"
											 << "\n- Number of fractional numbers : " << setw(40) << FractionCount            << " -"
											 << "\n- Total numbers                : " << setw(40) << IntCount + FractionCount << " -";
			}
	}
	return;
}