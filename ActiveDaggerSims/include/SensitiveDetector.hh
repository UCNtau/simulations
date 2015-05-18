#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "Analysis.hh"

class G4Step;

class SensitiveDetector : public G4VSensitiveDetector
{
	public:
		SensitiveDetector(G4String SDname);
		~SensitiveDetector();

	public:
		/// Mandatory base class method : it must to be overloaded:
		G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);

		void Initialize(G4HCofThisEvent* HCE);
		void EndOfEvent(G4HCofThisEvent* HCE);

	private:
		Analysis* Analyzer;
};

#endif
