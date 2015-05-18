#ifndef UserEventAction_h
#define UserEventAction_h 1

#include "Analysis.hh"
#include "G4UserEventAction.hh"
#include "G4Event.hh"

class UserEventAction : public G4UserEventAction
{
	public:
		UserEventAction();
		~UserEventAction();
		void BeginOfEventAction(const G4Event* anEvent);
		void EndOfEventAction(const G4Event* anEvent);
	
	private:
		Analysis* analyzer;
};

#endif
