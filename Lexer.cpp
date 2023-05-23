/*
Author: My Tran
Filename: Lexer.cpp
Description: Mini lexer and parser for CFGs in CNF
Assumptions:	-Grammars passed in will be properly constructed as erroneous grammars will lead to unexpected
				behavior from the program
				-Left hand side of rule should only contain one upper case letter followed by "->" representing
				implies symbol
				-Rules are provided in a comma separated string, terminated by a newline.
				-The first rule given in the comma separated list is assumed to be the start rule.
				-Upper case letters are reserved for Non-terminals
				-E is reserved for epsilon, representing the empty string
				-'|', ',', 'E' are reserved symbols
				-Terminals must be lower-case or non-reserved symbols
*/
#include "Lexer.h"
#include "Token.h"

//Adds next letter of input to the end of the lexeme.
void Lexer::AccumulateCharacter()
{
	lexeme = lexeme + grammar[grammarIndex];
	grammarIndex++;
}

//Preliminary tokenizer for next character from input string.
void Lexer::NextCharacter()
{
	//If the running index is within the bounds of the input, then there is still grammar content to parse
	if (grammarIndex < grammar.length())
	{
		nextCharacter = grammar[grammarIndex];

		//CRLF ends grammar as well
		if (nextCharacter == '\n')
		{
			nextToken = Token::END;
		}
		//ID ascii upper case letters
		else if (nextCharacter > 64 && nextCharacter < 91)
		{
			if (nextCharacter == 'E')
			{
				nextToken = Token::EPS;
			}
			else
			{
				nextToken = Token::UPPER_LETTER;
			}
		}
		//ID ascii lower case letters
		else if (nextCharacter > 96 && nextCharacter < 122)
		{
			nextToken = Token::LOWER_LETTER;
		}
		//ID ascii digit characters
		else if (nextCharacter > 47 && nextCharacter < 58)
		{
			nextToken = Token::DIGIT;
		}
		//ID any other tokens
		else
		{
			nextToken = IdentifyToken();
		}
	}
	//if we're at the end of the grammar
	else
	{
		nextToken = Token::END;
	}
}

//Tokenizes the characters from the input
Token Lexer::IdentifyToken()
{
	//Token value is assigned based on the current character
	switch (nextCharacter)
	{
	case '-':
		nextToken = Token::DASH;
		break;
	case '>':
		if (nextToken != Token::DASH)
		{
			std::cout << "SYNTAX ERROR: Invalid symbol\n";
			exit(1);
		}
		nextToken = Token::IMPLIES;
		break;
	case '#':
		nextToken = Token::SYMBOL;
		break;
	case '|':
		nextToken = Token::OR;
		break;
	case ',':
		nextToken = Token::COMMA;
		break;
	case ' ':
		break;
	default:
		nextToken = Token::END;
		break;
	}

	return nextToken;
}

//Skips the white space
void Lexer::SkipWhitespace()
{
	while (nextCharacter == ' ')
	{
		grammarIndex++;
		NextCharacter();
	}
}

//Non-default constructor for lexer class objects
Lexer::Lexer(std::string in)
{
	grammar = in;
	lexeme = "";
	nextCharacter = '\0';
	grammarIndex = 0;
}



//Performs single lexing operation on input grammar.
void Lexer::Lex()
{
	//All rules are of the forms:
	//A->BC
	//A->a
	SkipWhitespace();
	if (nextToken != Token::END)
	{
		AccumulateCharacter();
		SkipWhitespace();

		switch (nextToken)
		{
		case Token::UPPER_LETTER:	//Upper case letters can be variables or rules

			//Look ahead at the next character to see if lexeme is a rule or a variable
			NextCharacter();
			SkipWhitespace();

			//lexeme is not a rule if there is not a consec. string of upper case letters
			if (nextToken != Token::UPPER_LETTER && nextToken != Token::LOWER_LETTER && nextToken != Token::SYMBOL && nextToken != Token::DIGIT)
			{
				std::cout << "<variable>" + lexeme;
				std::endl(std::cout);
				tempRule = lexeme;
			}
			//if there are more upper case letters, accumulate them and they're rules
			else
			{
				do
				{
					AccumulateCharacter();
					NextCharacter();
				} while (nextToken == Token::UPPER_LETTER || nextToken == Token::LOWER_LETTER || nextToken == Token::SYMBOL || nextToken == Token::DIGIT);

				std::cout << "<rule>" + lexeme;
				std::endl(std::cout);
				grammarMap[tempRule][lexeme] = Token::RULE;
			}
			break;
		case Token::DASH:		//Dashes must be part of implies symbol
			NextCharacter();
			Lex();
			if (nextToken != Token::IMPLIES)
			{
				std::cout << "SYNTAX ERROR: Invalid symbol\n";
				exit(1);
			}
			break;
		case Token::IMPLIES:	//> must be directly preceded by '-' for implies
			std::cout << "<implies>" + lexeme;
			std::endl(std::cout);
			break;
		case Token::DIGIT:		//Digits, lowercase letters, and symbols are terminals and are always stand alone in CNF 
		case Token::LOWER_LETTER:
		case Token::SYMBOL:
		case Token::EPS:

			NextCharacter();
			SkipWhitespace();

			//lexeme is not a rule if there is not a consec. string of upper case letters
			if (nextToken != Token::UPPER_LETTER && nextToken != Token::LOWER_LETTER && nextToken != Token::SYMBOL && nextToken != Token::DIGIT)
			{
				std::cout << "<terminal>" + lexeme;
				std::endl(std::cout);
				//tempRule = lexeme;
				grammarMap[tempRule][lexeme] = Token::TERMINAL;
			}
			//if there are more upper case letters, accumulate them and they're rules
			else
			{
				do
				{
					AccumulateCharacter();
					NextCharacter();
				} while (nextToken == Token::UPPER_LETTER || nextToken == Token::LOWER_LETTER || nextToken == Token::SYMBOL || nextToken == Token::DIGIT);

				std::cout << "<rule>" + lexeme;
				std::endl(std::cout);
				grammarMap[tempRule][lexeme] = Token::RULE;
			}
			break;
		case Token::OR:			//Or symbol is reserved and represents that there are more RHS rules to the corresponding variable
			std::cout << "<or>" + lexeme;
			std::endl(std::cout);
			break;
		case Token::COMMA:		//Comma is reserved and indicates that there are more variables to evaluate
			std::cout << "<next rule>" + lexeme;
			std::endl(std::cout);
			break;
		}
	}
}

std::unordered_map<std::string, std::unordered_map<std::string, Token>> Lexer::GetMap()
{
	return grammarMap;
}

std::string Lexer::GetStart()
{
	return startRule;
}

void Lexer::Parse()
{
	std::cout << "Performing parse on grammar...\n";
	//priming lex to get start character
	std::cout << "<start>" + lexeme;
	NextCharacter();
	Lex();
	startRule = lexeme;
	tempRule = lexeme;
	//reduce the input grammar as the lexemes build
	grammar = grammar.substr(grammarIndex, grammar.length() - lexeme.length());
	lexeme = "";
	grammarIndex = 0;

	do
	{
		Lex();
		//reduce the input grammar as the lexemes build
		grammar = grammar.substr(grammarIndex, grammar.length() - grammarIndex);
		lexeme = "";
		grammarIndex = 0;
		NextCharacter();
	} while (nextToken != Token::END);	//perform lex until the grammar given is empty

	std::cout << "<end>" + lexeme << "\n";

	std::cout << "Parsing complete.\n";
}

//destructor for Lexer class object
Lexer::~Lexer()
{
	grammarMap.clear();
}
