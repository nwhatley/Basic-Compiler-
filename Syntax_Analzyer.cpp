
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Unit
{
	int lineNum;
	string token_name;
	string lexeme;

	Unit(int ln, string tn, string lex)
		:lineNum(ln), token_name(tn), lexeme(lex)
	{}
	Unit()
	{}
};







////////////////////// Syntax Analyzer //////////////////////
void RAT21SU(vector<Unit> & list, ostream & out);
void OptDecList(vector<Unit> & list, ostream & out);
void DecList(vector<Unit> & list, ostream & out);
void Declaration(vector<Unit> & list, ostream & out);
void Qualifier(vector<Unit> & list, ostream & out);
void Id(vector<Unit> & list, ostream & out);
void StatementList(vector<Unit> & list, ostream & out);
void Statement(vector<Unit> & list, ostream & out);
void Compound(vector<Unit> & list, ostream & out);
void Assign(vector<Unit> & list, ostream & out);
void RAT_if(vector<Unit> & list, ostream & out);
void RAT_if_Prime(vector<Unit> & list, ostream & out);
void Put(vector<Unit> & list, ostream & out);
void Get(vector<Unit> & list, ostream & out);
void RAT_while(vector<Unit> & list, ostream & out);
void Condition(vector<Unit> & list, ostream & out);
void Relop(vector<Unit> & list, ostream & out);




bool Expression( vector<Unit> & list, ostream & out);
bool Exp_Prime(vector<Unit> & list, ostream & out);
bool Term(vector<Unit> & list, ostream & out);
bool Term_Prime(vector<Unit> & list, ostream & out);
bool Factor(vector<Unit> & list, ostream & out);
bool Primary(vector<Unit> & list, ostream & out);
bool Empty(vector<Unit> list, ostream & out);
void error( Unit u, ostream & out, string err);

Unit pop(vector<Unit> & v);
void print(vector<Unit> v);
void CommentRemover(vector<Unit> & lexer_list);


////////////// Cont. ////////////////////////





////////////////////// Lexer Analyzer //////////////////////
void testFile(string testF, string resultF, string resultSyn);
void lexer(vector<Unit> & list, istream & in);


/////// Tools /////////
bool isAlpha(char aChar);
bool isNum(char aChar);
bool isAlphaNum(char aChar);
bool isWhiteSpace(char aChar);
bool isSeparator(char aChar);
bool isOperatorChar(char c);
bool isKeyword(string s);
bool isOperator(string s);

bool isCommentStart(char c);
bool isComment(string s);
int char_to_col(char alpha);
int num_to_col(char alpha);
bool DFSM(string omega);
bool DFSM_digit(string omega);




int main()
{
	string test1 = "lex_syn_test1.txt";
	string test2 = "lex_syn_test2.txt";
	string test3 = "lex_syn_test3.txt";

	string resultslex1 = "lex_result1.txt";
	string resultslex2 = "lex_result2.txt";
	string resultslex3 = "lex_result3.txt";

	string resultsSyn1 = "syn_result1.txt";
	string resultsSyn2 = "syn_result2.txt";
	string resultsSyn3 = "syn_result3.txt";

	testFile(test1, resultslex1, resultsSyn1);
	testFile(test2, resultslex2, resultsSyn2);
	testFile(test3, resultslex3, resultsSyn3);


	return 0;
}




void testFile(string testF, string resultF, string resultSyn)
{
	//-----//-----//-----// TEST Lexer //-----//-----//-----//

	vector<Unit> lexer_list;


	ifstream in;
	in.open(testF);

	//////// Parse File ////////
	lexer(lexer_list, in);
	CommentRemover(lexer_list);
	in.close();



	//////// Output Token List ////////
	ofstream out;
	out.open(resultF);


	out << "\nToken:" << "\t\t\t" << "Lexeme:" << endl;

	for (int i = 0; i < lexer_list.size(); i++)
	{
		out << lexer_list[i].token_name;
		if (lexer_list[i].token_name == "keyword" || lexer_list[i].token_name == "integer" || lexer_list[i].token_name == "unknown")
			out << "\t";

		out << "\t\t" << lexer_list[i].lexeme << "\t" << lexer_list[i].lineNum << endl;
	}

	out.close();


	//-----//-----//-----// TEST Syntax //-----//-----//-----//

	//pop(lexer_list);
	//lexer_list.pop_back();

	ofstream outSyn;
	outSyn.open(resultSyn);

  RAT21SU(lexer_list, outSyn);

	outSyn.close();


}





///////////////////////////////  Syntax Analyzer /////////////////////////////////

bool Expression( vector<Unit> & list, ostream & out)
{
	string location = "Expression";
	//cout << location << endl;

	out << "<" << location << "> -> <Term><Expression_Prime>" << endl;
	bool r = Term(list, out) && Exp_Prime(list, out);


	return r;

}

// Unit a creates a virtual stack.
bool Exp_Prime(vector<Unit> & list, ostream & out)
{

	string location = "Expression_Prime";
	//cout << location << endl;
	//print(list);
	if ( list[0].lexeme == "+" )
	{
		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		out << "<" << location << "> -> +<Term><Expression_Prime>" << endl;
		Unit a = pop(list);


		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		bool ter = Term(list, out);
		bool exp = Exp_Prime(list, out);


		return ter && exp;
	}
	else if ( list[0].lexeme == "-" )
	{
		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		out << "<" << location << "> -> -<Term><Expression_Prime>" << endl;
		Unit a = pop(list);

		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		bool r = Term(list, out) && Exp_Prime(list, out);


		return r;
	}

	else
		return true;
}

bool Term(vector<Unit> & list, ostream & out)
{
	string location = "Term";
	//cout << location << endl;

	out << "<" << location << "> -> <Factor><Term_Prime>" << endl;
	bool r = Factor(list, out) && Term_Prime(list, out);


	return r;
}

bool Term_Prime(vector<Unit> & list, ostream & out)
{
	string location = "Term_Prime";
	//cout << location << endl;

	if ( list[0].lexeme == "*" )
	{
		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		out << "<" << location << "> -> *<Factor><Term_Prime>" << endl;
		Unit a = pop(list);

		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		bool r = Factor(list, out) && Term_Prime(list, out);


		return r;
	}
	else if ( list[0].lexeme == "/" )
	{
		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		out << "<" << location << "> -> /<Factor><Term_Prime>" << endl;
		Unit a = pop(list);

		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		bool r = Factor(list, out) && Term_Prime(list, out);


		return r;
	}
	else
	{
		out << "<" << location << "> -> <Empty>" << endl;
		bool r = Empty(list, out);
		return true;
	}

}

bool Factor(vector<Unit> & list, ostream & out)
{
	string location = "Factor";
	//cout << location << endl;

	if ( list[0].lexeme == "-" )
	{
		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		out << "<" << location << "> -> -<Primary>" << endl;
		Unit a = pop(list);

		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		bool r = Primary(list, out);


		return r;
	}
	else
	{
		out << "<" << location << "> -> <Primary>" << endl;
		bool r = Primary(list, out);

		return r;
	}

}

bool Primary(vector<Unit> & list, ostream & out)
{
	string location = "Primary";
	//cout << location << endl;

	if ( list[0].token_name == "identifier"  )
	{
		out << "<" << location << "> -> <Identifier>" << endl;
		Unit a = pop(list);


		return true;
	}
	else if ( list[0].token_name == "integer" )
	{
		out << "<" << location << "> -> <Integer>" << endl;
		Unit a = pop(list);


		return true;
	}
	else if ( list[0].lexeme[0] == '(' )
	{
		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		out << "<" << location << "> -> (<expression>)" << endl;
		Unit a = pop(list);


		out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
		bool r = Expression(list, out);

		if(list[0].lexeme[0] == ')' && r == true)
		{
			out << "\nToken: " << list[0].token_name << "\t\t" << "Lexeme: " << list[0].lexeme << endl;
			out << "<" << location << "> -> (<expression>)" << endl;
			Unit a = pop(list);
		}
		else
		{
			error(list[0], out, "Missing ).");
			r = false;
		}


		return r;
	}
	else if ( list[0].lexeme == "true" )
	{
		out << "<" << location << "> -> true" << endl;
		Unit a = pop(list);


		return true;
	}
	else if ( list[0].lexeme == "false" )
	{
		out << "<" << location << "> -> false" << endl;
		Unit a = pop(list);

		return true;
	}

	error( list[0], out, "Could not be determined as an Identifier, Integer, (, true, or false.");

	return false;

}

bool Empty(vector<Unit> list, ostream & out)
{
	string location = "Empty";
	//cout << location << endl;
	out << "<" << location << "> -> epsilon" << endl;
	return true;
}


void error( Unit u, ostream & out, string err)
{
	out << "Error with token: " << u.token_name << " lexeme: "<< u.lexeme
			<<" at line number " << u.lineNum << endl;
	out << err << endl;
}


Unit pop(vector<Unit> & v)
// Takes a vector<Unit> and returns and deletes the front element
{
	Unit temp;

	if (v.size() > 0)
	{
		temp = v[0];
		for( int i = 0; i < v.size()-1 ; i++)
		{
			v[i] = v[i+1];
		}
		v.pop_back();
	}

	return temp;
}


void print(vector<Unit> v)
{
	for( int i = 0; i < v.size(); i++)
	{
		cout << v[i].lineNum << " " << v[i].token_name << " " << v[i].lexeme << endl;
	}
}



//////////////////////////////////////   Lexer Analyzer //////////////////////////////////////


void lexer(vector<Unit> & list, istream & in)
{
	int line_number = 1;

	bool start = false;
	bool stop = false;

	char c = in.peek();
	if ( c == '%')
	{
		in.get();
		c = in.peek();
		if ( c == '%')
		{
			start = true;
			in.get();
			list.push_back( Unit(line_number, "operator", "%%") );
		}
		// Rat21SU needs to start with %%
	}

	while(!in.eof() && start)
	{
		string lexeme = "";
		string token = "";
		char c = in.peek();



		while (!isWhiteSpace(c) && start && !stop)
		{

			// enters identifier (/keyword) state machine if first character is Alphanumerical

			// sentinel to check end of readable code
			if ( c == '%')
			{
				in.get();
				c = in.peek();
				if ( c == '%')
				{
					stop = true;
					in.get();
					list.push_back( Unit(line_number, "operator", "%%") );
				}
				// either way % is still a terminal symbol
				break;
			}

			if (isAlpha(c))
			{
				while (isAlphaNum(c))
				{
					lexeme += c;
					in.get();
					c = in.peek();
				}
				// Checks if identifier could be a language keyword
				if ( DFSM(lexeme) ) {
					if (isKeyword(lexeme)) {
						token = ("keyword");

					}
					else {
						token = "identifier";
					}
				}

				list.push_back( Unit(line_number, token, lexeme) );
				break;
			}
			// Enters integer state machine, could be function
			else if( isNum(c) )
			{

				while (isNum(c) || !isWhiteSpace(c) && !isSeparator(c) && !isOperatorChar(c))
				{
					lexeme += c;
					in.get();
					c = in.peek();
				}
				// Checks if lexeme is a proper integer (d+)
				if (DFSM_digit(lexeme))
					token = "integer";
				else
					token = "Unknown";

				list.push_back( Unit(line_number, token, lexeme) );

				break;
			}
			// Enters Separator state machine, could be function
			else if (isSeparator(c))
			{
				lexeme += c;
				in.get();
				c = in.peek();

				list.push_back( Unit(line_number, "separator", lexeme) );

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



				if (isOperator(lexeme)) {

					if(lexeme.size() == 1) {
						list.push_back( Unit(line_number, "operator", lexeme) );
					}
					else if (lexeme.size() > 1) {
            if (lexeme == "/*" || lexeme == "*/") {

			  list.push_back( Unit(line_number, "comment", lexeme) );

            }
            else if(lexeme[0] == lexeme[1] && lexeme != "=="){
						    for(int i = 0; i < lexeme.size(); i++) {

						    string temp(1, lexeme[i]);
							list.push_back( Unit(line_number, "operator", temp) );
              }
						}
            else {


			  list.push_back( Unit(line_number, "operator", lexeme) );
            }
					}
				}
				else {
					list.push_back( Unit(line_number, "unknown", lexeme) );
				}
				break;
			}


		}
		if (isWhiteSpace(c))
		{
			if(c == '\n')
				line_number++;

			in.get();
		}
		// sentinel to exit all parsing and represent of file.
		if (stop)
			break;
	}
}








/////////////////////////////// Tools /////////////////////////////////



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
		"/*",
		"*/",
		"+",
		"-",
		"=",
		"*",
		"/",
		"++",
		"%%"
	};

	for (int i = 0; i < operators.size(); i++)
	{
		if (s == operators[i])
			return true;
	}
	return false;
}

bool isKeyword(string s) // added "end" and " begin keywords" .05 points
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
		"begin",
		"end",
	};

	for (int i = 0; i < keywords.size(); i++)
	{
		if (s == keywords[i])
			return true;
	}
	return false;
}

bool isCommentStart(char c) {

	if(c == '/') {

		return true;
	}
	else {

		return false;
	}
}
bool isComment() {}

int char_to_col(char alpha) {

	if (std::isdigit(alpha) == 0) {
		return 1;
	}
	else {

		return 2;
	}
}

int num_to_col(char alpha) {

	if (std::isdigit(alpha)) {
		return 1;
	}
	else {

		return 2;
	}
}

bool DFSM(string omega) {

	int table[6][3] = {{0,1,2}, {1,3,5}, {2,3,4}, {3,3,4}, {4,3,4}, {5,5,5}};
	int state = 1;

	for (int i = 0; i <omega.size(); i++) {

		int col = char_to_col(omega[i]);
		state = table[state][col];
		//cout << state << " string" << endl;
	}
	if (state == 3 || state == 4){

		return true;
	}
	else {
		return false;
	}
}



bool DFSM_digit(string omega) {

	int table[4][3] = {{0,1,2}, {1,2,3}, {2,2,1}, {3,3,3}};
	int state = 1;

	for (int i = 0; i < omega.size(); i++) {

		int col = num_to_col(omega[i]);
		state = table[state][col];
		//cout << state << col << endl;
	}

	if (state == 2) {

		return true;
	}
	else {

		return false;
		}
	}




////////////////////////////////////     RULES  - SA   //////////////////////////////////////

//std::vector<Unit>::iterator position = lexer_List.begin(); // global iterator that tracks position, not perfect but will in thoery move through program tokens and lexemes
bool empty = false; // an Empty flag meant to represent epsilon and trigger the RAT function to move on after OPT Dec List

void RAT21SU(vector<Unit> & list, ostream & out) {
  out << "<Rat21SU> ::= %% <Opt Declaration List> <Statement List> %% " << endl;

  if (list[0].lexeme == "%%") { // checks first char

    pop(list); // advances one character
    OptDecList(list, out);  // Goes through OptDecList -> DecList -> Declaration() || Declaration(); DecList() -> Qualifier() -> Id()

      if (true) { // if previous lexeme == %% or the empty flag is true (either no Opt Dec List or reached the end)
        //pop(list);
        empty = false;

        while (true) {

          StatementList(list, out);
          if (list.size() == 1) { // end condition for loop

            break;
          }
        }

      }

  }

  else {

  	error(list[0], out, "Error Expected %%");
  }
}


void OptDecList(vector<Unit> & list, ostream & out) {

  out << "<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl;

  if (list[0].lexeme == "integer" || list[0].lexeme == "boolean") { // checks for qualifer at the begining of a Declaration

    DecList(list, out);
  }
  else if (list[0].token_name == "unknown") {

    error(list[0], out, "Error Expected <Declaration List>");
  }
  else {
    //pop(list);
    Empty(list, out); // would flip the empty variable to true
  }
}


void DecList(vector<Unit> & list, ostream & out) {

  out << " <Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List>" << endl;

  Declaration(list, out);

  if (list[0].lexeme == ";") { // checks for the end of a Declaration

    pop(list); // advances

    if(list[0].lexeme == "integer" || list[0].lexeme == "boolean") { // checks again for qualifer if in <Declaration> | <DecList>

      DecList(list, out);
    }
  }
  else {

    error(list[0], out, "Expected ;");
  }
}


void Declaration(vector<Unit> & list, ostream & out) {

  out << "<Declaration> ::=   <Qualifier >  <identifier> " << endl;

  Qualifier(list, out);
  Id(list, out); // bits that make up a Declaration
}


void Qualifier(vector<Unit> & list, ostream & out) {

  out << " <Qualifier> ::=  integer |  boolean   " << endl;

  if (list[0].lexeme == "integer" || list[0].lexeme == "boolean") {

    pop(list);
  }

  else {

    error(list[0], out, "Expected integer or boolean Qualifier" );
  }
}


void Id(vector<Unit> & list, ostream & out) {

  out << "<identifier>" << endl;

  if(list[0].token_name == "identifier") {

    pop(list); //reaches the end of a path so-to-speak
  }
  else {

    error(list[0], out, "Expected <identifier>");
  }
}


void StatementList(vector<Unit> & list, ostream & out) {

  out << "<Statement List> ::=   <Statement>   | <Statement> <Statement List>" << endl;

	bool flag = true;
  while (flag) {

    Statement(list, out);
		pop(list);
		//
    //position += 1; - no idea if I need one of these hear or not, will come back
    // checks the begining of each part of a Statement
    if (list[0].lexeme != "begin" && list[0].token_name != "identifier" && list[0].lexeme != "if" && list[0].lexeme != "get" && list[0].lexeme != "put" && list[0].lexeme != "while") {

      flag = false;
    }

  }
}


void Statement(vector<Unit> & list, ostream & out) {

  out << "<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Get>   |   <Put>   |  <While>" << endl;

  if (list[0].lexeme == "begin") {

    Compound(list, out); //
  }
  else if (list[0].token_name == "identifier") {

    Assign(list, out); // I finished up to here after that if and if prime would be needed to fill in
  }
  else if (list[0].lexeme == "if") {

    RAT_if(list, out);
  }
  else if (list[0].lexeme == "get") {

    Get(list, out);
  }
  else if (list[0].lexeme == "put") {

    Put(list, out);
  }
  else if (list[0].lexeme == "while") {

    RAT_while(list, out);
  }
  else {

    error(list[0], out, "Expected <Compound>  |  <Assign>  |   <If>  |  <Get>   |   <Put>   |  <While>" );
  }
}


void Compound(vector<Unit> & list, ostream & out) {

  out << "<Compound> ::=   begin <Statement List>  end" << endl;

  if (list[0].lexeme == "begin") {

    pop(list);

    StatementList(list, out);

  //  pop(list);

    if(list[0].lexeme != "end") {

      error(list[0], out, "Expected end keyword");
    }
  }
  else {

    error(list[0], out, "Expected begin keyword");
  }
}


void Assign(vector<Unit> & list, ostream & out) {

  out << "<Assign> ::=     <Identifier> = <Expression> ;" << endl;

  if (list[0].token_name == "identifier") {

    pop(list);

    if (list[0].lexeme == "=") {

      pop(list);
      Expression(list, out);  // Expression and Expression Prime would apply somewhere here

      //pop(list);

      if (list[0].lexeme == ";") {

        // does nothing
      }
      else {

        error(list[0], out, "Expected ;");
      }
    }
    else {

      error(list[0], out, "Expected =");
    }
  }
  else {

    error(list[0], out, "Expected <identifier>");
  }
}


void RAT_if(vector<Unit> & list, ostream & out) {
  out << " <If> ::= if ( <Condition> ) <Statement> endif | if ( <Condition> ) <Statement> else <Statement> endif " << endl;
  //<If> ::= if ( <Condition> ) <Statement > <ifPrime>

  if (list[0].lexeme == "if"){
     pop(list);
       if (list[0].lexeme == "("){
         pop(list);
         Condition(list, out);
         //pop(list);
         if (list[0].lexeme == ")"){
           pop(list);
           Statement(list, out);
           RAT_if_Prime(list, out);

         }
         else {
           error(list[0], out, "Expected )");
         }
       }
       else {
         error(list[0], out, "Expected (");
       }
   }
   else {
         error(list[0], out, "Expected keyord if" );
   }

}


void RAT_if_Prime(vector<Unit> & list, ostream & out) {

  out << "<ifPrime> ::= endif | else <Statement> endif" << endl;

  if(list[0].lexeme == "endif") {

    pop(list); // case 1
  }

  else if(list[0].lexeme == "else") {

    pop(list);

    Statement(list, out); // case 2

    if (list[0].lexeme == "endif") {

      pop(list); // repeat of case 1
    }
    else {

      error(list[0], out, "Expected keyword endif");
    }
  }

  else {

    error(list[0], out, "Expected keyord endif or keyword else" );
  }
}


void Put(vector<Unit> & list, ostream & out) {
  out << "<Put> ::= put ( <identifier> );  " << endl;
  if (list[0].lexeme == "put"){
     pop(list);
       if (list[0].lexeme == "("){
         pop(list);
         Id(list, out);
         //pop(list);
         if (list[0].lexeme == ")"){
            pop(list);
            if (list[0].lexeme == ";"){
            }
            else {
              error(list[0], out, "Expected ;");
            }
         }
         else {
           error(list[0], out, "Expected )");
         }
       }
       else {
         error(list[0], out, "Expected (");
       }
   }
   else {
         error(list[0], out, "Expected keyword put");
   }
}


void Get(vector<Unit> & list, ostream & out) {
  out << "<Get> ::= get ( <Identifier> );  " << endl;
  if (list[0].lexeme == "get"){
     pop(list);
       if (list[0].lexeme == "("){
         pop(list);
         Id(list, out);
         //pop(list);
         if (list[0].lexeme == ")"){
            pop(list);
            if (list[0].lexeme == ";"){
            }
            else {
              error(list[0], out, "Expected ;");
            }
         }
         else {
           error(list[0], out, "Expected )");
         }
       }
       else {
         error(list[0], out, "Expected (");
       }
   }
   else {
         error(list[0], out, "Expected keyword get");
   }
}


void RAT_while(vector<Unit> & list, ostream & out) {
  out << "<While> ::= while ( <Condition> ) <Statement> " << endl;
  if (list[0].lexeme == "while"){
     pop(list);
     if (list[0].lexeme == "("){
       pop(list);
       Condition(list, out);
       //pop(list);
       if (list[0].lexeme == ")"){
         pop(list);
         Statement(list, out);
       }
       else {
         error(list[0], out, "Expected )");
            }
      }
      else {
        error(list[0], out, " Expected (");
      }
   }
   else {
     error(list[0], out, "Expected keyword while");
   }
}


void Condition(vector<Unit> & list, ostream & out) {
  out << "<Condition> ::= <Expression> <Relop> <Expression>" << endl;
  Expression(list, out);
  Relop(list, out);
  Expression(list, out);
}


void Relop(vector<Unit> & list, ostream & out) {
  out << "<Relop> ::= == | > | < | /= " << endl;
  if (list[0].lexeme == "==" || list[0].lexeme == ">" || list[0].lexeme == "<" ||list[0].lexeme == "/=" ){
    pop(list);
  }
  else {
    error(list[0], out , "Expected Relop Operator");
  }
}

void Empty(vector<Unit> & list, ostream & out) {

empty = true;
}

void CommentRemover(vector<Unit>  & lexer_list) {

	int start = 0;
	int end = 0;
	for(int i = 0; i < lexer_list.size(); i++) {

		if (lexer_list[i].lexeme == "/*") {

			start = i;
		}
		else if(lexer_list[i].lexeme == "*/") {

			end = i + 1;
		}

	}
	lexer_list.erase(lexer_list.begin() + start, lexer_list.begin() + end);
}
