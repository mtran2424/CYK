/*
Author:	My Tran
Filename:	Token.h
Description:	Defines tokens that identify components of a CFG
*/
#pragma once
enum class Token
{
	EPS,		//'E' reserved for epsilon
	TERMINAL,	//terminating symbols
	VARIABLE,	//non-terminating symbols
	RULE,		//substrings of letters and nonreserved symbols
	DASH,		//beginning of implies symbol substring
	IMPLIES,	//"->"
	OR,			//'|'
	COMMA,		//','
	UPPER_LETTER,	//upper case letters
	LOWER_LETTER,	//lower case letters
	DIGIT,		//numerical digit
	SYMBOL,		//Undefined symbol
	START,		//token for start rule
	END			//end of lexeme token
};