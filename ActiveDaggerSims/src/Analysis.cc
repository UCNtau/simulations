/* This class handles the recording of data for the simulation (actual analysis is carried
 * out later using root from the data recorded from this class). This is a singleton, so
 * only one instance of the class exists. The instance can be obtained using Analysis::GetInstance()
 * Because there is only one instance, there are methods for starting new runs, events, etc
 * so the state of the simulation can be tracked from the analysis class. The class has
 * member variables that keep track of relevant information for the run and event and this
 * information is recorded into the root file. */

#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "TFile.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TClass.h"
#include "TROOT.h"
#include "TTree.h"
#include "G4SystemOfUnits.hh"
#include <cmath>

#define MAXENTRIES 200 //This is the default starting size of the array that stores photon hits.

Analysis* Analysis::singleton = 0;

Analysis::Analysis() {
	outFile = "default.root"; //Default variables and tree setup.
	currentRunNo = 0;
	currentEventNo = 0;
	tFile = NULL;
	tTree = new TTree("PMTHitsTree","List of Times and Energies of Photon Hits");
	tTree->Branch("runNo", &currentRunNo, "runNo/I");
	tTree->Branch("eventNo", &currentEventNo, "eventNo/I");
	tTree->Branch("partEnergy", &currentEnergy, "vertexEnergy/F"); //Energy of given event
	tTree->Branch("px", &px, "px/F"); //Momentum and position of given event
	tTree->Branch("py", &py, "py/F");
	tTree->Branch("pz", &pz, "pz/F");
	tTree->Branch("posx", &posx, "posx/F");
	tTree->Branch("posy", &posy, "posy/F");
	tTree->Branch("posz", &posz, "posz/F");
	tTree->Branch("time", &time, "time/F");
	tTree->Branch("energy", &energy, "energy/F");
}
Analysis::~Analysis() {
	if(tFile != NULL) {
		tFile->Close();
	}
}

Analysis* Analysis::GetInstance() {			//So we'll access the sole instance of the class by calling this.
	if( Analysis::singleton == NULL ) {		//If there isn't an instance yet, create the instance using the
		Analysis::singleton = new Analysis;	//Private constructor. If there is an instance, return it.
	}
	return Analysis::singleton;
}

void Analysis::SetOutfile(G4String fileName) { //Sets the output filename
	outFile = strdup(fileName);
}

void Analysis::NewEvent(G4int eventNo) { //will start a new event to record
	currentEventNo = eventNo;
}

void Analysis::NewRun(G4int runNo) { //Starts a new run to record
	currentRunNo = runNo;
}

void Analysis::PhotonHit(G4double hitTime, G4double hitEnergy) { //Should be called when a photon hits the PMT
	time = hitTime;
	energy = hitEnergy;
	if(tTree != NULL) { tTree->Fill(); }
}

void Analysis::AddToTree() {	//Should be called at the end of the event so the information stored in the analysis
								//class is pushed onto the tree.
	if(tTree != NULL) {
		tTree->Fill();
	}
}

void Analysis::WriteTree() {	//Should be called at the end of execution to write all the information stored into
								//The tree to disk.
	tFile = TFile::Open(outFile,"recreate");
	tTree->Write();
	tFile->Close();
}

void Analysis::SetEnergy(G4double particleEnergy) { //Sets the energy of the run (for tracking purposes)
	currentEnergy = (G4float)particleEnergy;
}

void Analysis::SetDirection(G4double x, G4double y, G4double z) { //Sets the direction of the run
	px = (G4float)x;
	py = (G4float)y;
	pz = (G4float)z;
}

void Analysis::SetPosition(G4double x, G4double y, G4double z) { //Sets position of the run.
	posx = (G4float)x;
	posy = (G4float)y;
	posz = (G4float)z;
}
