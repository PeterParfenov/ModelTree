#include <iostream>

#include "Track.h"
#include "TRandom.h"

ClassImp(Track);

Track::Track(const Track &orig) : TObject(orig), fTriggerBits(orig.fTriggerBits)
{
  fPx = orig.fPx;
  fPy = orig.fPy;
  fPz = orig.fPx;
  fMass = orig.fMass;
  fCharge = orig.fCharge;

  fVertex[0] = orig.fVertex[0];
  fVertex[1] = orig.fVertex[1];
  fVertex[2] = orig.fVertex[2];
}

////////////////////////////////////////////////////////////////////////////////
/// Create a track object.

Track::Track(Int_t pdg, Float_t px, Float_t py, Float_t pz,
             Float16_t mass, Double32_t charge,
             Double32_t vx, Double32_t vy, Double32_t vz)
{
  Set(pdg, px, py, pz, mass, charge, vx, vy, vz);
}

////////////////////////////////////////////////////////////////////////////////
/// Copy a track

Track &Track::operator=(const Track &orig)
{
  TObject::operator=(orig);
  fPx = orig.fPx;
  fPy = orig.fPy;
  fPz = orig.fPx;
  fMass = orig.fMass;
  fCharge = orig.fCharge;

  fVertex[0] = orig.fVertex[0];
  fVertex[1] = orig.fVertex[1];
  fVertex[2] = orig.fVertex[2];

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

void Track::Set(Int_t pdg, Float_t px, Float_t py, Float_t pz,
                Float16_t mass, Double32_t charge,
                Double32_t vx, Double32_t vy, Double32_t vz)
{
  fPdg = pdg;
  fPx = px;
  fPy = py;
  fPz = pz;
  fMass = mass;
  fCharge = charge;

  // fTriggerBits.SetBitNumber((UInt_t)(64 * gRandom->Rndm()));
  // fTriggerBits.SetBitNumber((UInt_t)(64 * gRandom->Rndm()));
  // fTriggerBits.SetBitNumber((UInt_t)(64 * gRandom->Rndm()));

  fVertex[0] = vx;
  fVertex[1] = vy;
  fVertex[2] = vz;
}

////////////////////////////////////////////////////////////////////////////////

void Track::Print(Option_t *option)
{
  // Print the data members to the standard output
  std::cout << "------------------------------------------------" << std::endl
            << "-I-                   Track                  -I-" << std::endl
            << "PDG code                      : " << fPdg << std::endl
            << "Charge                        : " << fCharge << std::endl
            << "Mass                          : " << fMass << std::endl
            << "Momentum {px, py, pz} (GeV/c) : {" << fPx << ", " << fPy << ", " << fPz << "}" << std::endl
            << "Position {x, y, z} (fm)       : {" << fVertex[0] << ", " << fVertex[0] << ", " << fVertex[0] << "}" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////