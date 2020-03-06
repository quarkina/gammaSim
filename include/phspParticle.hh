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
////////////////////////////////////////////////////////////////////////
// 

#ifndef phspParticle_h
#define phspParticle_h 1
#include <string> 
#include "globals.hh"
#include "G4ThreeVector.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class TFile;
class TTree;
class phspMessenger;

class phspParticle
{
public: // Without description

  phspParticle();
  virtual ~phspParticle();

  virtual void ReadFile();
  virtual G4int GetNbParticle();
  virtual TTree * GetParticleTree();
  
  void SetMaxPart(G4int); 
  void SetNbFiles(G4int);
  G4int GetNbFiles(); 
  void SetNbAvailFiles(G4int); 
  void SetFilename(const std::string&);
  void SetOutputFilename(const std::string&);
  void SetLogFilename(const std::string&);
  void SetIsGamma(G4bool);
  G4bool GetIsGamma();

  void SetIsAbsorber(G4bool);
  G4bool GetIsAbsorber();

  void SetDetAng(G4double);
  G4double GetDetAng();

  std::string GetFilename();
  std::string GetOutputFilename();

  static phspParticle* GetphspPointer();
  
private:

  const G4String* fileName;

  std::vector<G4ThreeVector> myPartPosition;
  std::vector<G4ThreeVector> myPartMomentum;

  std::vector<G4double> myPartEnergy;
  std::vector<G4double> myPartGlobalTime;

  TTree* tparticle;
 
  std::string fphspFilename;
  std::string fphspOutputFilename;
  std::string fphspLogFilename;

  phspMessenger* fphspMessenger;

  G4int fNbParticles;
  G4int fMaxPart;
  G4bool fIsGamma;
  G4bool fIsAbsorber;
  G4int fNbFiles;
  G4int fNbAvailFiles;
  G4double fDetAng;
  static phspParticle* fphsp;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif

