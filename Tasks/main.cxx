#include "Pythia8/Pythia.h"
#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include "PythiaEvent.h"

using namespace Pythia8;

int main() {
    // Initialize PYTHIA
    Pythia pythia;
    pythia.readString("Beams:idA = 2212");        // proton
    pythia.readString("Beams:idB = 2212");        // proton
    pythia.readString("Beams:eCM = 13000.0");     // Center-of-mass energy at 13 TeV
    pythia.readString("HardQCD:all = on");        // Enable QCD processes to produce jets
    pythia.init();

    auto tfile = new TFile("tmp.root", "RECREATE");
    TTree *tree = new TTree("tree", "Pythia Event Tree");
    PythiaEvent *event{nullptr};
    tree->Branch("event", &event);

    // Event loop: generate a specified number of events
    constexpr int numEvents = 500;
    for (int iEvent = 0; iEvent < numEvents; ++iEvent) {
        if (!pythia.next()) continue;  // Skip event if it fails
        event = new PythiaEvent(iEvent);
        // Vector to store FastJet-compatible particles
        // std::vector<PseudoJet> particles;

        // Loop over particles in the event
        for (int i = 0; i < pythia.event.size(); i++) {
            if (pythia.event[i].isFinal()) {
              event->setEvProperties(pythia.event[i].px(), 
                                     pythia.event[i].py(), 
                                     pythia.event[i].pz(), 
                                     pythia.event[i].e());
                
            }
        }
        

        // Check if there are enough particles for clustering
        // if (particles.size() < 2) continue;

        // // Step 2: Perform Jet Clustering with FastJet
        // double R = 0.4;  // Jet radius parameter
        // JetDefinition jetDef(antikt_algorithm, R);  // Anti-kt algorithm with radius R
        // ClusterSequence cs(particles, jetDef);

        // // Set minimum pt threshold for jets (e.g., 20 GeV)
        // std::vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets(20.0));

        // // Output jet information for this event
        // std::cout << "Event #" << iEvent << " has " << jets.size() << " jets:\n";
        // for (size_t iJet = 0; iJet < jets.size(); ++iJet) {
        //     std::cout << "  Jet " << iJet + 1 << ": pt = " << jets[iJet].pt()
        //               << ", eta = " << jets[iJet].eta()
        //               << ", phi = " << jets[iJet].phi() << "\n";
        // }
        tree->Fill();
        delete event;
    }

    tree->AutoSave();
    tfile->Close();
    // Statistics on the simulation
    pythia.stat();

    return 0;
}