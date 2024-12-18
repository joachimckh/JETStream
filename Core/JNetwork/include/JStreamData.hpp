#pragma once

#include <torch/torch.h>
#include <TFile.h>
#include <TTree.h>

#include "PythiaEvent.hpp"
#include "JUtility.hpp"

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

    auto ptTensor = torch::from_blob(jetPt.data(), {static_cast<int64_t>(jetPt.size())}, dataFloatOpts);
    auto etaTensor = torch::from_blob(jetEta.data(), {static_cast<int64_t>(jetEta.size())}, dataFloatOpts);
    auto phiTensor = torch::from_blob(jetPhi.data(), {static_cast<int64_t>(jetPhi.size())}, dataFloatOpts);
    auto energyTensor = torch::from_blob(jetEnergy.data(), {static_cast<int64_t>(jetEnergy.size())}, dataFloatOpts);
    auto massTensor = torch::from_blob(jetMass.data(), {static_cast<int64_t>(jetMass.size())}, dataFloatOpts);
    
    auto energyFractionTensor = torch::tensor(energyFraction, dataFloatOpts);


    auto labels = jet.getLeadingMotherPID();
    auto lab1 = jetstream::utils::getComposition(labels[0]);

    auto combinedData = torch::cat({ptTensor, etaTensor, phiTensor, energyTensor, massTensor, energyFractionTensor}, 0);

    return {combinedData, /* flavor */ torch::empty({0})};
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
  torch::TensorOptions dataIntOpts{torch::TensorOptions().dtype(torch::kInt).device(torch::kCPU)};

};

} // namespace data
} // namespace jetstream
