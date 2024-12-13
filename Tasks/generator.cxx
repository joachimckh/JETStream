#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/SISConePlugin.hh"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <TFile.h>
#include <TTree.h>

#include "PythiaEvent.hpp"

using namespace Pythia8;
using namespace fastjet;

int main(int argc, char *argv[]) {

  int nEvents{5000};
  const char* file_name{"tmp.root"};
  if (argc > 1) {
    nEvents = std::atoi(argv[1]);
    if (nEvents <= 0) {
      std::cerr << "Invalid number of events specified. Using default: 5000\n";
      nEvents = 5000;
    }
    file_name = argv[2];
  }

  Pythia pythia;
  pythia.readString("Beams:idA = 2212");    // proton
  pythia.readString("Beams:idB = 2212");    // proton
  pythia.readString("Beams:eCM = 13000.0"); // Center-of-mass energy at 13 TeV
  pythia.readString("HardQCD:all = on"); // Enable QCD processes to produce jets
  pythia.init();

  auto tfile = new TFile(Form("%s",file_name), "RECREATE");
  TTree *tree = new TTree("tree", "Pythia Event Tree");
  PythiaEvent *event{nullptr};
  tree->Branch("event", &event);

  constexpr double R = 0.4;
  constexpr double minJetPt = 5.0;
  // constexpr double overlap = 0.5;

  for (int iEvent{0}; iEvent < nEvents; iEvent++) {
    if (!pythia.next())
      continue;
    event = new PythiaEvent(iEvent);

    std::vector<PseudoJet> particles;

    for (const auto &track : pythia.event) {
      if (track.isFinal()) {
        event->setTrack(track.px(), track.py(), track.pz(), track.e());

        particles.push_back(
            PseudoJet(track.px(), track.py(), track.pz(), track.e()));
      }
    }

    if (particles.size() < 2)
      continue;

    /* anti kt*/
    JetDefinition jetDef(antikt_algorithm, R);
    ClusterSequence cs(particles, jetDef);
    std::vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets(minJetPt));
    for (const auto &jet : jets) {
      event->setJet(jet.pt(), jet.eta(), jet.phi(), jet.e(), jet.m(),
                    JetType::antikt);
    }
    if (jets.size() > 0) {
      event->setJetFound(true);
    }
    else {
      delete event;
      continue;
    }

    /* kt */
    JetDefinition jetDef_kt(kt_algorithm, R);
    ClusterSequence cs_kt(particles, jetDef_kt);
    std::vector<PseudoJet> jets_kt =
        sorted_by_pt(cs_kt.inclusive_jets(minJetPt));
    for (const auto &jet : jets_kt) {
      event->setJet(jet.pt(), jet.eta(), jet.phi(), jet.e(), jet.m(),
                    JetType::kt);
    }

    /* Cambridge/Aachen */
    JetDefinition jetDef_cambridge(kt_algorithm, R);
    ClusterSequence cs_cambridge(particles, jetDef_cambridge);
    std::vector<PseudoJet> jets_cambridge =
        sorted_by_pt(cs_cambridge.inclusive_jets(minJetPt));
    for (const auto &jet : jets_cambridge) {
      event->setJet(jet.pt(), jet.eta(), jet.phi(), jet.e(), jet.m(),
                    JetType::cambridge);
    }

    /* SISCone */
    // SISConePlugin plugin(R, overlap);
    // JetDefinition jetDef_sis(&plugin);
    // ClusterSequence cs_sis(particles, jetDef_sis);
    // std::vector<PseudoJet> jets_sis =
    // sorted_by_pt(cs_sis.inclusive_jets(minJetPt)); for (const auto &jet :
    // jets_sis) {
    //   event->setJet(jet.pt(), jet.eta(), jet.phi(), jet.e(), jet.m(),
    //   JetType::siscone);
    // }

    tree->Fill();
    delete event;
  }

  tree->AutoSave();
  tfile->Close();

  pythia.stat();

  return 0;
}