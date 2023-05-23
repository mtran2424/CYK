/*
Author: My Tran
Filename: Lexer.cpp
Description: Mini lexer and parser for CFGs in CNF
*/
#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include "Token.h"

class Lexer
{
private:
	std::string startRule;
	std::string tempRule;
	std::unordered_map<std::string, std::unordered_map<std::string, Token>> grammarMap;

	char nextCharacter;	//temp global value for iteration of input
	Token nextToken;	//temp token value to identify nextCharacter
	int grammarIndex;		//running index for input
	std::string grammar, lexeme;	//stores input grammar and currently accumulating lexeme respectively

	/*
	Purpose:	Adds next letter of input to the end of the lexeme.
	Pre:		input is not null
	Post:		Next character of input is concatenated to the end of lexeme and index is updated
	*/
	void AccumulateCharacter();

	/*
	Purpose:	Preliminary tokenizer for next character from input string.
	Pre:		input is not null
	Post:		nextCharacter is updated to next character in input and
				nextToken is set to the token identifying nextCharacter.
	*/
	void NextCharacter();

	/*
	Purpose:	Tokenizes the characters from the input
	Pre:		nextCharacter is not an alphanumeric
	Post:		returns nextToken
	*/
	Token IdentifyToken();

	/*
	Purpose:	Skips the white space
	Pre:		input is not empty
	Post:		Index variable is set to the next non-space and nextCharacter is updated
	*/
	void SkipWhitespace();

public:

	/*
	Purpose:	Non-default constructor for lexer class objects
	Pre:		Takes argument - string in, representing the input grammar
	Post:		Lexer object is created and input is initialized.
				Lexeme is defaulted to empty
				nextCharacter is defaulted to '\0'
				inputIndex is defaulted to 0
	*/
	Lexer(std::string);

	//Destructor
	~Lexer();

	/*
	Purpose:	Getter for the start rule from parsing.
	Pre:		None
	Post:		Returns startRule
	*/
	std::string GetStart();

	/*
	Purpose:	Performs single lexing operation on input grammar.
	Pre:		Input must have content
	Post:		lexeme is built from input.
	*/
	void Lex();

	/*
	Purpose:	Getter for the rule mapping of the grammar.
	Pre:		None
	Post:		Returns grammar rule map
	*/
	std::unordered_map<std::string, std::unordered_map<std::string, Token>> GetMap();

	/*
	Purpose:	Delimit rules, maps LHS production symbols to their RHS rules, and assigns start rule.
	Pre:		grammar is not empty
	Post:		grammarMap and startRule have been filled in
	*/
	void Parse();

};
