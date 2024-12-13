import torch
from torch.utils.data import Dataset
from torch_geometric.data import Data

import ROOT
ROOT.gInterpreter.AddIncludePath("Core/include")
ROOT.gInterpreter.ProcessLine('#include "Core/include/PythiaEvent.h"')
ROOT.gSystem.Load("libPythiaEvent.so")

class dJETStream(Dataset):
  def __init__(self, file_path="tmp.root", tree_name="tree", transform=False, np_data=True):
    self.file = ROOT.TFile(file_path)
    self.tree = self.file.Get(tree_name)
    self._entries = self.tree.GetEntries()
    self.event = ROOT.PythiaEvent()
    self.tree.SetBranchAddress("event", self.event)


  def __len__(self):
    return self._entries

  def __getitem__(self, idx):
    self.tree.GetEntry(idx)
        
    fJet = self.event.isJetFound()
    
    X = torch.tensor([self.event.getTrackPx(), self.event.getTrackPy(), 
                      self.event.getTrackPz(), self.event.getTrackEnergy()], dtype=torch.float)
    
    jets, number_of_jets = self.jetFinder(fJet)
    
    # data.jets = jets
    # data.number_of_jets = number_of_jets
    data = self.create_event(X, jets, number_of_jets)
    return data
    
  def create_event(self, x, jets, njets):
    x = x.t() 

    num_nodes = x.size(0)
    edge_index = torch.combinations(torch.arange(num_nodes), r=2).t()

    edge_index = torch.cat([edge_index, edge_index.flip(0)], dim=1)
    edge_index = edge_index[:, (edge_index < num_nodes).all(dim=0)]

    return Data(x=x, edge_index=edge_index, jets=jets, njets=njets)
  
  def jetFinder(self, fJet):
    number_of_jets = 0
    jets= torch.empty(0,5)
    if fJet:
      jet = self.event.getJet(ROOT.JetType.antikt)
      jets = torch.tensor([jet.pt, jet.eta, jet.phi, jet.energy, jet.mass], dtype=torch.float).t()
      number_of_jets = jets.size(0)
      if len(jets.shape) < 2:
        jets = jets.view(1, 5) 

    return jets, torch.tensor([number_of_jets])

  def getEntries(self):
    return self._entries

