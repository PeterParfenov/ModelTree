#include <iostream>

#include "Track.h"
#include "TRandom.h"

ClassImp(Track);

Track::Track(const Track &orig) : TObject(orig), fTriggerBits(orig.fTriggerBits)
{
  fPdg = orig.fPdg;
  fPvect[0] = orig.fPvect[0];
  fPvect[1] = orig.fPvect[1];
  fPvect[2] = orig.fPvect[2];
  fPvect[3] = orig.fPvect[3];
  fMass = orig.fMass;
  fCharge = orig.fCharge;
  fBaryon = orig.fBaryon;
  fStrange = orig.fStrange;
  fDiag1 = orig.fDiag1;
  fDiag2 = orig.fDiag2;
  fTform = orig.fTform;

  fVertex[0] = orig.fVertex[0];
  fVertex[1] = orig.fVertex[1];
  fVertex[2] = orig.fVertex[2];
  fVertex[3] = orig.fVertex[3];
}

////////////////////////////////////////////////////////////////////////////////
/// Create a track object.

Track::Track(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
             Float16_t mass, Int_t baryon, Int_t strange, Int_t charge,
             Double32_t t, Double32_t vx, Double32_t vy, Double32_t vz,
             Double32_t tform, Int_t diag1, Int_t diag2)
{
  Set(pdg, E, px, py, pz, mass, baryon, strange, charge, t, vx, vy, vz, tform, diag1, diag2);
}

////////////////////////////////////////////////////////////////////////////////
/// Copy a track

Track &Track::operator=(const Track &orig)
{
  TObject::operator=(orig);
  fPdg = orig.fPdg;
  fPvect[0] = orig.fPvect[0];
  fPvect[1] = orig.fPvect[1];
  fPvect[2] = orig.fPvect[2];
  fPvect[3] = orig.fPvect[3];
  fMass = orig.fMass;
  fCharge = orig.fCharge;
  fBaryon = orig.fBaryon;
  fStrange = orig.fStrange;
  fDiag1 = orig.fDiag1;
  fDiag2 = orig.fDiag2;
  fTform = orig.fTform;

  fVertex[0] = orig.fVertex[0];
  fVertex[1] = orig.fVertex[1];
  fVertex[2] = orig.fVertex[2];
  fVertex[3] = orig.fVertex[3];

  fTriggerBits = orig.fTriggerBits;

  return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// Note that we intend on using TClonesArray::ConstructedAt, so we do not
/// need to delete any of the arrays.

void Track::Clear(Option_t * /*option*/)
{
  TObject::Clear();
  fTriggerBits.Clear();
}

////////////////////////////////////////////////////////////////////////////////
/// Set the values of the Track data members.

void Track::Set(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
                Float16_t mass, Int_t baryon, Int_t strange, Int_t charge,
                Double32_t t, Double32_t vx, Double32_t vy, Double32_t vz,
                Double32_t tform, Int_t diag1, Int_t diag2)
{
  fPdg = pdg;
  fPvect[0] = E;
  fPvect[1] = px;
  fPvect[2] = py;
  fPvect[3] = pz;
  fMass = mass;
  fCharge = charge;
  fBaryon = baryon;
  fStrange = strange;
  fDiag1 = diag1;
  fDiag2 = diag2;
  fTform = tform;

  fVertex[0] = t;
  fVertex[1] = vx;
  fVertex[2] = vy;
  fVertex[3] = vz;
}

////////////////////////////////////////////////////////////////////////////////

void Track::Print(Option_t *option)
{
  // Print the data members to the standard output
  std::cout << "------------------------------------------------" << std::endl
            << "-I-                   Track                  -I-" << std::endl
            << "PDG code                         : " << fPdg << std::endl
            << "Baryon                           : " << fBaryon << std::endl
            << "Strange                          : " << fStrange << std::endl
            << "Charge                           : " << fCharge << std::endl
            << "Mass                             : " << fMass << std::endl
            << "Diag1                            : " << fDiag1 << std::endl
            << "Diag2                            : " << fDiag2 << std::endl
            << "Tform                            : " << fTform << std::endl
            << "Momentum {E, px, py, pz} (GeV/c) : {" << fPvect[0] << ", " << fPvect[1] << ", " << fPvect[2] << ", " << fPvect[3] << "}" << std::endl
            << "Position {t, x, y, z} (fm)       : {" << fVertex[0] << ", " << fVertex[1] << ", " << fVertex[2] << ", " << fVertex[3] << "}" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////