#ifndef TRACK_H
#define TRACK_H

#include "TROOT.h"
#include "TObject.h"
#include "TBits.h"
#include "TMath.h"

class Track : public TObject
{

private:
  Int_t fPdg;
  Float_t fPx;           //X component of the momentum
  Float_t fPy;           //Y component of the momentum
  Float_t fPz;           //Z component of the momentum
  Float16_t fMass;       //[0,0,8] The mass of this particle
  Double32_t fCharge;    //[-1,1,2] Charge of this track
  Double32_t fVertex[3]; //[-30,30,16] Track vertex position
  TBits fTriggerBits;    //Bits triggered by this track.

public:
  Track() : fTriggerBits(64) {}
  Track(const Track &orig);
  Track(Int_t pdg, Float_t px, Float_t py, Float_t pz,
        Float16_t mass, Double32_t charge,
        Double32_t vx, Double32_t vy, Double32_t vz);
  virtual ~Track() { Clear(); }
  Track &operator=(const Track &orig);

  void Set(Int_t pdg, Float_t px, Float_t py, Float_t pz,
           Float16_t mass, Double32_t charge,
           Double32_t vx, Double32_t vy, Double32_t vz);
  void Clear(Option_t *option = "");
  void Print(Option_t* option = "");

  void SetPdg(Int_t a) { fPdg = a; }
  void SetPx(Float_t a) { fPx = a; }
  void SetPy(Float_t a) { fPy = a; }
  void SetPz(Float_t a) { fPz = a; }
  void SetMass(Float16_t a) { fMass = a; }
  void SetCharge(Double32_t a) { fCharge = a; }
  void SetVertex(Int_t i, Double32_t a)
  {
    if (i < 3)
      fVertex[i] = a;
  }

  Int_t GetPdg() const { return fPdg; }
  Float_t GetPx() const { return fPx; }
  Float_t GetPy() const { return fPy; }
  Float_t GetPz() const { return fPz; }
  Float_t GetPt() const { return TMath::Sqrt(fPx * fPx + fPy * fPy); }
  Float16_t GetMass() const { return fMass; }
  Double32_t GetCharge() const { return fCharge; }
  Double32_t GetVertex(Int_t i = 0) { return (i < 3) ? fVertex[i] : 0; }
  TBits &GetTriggerBits() { return fTriggerBits; }

  ClassDef(Track, 2) //A track segment
};

#endif