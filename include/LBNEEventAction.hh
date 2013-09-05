//
//LBNEEventAction.hh
//

#ifndef LBNEEventAction_h
#define LBNEEventAction_h 1

//C++
#include <string>
#include <vector>
#include <map>

#include "G4UserEventAction.hh"
#include "globals.hh"


class G4Event;
class G4Track;
class G4TrajectoryContainer;
class LBNERunManager;

class LBNEEventAction : public G4UserEventAction
{
   
public:
   LBNEEventAction();
   ~LBNEEventAction();
   
   void BeginOfEventAction(const G4Event*);
   void EndOfEventAction(const G4Event*);


private:

   typedef std::map<int, int> IIMap;
   typedef std::map<int, double> IDMap;
 
private:

   LBNERunManager* pRunManager;
   
};



#endif

    
