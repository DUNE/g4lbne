//author: Matthew Michelotti

#include "LBNEKeyedInput.hh"
#include <fstream>
using namespace std;

//Constructs object with an empty varMap. (The varMap maps variable names to
//their contents.)
LBNEKeyedInput::LBNEKeyedInput() {}


//readFile, readString, and readStream behave similarly in that they read
//text input and add the appropriate variables to the varMap.
//See the class description for details on the format.

//-------------------------------------------------------------------------------
void LBNEKeyedInput::readFile(cstr_t filename) {
  ifstream in(filename.c_str());
  if(!in.is_open()) fail("unable to open file");
  readStream(in);
  in.close();
}
//-------------------------------------------------------------------------------
void LBNEKeyedInput::readString(cstr_t data) {
  istringstream in(data);
  readStream(in);
}
//-------------------------------------------------------------------------------
void LBNEKeyedInput::readStream(istream& in) {
  G4String line;
  while(!in.eof()) {
    getline(in, line);
    if(in.bad()) fail("error reading input stream");
    if(in.fail()) break;
    line.toUpper();
    handleLine(line);
  }
}

//-------------------------------------------------------------------------------
//return true iff a variable with the given name was read from input
G4bool LBNEKeyedInput::contains(G4String name) const {
  name.toUpper();
  return (varMap.find(name) != varMap.end());
}


//get(name, buffer, length) is a function overloaded many times. The type of
//buffer will determine which function is called. The get function will
//look up a variable with the given name, parse it, and write the resulting
//value into buffer. If buffer is a vector, then the optional length
//parameter may be used to require the input to be a certain length.
//The default length=-1 means that any length is allowed.

void LBNEKeyedInput::get(cstr_t name, G4int& buffer, G4int /*length*/)
  {getSingle(name, buffer);}
void LBNEKeyedInput::get(cstr_t name, G4double& buffer, G4int /*length*/)
  {getSingle(name, buffer);}
void LBNEKeyedInput::get(cstr_t name, G4bool& buffer, G4int /*length*/)
  {getSingle(name, buffer);}
void LBNEKeyedInput::get(cstr_t name, G4String& buffer, G4int /*length*/)
  {getSingle(name, buffer);}

void LBNEKeyedInput::get(cstr_t name, vector<G4int>& buffer, G4int length)
  {getVector(name, buffer, length);}
void LBNEKeyedInput::get(cstr_t name, vector<G4double>& buffer, G4int length)
  {getVector(name, buffer, length);}
void LBNEKeyedInput::get(cstr_t name, vector<G4bool>& buffer, G4int length)
  {getVector(name, buffer, length);}
void LBNEKeyedInput::get(cstr_t name, vector<G4String>& buffer, G4int length)
  {getVector(name, buffer, length);}

//-------------------------------------------------------------------------------
//Prints contents of the varMap to standard output.
void LBNEKeyedInput::printAll() const {
  G4cout << G4endl;
  G4cout << "printing LBNEKeyedInput..." << G4endl;
  G4cout << "--------------------------" << G4endl << G4endl;
  for(ssmap_t::const_iterator it = varMap.begin(); it != varMap.end(); it++) {
    G4cout << it->first << ": " << it->second << G4endl << G4endl;
  }
  G4cout << "--------------------------" << G4endl << G4endl;
}

//-------------------------------------------------------------------------------
//Processes a single line of input data. If line is empty or a comment,
//this will do nothing. If line is a continuation of an array, it will
//append the new contents to the pre-existing contents. Otherwise, it will
//make a new varMap entry with the appropriate contents.
void LBNEKeyedInput::handleLine(cstr_t line) {
  if(isLineComment(line)) return;

  istringstream in(line);
  G4String key;
  in >> key;
  if(in.fail()) return;

  readIndexSpec(in, key);
  
  G4String term;
  ostringstream value;
  value << varMap[key];
  while(!in.fail()) {
    in >> term;
    if(!in.fail()) value << term << ' ';
  }

  varMap[key] = value.str();
}

//-------------------------------------------------------------------------------
//Return true iff line is a comment or "". A comment starts with a series
//of C's, then a non-alphanumeric character like ' '.
G4bool LBNEKeyedInput::isLineComment(cstr_t line) {
  for(size_t i = 0; i < line.size(); i++) {
    if(line[i] == 'C') continue;
    if(isalnum(line[i])) return false;
    return (i != 0);
  }
  return true;
}

//-------------------------------------------------------------------------------
//Checks if there is a "#=" term (what I call the "index specifier") as the
//next thing in the input stream "in". After this function, the input stream
//will be positioned right after the index specifier, if it exists. Also
//checks that this is the correct index for the variable with the given name.
void LBNEKeyedInput::readIndexSpec(istringstream& in, cstr_t name) {
  streampos start = in.tellg();
  G4int index;
  in >> index;
  if(in.get() != '=' || in.fail()) {
    if(contains(name)) fail("duplicate variable", name);
    in.clear();
    in.seekg(start);
  }
  else if(index != countTerms(varMap[name]) + 1) {
    fail("illegal array indexing", name);
  }
}

//-------------------------------------------------------------------------------
//counts how many words (separated by spaces) are in value
G4int LBNEKeyedInput::countTerms(cstr_t value) {
  istringstream in(value);
  G4String term;
  G4int count = 0;
  in >> ws;
  while(!in.eof()) {
    in >> term >> ws;
    count++;
  }
  return count;
}
//-------------------------------------------------------------------------------
//calls G4Exception with a constructed message
//-message: description of the nature of the error
//-key: optional variable name relating to the error (default: "")
//-expected: optional integer that was expected but not actual (default: -1)
void LBNEKeyedInput::fail(cstr_t message, cstr_t key, G4int expected) {
  ostringstream exc;
  exc << "LBNEKeyedInput error:\n";
  if(key != "") exc << "  variable: " << key << '\n';
  exc << "  message: " << message << '\n';
  if(expected >= 0) exc << "  expected: " << expected << '\n';
  G4Exception(exc.str().c_str(),"",FatalException,"");
}
//-------------------------------------------------------------------------------
//Similar to getVector, but only reads in one value instead of a vector.
template<class T>
void LBNEKeyedInput::getSingle(cstr_t name, T& buffer) {
  vector<T> myVector;
  getVector(name, myVector, 1);
  buffer = myVector[0];
}
//-------------------------------------------------------------------------------
//A template function to parse a variable with the given name, and write
//the result to buffer. The general form of this variable is a sequence
//of terms, separated by single spaces. Each term will be parsed using one
//of the four parse() functions, depending on the type T. length is the
//required length of the resulting vector, or -1 if any length is allowed.
template<class T>
void LBNEKeyedInput::getVector(G4String name, vector<T>& buffer, G4int length)
{
  name.toUpper();
  if(buffer.size() != 0) fail("non-empty buffer", name);
  istringstream in(varMap[name]);
  in >> ws;
  if(in.eof()) fail("missing or empty variable", name);
  do {
    T term;
    parse(in, term, name);
    buffer.push_back(term);
    in >> ws;
  }while(!in.eof());
  if(length >= 0 && (size_t)length != buffer.size()) {
    fail("unexpected array length", name, length);
  }
}


//-------------------------------------------------------------------------------
//The parse(in, buffer, name) functions will read a single term from the input
//stream "in" and write it to buffer.  The exact function that is called
//depends on the type of buffer. name is the name of the variable, used
//for displaying a message in case of failure.

void LBNEKeyedInput::parse(istream& in, G4int& buffer, cstr_t name) {
  in >> buffer;
  checkGotTerm(in, "expected integer(s)", name);
}

void LBNEKeyedInput::parse(istream& in, G4double& buffer, cstr_t name) {
  in >> buffer;
  checkGotTerm(in, "expected floating point number(s)", name);
}

void LBNEKeyedInput::parse(istream& in, G4bool& buffer, cstr_t name) {
  G4String term;
  in >> term;
  if(term == "TRUE" || term == "1") buffer = true;
  else if(term == "FALSE" || term == "0") buffer = false;
  else in.setstate(ios::failbit);
  checkGotTerm(in, "expected boolean(s)", name);
}

void LBNEKeyedInput::parse(istream& in, G4String& buffer, cstr_t name) {
  in >> buffer;
  size_t size = buffer.size();
  if(size >= 2 && buffer[0u] == '\'' && buffer[size-1u] == '\'') {
    buffer.erase(size-1,1);
    buffer.erase(0,1);
  }
  else {
    in.setstate(ios::failbit);
  }
  checkGotTerm(in,
	 "expected string(s) in single quotes, no spaces within string", name);
}

//-------------------------------------------------------------------------------
//Called by parse functions to make sure the input stream "in" was
//successful in reading the data. If not, fail is called with the given
//message and variable name.
void LBNEKeyedInput::checkGotTerm(istream& in, cstr_t message, cstr_t name) {
  if(in.peek() != ' ' || in.fail()) fail(message, name);
}
