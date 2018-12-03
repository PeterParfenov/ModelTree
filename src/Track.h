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
  Double32_t fPvect[4]; //[e,px,py,pz] component of the momentum
  Float16_t fMass;      
  Int_t fCharge;        
  Int_t fBaryon;
  Int_t fStrange;
  Int_t fDiag1;
  Int_t fDiag2;
  Double32_t fTform;
  Double32_t fTformParent;
  Double32_t fVertex[4]; 
  Double32_t fVal[4];
  Double32_t fRes[3]; //Resonance coordinates
  TBits fTriggerBits; //Bits triggered by this track.

public:
  Track() : fTriggerBits(64) {}
  Track(const Track &orig);
  Track(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
        Float16_t mass, Int_t baryon, Int_t strange, Int_t charge,
        Double32_t t, Double32_t vx, Double32_t vy, Double32_t vz,
        Double32_t tform, Int_t diag1, Int_t diag2);
  virtual ~Track() { Clear(); }
  Track &operator=(const Track &orig);

  void Set(Int_t pdg, Double32_t E, Double32_t px, Double32_t py, Double32_t pz,
           Float16_t mass, Int_t baryon, Int_t strange, Int_t charge,
           Double32_t t, Double32_t vx, Double32_t vy, Double32_t vz,
           Double32_t tform, Int_t diag1, Int_t diag2);
  void Clear(Option_t *option = "");
  void Print(Option_t *option = "");

  void SetPdg(Int_t a) { fPdg = a; }
  void SetPvect(Int_t i, Float_t a)
  {
    if (i < 4)
      fPvect[i] = a;
  }
  void SetMass(Float16_t a) { fMass = a; }
  void SetCharge(Int_t a) { fCharge = a; }
  void SetBaryon(Int_t a) { fBaryon = a; }
  void SetStrange(Int_t a) { fStrange = a; }
  void SetDiag1(Int_t a) { fDiag1 = a; }
  void SetDiag2(Int_t a) { fDiag2 = a; }
  void SetTform(Double32_t a) { fTform = a; }
  void SetVertex(Int_t i, Double32_t a)
  {
    if (i < 4)
      fVertex[i] = a;
  }

  Int_t GetPdg() const { return fPdg; }
  Float_t GetPvect(Int_t i) const { return (i < 4) ? fPvect[i] : 0; }
  Float_t GetPt() const { return TMath::Sqrt(fPvect[1] * fPvect[1] + fPvect[2] * fPvect[2]); }
  Float16_t GetMass() const { return fMass; }
  Int_t GetCharge() const { return fCharge; }
  Int_t GetBaryon() const { return fBaryon; }
  Int_t GetStrange() const { return fStrange; }
  Int_t GetDiag1() const { return fDiag1; }
  Int_t GetDiag2() const { return fDiag2; }
  Double32_t GetTform() const { return fTform; }
  Double32_t GetVertex(Int_t i = 0) { return (i < 4) ? fVertex[i] : 0; }
  TBits &GetTriggerBits() { return fTriggerBits; }

  ClassDef(Track, 2) //A track segment
};

#endif