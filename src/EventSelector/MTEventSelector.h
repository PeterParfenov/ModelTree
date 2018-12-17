#ifndef MTEVENTSELECTOR_H
#define MTEVENTSELECTOR_H

#include <iostream>

#include <TROOT.h>
#include <TH1F.h>

class MTEventSelector
{
private:
  TH1F *fHist = nullptr;
  const Double_t dY = 2.;

public:
  MTEventSelector();
  virtual ~MTEventSelector();

  void SetEvent(TH1F *const &_hist){ fHist = _hist; }
  std::pair<Bool_t, Bool_t> CheckEvent();

  ClassDef(MTEventSelector, 0);
};

#endif