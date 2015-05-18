/* This code is the hook with Geant4's events. At the beginning and end of each event, the methods
 * here will be called by Geant4. Information about the event can be retrieved using methods from the G4Event
 * class (as is done here). There are lower level hooks as well, down to stepping hooks if more detailed
 * control or information is needed. */

#include "UserEventAction.hh"
#include "Analysis.hh"
#include "G4Event.hh"

UserEventAction::UserEventAction() {
	analyzer = Analysis::GetInstance();
}

UserEventAction::~UserEventAction() {;}

void UserEventAction::BeginOfEventAction(const G4Event* anEvent) {
	G4PrimaryVertex* vertex = anEvent->GetPrimaryVertex(); //PrimaryVertex stores the creation position
	G4PrimaryParticle* part = vertex->GetPrimary(); //Primary stores the particle information of the event
	G4double energy = part->GetKineticEnergy(); //Get energy
	G4ThreeVector direction = part->GetMomentumDirection();
	G4double x0 = vertex->GetX0(); //Initial positions
	G4double y0 = vertex->GetY0();
	G4double z0 = vertex->GetZ0();
	analyzer->NewEvent(anEvent->GetEventID()); //Event # (sequential)
	analyzer->SetDirection(direction.getX(), direction.getY(), direction.getZ()); //Tell analysis class information of particle
	analyzer->SetEnergy(energy);
	analyzer->SetPosition(x0, y0, z0);
}

void UserEventAction::EndOfEventAction(const G4Event* anEvent) { //After an event ends, then add the information to the TTree in the Analysis class.
}
