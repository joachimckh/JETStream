#ifndef PYTHIAEVENT_H
#define PYTHIAEVENT_H

#include <TObject.h>
#include <vector>

using std::vector;

struct Jet {
  std::vector<float> pt;
  std::vector<float> eta;
  std::vector<float> phi;
  std::vector<float> energy;
  std::vector<float> mass;

  void setJet(float pt, float eta, float phi, float energy, float mass) {
    this->pt.push_back(pt);
    this->eta.push_back(eta);
    this->phi.push_back(phi);
    this->energy.push_back(energy);
    this->mass.push_back(mass);
  }
};

class PythiaEvent : public TObject {
public:
  PythiaEvent();
  PythiaEvent(int evNumber);
  virtual ~PythiaEvent();

  void setEnergy(float e) { energy.push_back(e); }
  void setEvProperties(float x, float y, float z, float e);
  
  void setJet(float pt, float eta, float phi, float energy, float mass, int jetType);

private:
  int eventNumber;
  vector<float> px;
  vector<float> py;
  vector<float> pz;
  vector<float> energy;

  Jet antiktJet;
  Jet ktJet;
  Jet cambridgeJet;
  Jet sisconeJet;

  void setPx(float x) { px.push_back(x); }
  void setPy(float y) { py.push_back(y); }
  void setPz(float z) { pz.push_back(z); }
  

  ClassDef(PythiaEvent, 1);
};

#endif // PYTHIAEVENT_H
