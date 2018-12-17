#include "MTEventSelector.h"
#include <TMath.h>

ClassImp(MTEventSelector);

MTEventSelector::MTEventSelector() {}

MTEventSelector::~MTEventSelector()
{
  delete fHist;
  fHist = nullptr;
}

std::pair<Bool_t, Bool_t> MTEventSelector::CheckEvent()
{
  std::pair<Bool_t, Bool_t> result;
  Int_t nBinsRapidityGap = 0;

  if (fHist == nullptr)
  {
    result = {false, false};
    return result;
  }

  Int_t nBins = fHist->GetNbinsX();
  if (fHist->GetBinContent(1) <= 0 && fHist->GetBinContent(nBins) <= 0)
  {
    result = {false, false};
    return result;
  }

  if (fHist->GetBinContent(1) > 0)
  {
    for (int iBin = 1; iBin < nBins; iBin++)
    {
      if (fHist->GetBinContent(iBin+1) > 0 ) break;
      nBinsRapidityGap = iBin;
    }
    if (nBinsRapidityGap == nBins) result.first = false;
    if ( TMath::Abs(fHist->GetBinCenter(1) - fHist->GetBinCenter(nBinsRapidityGap)) >= dY ) result.first = true;
    else result.first = false;
  }

  if (fHist->GetBinContent(nBins) > 0)
  {
    for (int iBin = nBins-1; iBin >= 1; iBin--)
    {
      if (fHist->GetBinContent(iBin) > 0 ) break;
      nBinsRapidityGap = iBin;
    }
    if (nBinsRapidityGap == 1) result.second = false;
    if ( TMath::Abs(fHist->GetBinCenter(1) - fHist->GetBinCenter(nBinsRapidityGap)) >= dY ) result.second = true;
    else result.second = false;
  }

  return result;
}