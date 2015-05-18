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
// --------------------------------------------------------------
//      GEANT 4 - exampleN01
// --------------------------------------------------------------

#include <vector>

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "SensitiveDetector.hh"
#include "Analysis.hh"
#include "UserRunAction.hh"
#include "UserEventAction.hh"

#include "G4SDManager.hh"

#include "G4GDMLParser.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


//#include "G4LogicalVolumeStore.hh"
//#include "G4TransportationManager.hh"

int main(int argc, char **argv)
{
	G4RunManager* runManager = new G4RunManager;

	//Registers the geometry constructors.
	runManager->SetUserInitialization(new DetectorConstruction);

	//Registers the physics constructors
	G4VUserPhysicsList* physics = new PhysicsList;
	runManager->SetUserInitialization(physics);

	//Registers the particle generators
	G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction;
	runManager->SetUserAction(gen_action);
	
	//Registers our Run and Event hooks
	G4UserRunAction* runAction = new UserRunAction();
	G4UserEventAction* eventAction = new UserEventAction();
	runManager->SetUserAction(runAction);
	runManager->SetUserAction(eventAction);

	// Initialize G4 kernel
	runManager->Initialize();

#ifdef G4VIS_USE
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif    


	G4UImanager * UImanager = G4UImanager::GetUIpointer();
	
	Analysis* analyzer = Analysis::GetInstance();

	if (argc!=1)   // batch mode  
	{
		G4String command = "/control/execute ";
		G4String fileName = argv[1]; //File name for the macro to be run
		if(argc == 3) { //If we were provided with a macro file to run as well as a file name to output as
			G4String outFile = argv[2];
			analyzer->SetOutfile(outFile);
		}
		else {
			analyzer->SetOutfile("default.root");
		}
		UImanager->ApplyCommand(command+fileName);
	}
	else           // interactive mode : define UI session
	{ 
#ifdef G4UI_USE
		G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
		UImanager->ApplyCommand("/control/execute vis.mac");     
#endif
		ui->SessionStart();
		delete ui;
#endif

#ifdef G4VIS_USE
		delete visManager;
#endif     
	}
	
	analyzer->WriteTree(); //Last thing to do is to write to disk

	// Free the store: user actions, physics_list and detector_description are
	//                 owned and deleted by the run manager, so they should not
	//                 be deleted in the main() program !

	delete runManager;


	return 0;
}


