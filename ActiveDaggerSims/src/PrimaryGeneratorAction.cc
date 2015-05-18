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

/* This code sets up the particle generators for events in the simulation. Previously,
 * the code used the particle gun, which was a very basic generator that didn't have
 * much configuration ability. The particle gun could have been modified each time
 * a particle is generated (using random libraries and sampling from whatever relavent
 * distributions), but instead I went with a general particle source. The general
 * particle source (gps) is able to have various energy, spatial, and directional
 * distributions including fitted histograms and simple distributions (for example,
 * a gaussian distribution of energy, or a programmed histogram that is randomly sampled).
 * The GPS is controlled either from the code or from the Macro file given to the program. */

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "Analysis.hh"
#include "G4GeneralParticleSource.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction():G4VUserPrimaryGeneratorAction()
{
	partSource = new G4GeneralParticleSource();
	
	/*G4int n_particle = 1;
	particleGun = new G4ParticleGun(n_particle);

	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="e-"));
	particleGun->SetParticleEnergy(1.0*MeV);
	particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));*/
	
	Analyzer = Analysis::GetInstance();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	//delete particleGun;
	delete partSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	//Analyzer->SetEnergy(particleGun->GetParticleEnergy()/keV);
	//G4cout<<"Event:\t"<<anEvent->GetEventID()<<"\tEnergy:\t"<<particleGun->GetParticleEnergy()/keV<<G4endl;
	/*G4ThreeVector v(1.0,0.0,0.0);
	particleGun->SetParticleMomentumDirection(v);*/
	partSource->GeneratePrimaryVertex(anEvent);
}
