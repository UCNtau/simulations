
#include "SensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4HCofThisEvent.hh"
#include "G4HCtable.hh"
#include "G4UnitsTable.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"
#include "Analysis.hh"

SensitiveDetector::SensitiveDetector(G4String SDname) : G4VSensitiveDetector(SDname)
{
	Analyzer = Analysis::GetInstance();
}


SensitiveDetector::~SensitiveDetector()
{}


/*-----------------------------------------------------------------------------------------
 * This method is called for each step in a "sensitive volume" from a particle in a volume.
 * A sensitive detector is created with the geometry setup and attached to a physical volume.
 * From the step a lot of information is obtainable about the particle (energy deposition,
 * the pre & post step point, track information). The pre and post step points can also
 * be accessed which contain more information such as energy, time, momentum, or other kinematic
 * quantities about the particle before and after the step.
 *-----------------------------------------------------------------------------------------*/

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
	//Let's just look at the # of photons that hit our sensitive surface & are absorbed.
	//Right now the entire lightguide is the sensitive detector, so each step should generate a hit...

	G4StepPoint* postPoint = step->GetPostStepPoint(); //Get the next step of the particle
	G4VPhysicalVolume* postPV = postPoint->GetPhysicalVolume(); //Get information about geometry
	G4String postPVname = postPV->GetName();

	if(postPVname == "sipm_PV") { //If the particle will hit the PMT Surface (doesn't check for particle type...
		G4double energy = postPoint->GetTotalEnergy();
		//G4cout<<time/ns<<"\t"<<edep/eV<<G4endl;
		Analyzer->PhotonHit(0.0, energy); //Add a hit to the analyzer
		printf("Photon Hit!\n");
	}

	return true;
}

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{;}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*) //Called at the end of the event. G4HCofThisEvent
//Is the Geant4 Hits Collection of this event that stores the hits. This code does not use the G4CofThisEvent currently.
{;}
