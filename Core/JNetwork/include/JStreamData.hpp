#pragma once

#include <torch/torch.h>
#include <TFile.h>
#include <TTree.h>

#include "PythiaEvent.hpp"

namespace jetstream {
namespace data {

class JStreamData : public torch::data::Dataset<JStreamData> {
public:
  JStreamData() = default;

  explicit JStreamData(const char* path) : 
  f{new TFile(path, "READ")},
  tree{dynamic_cast<TTree*>(f->Get("tree"))},
  event{nullptr}
  {
  size_ = tree->GetEntries();
  tree->SetBranchAddress("event", &event);
  }

  torch::data::Example<> get(size_t index) override {
    tree->GetEntry(index);
    const int nJets = event->getNJets();

    const auto jet = event->getJet(JetType::antikt);
    auto jetPt = jet.getPt();
    auto jetEta = jet.getEta();
    auto jetPhi = jet.getPhi();
    auto jetEnergy = jet.getEnergy();
    auto jetMass = jet.getMass();

    auto energyFraction = event->getEnergyFraction();
  
    auto data = torch::from_blob(jetPt.data(), {static_cast<int64_t>(jetPt.size())}, dataFloatOpts);
    return {data, torch::empty({0})};
    // return {torch::zeros({3, 32, 32}), torch::zeros({1})};
  }

  torch::optional<size_t> size() const override {
    return size_;
  }

private:
  int64_t size_{-1};
  TFile *f;
  TTree *tree;
  PythiaEvent *event;

  torch::TensorOptions dataFloatOpts{torch::TensorOptions().dtype(torch::kFloat).device(torch::kCPU)};

};

} // namespace data
} // namespace jetstream
