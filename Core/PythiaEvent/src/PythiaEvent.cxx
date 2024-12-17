#include "PythiaEvent.hpp"

ClassImp(PythiaEvent);
ClassImp(Jet);

PythiaEvent::PythiaEvent()
    : TObject(), eventNumber{0}, px{0}, py{0}, pz{0}, energy{0}, jetFound{false}, energyFraction{0} {};


PythiaEvent::PythiaEvent(int evNumber)
    : TObject(), eventNumber{evNumber}, px{0}, py{0}, pz{0}, energy{0}, jetFound{false}, energyFraction{0} {};

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

void PythiaEvent::setJetSubstructure(const int& cstind, const float& pt, const float& e, const int& flavor, const float& deltaR, const JetType jetType)
{
  switch (jetType) {
  case JetType::antikt:
    antiktJet.setJetConstituents(cstind, pt, e, flavor, deltaR);
    break;
  case JetType::kt:
    ktJet.setJetConstituents(cstind, pt, e, flavor, deltaR);
    break;
  case JetType::cambridge:
    cambridgeJet.setJetConstituents(cstind, pt, e, flavor, deltaR);
    break;
  case JetType::siscone:
    sisconeJet.setJetConstituents(cstind, pt, e, flavor, deltaR);
    break;
  default:
    break;
  }
};

