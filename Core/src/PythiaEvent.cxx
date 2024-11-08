#include "PythiaEvent.h"

ClassImp(PythiaEvent)

    PythiaEvent::PythiaEvent()
    : TObject(), eventNumber(0), px(0), py(0), pz(0), energy(0){

                                                      };

PythiaEvent::PythiaEvent(int evNumber)
    : TObject(), eventNumber(evNumber), px(0), py(0), pz(0), energy(0) {
        // constructor
      };

PythiaEvent::~PythiaEvent() {};

void PythiaEvent::setEvProperties(float x, float y, float z, float e) {
  setPx(x);
  setPy(y);
  setPz(z);
  setEnergy(e);
};

void PythiaEvent::setJet(float pt, float eta, float phi, float energy, float mass, int jetType) {
  switch (jetType) {
    case 0:
      antiktJet.setJet(pt, eta, phi, energy, mass);
      break;
    case 1:
      ktJet.setJet(pt, eta, phi, energy, mass);
      break;
    case 2:
      cambridgeJet.setJet(pt, eta, phi, energy, mass);
      break;
    case 3:
      sisconeJet.setJet(pt, eta, phi, energy, mass);
      break;
    default:
      break;
  }
};
