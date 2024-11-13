import torch
from torch.utils.data import Dataset
from torch_geometric.data import Data

import ROOT
ROOT.gInterpreter.AddIncludePath("Core/include")
ROOT.gInterpreter.ProcessLine('#include "Core/include/PythiaEvent.h"')
ROOT.gSystem.Load("libPythiaEvent.so")

class dJETStream(Dataset):
  def __init__(self, file_path="tmp.root", tree_name="tree", transform=False, np_data=True,n_jets=20):
    self.file = ROOT.TFile(file_path)
    self.tree = self.file.Get(tree_name)
    self._entries = self.tree.GetEntries()
    self.event = ROOT.PythiaEvent()
    self.tree.SetBranchAddress("event", self.event)

    self.max_jets = n_jets

  def __len__(self):
    return self._entries

  def __getitem__(self, idx):
    self.tree.GetEntry(idx)
        
    fJet = self.event.isJetFound()
    y = torch.tensor([fJet],dtype=torch.long)
    
    X = torch.tensor([self.event.getTrackPx(), self.event.getTrackPy(), 
                      self.event.getTrackPz(), self.event.getTrackEnergy()], dtype=torch.float)
    
    jets = self.jetFinder(fJet)
    data = self.create_event(X, y)
    data.jets = jets
    
    return data
    
  def create_event(self, x, label):
    x = x.t() 

    num_nodes = x.size(0)
    edge_index = torch.combinations(torch.arange(num_nodes), r=2).t()

    edge_index = torch.cat([edge_index, edge_index.flip(0)], dim=1)
    edge_index = edge_index[:, (edge_index < num_nodes).all(dim=0)]

    return Data(x=x, edge_index=edge_index, y=label)
  
  def jetFinder(self, fJet):

    if fJet:
      jet = self.event.getJet(ROOT.JetType.antikt)
      jets = torch.tensor([jet.pt, jet.eta, jet.phi, jet.energy, jet.mass], dtype=torch.float).t()
      if len(jets.shape) < 2:
        jets = jets.view(1, 5) 

    else:
      jets = torch.zeros(5, dtype=torch.float)
      jets = jets.view(1, 5)

    num_padding = self.max_jets - len(jets)
    if num_padding > 0:
      jets = torch.cat([jets, torch.zeros(num_padding, 5)], dim=0)
    return jets

  def getEntries(self):
    return self._entries

