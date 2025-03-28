#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/SISConePlugin.hh"

#include <TFile.h>
#include <TTree.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <numbers>

#include "PythiaEvent.hpp"

using namespace Pythia8;
using namespace fastjet;

int main(int argc, char *argv[]) {

  int nTotalJets{5000};
  const char *file_name{"tmp.root"};
  if (argc > 1) {
    nTotalJets = std::atoi(argv[1]);
    if (nTotalJets <= 0) {
      std::cerr << "Invalid number of total jets specified. Using default: 5000\n";
      nTotalJets = 5000;
    }
    file_name = argv[2];
  }

  Pythia pythia;
  ////
  // Beam setup
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("Beams:eCM = 14000.");

  pythia.readString("PartonLevel:all = on");
  pythia.readString("HadronLevel:all = on");
  pythia.readString("ProcessLevel:all = on");
  // pythia.readString("Check:event = on");

  // Enable Z + jet production (Z + q and Z + g)
  pythia.readString("WeakSingleBoson:ffbar2gmZ = on");

  // Decay: only allow Z → ν_e, ν_μ, ν_τ
  pythia.readString("23:onMode = off");
  pythia.readString("23:onIfAny = 12 14 16");

  // Set a cut on the hard process pT (to ensure energetic jets)
  pythia.readString("PhaseSpace:pTHatMin = 200.");

  
  ////


  pythia.readString("Random:setSeed = on");
  pythia.readString("Random:seed = 12345");
  pythia.readString("Main:timesAllowErrors = 10000");

  pythia.init();

  auto tfile = new TFile(Form("%s", file_name), "RECREATE");
  auto tree = new TTree("tree", "Pythia Jet Tree");
  std::unique_ptr<Jet> jetEvent{nullptr};

  Jet* eventPtr = nullptr;
  tree->Branch("event", &eventPtr);
  // tree->Branch("event", &event);

  constexpr double R = 0.4;
  constexpr double minJetPt = 5.0;

  int jetsFound{0};
  int iEvent{0};
  int totalJets{0};
  while (jetsFound < nTotalJets) {
    if (!pythia.next())
      continue;
    iEvent++;

    std::vector<PseudoJet> particles;
    
    std::vector<const Particle*> hardPartons;

    double totalEnergy{0.0};
    for (int iTrack{0}; iTrack < pythia.event.size(); ++iTrack) {
      const Particle &track = pythia.event[iTrack];
      if (track.isFinal() && track.isVisible() && track.isHadron()) {
        // event->setTrack(track.px(), track.py(), track.pz(), track.e());
        // std::cout << "track for clustering found" << std::endl;
        PseudoJet pseudoJet(track.px(), track.py(), track.pz(), track.e());
        pseudoJet.set_user_index(iTrack);
        particles.push_back(pseudoJet);
        totalEnergy += track.e();
      }

      // if (!track.isParton()) continue;     // only partons
      // if (!track.isFinal()) continue;      // only final-state partons before hadronization
      if (abs(track.id()) > 21) continue; 
      hardPartons.push_back(&track);
      // std::cout << "hard parton found" << std::endl;
      
    }
    if (particles.size() < 2)
      continue;

    /* anti kt */
    JetDefinition jetDef(antikt_algorithm, R);
    ClusterSequence cs(particles, jetDef);
    std::vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets(minJetPt));
    if (jets.size() > 0) {
      totalJets += jets.size();
    } else {
      continue;
    }

    // calculate angularity
    // NSubjettiness tau(1, jetDef);
    // double tau1 = tau.getTau(jet);
    double jetEnergy{0.0};
    int jdx{0};
    for (const auto &jet : jets) {
      jetEvent = std::make_unique<Jet>();
      jetEvent->setJet(jet.pt(), jet.eta(), jet.phi(), jet.e(), jet.m());
      jetEnergy += jet.e();

      float leadingPt{-1.0f};
      int leadPID{0};
      int leadIndex{-1};
      for (const auto &constituent : jet.constituents()) {
        int index = constituent.user_index();
        if (index < 0 || index >= pythia.event.size())
          continue;

        const Particle &part = pythia.event[index];
        float deltaR = jet.delta_R(constituent);
        float charge = part.charge();

        if (constituent.pt() > leadingPt) {
          leadingPt = constituent.pt();
          leadPID = part.id();
          leadIndex = index;
        }

        jetEvent->setJetConstituents(jdx, index, constituent.pt(), constituent.eta(), constituent.phi(), constituent.e(), charge, part.id(), deltaR);
        // jetEvent->setMotherPID(getInitiatingPartonPID(pythia.event, leadIndex));
      }
      jetEvent->setLeadJet(leadingPt, leadPID, leadIndex);
      jetEvent->setEnergyFraction(jet.e() / totalEnergy);

      // int jetType = -1; // -1 = unknown, 0 = quark, 1 = gluon
      double minDR2 = 999.0;
      int pid=0;
      for (const auto* parton : hardPartons) {
        double dEta = jet.eta() - parton->eta();
        double dPhi = std::abs(jet.phi() - parton->phi());
        if (dPhi > std::numbers::pi) dPhi = 2 * std::numbers::pi - dPhi;
        double dr2 = dEta * dEta + dPhi * dPhi;
        // std::cout << "dr2: " << dr2 << std::endl;
        if (dr2 < minDR2) {
          minDR2 = dr2;
          pid = parton->id();
          // std::cout << "actually made it here" << std::endl;
        }
      }
      jetEvent->setMotherPID(pid);
      
      eventPtr = jetEvent.get();
      tree->Fill();
      jetsFound++;
      jdx++;
    }
    
    // event->setEnergyFraction(jetEnergy / totalEnergy);

    // eventPtr = event.get();
    // tree->Fill();
    // delete event;
  }

  tree->AutoSave();
  tfile->Close();

  pythia.stat();
  std::cout << "Number of events with jets: " << jetsFound << "/" << nTotalJets << std::endl;
  std::cout << "Total number of jets found: " << totalJets << std::endl;
  std::cout << "Total number of events ran: " << iEvent << std::endl;

  return 0;
}
