#ifndef PYTHIAEVENT_H
#define PYTHIAEVENT_H

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

  Jet getJet() const { return antiktJet; }

private:
  int eventNumber;
  vector<float> px, py, pz, energy;

  Jet antiktJet;
  Jet ktJet;
  Jet cambridgeJet;
  Jet sisconeJet;

  ClassDef(PythiaEvent, 1);
};

#endif // PYTHIAEVENT_H
