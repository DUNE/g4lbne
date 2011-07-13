//*****************************************************************************
// class: LBNEKeyedInput
// author: Matthew Michelotti
// created: June 2010
//
// See LBNEKeyedInput.cc for documentation of member functions.
//
// This class is designed to read input similar to how GEANT3 read input
// using FFKEY calls. Each line of input is considered individually.
// The first word on the line is the key (the variable name). The remaining
// "words" in the line (delimited by spaces) form an array of integers,
// doubles, booleans, or strings, depending on the desired type of variable.
// An array can span multiple lines by using the same key multiple times
// and writing "#=" after the key, replacing # with the index number where the
// array left off (the first array index is 1). Comments can be placed on a
// line if the first series of alpha-numeric characters consists only of 'C',
// starting at the first column. None of the text is case sensitive.
//
// Example:
//
//   C This is a comment at the top of the file.
//   cc This is another comment.
//   MyInt           5
//   MyDoubleArr     3.0  -1.925E-3  4.  .2  7
//   MyString        'HELLO'
//   MyBoolArr       TRUE  false  0  1
//
//   Data0       1=  1.0   2.0   3.0   4.0
//   Data1       1= -1.0  -2.0  -3.0  -4.0
//
//   Data0       5=  5.0   6.0   7.0   8.0
//   Data1       5= -5.0  -6.0  -7.0  -8.0
//   
//*****************************************************************************

#ifndef LBNEKeyedInput_h
#define LBNEKeyedInput_h

#include "globals.hh"
#include <map>
#include <vector>
#include <sstream>

class LBNEKeyedInput {

private:
  typedef std::map<G4String,G4String> ssmap_t;
  typedef const G4String& cstr_t;

public:
  explicit LBNEKeyedInput();

  void readFile(cstr_t filename);
  void readString(cstr_t data);
  void readStream(std::istream& in);

  G4bool contains(G4String name) const;

  void get(cstr_t name, G4int& buffer, G4int length=-1);
  void get(cstr_t name, G4double& buffer, G4int length=-1);
  void get(cstr_t name, G4bool& buffer, G4int length=-1);
  void get(cstr_t name, G4String& buffer, G4int length=-1);

  void get(cstr_t name, std::vector<G4int>& buffer, G4int length=-1);
  void get(cstr_t name, std::vector<G4double>& buffer, G4int length=-1);
  void get(cstr_t name, std::vector<G4bool>& buffer, G4int length=-1);
  void get(cstr_t name, std::vector<G4String>& buffer, G4int length=-1);

  void printAll() const;

private:
  void handleLine(cstr_t line);
  static G4bool isLineComment(cstr_t line);
  void readIndexSpec(std::istringstream& in, cstr_t name);
  static G4int countTerms(cstr_t value);
  static void fail(cstr_t message, cstr_t key="",
		   G4int expected=-1);

  template<class T>
  void getSingle(cstr_t name, T& buffer);

  template<class T>
  void getVector(G4String name, std::vector<T>& buffer, G4int length);

  static void parse(std::istream& in, G4int& buffer, cstr_t name);
  static void parse(std::istream& in, G4double& buffer, cstr_t name);
  static void parse(std::istream& in, G4bool& buffer, cstr_t name);
  static void parse(std::istream& in, G4String& buffer, cstr_t name);

  static void checkGotTerm(std::istream& in, cstr_t message, cstr_t name);

  ssmap_t varMap;
};


#endif
