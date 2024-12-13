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

  void setJet(float pt, float eta, float phi, float energy, float mass) {
    this->pt.push_back(pt);
    this->eta.push_back(eta);
    this->phi.push_back(phi);
    this->energy.push_back(energy);
    this->mass.push_back(mass);
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


private:
  int eventNumber;
  vector<float> px, py, pz, energy;
  bool jetFound;

  Jet antiktJet;
  Jet ktJet;
  Jet cambridgeJet;
  Jet sisconeJet;

  ClassDef(PythiaEvent, 1);
};
