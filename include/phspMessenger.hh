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
/// \file hadronic/Hadr01/include/HistoMessenger.hh
/// \brief Definition of the HistoMessenger class
//
// $Id: HistoMessenger.hh 107541 2017-11-22 08:24:57Z gcosmo $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef phspMessenger_h
#define phspMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class phspParticle;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class phspMessenger: public G4UImessenger
{
public:

  phspMessenger(phspParticle* );
  virtual ~phspMessenger();
  virtual void SetNewValue(G4UIcommand* ,G4String );

private:

  phspParticle*           fphsp;
  G4UIdirectory*          fPhspDir;   
  G4UIcmdWithAString*     fFileCmd;
  G4UIcmdWithAString*     fLogFileCmd;
  G4UIcmdWithAnInteger*   fNbPartCmd;
  G4UIcmdWithAnInteger*   fNbFilesCmd;
  G4UIcmdWithAnInteger*   fNbAvailFilesCmd;
  G4UIcmdWithABool*       fIsGammaCmd;
  G4UIcmdWithABool*       fIsAbsorberCmd;
  G4UIcmdWithADoubleAndUnit*       fAngCmd;
  G4UIcmdWithAString*     fOutputFileCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
