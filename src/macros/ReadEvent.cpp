#include <iostream>

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStopwatch.h"

#include "Event.h"
#include "Track.h"

int main(int argc, char** argv)
{
  TStopwatch timer;
  timer.Start();

  TString inFileName, outFileName;
  std::cout << "ReadEvent: Processing." << std::endl;

  if (argc < 4)
   {
      std::cerr << "./nurqmd2tc -i INPUTFILE -o OUTPUTFILE" << std::endl;
      return 10;
   }
   for (int i = 1; i < argc; i++)
   {
      if (std::string(argv[i]) != "-i" &&
          std::string(argv[i]) != "-o")
      {
         std::cerr << "\nurqmd2tc: Unknown parameter: " << argv[i] << std::endl;
         return 11;
      }
      else
      {
         if (std::string(argv[i]) == "-i" && i != argc - 1)
         {
            inFileName = argv[++i];
         }
         if (std::string(argv[i]) == "-i" && i == argc - 1)
         {
            std::cerr << "\nurqmd2tc: Input file name was not specified!" << std::endl;
            return 20;
         }
         if (std::string(argv[i]) == "-o" && i != argc - 1)
         {
            outFileName = argv[++i];
         }
         if (std::string(argv[i]) == "-o" && i == argc - 1)
         {
            std::cerr << "\nurqmd2tc: Output file name was not specified!" << std::endl;
            return 21;
         }
      }
   }


  const int diag_name[] = {0, 12, 17, 27, 37, 47, 57, 67, 77};
  const int Ndiags = 9;

  TH1F *hMult = new TH1F("hMult", "Charge particle multiplicity", 1000, 0, 1000.); hMult->Sumw2();

  TH1F *hRapidity[Ndiags];
  TH1F *hEta[Ndiags];
  TH1F *hPt[Ndiags];
  TH1F *hT[Ndiags];
  TH1F *hTform[Ndiags];
  TH1F *hTformZeroFreezeout = new TH1F("hTformZeroFreezeout", "Time (formation) for T_{freeze-out} = 0;t_{form};N_{count}", 1000, 0, 1e4);
  TH2F *hTformZ[Ndiags];
  TH1F* hMultF = new TH1F("hMultF","Forward charged particle multiplicity",150,0,150); hMultF->Sumw2();
  TH1F* hMultB = new TH1F("hMultB","Backward charged particle multiplicity",150,0,150); hMultB->Sumw2();
  TH1F* hMultNormF = new TH1F("hMultNormF","Normalized forward charged particle multiplicity",150,0,150); hMultF->Sumw2();

  TH2F* hYvsN0diag = new TH2F("hYvsN0diag","dN/dy vs diagram number of the first particle in the event;N^{0}_{diag};dN/dy",200,-0.5,199.5,600,-15.,15.);
  TH2F* hNFvsN0diag = new TH2F("hNFvsN0diag","N_{F} vs diagram number of the first particle in the event;N^{0}_{diag};N_{F}",200,-0.5,199.5,150,0.,150.);
  TH2F* hNFNormvsN0diag = new TH2F("hNFNormvsN0diag","N_{F} normalized vs diagram number of the first particle in the event;N^{0}_{diag};N_{F}",200,-0.5,199.5,150,0.,150.);
  TH2F* hNBvsN0diag = new TH2F("hNBvsN0diag","N_{B} vs diagram number of the first particle in the event;N^{0}_{diag};N_{B}",200,-0.5,199.5,150,0.,150.);
  TH2F* hNBNormvsN0diag = new TH2F("hNBNormvsN0diag","N_{B} normalized vs diagram number of the first particle in the event;N^{0}_{diag};N_{B}",200,-0.5,199.5,150,0.,150.);
  TH2F* hQFvsN0diag = new TH2F("hQFvsN0diag","Q_{F} vs diagram number of the first particle in the event;N^{0}_{diag};Q_{F}",200,-0.5,199.5,31,-15.5,15.5);
  TH2F* hQBvsN0diag = new TH2F("hQBvsN0diag","Q_{B} vs diagram number of the first particle in the event;N^{0}_{diag};Q_{B}",200,-0.5,199.5,31,-15.5,15.5);
  TH2F* hQassymvsN0diag = new TH2F("hQassymvsN0diag","Q_{assymetry} vs diagram number of the first particle in the event;N^{0}_{diag};Q_{assym}",200,-0.5,199.5,61,-30.5,30.5);

  TH2F* hYvsNdiag = new TH2F("hYvsNdiag","dN/dy vs diagram number;N_{diag};dN/dy",200,-0.5,199.5,600,-15.,15.);
  TH2F* hNFvsNdiag = new TH2F("hNFvsNdiag","N_{F} vs diagram number;N_{diag};N_{F}",200,-0.5,199.5,150,0.,150.);
  TH2F* hNFNormvsNdiag = new TH2F("hNFNormvsNdiag","N_{F} normalized vs diagram number;N_{diag};N_{F}",200,-0.5,199.5,150,0.,150.);
  TH2F* hNBvsNdiag = new TH2F("hNBvsNdiag","N_{B} vs diagram number;N_{diag};N_{B}",200,-0.5,199.5,150,0.,150.);
  TH2F* hNBNormvsNdiag = new TH2F("hNBNormvsNdiag","N_{B} normalized vs diagram number;N_{diag};N_{B}",200,-0.5,199.5,150,0.,150.);
  TH2F* hQFvsNdiag = new TH2F("hQFvsNdiag","Q_{F} vs diagram number;N_{diag};Q_{F}",200,-0.5,199.5,31,-15.5,15.5);
  TH2F* hQBvsNdiag = new TH2F("hQBvsNdiag","Q_{B} vs diagram number;N_{diag};Q_{B}",200,-0.5,199.5,31,-15.5,15.5);
  TH2F* hQassymvsNdiag = new TH2F("hQassymvsNdiag","Q_{assymetry} vs diagram number;N_{diag};Q_{assym}",200,-0.5,199.5,61,-30.5,30.5);

  Int_t n[Ndiags], nMultF, nMultB, QF, QB;
  Int_t nMultF1, nMultB1, QF1, QB1;

  for (int i = 0; i < Ndiags; i++)
  {
    hRapidity[i] = new TH1F(Form("hRapidity%i", i),
                            Form("rapidity yild of all particles %i;y;#frac{1}{N}#frac{dN}{dy}", diag_name[i]),
                            200, -10., 10.);
    hEta[i] = new TH1F(Form("hEta%i", i),
                       Form("#eta yild of all particles %i;#eta;#frac{1}{N}#frac{dN}{d#eta}", diag_name[i]),
                       200, -10., 10.);
    hPt[i] = new TH1F(Form("hPt%i", i),
                      Form("p_{T}-differential yild of charged hadrons %i;p_{T}, [GeV/c];#frac{1}{2#pi p_{T}} #frac{dN}{dp_{T}}", diag_name[i]), 100, 0., 5.);
    hT[i] = new TH1F(Form("hT%i", i), Form("Time (freeze-out) %i;t;N_{count}", diag_name[i]), 1000, 0, 1e4);
    hTform[i] = new TH1F(Form("hTform%i", i), Form("Time (formation) %i;t_{form};N_{count}", diag_name[i]), 1000, 0, 1e2);
    n[i] = 0;
    hTformZ[i] = new TH2F(Form("hTformZ%i",i), Form("t_{form} vs z_{form} %i; z, [fm]; t_{form}, [fm/c]",diag_name[i]),1000,-15e3,15e3,1000,0.,1e4);
  }

  // TFile *fi = new TFile("/home/peter/Documents/Dataset/root_converted/uioqgsm.root", "read");
  // TFile *fi = new TFile("/lustre/nyx/hades/user/parfenov/uioqgsm_100k_wo_res.root", "read");
  // TFile *fi = new TFile("/lustre/nyx/hades/user/parfenov/Soft/ModelTree/build/urqmd_test.root", "read");
  TFile *fi = new TFile(inFileName.Data(), "read");
  TTree *tree = (TTree *)fi->Get("events");

  Event *ev = new Event();
  tree->SetBranchAddress("event", &ev);

  Int_t nentries = tree->GetEntriesFast();
  Int_t ntracks;
  Double_t rapidity, eta, mom, massT;
  Int_t nChargeCounter;

  for (int i = 0; i < nentries; i++)
  {
    nChargeCounter = 0;
    nMultF = 0; nMultF1 = 0;
    nMultB = 0; nMultB1 = 0;
    QF = 0; QF1 = 0;
    QB = 0; QB1 = 0;
    tree->GetEntry(i);
    if (i % 1000 == 0)
    {
      std::cout << "Real Time: " << timer.RealTime() << std::endl;
      timer.Continue();
      ev->Print();
    }

    ntracks = ev->GetNtrack();
    if (ntracks == 0) continue;

    /*if (ev->GetTrack(0)->GetDiag1() == 1 ||
        ev->GetTrack(0)->GetDiag1() == 4 ||
        ev->GetTrack(0)->GetDiag1() == 6)
      continue;
    */
    for (int j = 0; j < ntracks; j++)
    {
      mom = TMath::Sqrt(ev->GetTrack(j)->GetPvect(3) * ev->GetTrack(j)->GetPvect(3) + ev->GetTrack(j)->GetPt() * ev->GetTrack(j)->GetPt());
      massT = TMath::Sqrt(ev->GetTrack(j)->GetPt()*ev->GetTrack(j)->GetPt() + ev->GetTrack(j)->GetMass()*ev->GetTrack(j)->GetMass());
      // rapidity = 0.5 * TMath::Log((ev->GetTrack(j)->GetPvect(0) + ev->GetTrack(j)->GetPvect(3)) / (ev->GetTrack(j)->GetPvect(0) - ev->GetTrack(j)->GetPvect(3)));
      rapidity = TMath::Log(ev->GetTrack(j)->GetPvect(0)+ev->GetTrack(j)->GetPvect(3)) - TMath::Log(massT);
      // eta = 0.5 * TMath::Log((mom + ev->GetTrack(j)->GetPvect(3)) / (mom - ev->GetTrack(j)->GetPvect(3)));
      eta = TMath::Log(mom+ev->GetTrack(j)->GetPvect(3)) - TMath::Log(ev->GetTrack(j)->GetPt());

      hYvsN0diag->Fill(ev->GetTrack(0)->GetDiag1(),rapidity);

      // if (TMath::Abs(rapidity) > 2.5)
      // continue;
      // if (ev->GetTrack(j)->GetDiag1() == 1 ||
      //     ev->GetTrack(j)->GetDiag1() == 4 ||
      //     ev->GetTrack(j)->GetDiag1() == 6)
      //   continue;

      if (ev->GetTrack(j)->GetCharge() != 0){
        nChargeCounter++;
        if (eta>0 /*&& eta<4*/) { nMultF++; QF+= ev->GetTrack(j)->GetCharge(); }
        if (eta<0 /*&& eta>-4*/) { nMultB++; QB += ev->GetTrack(j)->GetCharge(); }
      }
      // if (TMath::Abs(eta) >= 2.4)
      //  continue;
      hEta[0]->Fill(eta);
      hRapidity[0]->Fill(rapidity);
      // if (TMath::Abs(eta) < 2.4)
      if (TMath::Abs(eta) < 2.4 && ev->GetTrack(j)->GetCharge() != 0) hPt[0]->Fill(ev->GetTrack(j)->GetPt(), 1. / (2 * TMath::Pi() * ev->GetTrack(j)->GetPt()*(2.4 + 2.4)));
      hT[0]->Fill(ev->GetTrack(j)->GetVertex(0));
      hTform[0]->Fill(ev->GetTrack(j)->GetTform());
      hTformZ[0]->Fill(ev->GetTrack(j)->GetVal(3),ev->GetTrack(j)->GetTform());
      if (ev->GetTrack(j)->GetVertex(0) == 0)
        hTformZeroFreezeout->Fill(ev->GetTrack(j)->GetTform());
      for (int iDiag = 1; iDiag < Ndiags; iDiag++)
      {
        if (ev->GetTrack(0)->GetDiag1() == diag_name[iDiag])
        {
          hRapidity[iDiag]->Fill(rapidity);
          hPt[iDiag]->Fill(ev->GetTrack(j)->GetPt());
          hT[iDiag]->Fill(ev->GetTrack(j)->GetVertex(0));
          hTform[iDiag]->Fill(ev->GetTrack(j)->GetTform());
          hTformZ[iDiag]->Fill(ev->GetTrack(j)->GetVal(3),ev->GetTrack(j)->GetTform());
        }
      }
    }
    hMult->Fill(nChargeCounter);
    hMultF->Fill(nMultF);
    hMultNormF->Fill(nMultF,nMultB);
    hMultB->Fill(nMultB);
    hNFvsN0diag->Fill(ev->GetTrack(0)->GetDiag1(),nMultF);
    hNFNormvsN0diag->Fill(ev->GetTrack(0)->GetDiag1(),nMultF,nMultB);
    hNBvsN0diag->Fill(ev->GetTrack(0)->GetDiag1(),nMultB);
    hNBNormvsN0diag->Fill(ev->GetTrack(0)->GetDiag1(),nMultB,nMultF);
    hQFvsN0diag->Fill(ev->GetTrack(0)->GetDiag1(),QF);
    hQBvsN0diag->Fill(ev->GetTrack(0)->GetDiag1(),QB);
    hQassymvsN0diag->Fill(ev->GetTrack(0)->GetDiag1(), (QF-QB));
  }

  Double_t scaleFactorEta = hEta[0]->Integral("width") * (10. + 10.) / 200;
  Double_t scaleFactorRapidity = hRapidity[0]->Integral("width") * (10. + 10.) / 200;
  Double_t scaleFactorPt = hMult->GetEntries() * (5. - 0.) / 100;

  hEta[0]->Scale(1. / scaleFactorEta);
  hPt[0]->Scale(1. / scaleFactorPt);
  hRapidity[0]->Scale(1. / scaleFactorRapidity);

  // TFile *fo = new TFile("./uioqgsm_hists.root", "recreate");
  // TFile *fo = new TFile("./urqmd_test_hists.root", "recreate");
  TFile *fo = new TFile(outFileName.Data(), "recreate");
  fo->cd();
  hYvsN0diag->Write();
  hNFvsN0diag->Write();
  hNFNormvsN0diag->Write();
  hNBvsN0diag->Write();
  hNBNormvsN0diag->Write();
  hQFvsN0diag->Write();
  hQBvsN0diag->Write();
  hQassymvsN0diag->Write();
  hMult->Write();
  hMultF->Write();
  hMultNormF->Write();
  hMultB->Write();
  hTformZeroFreezeout->Write();
  for (int i = 0; i < Ndiags; i++)
  {
    hEta[i]->Write();
    hRapidity[i]->Write();
    hPt[i]->Write();
    hT[i]->Write();
    hTform[i]->Write();
    hTformZ[i]->Write();
  }

  fo->Close();
  fi->Close();

  timer.Stop();
  timer.Print();

  return 0;
}
