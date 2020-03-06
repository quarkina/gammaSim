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
/// \file hadronic/Hadr01/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// $Id: HistoManager.cc 107541 2017-11-22 08:24:57Z gcosmo $
//
//---------------------------------------------------------------------------
//
// ClassName:   HistoManager
//
//
// Author:      V.Ivanchenko 30/01/01
//
// Modified:
// 04.06.2006 Adoptation of Hadr01 (V.Ivanchenko)
// 16.11.2006 Add beamFlag (V.Ivanchenko)
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "HistoManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4UnitsTable.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonZeroShort.hh"
#include "G4KaonZeroLong.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "Histo.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4HadronicProcess.hh"
#include "phspParticle.hh"
#include <fstream>
#include <iostream>
#include <sstream>

#include <stdio.h>
#include <time.h>
#define LOGNAME_FORMAT "data/%Y%m%d_%H%M%S"
#define LOGNAME_SIZE 25

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager* HistoManager::fManager = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager* HistoManager::GetPointer()
{
  if(!fManager) {
    static HistoManager manager;
    fManager = &manager;
  }
  return fManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager::HistoManager()
: fPrimaryDef(0),
  fEdepMax(1.0*GeV),
  fLength (300.*mm),
  fPrimaryKineticEnergy(0.0),  
  fVerbose(0),  
  fNBinsE (100),
  fNSlices(300),
  fNHisto (25),
  fBeamFlag(true),
  fHistoBooked(false)
{
  fHisto     = new Histo();
  fHisto->SetVerbose(fVerbose);
  BookHisto();
  fNeutron   = G4Neutron::Neutron();
  fGamma   = G4Gamma::Gamma();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager::~HistoManager()
{
  delete fHisto;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::BookHisto()
{
  fHistoBooked = true;
  fHisto->Add1D("1","Energy deposition (MeV/mm/event) in the target",
               fNSlices,0.0,fLength/mm,MeV/mm);
  fHisto->Add1D("2","Log10 Energy (MeV) of gammas",fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("3","Log10 Energy (MeV) of electrons",fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("4","Log10 Energy (MeV) of positrons",fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("5","Log10 Energy (MeV) of protons",fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("6","Log10 Energy (MeV) of neutrons",fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("7","Log10 Energy (MeV) of charged pions",fNBinsE,-4.,6.,1.0);
  fHisto->Add1D("8","Log10 Energy (MeV) of pi0",fNBinsE,-4.,6.,1.0);
  fHisto->Add1D("9","Log10 Energy (MeV) of charged kaons",fNBinsE,-4.,6.,1.0);
  fHisto->Add1D("10","Log10 Energy (MeV) of neutral kaons",fNBinsE,-4.,6.,1.0);
  fHisto->Add1D("11","Log10 Energy (MeV) of deuterons and tritons",
                fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("12","Log10 Energy (MeV) of He3 and alpha",fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("13","Log10 Energy (MeV) of Generic Ions",fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("14","Log10 Energy (MeV) of muons",fNBinsE,-4.,6.,1.0);
  fHisto->Add1D("15","log10 Energy (MeV) of side-leaked neutrons",
                fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("16","log10 Energy (MeV) of forward-leaked neutrons",
                fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("17","log10 Energy (MeV) of backward-leaked neutrons",
                fNBinsE,-5.,5.,1.0);
  fHisto->Add1D("18","log10 Energy (MeV) of leaking protons",
                fNBinsE,-4.,6.,1.0);
  fHisto->Add1D("19","log10 Energy (MeV) of leaking charged pions",
                fNBinsE,-4.,6.,1.0);
  fHisto->Add1D("20","Log10 Energy (MeV) of pi+",fNBinsE,-4.,6.,1.0);
  fHisto->Add1D("21","Log10 Energy (MeV) of pi-",fNBinsE,-4.,6.,1.0);
  fHisto->Add1D("22",
                "Energy deposition in the target normalized to beam energy",
                110,0.0,1.1,1.0);
  fHisto->Add1D("23",
                "EM energy deposition in the target normalized to beam energy",
                110,0.0,1.1,1.0);
  fHisto->Add1D("24",
               "Pion energy deposition in the target normalized to beam energy",
                110,0.0,1.1,1.0);
  fHisto->Add1D("25",
             "Proton energy deposition in the target normalized to beam energy",
                110,0.0,1.1,1.0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::BeginOfRun()
{
  fEventID = 0;
  fID = 0;

  static char name[LOGNAME_SIZE];
  
  time_t now = time(0);
  strftime(name, sizeof(name), LOGNAME_FORMAT, localtime(&now));

  fNeutronFile +=name;
  fNeutronFile +="_NeutronData.dat";

  fGammaFile +=name;
  fGammaFile +="_GammaData.dat";



  std::ostringstream strs;
  strs << phspParticle::GetphspPointer()->GetDetAng()/deg;
  std::string str = strs.str();


  fDetectorFile ="data/";
  fDetectorFile += phspParticle::GetphspPointer()->GetOutputFilename();
  fDetectorFile +="_";
  fDetectorFile +=str;
  fDetectorFile +="deg";
 
  fEnergyFile ="data/";
  fEnergyFile += phspParticle::GetphspPointer()->GetOutputFilename();
  fEnergyFile +="_";
  fEnergyFile +=str;
  fEnergyFile +="deg";

  fPerfectDetFile ="data/";
  fPerfectDetFile += phspParticle::GetphspPointer()->GetOutputFilename();
  fPerfectDetFile +="_";
  fPerfectDetFile +=str;
  fPerfectDetFile +="deg";

  if(phspParticle::GetphspPointer()->GetIsAbsorber()){
    fDetectorFile +="_Absorber";
    fEnergyFile +="_Absorber";
  }

  
  if(phspParticle::GetphspPointer()->GetIsGamma()){
    fDetectorFile +="_Gamma_DetectorData.dat";
    fEnergyFile +="_Gamma_EnergyData.dat";
    fPerfectDetFile +="_Gamma_PerfectDetData.dat";
  }
  else {
    fDetectorFile +="_Neutron_DetectorData.dat";
    fEnergyFile +="_Neutron_EnergyData.dat";
    fPerfectDetFile +="_Neutron_PerfectDetData.dat";
  }
  
  fGammaCounter = 0;
  fDetCounter = 0;
  fDetE        = 0;
  fAbsZ0       = -0.5*fLength;
  fNevt       = 0;
  fNelec      = 0;
  fNposit     = 0;
  fNgam       = 0;
  fNstep      = 0;
  fNGamma_leak = 0;
  fNpiofNleak = 0;
  fNions      = 0;
  fNdeut      = 0;
  fNalpha     = 0;
  fNkaons     = 0;
  fNmuons     = 0;
  fNcpions    = 0;
  fNpi0       = 0;
  fNneutron   = 0;
  fNproton    = 0;
  fNaproton   = 0;
  fNneu_forw  = 0;
  fNneu_leak  = 0;
  fNneu_back  = 0;

  fEdepSum     = 0.0;
  fEdepSum2    = 0.0;

  if(!fHistoBooked) { BookHisto(); }
  fHisto->Book();

  if(fVerbose > 0) {
    G4cout << "HistoManager: Histograms are booked and run has been started"
           <<G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::EndOfRun()
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::BeginOfEvent()
{
  fEdepEvt = 0.0;
  fEdepEM  = 0.0;
  fEdepPI  = 0.0;
  fEdepP   = 0.0;
  fDetE    = 0.0;
  fEventID ++;
  // G4cout << "Event: " << fEventID << G4endl;
  fDet = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::EndOfEvent()
{
  fEdepSum  += fEdepEvt;
  fEdepSum2 += fEdepEvt*fEdepEvt;
  fHisto->Fill(21,fEdepEvt/fPrimaryKineticEnergy,1.0);
  fHisto->Fill(22,fEdepEM/fPrimaryKineticEnergy,1.0);
  fHisto->Fill(23,fEdepPI/fPrimaryKineticEnergy,1.0);
  fHisto->Fill(24,fEdepP/fPrimaryKineticEnergy,1.0);

 
  fDetCounter++;
  std::ofstream fileDetector;
  fileDetector.open (fDetectorFile,std::fstream::app);

  std::ofstream fileEnergy;
  fileEnergy.open (fEnergyFile,std::fstream::app);

  if(fDetE > 0){
    fileEnergy << std::setprecision(6) << fDetE << "\t" << fPrimaryKineticEnergy << "\n";
  }
  fileEnergy.close();

  //std::cout << fPrimaryKineticEnergy << std::endl;
  if(fhitVec.size()>0){
    for (size_t i = 0;i<fhitVec.size();i++){
      fileDetector << fID << "\t" << forgVolVec[i] << "\t" <<  fPrimaryKineticEnergy << "\t" <<  fdepVec[i] << "\t" <<  fhitVec[i] << "\t" << fdetTimeVec[i] << "\n";    
      //std::cout << fID <<  " det time: " << fdetTimeVec[i]  << std::endl;
    }
    fileDetector.close(); 
  }
  
  fhitVec.clear();
  fdepVec.clear();
  fparentVec.clear();
  fnameVec.clear();
  forgVolVec.clear();
  fdetTimeVec.clear();


  std::ofstream filePerfectDet;
  filePerfectDet.open (fPerfectDetFile,std::fstream::app);

  if(fDet == true){
    filePerfectDet << fPrimaryKineticEnergy  << "\t" << fTrackID <<"\n";
  }

  filePerfectDet.close();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::ScoreNewTrack(const G4Track* track)
{
  const G4ParticleDefinition* pd = track->GetDefinition();
  G4String name = pd->GetParticleName();
  G4double e = track->GetKineticEnergy();
  if(0 == track->GetParentID()) {
    fNevt++;
    fPrimaryKineticEnergy = e;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::AddTargetStep(const G4Step* step)
{
 
}

void HistoManager::AddParticleDetector(const G4Track* track)
{
  if (fDet == false){
    G4cout <<"event: " << fEventID << "\t track id: " << track->GetTrackID() << " energy: " << track->GetKineticEnergy() << " total energy: " << track->GetTotalEnergy() << " primary energy: " << fPrimaryKineticEnergy << G4endl;
    fTrackID = track->GetTrackID();
  }
  fDet = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::AddEnergyDepositDetector(const G4Step* aStep)
{
  const G4Track* track = aStep->GetTrack();

  G4double edep   = aStep->GetTotalEnergyDeposit();
  G4double copyNo = track->GetVolume()->GetCopyNo();
  G4int parent    = track->GetParentID();
  G4String name   = track->GetParticleDefinition()->GetParticleName();
  G4double time   = track->GetGlobalTime();
  
  const G4String orgVol = track->GetOriginTouchable()->GetVolume()->GetName();
  
  G4bool rec = false;
  
  for (size_t i = 0;i<fhitVec.size();i++){
    //if((fhitVec[i]==copyNo) && (fparentVec[i] == parent) && (fnameVec[i] == name) && (forgVolVec[i] == orgVol)){
    if(fhitVec[i]==copyNo){
      fdepVec[i]+=edep;
      rec = true;
    }
  }
  
  if(rec == false){
    fID++;
    fdepVec.push_back(edep);
    fhitVec.push_back(copyNo);
    fparentVec.push_back(parent);
    fnameVec.push_back(name);
    forgVolVec.push_back(orgVol);
    fdetTimeVec.push_back(time);
  }

  fDetE+=edep;
}

void HistoManager::CountGammaParticle(const G4Track* track)
{
  fGammaCounter++;
  
}

void HistoManager::AddLeakingParticle(const G4Track* track)
{
  const G4ParticleDefinition* pd = track->GetDefinition(); 
  //  G4double e = std::log10(track->GetKineticEnergy()/MeV);
  G4double e = track->GetKineticEnergy()/MeV;

  G4ThreeVector pos = track->GetPosition();
  G4ThreeVector dir = track->GetMomentumDirection();
  G4double x = pos.x();
  G4double y = pos.y();
  G4double z = pos.z();
 
  G4bool isLeaking = false;
  
  // Forward 
  if(z > -fAbsZ0 && dir.z() > 0.0) {
    isLeaking = true;
    if(pd == fNeutron) {
      ++fNneu_forw;
      // fHisto->Fill(15,e,1.0);
    } else isLeaking = true;

    // Backward
  } else if (z < fAbsZ0 && dir.z() < 0.0) {
    isLeaking = true;
    if(pd == fNeutron) {
      ++fNneu_back;
      //fHisto->Fill(16,e,1.0);
    } else isLeaking = true;

    // Side
  } else if (std::abs(z) <= -fAbsZ0 && x*dir.x() + y*dir.y() > 0.0) {
    isLeaking = true;
    if(pd == fNeutron) {
      ++fNneu_leak;
      //fHisto->Fill(14,e,1.0);
    } else isLeaking = true;
  }

  // Gamma and pions
  if(isLeaking) {
    if(pd == G4Gamma::Gamma()) {
      //fHisto->Fill(17,e,1.0);
      ++fNGamma_leak;
    } else if (pd == G4PionPlus::PionPlus() || 
               pd == G4PionMinus::PionMinus()) {
      //fHisto->Fill(18,e,1.0);
      ++fNpiofNleak;
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::SetVerbose(G4int val)        
{
  fVerbose = val; 
  fHisto->SetVerbose(val);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::Fill(G4int id, G4double x, G4double w)
{
  fHisto->Fill(id, x, w);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

