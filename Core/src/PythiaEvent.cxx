#include "PythiaEvent.h"

ClassImp(PythiaEvent)

PythiaEvent::PythiaEvent():
TObject(),
eventNumber(0),
px(0),
py(0),
pz(0),
energy(0)
{
  // default constructor
}

PythiaEvent::PythiaEvent(int evNumber):
TObject(),
eventNumber(evNumber),
px(0),
py(0),
pz(0),
energy(0)
{
  // constructor
}

PythiaEvent::~PythiaEvent()
{
  // destructor
}

void PythiaEvent::setEvProperties(float x, float y, float z, float e)
{
  setPx(x); 
  setPy(y); 
  setPz(z); 
  setEnergy(e);
}