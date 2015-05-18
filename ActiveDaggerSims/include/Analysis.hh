/* Analysis.hh
 * Author: adotti (Modified by Nathan Callahan)
 * Date: 6/20/2013
 * 
 * This defines the Analysis class, which is a singleton. The idea is to just use the singleton instance and
 * to pass data to the Analysis class. The first iteration is completely ignorant of geant4 (except the units).
 * Concievably the class could be made aware of hits collection/etc but I'll save that for another time and
 * focus on simplicity here.*/
 
#ifndef ANALYSIS_HH_
#define ANALYSIS_HH_

#include "G4UnitsTable.hh"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"

class Analysis {
	public:
		static Analysis* GetInstance();
		virtual ~Analysis();
		void NewEvent(G4int eventNo);
		void NewRun(G4int runNo);
		void PhotonHit(G4double time, G4double energy);
		void AddToTree();
		void WriteTree();
		void SetOutfile(G4String fileName);
		void SetEnergy(G4double particleEnergy);
		void SetDirection(G4double x, G4double y, G4double z);
		void SetPosition(G4double x, G4double y, G4double z);
	
	private:
		Analysis();
		static Analysis* singleton;
		G4int currentRunNo;
		G4int currentEventNo;
		G4float currentEnergy;
		G4float px;
		G4float py;
		G4float pz;
		G4float posx;
		G4float posy;
		G4float posz;
		G4int flag;
		G4float time;
		G4float energy;
		TFile* tFile;
		TTree* tTree;
		G4String outFile;
};

#endif /* ANALYSIS_HH_ */
