#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"

#include <cstdlib>
#include <iostream>
#include <vector>

#include "PythiaEvent.h"
#include <TFile.h>
#include <TTree.h>

using namespace Pythia8;
using namespace fastjet;

int main(int argc, char *argv[]) {

  int nEvents = 5000;
  if (argc > 1) {
    nEvents = std::atoi(argv[1]);
    if (nEvents <= 0) {
      std::cerr << "Invalid number of events specified. Using default: 5000\n";
      nEvents = 5000;
    }
  }

  Pythia pythia;
  pythia.readString("Beams:idA = 2212");    // proton
  pythia.readString("Beams:idB = 2212");    // proton
  pythia.readString("Beams:eCM = 13000.0"); // Center-of-mass energy at 13 TeV
  pythia.readString("HardQCD:all = on"); // Enable QCD processes to produce jets
  pythia.init();

  auto tfile = new TFile("tmp.root", "RECREATE");
  TTree *tree = new TTree("tree", "Pythia Event Tree");
  PythiaEvent *event{nullptr};
  tree->Branch("event", &event);

  constexpr double R = 0.4;
  constexpr double minJetPt = 5.0;

  for (int iEvent{0}; iEvent < nEvents; iEvent++) {
    if (!pythia.next())
      continue;
    event = new PythiaEvent(iEvent);

    std::vector<PseudoJet> particles;

    for (const auto & track : pythia.event) {
      if (track.isFinal()) {
        event->setEvProperties(track.px(), track.py(),
                               track.pz(), track.e());

        particles.push_back(
            PseudoJet(track.px(), track.py(),
                      track.pz(), track.e()));
      }
    }

    if (particles.size() < 2)
      continue;

    JetDefinition jetDef(antikt_algorithm, R);
    ClusterSequence cs(particles, jetDef);

    std::vector<PseudoJet> jets =
        sorted_by_pt(cs.inclusive_jets(minJetPt)); /* min jet pt */

    for (const auto &jet : jets) {
      event->setJetProperties(jet.pt(), jet.eta(),
                              jet.phi());
    }
    tree->Fill();
    delete event;
  }

  tree->AutoSave();
  tfile->Close();

  pythia.stat();

  return 0;
}