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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "phspMessenger.hh"

#include <sstream>

#include "phspParticle.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UImanager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

phspMessenger::phspMessenger(phspParticle* phsp)
  : G4UImessenger(), fphsp(phsp)
{

  fPhspDir = new G4UIdirectory("/phsp/");
  fPhspDir->SetGuidance("Phase-space control");
 
  fFileCmd = new G4UIcmdWithAString("/phsp/filename",this);
  fFileCmd->SetGuidance("set input file name");

  fLogFileCmd = new G4UIcmdWithAString("/phsp/logFile",this);
  fLogFileCmd->SetGuidance("set output logfile name");

  fNbPartCmd = new G4UIcmdWithAnInteger("/phsp/NbParticles",this);
  fNbPartCmd->SetGuidance("set max Num particles");

  fIsGammaCmd = new G4UIcmdWithABool("/phsp/isGamma",this);
  fIsGammaCmd->SetGuidance("is it a gamma? otherwise neutron");
  fIsGammaCmd->SetDefaultValue(true);
  
  fNbFilesCmd = new G4UIcmdWithAnInteger("/phsp/NbFiles",this);
  fNbFilesCmd->SetGuidance("set max Num files");
  
  fNbAvailFilesCmd = new G4UIcmdWithAnInteger("/phsp/NbAvailableFiles",this);
  fNbAvailFilesCmd->SetGuidance("number of available files in base directory");

  fAngCmd = new G4UIcmdWithADoubleAndUnit("/phsp/detAng",this);
  fAngCmd->SetGuidance("Set rotation of the detector");
  fAngCmd->SetParameterName("angle",false);
  fAngCmd->SetUnitCategory("Angle");
  fAngCmd->SetDefaultValue(0.0);
  fAngCmd->SetDefaultUnit("deg");
  fAngCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fOutputFileCmd = new G4UIcmdWithAString("/phsp/outputfilename",this);
  fOutputFileCmd->SetGuidance("set output file name");

  fIsAbsorberCmd = new G4UIcmdWithABool("/phsp/isAbsorber",this);
  fIsAbsorberCmd->SetGuidance("True if there is an absorber structure");
  fIsAbsorberCmd->SetDefaultValue(false);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

phspMessenger::~phspMessenger()
{
  delete fFileCmd;
  delete fNbPartCmd;
  delete fPhspDir;
  delete fNbFilesCmd;
  delete fNbAvailFilesCmd;
  delete fIsGammaCmd;
  delete fIsAbsorberCmd;
  delete fAngCmd;
  delete fLogFileCmd;
  delete fOutputFileCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void phspMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == fIsGammaCmd) { 
    fphsp->SetIsGamma(fIsGammaCmd->GetNewBoolValue(newValues));
  }
  
  else if (command == fNbPartCmd) { 
    fphsp->SetMaxPart(fNbPartCmd->GetNewIntValue(newValues));
  }

  else if (command == fNbFilesCmd) { 
    fphsp->SetNbFiles(fNbFilesCmd->GetNewIntValue(newValues));
  }

  else if (command == fNbAvailFilesCmd) { 
    fphsp->SetNbAvailFiles(fNbAvailFilesCmd->GetNewIntValue(newValues));
  }
  
  else if (command == fFileCmd) { fphsp->SetFilename(newValues);
    G4UImanager::GetUIpointer()->ApplyCommand("/run/initialize ");
    for (G4int i=0;i<fphsp->GetNbFiles();i++){
      fphsp->ReadFile();
    }
  }

  else if (command == fLogFileCmd) { fphsp->SetLogFilename(newValues);
  }

  else if (command == fAngCmd) { fphsp->SetDetAng(fAngCmd->GetNewDoubleValue(newValues));   
  }

  else if (command == fOutputFileCmd) { fphsp->SetOutputFilename(newValues);
  }

  else if (command == fIsAbsorberCmd) { 
    fphsp->SetIsAbsorber(fIsAbsorberCmd->GetNewBoolValue(newValues));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
