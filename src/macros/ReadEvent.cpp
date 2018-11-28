#include <iostream>

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TStopwatch.h"

#include "Event.h"
#include "Track.h"

int main()
{
  TStopwatch timer;
  timer.Start();
  std::cout << "ReadEvent: Processing." << std::endl;

  const int diag_name[] = {0, 1, 6, 11, 21, 31};
  const int Ndiags = 6;

  TH1F *hRapidity[Ndiags];
  TH1F *hPt[Ndiags];
  TH1F *hT[Ndiags];
  TH1F *hTform[Ndiags];
  TH1F *hTformZeroFreezeout = new TH1F("hTformZeroFreezeout", "Time (formation) for T_{freeze-out} = 0;t_{form};N_{count}", 1000, 0, 1e4);

  Int_t n[Ndiags];

  for (int i = 0; i < Ndiags; i++)
  {
    hRapidity[i] = new TH1F(Form("hRapidity%i", i),
                            Form("rapidity distribution %i;y;N_{count}", diag_name[i]),
                            200, -10., 10.);
    hPt[i] = new TH1F(Form("hPt%i", i),
                      Form("Pt distribution %i", diag_name[i]), 100, 0., 5.);
    hT[i] = new TH1F(Form("hT%i", i), Form("Time (freeze-out) %i;t;N_{count}", diag_name[i]), 1000, 0, 1e4);
    hTform[i] = new TH1F(Form("hTform%i", i), Form("Time (formation) %i;t_{form};N_{count}", diag_name[i]), 1000, 0, 1e4);
    n[i] = 0;
  }

  TFile *fi = new TFile("/home/peter/Documents/Dataset/root_converted/uioqgsm.root", "read");
  TTree *tree = (TTree *)fi->Get("events");

  Event *ev = new Event();
  tree->SetBranchAddress("event", &ev);

  Int_t nentries = tree->GetEntriesFast();
  Int_t ntracks;
  Double_t rapidity;

  for (int i = 0; i < nentries; i++)
  {
    tree->GetEntry(i);
    if (i % 1000 == 0)
    {
      std::cout << "Real Time: " << timer.RealTime() << std::endl;
      timer.Continue();
      ev->Print();
    }

    ntracks = ev->GetNtrack();

    for (int j = 0; j < ntracks; j++)
    {
      rapidity = 0.5 * TMath::Log((ev->GetTrack(j)->GetPvect(0) + ev->GetTrack(j)->GetPvect(3)) / (ev->GetTrack(j)->GetPvect(0) - ev->GetTrack(j)->GetPvect(3)));
      hRapidity[0]->Fill(rapidity);
      hPt[0]->Fill(ev->GetTrack(j)->GetPt());
      hT[0]->Fill(ev->GetTrack(j)->GetVertex(0));
      hTform[0]->Fill(ev->GetTrack(j)->GetTform());
      if (ev->GetTrack(j)->GetVertex(0) == 0)
        hTformZeroFreezeout->Fill(ev->GetTrack(j)->GetTform());
      for (int iDiag = 1; iDiag < Ndiags; iDiag++)
      {
        if (ev->GetTrack(j)->GetDiag1() == diag_name[iDiag])
        {
          hRapidity[iDiag]->Fill(rapidity);
          hPt[iDiag]->Fill(ev->GetTrack(j)->GetPt());
          hT[iDiag]->Fill(ev->GetTrack(j)->GetVertex(0));
          hTform[iDiag]->Fill(ev->GetTrack(j)->GetTform());
        }
      }
    }
  }

  TFile *fo = new TFile("/home/peter/Documents/Dataset/root_converted/uioqgsm_hists.root", "recreate");
  fo->cd();
  hTformZeroFreezeout->Write();
  for (int i = 0; i < Ndiags; i++)
  {
    hRapidity[i]->Write();
    hPt[i]->Write();
    hT[i]->Write();
    hTform[i]->Write();
  }

  fo->Close();
  fi->Close();

  timer.Stop();
  timer.Print();

  return 0;
}