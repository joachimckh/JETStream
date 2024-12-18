#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/SISConePlugin.hh"

#include <TFile.h>
#include <TTree.h>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "PythiaEvent.hpp"

using namespace Pythia8;
using namespace fastjet;

int main(int argc, char *argv[]) {

  int nEvents{5000};
  const char *file_name{"tmp.root"};
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

  auto tfile = new TFile(Form("%s", file_name), "RECREATE");
  TTree *tree = new TTree("tree", "Pythia Jet Tree");
  PythiaEvent *event{nullptr};
  tree->Branch("event", &event);

  constexpr double R{0.4};
  constexpr double minJetPt{10.0};

  int jetsFound{0};
  int iEvent{0};
  int totalJets{0};
  // for (int iEvent{0}; iEvent < nEvents; iEvent++) {
  while (jetsFound < nEvents) {
    if (!pythia.next())
      continue;
    event = new PythiaEvent(iEvent);
    iEvent++;

    std::vector<PseudoJet> particles;

    double totalEnergy{0.0};
    for (int iTrack{0}; iTrack < pythia.event.size(); ++iTrack) {
      const Particle &track = pythia.event[iTrack];
      if (track.isFinal()) {
        event->setTrack(track.px(), track.py(), track.pz(), track.e());

        PseudoJet pseudoJet(track.px(), track.py(), track.pz(), track.e());
        pseudoJet.set_user_index(iTrack);
        particles.push_back(pseudoJet);

        totalEnergy += track.e();
      }
    }
    if (particles.size() < 2)
      continue;

    /* anti kt */
    JetDefinition jetDef(antikt_algorithm, R);
    ClusterSequence cs(particles, jetDef);
    std::vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets(minJetPt));
    if (jets.size() > 0) {
      event->setJetFound(true);
      event->setNJets(jets.size());
      totalJets += jets.size();
      jetsFound++;
    } else {
      delete event;
      continue;
    }

    // calculate angularity
    // NSubjettiness tau(1, jetDef);
    // double tau1 = tau.getTau(jet);
    double jetEnergy{0.0};
    int jdx{0};
    for (const auto &jet : jets) {
      event->setJet(jet.pt(), jet.eta(), jet.phi(), jet.e(), jet.m(), 
                    JetType::antikt);
      jetEnergy += jet.e();
      for (const auto &constituent : jet.constituents()) {
        int index = constituent.user_index();
        if (index < 0 || index >= pythia.event.size())
          continue;
        const Particle &part = pythia.event[index];
        float deltaR = jet.delta_R(constituent);
        event->setJetSubstructure(jdx, index, constituent.pt(), constituent.e(), part.id(), deltaR, JetType::antikt);
      }
      jdx++;
    }
    event->setEnergyFraction(jetEnergy / totalEnergy);

    tree->Fill();
    delete event;
  }

  tree->AutoSave();
  tfile->Close();

  pythia.stat();
  std::cout << "Number of events with jets: " << jetsFound << "/" << nEvents << std::endl;
  std::cout << "Total number of jets found: " << totalJets << std::endl;
  std::cout << "Total number of events ran: " << iEvent << std::endl;

  return 0;
}
