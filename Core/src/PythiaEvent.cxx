#include "PythiaEvent.h"

ClassImp(PythiaEvent);
ClassImp(Jet);

PythiaEvent::PythiaEvent()
    : TObject(), eventNumber(0), px(0), py(0), pz(0), energy(0), jetFound(false) {};


PythiaEvent::PythiaEvent(int evNumber)
    : TObject(), eventNumber(evNumber), px(0), py(0), pz(0), energy(0), jetFound(false) {};

PythiaEvent::~PythiaEvent() {};

void PythiaEvent::setTrack(float px, float py, float pz, float energy) {
  this->px.push_back(px);
  this->py.push_back(py);
  this->pz.push_back(pz);
  this->energy.push_back(energy);
};

void PythiaEvent::setJet(float pt, float eta, float phi, float energy,
                         float mass, JetType jetType) {
  switch (jetType) {
  case JetType::antikt:
    antiktJet.setJet(pt, eta, phi, energy, mass);
    break;
  case JetType::kt:
    ktJet.setJet(pt, eta, phi, energy, mass);
    break;
  case JetType::cambridge:
    cambridgeJet.setJet(pt, eta, phi, energy, mass);
    break;
  case JetType::siscone:
    sisconeJet.setJet(pt, eta, phi, energy, mass);
    break;
  default:
    break;
  }
};

Jet PythiaEvent::getJet(JetType jetType) const
{
  switch (jetType) {
  case JetType::antikt:
    return antiktJet;
    break;
  case JetType::kt:
    return ktJet;
    break;
  case JetType::cambridge:
    return cambridgeJet;
    break;
  case JetType::siscone:
    return sisconeJet;
    break;
  default:
    break;
  }
};

