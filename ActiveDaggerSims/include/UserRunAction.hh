#ifndef UserRunAction_h
#define UserRunAction_h 1

#include "Analysis.hh"
#include "G4UserRunAction.hh"
#include "G4Run.hh"

class UserRunAction : public G4UserRunAction
{
	public:
		UserRunAction();
		~UserRunAction();
		void BeginOfRunAction(const G4Run* aRun);
		void EndOfRunAction(const G4Run* aRun);
	
	private:
		Analysis* analyzer;
};

#endif
