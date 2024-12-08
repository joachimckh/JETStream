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
        
    y = torch.tensor([self.event.isJetFound()],dtype=torch.long)
    
    X = torch.tensor([self.event.getTrackPx(), self.event.getTrackPy(), 
                      self.event.getTrackPz(), self.event.getTrackEnergy()], dtype=torch.float)

    return self.create_event(X, y)
    
  def create_event(self, x, label):
    x = x.t() 

    num_nodes = x.size(0)
    edge_index = torch.combinations(torch.arange(num_nodes), r=2).t()

    edge_index = torch.cat([edge_index, edge_index.flip(0)], dim=1)
    edge_index = edge_index[:, (edge_index < num_nodes).all(dim=0)]

    return Data(x=x, edge_index=edge_index, y=label)

  def getEntries(self):
    return self._entries

# Padding function to handle variable-length input for batching
def pad_collate(batch):
  max_num_nodes = max([data.x.size(0) for data, _ in batch])
  padded_x = [torch.cat([data.x, torch.zeros(max_num_nodes - data.x.size(0), data.x.size(1))], dim=0) for data, _ in batch]
  edge_index = [data.edge_index for data, _ in batch] 
  labels = torch.stack([label for _, label in batch])
  return padded_x, edge_index, labels

if __name__=="__main__":
  dataset = dJETStream()
  print(dataset.getEntries())

  tmp = dataset.__getitem__(3)
  print("#" * 10)
  print("data: ", tmp)
  print("-" * 10)