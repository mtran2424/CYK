/*
Author:	My Tran
Filename:	Driver.cpp
Description:	Program tests the functionality of the CYK class, and the Lexer
				class undirectly through CYK.
Assumptions:	-Grammars passed into CYK object are properly constructed and in CNF
				-Files are formatted properly to be read. (Detailed in README.txt)
				-Input strings should be valid symbols to assure proper program behavior
				-Program will either be run on linux or windows
*/
#include "Lexer.h"
#include "CYK.h"
#include <fstream>

int main()
{
	std::vector<std::string> grammarArray, inputArray;
	std::ifstream fin;
	std::ofstream fout;

	//associate input files to be read
	fin.open("CFG.txt");
	fout.open("log.txt");

	//Check successful file association
	if (!fin.is_open())
	{
		std::cout << "File could not be opened\n";
		return 1;
	}
	else
	{
		//Read all the grammars to be parsed and used in CYK
		std::string tempGrammar;
		while (getline(fin, tempGrammar))
		{
			//clear trailing whitespace on grammar
			tempGrammar.erase(tempGrammar.find_last_not_of(" \t\n\r\f\v") + 1);

			grammarArray.push_back(tempGrammar);
		}

		//close input stream to change to input file
		fin.close();

		//Check successful file association
		fin.open("input.txt");
		if (!fin.is_open())
		{
			std::cout << "File could not be opened\n";
			return 1;
		}
		else
		{
			//Read all the grammars to be checked using CYK
			std::string tempInput;
			while (getline(fin, tempInput))
			{
				tempInput.erase(tempInput.find_last_not_of(" \t\n\r\f\v") + 1);

				inputArray.push_back(tempInput);
			}

			//Close input file stream
			fin.close();
		}
	}

	//Testing loop to check all strings with all grammars
	for (std::string s : grammarArray)
	{
		//Flush buffers
		fout.flush();
		std::cout.flush();

		//Each CYK object is instantiated with a parsed grammar
		CYK* test = new CYK("", s);

		//test all input strings for grammar s
		for (std::string w : inputArray)
		{

			fout << "\n" << s << "\n";
			test->SetInput(w);

			fout << w << ": " << ((test->IsAccepted()) ? "accepted\n" : "rejected\n");
			fout << test->ToString() + "\n";
			fout << "-----------------------------------------------------------------------------------------------------------------------------";

		}
	}

	//close file output stream
	fout.close();

	return 0;
}