#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TNetFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TTreePerfStats.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TStopwatch.h"

#include "Event.h"
#include "Track.h"
#include "EventHeader.h"
#include "ListOfPDG.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  Int_t nevent = 400;   // by default create 400 events
  Int_t comp = 1;       // by default file is compressed
  Int_t split = 1;      // by default, split Event in sub branches
  Int_t enable_imt = 0; // Whether to enable IMT mode.
  Int_t verbosity = -0; // Verbosity level
#ifdef R__HAS_DEFAULT_LZ4
  Int_t compAlg = 4; // Allow user to specify underlying compression algorithm.
#else
  Int_t compAlg = 1;
#endif
  Int_t branchStyle = 1; //new style by default
  TString inEventName, inTrackName, outFileName;
  std::ifstream isEvent, isTrack;
  std::stringstream ss, ss1;
  std::string str, str1;

  Double_t B, PsiRP, Time,
      r0, rx, ry, rz, p0, px, py, pz,
      m, ityp, i3, ichg, baryon, strange, dump;
  Int_t Nev, Npart, count,
      pid;

  //  const Int_t count1 = 3, count2 = 12;
  const Double_t minPt = 0.2;
  const Int_t run = 1;
  const Int_t date = 251118;

  if (argc < 6)
  {
    std::cerr << "./nuioqgsm2tc -iEvent INPUTFILE -iTrack INPUTFILE -o OUTPUTFILE [OPTIONAL: -v 0,1,2 (verbosity level)]" << std::endl;
    return 10;
  }
  for (int i = 1; i < argc; i++)
  {
    if (std::string(argv[i]) != "-iEvent" &&
        std::string(argv[i]) != "-iTrack" &&
        std::string(argv[i]) != "-o" &&
        std::string(argv[i]) != "-v")
    {
      std::cerr << "\nuioqgsm2tc: Unknown parameter: " << argv[i] << std::endl;
      return 11;
    }
    else
    {
      if (std::string(argv[i]) == "-iEvent" && i != argc - 1)
      {
        inEventName = argv[++i];
      }
      if (std::string(argv[i]) == "-iEvent" && i == argc - 1)
      {
        std::cerr << "\nuioqgsm2tc: Input Event file name was not specified!" << std::endl;
        return 20;
      }
      if (std::string(argv[i]) == "-iTrack" && i != argc - 1)
      {
        inTrackName = argv[++i];
      }
      if (std::string(argv[i]) == "-iTrack" && i == argc - 1)
      {
        std::cerr << "\nuioqgsm2tc: Input Track file name was not specified!" << std::endl;
        return 20;
      }
      if (std::string(argv[i]) == "-o" && i != argc - 1)
      {
        outFileName = argv[++i];
      }
      if (std::string(argv[i]) == "-o" && i == argc - 1)
      {
        std::cerr << "\nuioqgsm2tc: Output file name was not specified!" << std::endl;
        return 21;
      }
      if (std::string(argv[i]) == "-v" && i != argc - 1)
      {
        verbosity = atof(argv[++i]);
      }
      if (std::string(argv[i]) == "-v" && i == argc - 1)
      {
        std::cerr << "\nuioqgsm2tc: Verbosity!" << std::endl;
        return 20;
      }
    }
  }

  if (verbosity != 0 && verbosity != 1 && verbosity != 2)
  {
    std::cerr << "\nuioqgsm2tc: Incorrect verbosity level. Take it by default." << std::endl;
    verbosity = 0;
  }

#ifdef R__USE_IMT
  if (enable_imt)
  {
    ROOT::EnableImplicitMT();
  }
#else
  if (enable_imt)
  {
    std::cerr << "IMT mode requested, but this version of ROOT "
                 "is built without IMT support."
              << std::endl;
    return 1;
  }
#endif

  TFile *hfile;
  TTree *tree;
  // TTreePerfStats *ioperf = nullptr;
  Event *event = 0;

  // Fill event, header and tracks with some random numbers
  //   Create a timer object to benchmark this loop
  TStopwatch timer;
  timer.Start();
  Long64_t nb = 0;
  Int_t ev;
  Int_t bufsize;
  Double_t told = 0;
  Double_t tnew = 0;
  Int_t printev = 1000;

  //         Write case
  // Create a new ROOT binary machine independent file.
  // Note that this file may contain any kind of ROOT objects, histograms,
  // pictures, graphics objects, detector geometries, tracks, events, etc..
  // This file is now becoming the current directory.
  hfile = new TFile(outFileName.Data(), "RECREATE", "TTree benchmark ROOT file");
  hfile->SetCompressionLevel(comp);
  hfile->SetCompressionAlgorithm(compAlg);

  // Create histogram to show write_time in function of time
  Float_t curtime = -0.5;
  Int_t ntime = nevent / printev;

  // Create a ROOT Tree and one superbranch
  tree = new TTree("events", "An example of a ROOT tree");
  tree->SetAutoSave(1000000000); // autosave when 1 Gbyte written
  tree->SetCacheSize(10000000);  // set a 10 MBytes cache (useless when writing local files)
  bufsize = 64000;
  if (split)
    bufsize /= 4;
  event = new Event(); // By setting the value, we own the pointer and must delete it.
  TTree::SetBranchStyle(branchStyle);
  TBranch *branch = tree->Branch("event", &event, bufsize, split);
  branch->SetAutoDelete(kFALSE);
  if (split >= 0 && branchStyle)
    tree->BranchRef();
  Float_t ptmin = 1;

  Int_t ObjectNumber = 0;

  isEvent.open(inEventName.Data());
  if (isEvent.fail())
  {
    std::cerr << "uioqgsm2tc: Cannot open input Event file: " << inEventName.Data() << std::endl;
    return 30;
  }

  isTrack.open(inTrackName.Data());
  if (isTrack.fail())
  {
    std::cerr << "uioqgsm2tc: Cannot open input Track file: " << inTrackName.Data() << std::endl;
    return 31;
  }

  ev = 0;
  while (!isEvent.eof())
  {
    if (ev % printev == 0)
    {
      tnew = timer.RealTime();
      printf("\tevent:%d,\trtime=%f s\n", ev, tnew - told);
      curtime += 1;
      told = tnew;
      timer.Continue();
    }

    // ss.str("");
    // ss.clear();
    // getline(isEvent, str);

    // Read event number & impact parameter & number of particles
    ss.str("");
    ss.clear();
    getline(isEvent, str);
    ss << str;
    ss >> Nev >> B >> Npart;

    //Save current Object count
    ObjectNumber = TProcessID::GetObjectCount();

    event->GetHighPt()->Delete();
    event->GetMuons()->Delete();
    event->Clear();

    event->SetB(B);
    event->SetHeader(Nev, run, date);
    if (Npart <= 0)
      continue;
    // Loop on particles on all time in this event
    for (int j = 0; j < Npart; j++)
    {
      ss1.str("");
      ss1.clear();
      getline(isTrack, str1);
      ss1 << str1;
      if (str1 == "") break;
      ss1 >> r0 >> rx >> ry >> rz >> p0 >> px >> py >> pz >> m >> ityp >> i3 >> baryon >> strange >> ichg 
      >> dump >> dump >> dump >> dump >> dump >> dump;
      pid = (particleQGSM.find(TMath::Abs(ityp)) != particleQGSM.end()) ? TMath::Sign(particleQGSM.at(TMath::Abs(ityp)), ichg) : -999.;
      event->AddTrack(pid, px, py, pz, m, ichg, rx, ry, rz, minPt);
    }

    //Restore Object count
    //To save space in the table keeping track of all referenced objects
    //we assume that our events do not address each other. We reset the
    //object count to what it was at the beginning of the event.
    TProcessID::SetObjectCount(ObjectNumber);
    nb += tree->Fill(); //fill the tree
    if (ev % printev == 0 && verbosity == 1)
      event->Print();
    if (ev % printev == 0 && verbosity == 2)
      event->Print("all");
    ev++;
  }

  hfile = tree->GetCurrentFile(); //just in case we switched to a new file
  hfile->Write();
  tree->Print();
  // We own the event (since we set the branch address explicitly), we need to delete it.
  delete event;
  event = 0;

  //  Stop timer and print results
  timer.Stop();
  Float_t mbytes = 0.000001 * nb;
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();

  printf("\n%d events and %lld bytes processed.\n", nevent, nb);
  printf("RealTime=%f seconds, CpuTime=%f seconds\n", rtime, ctime);
  printf("compression level=%d, split=%d, IMT=%d, compression algorithm=%d\n", comp, split,
         enable_imt, compAlg);
  printf("You write %f Mbytes/Realtime seconds\n", mbytes / rtime);
  printf("You write %f Mbytes/Cputime seconds\n", mbytes / ctime);
  //printf("file compression factor = %f\n",hfile.GetCompressionFactor());
  hfile->Close();
  return 0;
}
