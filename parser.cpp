#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
using namespace std;


/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.  
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .  
       and then append the two files into one: 
          cat scanner.cpp parser.cpp > myparser.cpp
*/

//=====================================================
// File scanner.cpp written by: Group Number: 7 
//=====================================================

// ----------- Four Utility Functions and Globals ------------------//


enum tokentype {CONNECTOR, DESTINATION, EOFM, ERROR, IS, OBJECT, PERIOD, PRONOUN, SUBJECT, VERB, VERBNEG, VERBPAST, VERBPASTNEG, WAS, WORD1, WORD2};
string reservedWords[30][2] = {{"masu",         ""}, {"masen",        ""}, {"mashita",      ""}, {"masendeshita", ""}, {"desu",         ""}, {"deshita",      ""}, {"o",            ""}, {"wa",           ""}, {"ni",           ""}, {"watashi",      "I/me"}, {"anata",        "you"}, {"kare",         "he/him"}, {"kanojo",       "she/her"}, {"sore",         "it"}, {"mata",         "Also"},  {"soshite",      "Then"}, {"shikashi",     "However"}, {"dakara",       "Therefore"}, {"eofm",         ""}  };


tokentype reservedWordsType[30] = { VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, PRONOUN, PRONOUN, PRONOUN, PRONOUN, CONNECTOR, CONNECTOR, CONNECTOR, CONNECTOR, EOFM};


string tokenName[30] = {"ERROR", "WORD1", "WORD2", "PERIOD", "VERB", "VERBPAST", "VERBNEG", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR", "EOFM"};
tokentype next_token();
bool word(string s);
bool period(string s);
int scanner(tokentype &a, string &w);
bool match(tokentype expected);
void Story();
void S();
void AfterSubject();
void AfterNoun();
void AfterObject();
void Noun();
void Verb();
void Be();
void Tense();
void Syntax_Error1(tokentype);
void Syntax_Error2(string);
tokentype  saved_token;     //the scanner token global buffer
bool   token_available;     // global flag indicating whether
string saved_lexeme;
bool error1 = false;
bool error2 = false;




// ** Need the updated match and next_token (with 2 global vars)


// Done by: Kunal Banekar 
tokentype next_token()
{  
  if (!token_available)   //if there is no saved token                                                                                                 
    {
      scanner(saved_token, saved_lexeme);  //to grab a new token                                                                 
      cout << "Scanner called using word: " << saved_lexeme << endl;
      if(saved_token == ERROR)
	{
	  cout << "LEXICAL ERROR: " << saved_lexeme << " is not a valid token" << endl;
	}
      token_available = true;  //Saved                                                                                           
    }
  return saved_token;    //To return the saved token                                                                                                                                 
}

//Done by:Kunal Banekar                                                                                                   
bool match(tokentype expected)
{
  
  if (next_token() != expected)  //If mismatched occur                                                                                                    
    {
      Syntax_Error1(expected); //syntax error message                                                                                                                  
      scanner(saved_token, saved_lexeme);
      match(saved_token);
      //error handling                                                                                                                                             
    }
  else  //else match has occurred                                                                                                                                                      
    {
      token_available = false;  // eat up the token                                                                                                                               
      return true;              // say there was a match                                                                                                                           
    }
}



// ----- RDP functions - one per non-term -------------------//

// ** Make each non-terminal into a function here                                                                                                

//RE:<story>  ::= <s> { <s> }                                                                                                                
//Done by:Kanishq Patel                                                                                                               
void Story()
{
  cout << "Processing <Story> " << endl;
  cout << endl;
  S();
  while(next_token() != EOFM)
    {
      switch(next_token())
        {
        case CONNECTOR:
        case WORD1:
        case PRONOUN: S();
          break;
        default: return;
        }
    }
}

//RE:<s>  ::=  [CONNECTOR] <noun> SUBJECT <afterSubject>                                                                                     
//Done by: Kanishq Patel                                                                                                              
void S()
{
  cout << "Processing <s>" << endl;
  if(next_token() == CONNECTOR)
    {
      match(CONNECTOR);
      cout << "Matched CONNECTOR" << endl;
    }
  Noun(); 
  match(SUBJECT); 
  cout << "Matched SUBJECT" << endl;
  AfterSubject();
  
}

//RE:<afterSubject>  ::= <verb> <tense> PERIOD | <noun>  <afterNoun>                                                                         
//Done by: Summit Kulkarni 
void AfterSubject()
{
  cout << "Processing <afterSubject>" << endl;
  switch(next_token())
    {
    case WORD2: Verb(); Tense(); match(PERIOD); cout << "Matched PERIOD" << endl;
      break;
    case WORD1: Noun(); AfterNoun();
      break;
    case PRONOUN: Noun(); AfterNoun();
      break;
    default: Syntax_Error2("<After-Subject");
    }
}

//RE:<afterNoun>  ::= <be>  PERIOD | DESTINATION  <verb> <tense> PERIOD | OBJECT <afterObject>                                               
//Done by: Summit Kulkarni
void AfterNoun()
{
  cout << "Processing <afterNoun>" << endl;
  switch (next_token())
    {
    case IS :
    case WAS : Be(); match(PERIOD); cout << "Matched PERIOD" << endl; 
      break;
    case DESTINATION : match(DESTINATION); cout << "Matched DESTINATION" << endl; Verb(); Tense(); match(PERIOD); cout << "Matched PERIOD" << endl; 
      break;
    case OBJECT : match(OBJECT); cout << "Matched OBJECT" << endl; AfterObject(); 
      break;
    default : Syntax_Error2("After-Noun");
    }
}

//RE:<afterObject>_ ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD                                                     
//Done by:Kunal Banekar                                                                                                               
void AfterObject()
{
  cout << "Processing <afterObject>" << endl;
  switch(next_token())
    {
    case WORD2: Verb(); Tense(); match(PERIOD); cout << "Matched PERIOD" << endl;
      break;
    case WORD1:
    case PRONOUN: Noun(); match(DESTINATION); cout << "Matched DESTINATION" << endl; Verb(); Tense(); match(PERIOD); cout << "Matched PERIOD" << endl; 
      break;
    default: Syntax_Error2("After-Object");
    }
  
}


//RE:<noun>  ::= WORD1 | PRONOUN                                                                                                    
//Done by: Kunal Banekar                                                                                                        
void Noun()
{
  cout << "Processing <noun>" << endl;
  switch(next_token())
    {
    case WORD1: match(WORD1); cout << "Matched WORD1" << endl;
      break;
    case PRONOUN: match(PRONOUN); cout << "Matched PRONOUN" << endl;
      break;
    default: Syntax_Error2("Noun");
    }
}

//RE:<verb>  ::= WORD2                                                                                                                       
//Done by: Kanishq Patel           
void Verb()
{
  cout << "Processing <verb>" << endl;
  match(WORD2);
  cout << "Matched WORD2" << endl;
}

//RE:<be>  ::= IS | WAS                                                                                                           
//Done by: Kanishq Patel                                                                                                   
void Be()
{
  cout << "Processing <be>" << endl;
  switch(next_token())
    {
    case IS: match(IS); cout << "Matched IS" << endl;
      break;
    case WAS: match(WAS); cout << "Matched WAS" << endl;
      break;
    default: Syntax_Error2("BE");
    }
  
}

//RE:<tense>  ::= VERBPAST  | VERBPASTNEG | VERB | VERBNEG                                                                       
//Done by:Summit Kulkarni
void Tense()
{
  cout << "Processing <tense>" << endl;
  switch(next_token())
    {
    case VERBPAST: match(VERBPAST); cout << "Matched VERBPAST" << endl;
      break;
    case VERBPASTNEG: match(VERBPASTNEG); cout << "Matched VERBPASTNEG" << endl;
      break;
    case VERB: match(VERB); cout << "Matched VERB" << endl;
      break;
    case VERBNEG: match(VERBNEG); cout << "Matched VERBNEG" << endl;
      break;
    default: Syntax_Error2("TENSE");
    }
}
// ** Be sure to put the corresponding grammar rule above each function                                                                      



// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)



//Done by: Summit Kulkarni                                                                                                                                                      
void Syntax_Error1(tokentype thetoken)
{
  cout << "SYNTAX ERROR: Expected " << tokenName[thetoken] << " but found " << saved_lexeme <<endl;
  error1 = true; 
  exit(1);  
}

//Done by: Kunal Banekar                                                                                                                                                     
void Syntax_Error2(string func)
{
  cout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in parser  " << func << endl;
  error2 = true;
  exit(1);
}

// --------- Two DFAs ---------------------------------

//Done by: Kunal Banekar, Kanishq Patel, Summit Kulkarni                                                                                                                  
//RE: (vowel | vowel n | consonant vowel | consonant vowel n |consonant-pair vowel | consonant-pair vowel n)^+   
bool word(string s) {
  int state = 0;
  int charpos = 0;
  
  while (s[charpos] != '\0') {
    
    char k = s[charpos];
    
    
    //State 0    
    
    if (state == 0 && (k == 'a' || tolower(k) == 'e' || tolower(k) == 'i' || k == 'o' || k == 'u'))
      state = 1;
    else if (state == 0 && (k == 'b' || k == 'g' || k == 'h' || k == 'k' || k == 'm' || k == 'n' || k == 'p' || k == 'r'))
      state = 2;
    else if (state == 0 && (k == 'c'))
      state = 3;
    else if (state == 0 && (k == 'd' || k == 'j' || k == 'w' || k == 'y' || k == 'z'))
      state = 4;
    else if (state == 0 && (k == 's'))
      state = 5;
    else if (state == 0 && (k == 't'))
      state = 6;
    
    
    //State 1     
    
    else if (state == 1 && (k == 'a' || tolower(k) == 'e' || tolower(k) == 'i' || k == 'o' || k == 'u'))
      state = 1;
    else if (state == 1 && (k == 'b' || k == 'g' || k == 'h' || k == 'k' || k == 'm' || k == 'p' || k == 'r'))
      state = 2;
    else if (state == 1 && (k == 'c'))
      state = 3;
    else if (state == 1 && (k == 'd' || k == 'j' || k == 'w' || k == 'y' || k == 'z'))
      state = 4;
    else if (state == 1 && (k == 's'))
      state = 5;
    else if (state == 1 && (k == 't'))
      state = 6;
    else if (state == 1 && (k == 'n'))
      state = 7;
    
    //State 2 
    
    else if (state == 2 && (k == 'y'))
      state = 4;
    else if (state == 2 && (k == 'a' || tolower(k) == 'e' || tolower(k) == 'i' || k == 'o' || k == 'u'))
      state = 1;
    
    
    //State 3 
    
    else if (state == 3 && (k == 'h'))
      state = 4;
    
    
    //State 4
    
    else if (state == 4 && (k == 'a' || tolower(k) == 'e' || tolower(k) == 'i' || k == 'o' || k == 'u'))
      state = 1;
    
    
    //State 5 block    
    
    else if (state == 5 && (k == 'a' || tolower(k) == 'e' || tolower(k) == 'i' || k == 'o' || k == 'u'))
      state = 1;
    else if (state == 5 && (k == 'h'))
      state = 4;

    
    //State 6 
    
    else if (state == 6 && (k == 'a' || tolower(k) == 'e' || tolower(k) == 'i' || k == 'o' || k == 'u'))
      state = 1;
    else if (state == 6 && (k == 's'))
      state = 5;
    
    
    //State 7 
    
    else if (state == 7 && (k == 'a' || tolower(k) == 'e' || tolower(k) == 'i' || k == 'o' || k == 'u'))
      state = 1;
    else if (state == 7 && (k == 'b' || k == 'g' || k == 'h' || k == 'k' || k == 'm' || k == 'n' || k == 'p' || k == 'r'))
      state = 2;
    else if (state == 7 && (k == 'c'))
      state = 3;
    else if (state == 7 && (k == 'd' || k == 'j' || k == 'w' || k == 'y' || k == 'z'))
      state = 4;
    else if (state == 7 && (k == 's'))
      state = 5;
    else if (state == 7 && (k == 't'))
      state = 6;
    else
      return (false);
    charpos++;
  }//end of while
  
  // where did I end up????
  if (state == 1 || state == 7) return (true);  // end in a final state
  else return (false);
}


//Done by: Kanishq Patel
bool period(string s) {
  int state = 0;
  int charpos = 0;
  
  while (s[charpos] != '\0') {
    if (state == 0 && (s[charpos] == '.'))
      state = 1;
    else
      return (false);
    charpos++;
  }
  
  if (state == 1) return (true);
  else return (false);
}


//----------------Scanner-Functions--------------------//


ifstream fin;		
//Done by: Summit Kulkarni, Kunal Banekar, Kanishq Patel
int scanner(tokentype &tt, string &w) {
  
  // ** Grab the next word from the file via fin
  // 1. If it is eofm, return right now.   
  
  /*  **
  2. Call the token functions (word and period) 
     one after another (if-then-else).
     Generate a lexical error message if both DFAs failed.
     Let the tokentype be ERROR in that case.

  3. If it was a word,
     check against the reservedwords list.
     If not reserved, tokentype is WORD1 or WORD2
     decided based on the last character.

  4. Return the token type & string  (pass by reference)
  */

  bool reserved = false;
  
  fin >> w;
  if (w == "eofm")
    return 0;
  if (period(w))
    tt = PERIOD;
  else if (word(w)) {
    for (int i = 0; i < 30; i++) {
      if (w == reservedWords[i][0]) {
	tt = reservedWordsType[i];
	reserved = true;
	break;
      }
    }
    if (!reserved) {
      if (w[w.length() - 1] == 'I' || w[w.length() - 1] == 'E')
	tt = WORD2;
      else
	tt = WORD1;
    }
  } else tt = ERROR;
  return 1;
  
  
  
}//the end of scanner


//----------- Driver ---------------------------//

// The new test driver to start the parser
int main()
{
  
  //- opens the input file
  tokentype thetype;
  string theword;
  string filename;
  
  cout << "Enter the input file name: ";
  cin >> filename;
  
  fin.open(filename.c_str());
  //- calls the <story> to start parsing
  Story();
  cout << endl;
  if(error1 == false && error2 == false)
    cout << "Succesfully Parsed <story>." << endl;
  //- closes the input file 
  fin.close();
  return 0;
}// end

