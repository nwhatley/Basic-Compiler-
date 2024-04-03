#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool isAlpha(char aChar);
bool isNum(char aChar);
bool isAlphaNum(char aChar);
bool isWhiteSpace(char aChar);
bool isSeparator(char aChar);
bool isOperatorChar(char c);
bool isKeyword( string s);
bool isOperator(string s);
void testFile(string testF, string resultF);

void lexer(vector<string> & tokens, vector<string> & lexemes, istream & in);

int main()
{
	string test1 = "testCode1.txt";
	string test2 = "testCode2.txt";
	string test3 = "testCode3.txt";
	
	string results1 = "results1.txt";
	string results2 = "results2.txt";
	string results3 = "results3.txt";
	
	testFile(test1, results1);
	testFile(test2, results2);
	testFile(test3, results3);
	
	
	return 0;
}


void lexer(vector<string> & tokens, vector<string> & lexemes, istream & in)
{
	while(!in.eof())
	{
		string lexeme = "";
		char c = in.peek();
		
		while (!isWhiteSpace(c))
		{
			// enters identifier (/keyword) state machine if first character is Alphanumerical
			if (isAlpha(c))
			{
				while (isAlphaNum(c))
				{
					lexeme += c;
					in.get();
					c = in.peek();
				}
				// Checks if identifier could be a language keyword
				if ( isKeyword( lexeme ))
					tokens.push_back("keyword");
				else
					tokens.push_back("identifier");
				
				lexemes.push_back(lexeme);
				
				break;
			}
			// Enters integer state machine, could be function
			else if( isNum(c) )
			{
				
				while (isNum(c))
				{
					lexeme += c;
					in.get();
					c = in.peek();
				}
				// Checks if lexeme is a proper integer (d+)
				if (isAlpha(c))
					tokens.push_back("unknown");
				else
					tokens.push_back("integer");
				
				lexemes.push_back(lexeme);
				
				break;
			}
			// Enters Separator state machine, could be function
			else if (isSeparator(c))
			{
				lexeme += c;
				in.get();
				c = in.peek();
				
				tokens.push_back("separator");
				lexemes.push_back(lexeme);
				
				break;
			}
			// Enters Operator state machine, could be function
			else if (isOperatorChar(c))
			{
				while (isOperatorChar(c))
				{
					lexeme += c;
					in.get();
					c = in.peek();
				}
				
				if (isOperator(lexeme))
					tokens.push_back("operator");
				else
					tokens.push_back("unknown");
				lexemes.push_back(lexeme);
				
				break;
			}
			
			// sentinel to check end of readable code
			if (c == '%')
				break;
		}
		if (isWhiteSpace(c))
		{
			in.get();
		}
		// sentinel to exit all parsing and represent of file.
		if (c == '%')
			break;
	}
}



void testFile(string testF, string resultF)
{
		///////////////////////// TEST FIRST FILE /////////////////////////
	vector<string> lexemes;
	vector<string> tokens;

	ifstream in;
	in.open(testF);
	
	//////// Parse File ////////
	lexer( tokens, lexemes, in);
	in.close();
	
	
	
	//////// Output Token List ////////
	ofstream out;
	out.open(resultF);
	
	
	out << "Token:" << "\t\t\t" << "Lexeme:" << endl;
	
	for (int i = 0; i < tokens.size(); i++)
	{
		out << tokens[i];
		if (tokens[i] == "keyword" || tokens[i] == "integer" || tokens[i] == "unknown")
			out << "\t";
		
		out << "\t\t" << lexemes[i] << endl;
	}
	
	out.close();
	
}



//////////// Tools /////////



bool isAlpha(char aChar)
{
	string lower = "abcdefghijklmnopqrstuvwxyz";
	string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	for (int i = 0; i < lower.size(); i++)
	{
		if (aChar == lower[i] || aChar == upper[i])
			return true;
		
	}
	return false;
}

bool isNum(char aChar)
{
	string digits = "0123456789";
	
	for (int i = 0; i < digits.size(); i++)
	{
		if (aChar == digits[i])
			return true;
		
	}
	return false;
}

bool isAlphaNum(char aChar)
{
	return isAlpha(aChar) || isNum(aChar);
}

//isspace()
bool isWhiteSpace(char aChar)
{
	switch (aChar)
	{
		case ' ':
		case '\t':
		case '\n':
		case '\v':
		case '\f':
		case '\r':
			return true;
	}
	return false;
}

bool isSeparator(char aChar)
{
	switch (aChar)
	{
		case '(':
		case ')':
		case ';':
			return true;
	}
	return false;
}

bool isOperatorChar(char c)
{
	switch (c)
	{
		case '=':
		case '>':
		case '<':
		case '/':
		case '+':
		case '-':
		case '*':
			return true;
	}
	return false;
}

bool isOperator(string s)
{
	vector<string> operators{
		"==",
		">",
		"<",
		"/=",
		"+",
		"-",
		"=",
		"*",
		"/"
	};
	
	for (int i = 0; i < operators.size(); i++)
	{
		if (s == operators[i])
			return true;
	}
	return false;
}

bool isKeyword( string s)
{
	vector<string> keywords{
		"integer",
		"boolean",
		"if",
		"else",
		"endif",
		"put",
		"get",
		"while",
		"true",
		"false",
	};
	
	for (int i = 0; i < keywords.size(); i++)
	{
		if (s == keywords[i])
			return true;
	}
	return false;
}



/*
 Declaration Keywords:
 integer
 boolean
 
 Statement Keywords:
 if
 else
 endif
 put
 get
 while
 true
 false
 
 Operators:
	Relop:
		==
		>
		<
		/=
	Arithmetic:
		+
		- (subtraction)
		*
		/
		- (Negative)
	Symbols/Separators:
		(
		)
		;

*/


// L | L(L|D|_)*(L|D|_(L|D)+)+