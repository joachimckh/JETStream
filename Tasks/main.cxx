#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"

#include <iostream>
#include <vector>


#include <TFile.h>
#include <TTree.h>
#include "PythiaEvent.h"

using namespace Pythia8;
using namespace fastjet;


int main() {
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

    
    constexpr int numEvents = 500;
    
    for (int iEvent = 0; iEvent < numEvents; ++iEvent) {
        if (!pythia.next()) continue;  
        event = new PythiaEvent(iEvent);
        
        std::vector<PseudoJet> particles;

        
        for (int i = 0; i < pythia.event.size(); i++) {
            if (pythia.event[i].isFinal()) {
              event->setEvProperties(pythia.event[i].px(), 
                                     pythia.event[i].py(), 
                                     pythia.event[i].pz(), 
                                     pythia.event[i].e());

              particles.push_back(PseudoJet(pythia.event[i].px(),
                                              pythia.event[i].py(),
                                              pythia.event[i].pz(),
                                              pythia.event[i].e()));
                
            }
        }
        
        if (particles.size() < 2) continue;
        double R = 0.4;  
        JetDefinition jetDef(antikt_algorithm, R); 
        ClusterSequence cs(particles, jetDef);

        
        std::vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets(5.0)); /* min jet pt */

        
        std::cout << "Event #" << iEvent << " has " << jets.size() << " jets:\n";
        for (size_t iJet = 0; iJet < jets.size(); ++iJet) {
            std::cout << "  Jet " << iJet + 1 << ": pt = " << jets[iJet].pt()
                      << ", eta = " << jets[iJet].eta()
                      << ", phi = " << jets[iJet].phi() << "\n";
        }
        tree->Fill();
        delete event;
    }

    tree->AutoSave();
    tfile->Close();
    
    pythia.stat();

    return 0;
}