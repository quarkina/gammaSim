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
// Andrea gutierrez - Delft University of Technology
// read and handle phase-space files 
// Feb 2018
////////////////////////////////////////////////////////////////////////
// 

#include <TFile.h>
#include <TTree.h>

#include "phspParticle.hh"
#include "phspMessenger.hh"

#include "HistoManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip>
#include <string> 
#include <sstream>

#include "G4UImanager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


phspParticle* phspParticle::fphsp = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

phspParticle* phspParticle::GetphspPointer()
{
  if(!fphsp) {
    static phspParticle phsp;
    fphsp = &phsp;
  }
  return fphsp;
}



phspParticle::phspParticle()
{
  fphspMessenger = new phspMessenger(this);
  fNbParticles = 0;
  fMaxPart = 1000000000;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

phspParticle::~phspParticle()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void phspParticle::ReadFile()
{ 
  std::ifstream infileData;
  std::ofstream logfile;

   std::ostringstream strs;
  strs << fDetAng/deg;
  std::string str = strs.str();

  fphspLogFilename ="data/";
  fphspLogFilename += fphspOutputFilename;
  fphspLogFilename +="_";
  fphspLogFilename +=str;
  fphspLogFilename +="deg";

  if(fIsAbsorber){
    fphspLogFilename +="_Absorber";
  }

  fphspLogFilename +=".txt";
  logfile.open (fphspLogFilename,std::fstream::app);
  G4cout << "logfile: " << fphspLogFilename << G4endl;

  fNbParticles = 0;
  
  // picking a random number -> filename
  G4int randNumber;
  
  randNumber = (G4int) (1+G4UniformRand()*(fNbAvailFiles-1));
  G4String baseFile = fphspFilename;
      
  std::ostringstream oss;
  oss<< randNumber;
  baseFile+=oss.str();
  baseFile+=".root";
 
  G4cout << "Reading" << baseFile << " ... " << G4endl;
  logfile << baseFile << "\t";
  
  TFile *f = new TFile(baseFile);
  if (fIsGamma == true){
    tparticle = (TTree*)f->Get("gamma");
    logfile << "gamma " << tparticle->GetEntries() << "\n";
  }
  else if (fIsGamma == false){
    tparticle = (TTree*)f->Get("neutron");
    logfile << "neutron " << tparticle->GetEntries() << "\n";
  }
  fNbParticles =  tparticle->GetEntries();
 
  logfile.close();
  std::ostringstream os;
  os << fNbParticles;
 
  if (fIsGamma == true)
    G4UImanager::GetUIpointer()->ApplyCommand("/gun/particle gamma");
  else if (fIsGamma == false)
    G4UImanager::GetUIpointer()->ApplyCommand("/gun/particle neutron");

  G4String copCmd = "/run/beamOn " + os.str();
  G4cout << copCmd << G4endl;
  G4UImanager::GetUIpointer()->ApplyCommand(copCmd);
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int phspParticle::GetNbParticle(){
  return fNbParticles;
}

void phspParticle::SetFilename(const std::string& name) 
{
  fphspFilename = name;
}

void phspParticle::SetOutputFilename(const std::string& name) 
{
  fphspOutputFilename = name;
}

void phspParticle::SetLogFilename(const std::string& name) 
{
  fphspLogFilename = name;
}

void phspParticle::SetIsGamma(G4bool isGamma) 
{
  fIsGamma = isGamma;
}

G4bool phspParticle::GetIsGamma() 
{
  return fIsGamma;
}

void phspParticle::SetMaxPart(G4int nbPart) 
{
  fMaxPart = nbPart;
}

void phspParticle::SetNbFiles(G4int nbFiles) 
{
  fNbFiles = nbFiles;
}

G4int phspParticle::GetNbFiles() 
{
  return fNbFiles;
}

void phspParticle::SetNbAvailFiles(G4int nbFiles) 
{
  fNbAvailFiles = nbFiles;
}

std::string phspParticle::GetFilename() 
{
  return fphspFilename;
}

std::string phspParticle::GetOutputFilename() 
{
  return fphspOutputFilename;
}

void phspParticle::SetDetAng(G4double ang) 
{
  fDetAng = ang;
}

G4double phspParticle::GetDetAng() 
{
  return fDetAng;
}

void phspParticle::SetIsAbsorber(G4bool isAbs) 
{
  fIsAbsorber = isAbs;
}

G4bool phspParticle::GetIsAbsorber() 
{
  return fIsAbsorber;
}

TTree * phspParticle::GetParticleTree() {

  return tparticle;

}
