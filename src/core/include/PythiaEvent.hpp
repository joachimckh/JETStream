#pragma once

#include <TObject.h>
#include <vector>

using std::vector;

class Jet : public TObject {
public:
  Jet() = default;
  virtual ~Jet() {}

  void setJet(const float &pt, const float &eta, const float &phi,
              const float &energy, const float &mass) {
    this->pt = pt;
    this->eta = eta;
    this->phi = phi;
    this->energy = energy;
    this->mass = mass;
  }

  void setJetConstituents(const int &jetid, const int &cstind, const float &pt, const float& eta, const float &phi,
                          const float &e, const float& charge, const int &flavour,
                          const float &deltaR) {
    this->jetIdx.push_back(jetid);
    this->constituents_useridx.push_back(cstind);
    this->constituentPt.push_back(pt);
    this->constituentEta.push_back(eta);
    this->constituentPhi.push_back(phi);
    this->constituentEnergy.push_back(e);
    this->constituentCharge.push_back(charge);
    this->flavour.push_back(flavour);
    this->deltaR.push_back(deltaR);
  }

  void setLeadJet(const float &pt, const float &pid, const int& index) {
    this->leadingPt = pt;
    this->leadingPID = pid;
    this->leadingIndex = index;
  }

  void setEnergyFraction(const double &energyFraction) {
    this->energyFraction = energyFraction;
  }

  void setMotherPID(const int &pid) {
    this->motherPID = pid;
  }




  float getPt() const { return pt; }
  float getEta() const { return eta; }
  float getPhi() const { return phi; }
  float getEnergy() const { return energy; }
  float getMass() const { return mass; }

  vector<int> getConstituentUseridx() const { return constituents_useridx; }
  vector<int> getConstituentFlavour() const { return flavour; }
  vector<float> getConstituentPt() const { return constituentPt; }
  vector<float> getConstituentEta() const { return constituentEta; }
  vector<float> getConstituentPhi() const { return constituentPhi; }
  vector<float> getConstituentEnergy() const { return constituentEnergy; }
  vector<float> getConstituentCharge() const { return constituentCharge; }
  vector<float> getDeltaR() const { return deltaR; }

  vector<int> getJetIdx() const { return jetIdx; }

  float getLeadingPt() const { return leadingPt; }
  int getLeadingPID() const { return leadingPID; }
  int getLeadingIndex() const { return leadingIndex; }
  float getEnergyFraction() const { return energyFraction; }
  int getMotherPID() const { return motherPID; }

private:
  float pt;
  float eta;
  float phi;
  float energy;
  float mass;

  vector<int> constituents_useridx;
  vector<int> flavour;
  vector<float> constituentPt;
  vector<float> constituentEta;
  vector<float> constituentPhi;
  vector<float> constituentEnergy;
  vector<float> constituentCharge;
  vector<float> deltaR;

  float energyFraction;

  vector<int> jetIdx;

  int motherPID;
  int leadingPID;
  float leadingPt;
  int leadingIndex;

  ClassDefNV(Jet, 1);
};
