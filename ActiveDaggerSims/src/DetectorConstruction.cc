#include "G4GDMLParser.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Material.hh"
#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "valgrind/callgrind.h"

/* This code will construct the detector geometry. The actual geometry (meaning the shapes
 * and positions) are read in through GDML, but the material properties and sensitive detectors
 * are made through the DetectorConstruction::Construct() method. This is another mandatory
 * Geant4 class. */

DetectorConstruction::DetectorConstruction() {
}

G4VPhysicalVolume* DetectorConstruction::Construct() { //Construct! Returns the volume to be simulated.
	const G4double hc = 1239.842; //planck's constant * speed of light, for converting wavelengths to eV

	//This sets up the GDML Parser
	G4GDMLParser parser;
	//parser.SetStripFlag(false);
	CALLGRIND_TOGGLE_COLLECT;
	parser.Read("geom.gdml"); //Reads the gdml
	CALLGRIND_TOGGLE_COLLECT;
	
	G4LogicalVolume* wlsVol = parser.GetVolume("WLS");
	G4Material* wlsMat = wlsVol->GetMaterial();
	
	const G4int rIndexNumEntries = 2; //Refractive index. So basically constant.
	G4double rIndexEnergy[rIndexNumEntries] = { 1*eV, 100*eV };
	G4double rIndexWLS[rIndexNumEntries] = { 1.58, 1.58 };
	//G4double absorptionWLS[rIndexNumEntries] = { 5.0*m, 5.0*m };
	
	const G4int wlsNumEntries = 7;
	
	/*G4double wlsEnergyAbs[wlsNumEntries] = {(hc/360.0)*eV, (hc/425.0)*eV, (hc/438.0)*eV, (hc/440.0)*eV, (hc/475.0)*eV};
	G4double wlsAbs[wlsNumEntries] = {1.0*m, .1 * mm, .125 * mm, .11 * mm, 1.0*m};
	
	G4double wlsEnergyEmis[wlsNumEntries] = {(hc/460.0)*eV, (hc/490.0)*eV, (hc/510.0)*eV, (hc/520.0)*eV, (hc/575.0)*eV};
	G4double wlsEmis[wlsNumEntries] = {0.0, 1.0, .55, .58, 0};*/
	
	printf("%f, %f, %f, %f, %f, %f, %f\n", (hc/500.0), (hc/475.0), (hc/450.0), (hc/438.0), (hc/425.0), (hc/360.0), (hc/300.0)); 
	
	G4double wlsEnergyAbs[wlsNumEntries] = {(hc/500.0)*eV, (hc/475.0)*eV, (hc/450.0)*eV, (hc/438.0)*eV, (hc/425.0)*eV, (hc/360.0)*eV, (hc/300)*eV};
	G4double wlsAbs[wlsNumEntries] = {1000*mm, 20*mm, 5*mm, 7*mm, 3*mm, 20*mm, 1000*mm};
	
	G4double wlsEnergyEmis[wlsNumEntries] = {(hc/600.0)*eV, (hc/575.0)*eV, (hc/520.0)*eV, (hc/510.0)*eV, (hc/490.0)*eV, (hc/460.0)*eV, (hc/300.0)*eV};
	G4double wlsEmis[wlsNumEntries] = {0.0, 0.0, .58, .55, 1.0, 0.0, 0.0};
	
	G4MaterialPropertiesTable* wlsMPT = new G4MaterialPropertiesTable();
	
	wlsMPT->AddProperty("RINDEX", rIndexEnergy, rIndexWLS, rIndexNumEntries);
	//wlsMPT->AddProperty("ABSLENGTH", rIndexEnergy, absorptionWLS, rIndexNumEntries);
	wlsMPT->AddProperty("WLSABSLENGTH", wlsEnergyAbs, wlsAbs, wlsNumEntries);
	wlsMPT->AddProperty("WLSCOMPONENT", wlsEnergyEmis, wlsEmis, wlsNumEntries);
	wlsMPT->AddConstProperty("WLSTIMECONSTANT", .5*ns);
	
	wlsMat->SetMaterialPropertiesTable(wlsMPT);
	
	//
	
	G4LogicalVolume* sipmVol = parser.GetVolume("sipm");
	G4Material* sipmMat = sipmVol->GetMaterial();
	
	G4double rIndexsipm[rIndexNumEntries] = { 1.54, 1.54 };
	G4double absorptionsipm[rIndexNumEntries] = { .1*mm, .1*mm };
	
	G4MaterialPropertiesTable* sipmMPT = new G4MaterialPropertiesTable();
	
	sipmMPT->AddProperty("RINDEX", rIndexEnergy, rIndexsipm, rIndexNumEntries);
	sipmMPT->AddProperty("ABSLENGTH", rIndexEnergy, absorptionsipm, rIndexNumEntries);
	
	sipmMat->SetMaterialPropertiesTable(sipmMPT);
	
	SensitiveDetector* sipmSense = new SensitiveDetector("/sipm");
	G4SDManager* sdman = G4SDManager::GetSDMpointer();
	sdman->AddNewDetector(sipmSense);
	sipmVol->SetSensitiveDetector(sipmSense);

	/*G4LogicalVolume* scintillatorSlowVolume = parser.GetVolume("ScintSlow"); //Reads in the Slow Scintillator volume
	//In order to modify its properties.
	G4Material* scintSlowMat = scintillatorSlowVolume->GetMaterial(); //Gets the material to modify

	G4LogicalVolume* scintillatorFastVolume = parser.GetVolume("ScintFast");
	G4Material* scintFastMat = scintillatorFastVolume->GetMaterial();

	const G4int NUMENTRIESSLOW = 7; //Number of entries in the Material Properties Table
	const G4int NUMENTRIESFAST = 5;

	//This array holds the energies for the scintillation yield histogram
	G4double scntSlow_PP[NUMENTRIESSLOW] = { hc/(385)*eV, hc/(410)*eV, hc/(415)*eV, hc/(430)*eV, hc/(450)*eV, hc/(470)*eV, hc/(520)*eV };
	G4double scntFast_PP[NUMENTRIESFAST] = { hc/(380)*eV, hc/(410)*eV, hc/(420)*eV, hc/(460)*eV, hc/(500)*eV };

	//This array holds the "yield" of the scintillation light as a function of energy, so:
	//a weight of scntSlow_Fast[2] at an energy of scntSlow_PP[2]. This will have to be modified\
	//Based on the type of plastic. The _Slow and _Fast means that there are 2 different responses
	//For each scintillator (as in NaI, which has a fast and a slow component), but for the plastic
	//One should dominate (but it may depend on the plastic). So I set the slow component to be 0
	//And only put in numbers for the fast component for both the slow and the fast plastic.
	G4double scntSlow_Slow[NUMENTRIESSLOW] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0};
	G4double scntSlow_Fast[NUMENTRIESSLOW] = { 0.0, 0.9, 0.8, .55, 1.0, 0.3, 0.0};

	G4double scntFast_Fast[NUMENTRIESFAST] = { 0.0, 1.0, 0.5, 0.15, 0.0 };
	G4double scntFast_Slow[NUMENTRIESFAST] = { 0.0, 0.0, 0.0, 0.0, 0.0 };

	G4MaterialPropertiesTable* scntSlow_MPT = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable* scntFast_MPT = new G4MaterialPropertiesTable();

	const G4int rIndexNumEntries = 2; //Refractive index. So basically constant.
	G4double rIndexEnergy[rIndexNumEntries] = { 1*eV, 100*eV };
	G4double rIndex[rIndexNumEntries] = { 1.58, 1.58 };
	G4double absorption[rIndexNumEntries] = { 100.0*m, 100.0*m };

	//Add entries to the MPT.
	scntSlow_MPT->AddProperty("FASTCOMPONENT", scntSlow_PP, scntSlow_Fast, NUMENTRIESSLOW);
	scntSlow_MPT->AddProperty("SLOWCOMPONENT", scntSlow_PP, scntSlow_Slow, NUMENTRIESSLOW);
	scntSlow_MPT->AddProperty("RINDEX", rIndexEnergy, rIndex, rIndexNumEntries);
	scntSlow_MPT->AddProperty("ABSLENGTH", rIndexEnergy, absorption, rIndexNumEntries);

	scntFast_MPT->AddProperty("FASTCOMPONENT", scntFast_PP, scntFast_Fast, NUMENTRIESFAST);
	scntFast_MPT->AddProperty("SLOWCOMPONENT", scntFast_PP, scntFast_Slow, NUMENTRIESFAST);
	scntFast_MPT->AddProperty("RINDEX", rIndexEnergy, rIndex, rIndexNumEntries);
	scntFast_MPT->AddProperty("ABSLENGTH", rIndexEnergy, absorption, rIndexNumEntries);

	scntSlow_MPT->AddConstProperty("SCINTILLATIONYIELD", 6300.0/MeV); //# of photons per energy of particle
	scntSlow_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0); //Spread of # of particles
	scntSlow_MPT->AddConstProperty("FASTTIMECONSTANT",  285.0*ns); //Decay constant. Remember I only used the fast for both plastics.
	scntSlow_MPT->AddConstProperty("SLOWTIMECONSTANT", 2300.*ns);
	scntSlow_MPT->AddConstProperty("YIELDRATIO", 1.0); //Ratio of fast light to slow light (for each plastic)

	scntFast_MPT->AddConstProperty("SCINTILLATIONYIELD", 10400.0/MeV);
	scntFast_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
	scntFast_MPT->AddConstProperty("FASTTIMECONSTANT",  1.8*ns);
	scntFast_MPT->AddConstProperty("SLOWTIMECONSTANT", 2300.*ns);
	scntFast_MPT->AddConstProperty("YIELDRATIO", 1.0);

	scintSlowMat->SetMaterialPropertiesTable(scntSlow_MPT);
	scintFastMat->SetMaterialPropertiesTable(scntFast_MPT);
	
	scintSlowMat->GetIonisation()->SetBirksConstant(0.15*mm/MeV); //Birks constant.
	scintFastMat->GetIonisation()->SetBirksConstant(0.15*mm/MeV);
	
	*/
	G4LogicalVolume* worldVolume = parser.GetVolume("World"); //Set properties for air
	G4Material* airMat = worldVolume->GetMaterial();
	G4MaterialPropertiesTable* air_MPT = new G4MaterialPropertiesTable();
	G4double rIndexAir[rIndexNumEntries] = { 1.0, 1.0 };
	air_MPT->AddProperty("RINDEX", rIndexEnergy, rIndexAir, rIndexNumEntries);
	airMat->SetMaterialPropertiesTable(air_MPT);
	
	/*
	G4LogicalVolume *pmtVolume = parser.GetVolume("PMTSurface"); //Set properties for the PMT
	G4OpticalSurface* pmtOpSurface = new G4OpticalSurface("PMTOpSurface", glisur, polished, dielectric_metal);
	const G4int num = 2;
	G4double pmt_EFF[num]={0.5, 0.5};	//50% detection efficiency (for now) (This is the Quantum Efficiency
	G4double pmt_REFL[num]={0.0, 0.0};	//No reflection from PMT face (for now)
	G4MaterialPropertiesTable* pmt_MPT = new G4MaterialPropertiesTable();
	pmt_MPT->AddProperty("EFFICIENY", rIndexEnergy, pmt_EFF, num);
	pmt_MPT->AddProperty("REFLECTIVITY", rIndexEnergy, pmt_REFL, num);
	pmtOpSurface->SetMaterialPropertiesTable(pmt_MPT);
	new G4LogicalSkinSurface("PMTSurface", pmtVolume, pmtOpSurface);
	
	SensitiveDetector* lightguideSense = new SensitiveDetector("/lightguide"); //Make a new sensitive detector
	//From the lightguide volume.
	G4SDManager* sdman = G4SDManager::GetSDMpointer();
	sdman->AddNewDetector(lightguideSense);
	G4LogicalVolume *lightguideVolume = parser.GetVolume("Lightguide"); //Set lightguide properties
	lightguideVolume->SetSensitiveDetector(lightguideSense);
	G4Material* lightguideMaterial = lightguideVolume->GetMaterial();
	G4MaterialPropertiesTable* lightguideMPT = new G4MaterialPropertiesTable();
	lightguideMPT->AddProperty("RINDEX", rIndexEnergy, rIndex, rIndexNumEntries);
	lightguideMPT->AddProperty("ABSLENGTH", rIndexEnergy, absorption, rIndexNumEntries);
	lightguideMaterial->SetMaterialPropertiesTable(lightguideMPT);*/
	
	return parser.GetWorldVolume(); //Return the world volume as the volume to be simulated.
}

DetectorConstruction::~DetectorConstruction() {}
