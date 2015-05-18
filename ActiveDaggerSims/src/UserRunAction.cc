/* This code is the hook with Geant4's runs. At the beginning and end of each event, the methods
 * here will be called by Geant4. Information about the run can be obtained using methods from the
 * Geant4 run class. */
 
#include "UserRunAction.hh"
#include "Analysis.hh"
#include "G4Run.hh"

UserRunAction::UserRunAction() {
	analyzer = Analysis::GetInstance();
}

UserRunAction::~UserRunAction() {;}

void UserRunAction::BeginOfRunAction(const G4Run* aRun) {
	analyzer->NewRun(aRun->GetRunID()); //Tell the analysis class a new run started and give it the run #
}

void UserRunAction::EndOfRunAction(const G4Run* aRun) {
}
