#pragma once

#include <TObject.h>
#include <vector>

using std::vector;

enum class JetType { antikt, kt, cambridge, siscone };

struct Jet : public TObject {
  Jet() = default;
  virtual ~Jet() {}
  vector<float> pt;
  vector<float> eta;
  vector<float> phi;
  vector<float> energy;
  vector<float> mass;

  void setJet(const float& pt,const float& eta,const float& phi,const float& energy,const float& mass) {
    this->pt.push_back(pt);
    this->eta.push_back(eta);
    this->phi.push_back(phi);
    this->energy.push_back(energy);
    this->mass.push_back(mass);
  }

  vector<int> constituents_useridx;
  vector<int> flavor;
  vector<float> constituentPt;
  vector<float> constituentEnergy;
  vector<float> deltaR;

  void setJetConstituents(const int& cstind, const float& pt, const float& e, const int &flavor, const float& deltaR) 
  { 
    this->constituents_useridx.push_back(cstind);
    this->constituentPt.push_back(pt);
    this->constituentEnergy.push_back(e);
    this->flavor.push_back(flavor);
    this->deltaR.push_back(deltaR);
  }
  

  ClassDefNV(Jet, 1);
};

class PythiaEvent : public TObject {
public:
  PythiaEvent();
  PythiaEvent(int evNumber);
  virtual ~PythiaEvent();

  void setTrack(float px, float py, float pz, float energy);
  void setJet(float pt, float eta, float phi, float energy, float mass,
              JetType jetType);

  Jet getJet(JetType jetType) const;
  void setJetFound(bool jetFound) { this->jetFound = jetFound; }
  bool isJetFound() const { return jetFound; }

  vector<float> getTrackPx() const { return px; }
  vector<float> getTrackPy() const { return py; }
  vector<float> getTrackPz() const { return pz; }
  vector<float> getTrackEnergy() const { return energy; }

  void setJetSubstructure(const int& cstind, const float& pt, const float& e, const int& flavor, const float& deltaR, const JetType jetType);
  void setEnergyFraction(const double &energyFraction) { this->energyFraction = energyFraction; }

private:
  int eventNumber;
  vector<float> px, py, pz, energy;
  bool jetFound;

  double energyFraction;

  Jet antiktJet;
  Jet ktJet;
  Jet cambridgeJet;
  Jet sisconeJet;

  ClassDef(PythiaEvent, 1);
};
