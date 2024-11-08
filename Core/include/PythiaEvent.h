#ifndef PYTHIAEVENT_H
#define PYTHIAEVENT_H

#include <TObject.h>
#include <vector>

using std::vector;

class PythiaEvent : public TObject {
public:
  PythiaEvent();
  PythiaEvent(int evNumber);
  virtual ~PythiaEvent();

  void setEnergy(float e) { energy.push_back(e); }
  void setEvProperties(float x, float y, float z, float e);
  void setJetProperties(float pt, float eta, float phi);

private:
  int eventNumber;
  vector<float> px;
  vector<float> py;
  vector<float> pz;
  vector<float> energy;

  vector<float> jet_pt;
  vector<float> jet_eta;
  vector<float> jet_phi;

  void setPx(float x) { px.push_back(x); }
  void setPy(float y) { py.push_back(y); }
  void setPz(float z) { pz.push_back(z); }
  void setJetPt(float pt) { jet_pt.push_back(pt); }
  void setJetEta(float eta) { jet_eta.push_back(eta); }
  void setJetPhi(float phi) { jet_phi.push_back(phi); }

  ClassDef(PythiaEvent, 1);
};

#endif // PYTHIAEVENT_H
