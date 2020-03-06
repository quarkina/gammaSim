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
/// \file hadronic/Hadr01/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 101905 2016-12-07 11:34:39Z gunter $
//
/////////////////////////////////////////////////////////////////////////
//
// DetectorConstruction
//
// Created: 31.01.2003 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of Hadr01 (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
// 

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

#include "DetectorSD.hh"

#include "G4SDManager.hh"
#include "HistoManager.hh"
#include "G4NistManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "phspParticle.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fWorldMaterial(0),
   fLogicWorld(0),
   fLogicSlit(0),
   fDetectorMessenger(0)
{
  fDetectorMessenger = new DetectorMessenger(this);
  
  fWorldR = 5.*cm;
  fRadius = 10.*cm;
  fWorldMaterial = 
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  ConstructMaterials();
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ 
  delete fDetectorMessenger;
}

void DetectorConstruction::ConstructMaterials()
{
  G4NistManager* man = G4NistManager::Instance();
  Lutetium = man->FindOrBuildElement("Lu");
  Yttrium  = man->FindOrBuildElement("Y");
  Oxygen   = man->FindOrBuildElement("O");
  Cerium   = man->FindOrBuildElement("Ce");
  Silicon  = man->FindOrBuildElement("Si");
  Tungsten = man->FindOrBuildMaterial("G4_W");
  
  
  Boron = man->FindOrBuildMaterial("G4_B");
  Nitride = man->FindOrBuildMaterial("G4_N");
  
  BN = new G4Material("BN",2.3*g/cm3,(G4int) 2);
  BN->AddMaterial(Boron, 50.0*perCent);
  BN->AddMaterial(Nitride, 50.0*perCent);
  
  LYSO = new G4Material("LYSO",7.1*g/cm3,(G4int) 5);
  LYSO->AddElement(Lutetium,71.43*perCent);
  LYSO->AddElement(Yttrium,4.03*perCent);
  LYSO->AddElement(Silicon,6.37*perCent);
  LYSO->AddElement(Oxygen,18.14*perCent);
  LYSO->AddElement(Cerium,0.02*perCent);
  
  LYSO->GetIonisation()->SetBirksConstant((0.0076*g/(MeV*cm2))/(7.4*g/cm3));
  
  
  //Define Oxygen
  G4double A = 16.0 * g/mole;
  G4double Z = 8;
  G4Element* elO = new G4Element ("Oxygen", "O", Z, A);
  
  //Define Hydrogen 
  A = 1.01 * g/mole;
  Z = 1;
  G4Element* elH = new G4Element ("Hydrogen", "H", Z, A);
  
  //Define Carbon
  A = 12.01 * g/mole;
  Z = 6;
  G4Element* elC = new G4Element ("Carbon", "C", Z, A);
  
  //PMMA: 
  PMMA = new G4Material("PMMA", 1.19*g/cm3, 3);
  PMMA -> AddElement(elC, 5);
  PMMA -> AddElement(elO, 2);
  PMMA -> AddElement(elH, 8);
  
  
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Sizes
  G4double worldR  = fWorldR + 50*cm;
  G4double targetZ = HistoManager::GetPointer()->Length()*0.5; 
  G4double worldZ  = targetZ + 50*cm;
  G4cout << "Radius: " << fRadius << G4endl;

  // World
  G4Box* solidW = new G4Box("World",worldR,worldR,worldZ);
  fLogicWorld = new G4LogicalVolume( solidW,fWorldMaterial,"World");
  G4VPhysicalVolume* world = new G4PVPlacement(0,G4ThreeVector(),
                                       fLogicWorld,"World",0,false,0);

  // Target volume
  G4Tubs* solidA = new G4Tubs("Target",0.,fRadius,targetZ,0.,twopi);
  G4LogicalVolume* fLogicTarget = new G4LogicalVolume(solidA,PMMA,"Target");

  new G4PVPlacement(0,G4ThreeVector(0.0,0.0,targetZ),fLogicTarget,"Target",
		    fLogicWorld,false,0);
  
  //G4double origin    = 50*mm; //origin from protonSim
  //G4double distSys   = 0*mm; //lateral distance from origin (set to 10*mm)
  
  G4double gapColDet = 5*mm; //5 mm gap between collimator and detector

  G4double scintThickness = 1*mm;//22mm
  G4double scintSide      = 200*mm;//64mm
  G4double scintLong      = 400*mm;

  G4double colThickness = 150*mm;//150 mm
  G4double colSide      = 1.5*mm;
  G4double colGap       = 2.5*mm;

  G4double scintPixSize = 4*mm;

  G4double absThickness = 50*mm;//50 mm
  G4double modThickness = 50*mm; //50 mm
  
  //Scintillator pixel:  
  G4Box* scint = new G4Box("Scint",scintThickness/2.,scintPixSize/2.,scintPixSize/2.);
  fLogicScint  = new G4LogicalVolume(scint, LYSO,"Scint");
  G4int copy = 0;

  //collimator

  G4Box* slit = new G4Box("Slit",colThickness/2.,scintSide/2.,colSide/2.);
  fLogicSlit  = new G4LogicalVolume(slit,Tungsten,"Slit");
  G4int itr = 0;

  G4Box* absorber = new G4Box("Absorber",absThickness/2.,scintSide/2.,colSide/2.);
  fLogicAbsorber = new G4LogicalVolume(absorber,PMMA,"Absorber");
  G4int itrAbs = 0;
  
  G4double og = 300*mm;
  G4double ogCol = og - scintThickness/2.0 - gapColDet - colThickness/2.;
  G4double ogAbs = og - scintThickness/2.0 - gapColDet - colThickness - gapColDet - absThickness/2.;

  G4double phi = phspParticle::GetphspPointer()->GetDetAng();//0*deg;
  
  G4RotationMatrix rotm1  = G4RotationMatrix();
  rotm1.rotateY(phi);
  G4ThreeVector yaxis = G4ThreeVector(0,1,0);
  
  //scintillator loop
  for(G4double ypos=(-1*scintSide/2. + scintPixSize/2.);ypos<scintSide/2.;ypos+=scintPixSize){
    for(G4double zpos=0;zpos<scintLong;zpos+=scintPixSize){
      
      G4ThreeVector w0 = G4ThreeVector(1,0,0)*(og+scintThickness/2.) + G4ThreeVector(0,ypos,zpos);
      G4ThreeVector position1 = w0.rotate(phi, yaxis);
      G4Transform3D transform1 = G4Transform3D(rotm1,position1);
      new G4PVPlacement(transform1,fLogicScint, "Scint",fLogicWorld, false, copy);
      //G4cout <<copy << "\t" << og << "\t" << ypos << "\t" << zpos-75*mm << G4endl;
      copy++;    
    }
  }  
  
  //collimator loop
  /*
  for(G4double zpos=0;zpos<scintLong+scintPixSize;zpos+=scintPixSize){
    G4ThreeVector wCol = G4ThreeVector(1,0,0)*ogCol + G4ThreeVector(0,0,zpos-scintPixSize/2.);
    G4ThreeVector positionCol = wCol.rotate(phi, yaxis);
    G4Transform3D transformCol = G4Transform3D(rotm1,positionCol);
    new G4PVPlacement(transformCol,fLogicSlit, "Slit",fLogicWorld, false, itr);    
    itr++;
  }
  */    

  //place absorber if true.
  if(phspParticle::GetphspPointer()->GetIsAbsorber()){
    //absorber loop
    for(G4double zpos=0;zpos<scintLong+scintPixSize;zpos+=scintPixSize){
      G4ThreeVector wAbs = G4ThreeVector(1,0,0)*ogAbs + G4ThreeVector(0,0,zpos-scintPixSize/2.);
      G4ThreeVector positionAbs = wAbs.rotate(phi, yaxis);
      G4Transform3D transformAbs = G4Transform3D(rotm1,positionAbs);
      new G4PVPlacement(transformAbs,fLogicAbsorber, "Absorber",fLogicWorld, false, itrAbs);    
      itrAbs++;
    }
  }

  /*
  for(G4double zpos=( -1*scintPixSize/2.);zpos<scintLong+scintPixSize;zpos+=scintPixSize){
    G4ThreeVector wMod = G4ThreeVector(1,0,0)*ogMod + G4ThreeVector(0,0,zpos-75*mm-scintPixSize/2.);   
    G4ThreeVector positionMod = wMod.rotate(phi, yaxis);
    G4Transform3D transformMod = G4Transform3D(rotm1,positionMod);
    new G4PVPlacement(transformMod,fLogicModerator, "Moderator",fLogicWorld, false, itrMod);    
    itrMod++;
  }
  */
  
  return world;
  
}

void DetectorConstruction::ConstructSDandField()
{
  
    static G4ThreadLocal G4bool initialized = false;
    if ( ! initialized ) {
        // Prepare sensitive detectors
	DetectorSD* fDetectorSD = new DetectorSD("DetectorSD");
	(G4SDManager::GetSDMpointer())->AddNewDetector( fDetectorSD );
        fLogicScint->SetSensitiveDetector(fDetectorSD);
        initialized=true;
    }
 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(const G4String& mat)
{
  // search the material by its name
  G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial(mat);

  if (material && material != fWorldMaterial) {
    fWorldMaterial = material;
    if(fLogicWorld) { fLogicWorld->SetMaterial(fWorldMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetRadius(G4double val)  
{
  if(val > 0.0) {
    fRadius = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}
