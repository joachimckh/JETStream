#ifndef PYTHIAEVENT_H
#define PYTHIAEVENT_H

#include <TObject.h>
#include <vector>

using std::vector;

class PythiaEvent : public TObject
{
 public:
  PythiaEvent();
  PythiaEvent(int evNumber);
  virtual ~PythiaEvent();

  void setPx(float x) { px.push_back(x); }
  void setPy(float y) { py.push_back(y); }
  void setPz(float z) { pz.push_back(z); }
  void setEnergy(float e) { energy.push_back(e); }
  void setEvProperties(float x, float y, float z, float e);

  private:
    int eventNumber;
    vector<float> px;
    vector<float> py;
    vector<float> pz;
    vector<float> energy;
  
  ClassDef(PythiaEvent, 1);
};

#endif // PYTHIAEVENT_H
