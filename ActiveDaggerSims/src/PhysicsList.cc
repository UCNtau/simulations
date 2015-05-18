//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id$
//
//

#include "PhysicsList.hh"
#include "G4ParticleTypes.hh"
#include "G4Scintillation.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eMultipleScattering.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4EmSaturation.hh"
#include "G4Cerenkov.hh"
#include "G4OpAbsorption.hh"
#include "G4OpWLS.hh"

#include "G4EmPenelopePhysics.hh"
#include "G4VModularPhysicsList.hh"


PhysicsList::PhysicsList()
{
	SetVerboseLevel(1);
	//physicsListManager = new G4EmPenelopePhysics(); //The manager will take care of putting in the PENELOPE physics.
}

PhysicsList::~PhysicsList()
{
	//delete physicsListManager;
}

void PhysicsList::ConstructParticle()
{
	// In this method, static member functions should be called
	// for all particles which you want to use.
	// This ensures that objects of these particle types will be
	// created in the program.

	//This method only tells geant4 which particles to use, still need the actual processes.

	//physicsListManager->ConstructParticle();
	G4OpticalPhoton::OpticalPhotonDefinition(); //We need optical photons in addition to PENELOPE's particles
}

void PhysicsList::ConstructProcess() //Here is the meat of the physics, description of the processes.
{
	AddTransportation(); //Mandatory pretty much
	//physicsListManager->ConstructProcess(); //Put in PENELOPE stuff
	
	//The PENELOPE physics does brehmsstrahlung losses, but those do not create optical photons.
	//The only optical photon processes present are those we add in here. In this case, Cerenkov
	//And Scintillation are the only ones that we will see.
	/*G4Cerenkov* theCerenkovProcess = new G4Cerenkov("Cerenkov");
	G4Scintillation* theScintillationProcess = new G4Scintillation("Scintillation");
	
	//I haven't decided on this one's proper value... Could need some tweaking.
	theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
	theCerenkovProcess->SetMaxBetaChangePerStep(10.0);
	theCerenkovProcess->SetTrackSecondariesFirst(true);

	theScintillationProcess->SetTrackSecondariesFirst(true);*/

	theParticleIterator->reset();
	while( (*theParticleIterator)() ){
		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();
		/*if (theCerenkovProcess->IsApplicable(*particle)) {
			pmanager->AddProcess(theCerenkovProcess);
			pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
		}
		if (theScintillationProcess->IsApplicable(*particle)) {
			pmanager->AddProcess(theScintillationProcess);
			pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
			pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
		}*/
		if (particleName == "opticalphoton") {
			pmanager->AddDiscreteProcess(new G4OpAbsorption());
			//pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
			//pmanager->AddDiscreteProcess(theMieHGScatteringProcess);
			pmanager->AddDiscreteProcess(new G4OpBoundaryProcess());
			pmanager->AddDiscreteProcess(new G4OpWLS("OpWLS"));
		}
	}
}

void PhysicsList::SetCuts()
{
	SetCutsWithDefault();
}
