import torch
from torch.utils.data import Dataset
from torch_geometric.data import Data

from torch_geometric.utils import get_laplacian, to_dense_adj

import ROOT
# ROOT.gInterpreter.AddIncludePath("src/core/include")
# ROOT.gInterpreter.ProcessLine('#include "src/core/include/PythiaEvent.h"')
# ROOT.gSystem.Load("libPythiaEvent.so")

ROOT.gInterpreter.AddIncludePath("src/core/include")
ROOT.gInterpreter.ProcessLine('#include "src/core/include/PythiaEvent.hpp"')
ROOT.gSystem.Load("build/src/core/libcore.dylib")

class dJETStream(Dataset):
  def __init__(self, file_path="tmp.root", tree_name="tree", transform=False, np_data=True):
    self.file = ROOT.TFile(file_path)
    self.tree = self.file.Get(tree_name)
    self._entries = self.tree.GetEntries()
    self.event = ROOT.Jet()
    self.tree.SetBranchAddress("event", self.event)


  def __len__(self):
    return self._entries

  def __getitem__(self, idx):
    self.tree.GetEntry(idx)
        
    
    # X = torch.tensor([self.event.getTrackPx(), self.event.getTrackPy(), 
    #                   self.event.getTrackPz(), self.event.getTrackEnergy()], dtype=torch.float)

    X, dij = self.constituentTensor()
    
    motherJetPid = self.jetFinder()
    tar = self.createTargetSpecies(motherJetPid)
    
    data = self.createEvent(X, dij, tar)
    return data
  
  def jetFinder(self):
    jet = self.event
    # jetProperties = torch.tensor([jet.getPt(), 
    #                      jet.getEta(), 
    #                      jet.getPhi(), 
    #                      jet.getEnergy(), 
    #                      jet.getMass()], dtype=torch.float)#.t()
    
    # lead jet, this is the target
    motherJetPid = jet.getMotherPID()
    # print("leadJetPid", motherJetPid)

    return motherJetPid
  
  def constituentTensor(self):
    jet = self.event

    constituentpT = torch.tensor(jet.getConstituentPt(), dtype=torch.float)
    constituentEta = torch.tensor(jet.getConstituentEta(), dtype=torch.float)
    constituentPhi = torch.tensor(jet.getConstituentPhi(), dtype=torch.float)
    constituentE = torch.tensor(jet.getConstituentEnergy(), dtype=torch.float)
    constituentCharge = torch.tensor(jet.getConstituentCharge(), dtype=torch.float)
    constituentFractionalPt = torch.tensor(jet.getConstituentPt(), dtype=torch.float) / jet.getPt()
    constituentFractionalEnergy = torch.tensor(jet.getConstituentEnergy(), dtype=torch.float) / jet.getEnergy()
    detaij = torch.tensor(jet.getConstituentEta(), dtype=torch.float) - jet.getEta()
    dphij = torch.tensor(jet.getConstituentPhi(), dtype=torch.float) - jet.getPhi()

    X = torch.stack([constituentpT, 
                     constituentEta, 
                     constituentPhi, 
                     constituentE, 
                     constituentCharge,
                     constituentFractionalPt,
                     constituentFractionalEnergy
                     ], dim=1)
    dij = torch.sqrt(detaij**2 + dphij**2) # distance between constituents and jet axis
    return X, dij
  
  def createEvent(self, x, dij, tar):
    # x = x.t() 

    
    num_nodes = x.size(0)
    # print("num_nodes", num_nodes)
    tensorEta = x[:, 1].view(-1,1).expand(num_nodes,2) # eta
    tensorPhi = x[:, 2].view(-1,1).expand(num_nodes,2) # phi

    dEta_ij = (tensorEta[:, 0] - tensorEta[:,1].view(-1,1))**2 # delta eta_ij = eta_i - eta_j
    dPhi_ij = (tensorPhi[:, 0] - tensorPhi[:,1].view(-1,1))**2

    d_ij = torch.sqrt(dEta_ij + dPhi_ij) # adjacency matrix
    # print("d_ij")
    # print(d_ij)
    edge_index = d_ij.nonzero().t().contiguous()
    edge_weight = d_ij[edge_index[0], edge_index[1]]
    # edge_index = to_edge_index(d_ij)

    # print("edge_index")
    # print(edge_index)
    # print("edge_weight")
    # print(edge_weight)


    return Data(x=x, edge_index=edge_index, edge_attr=edge_weight, tar=tar, num_nodes=num_nodes)
  
  def getEntries(self):
    return self._entries
  
    

  def createTargetSpecies(self, pid):
    # b-tag
    # c-tag
    # tau-tag
    # lf-tag 
    # some other time
    tmp = -999
    if abs(pid) == 21:
      tmp = 0 # gluon
    elif 1 <= abs(pid) <= 6:
      tmp = 1 # quark
    else:
      tmp = 2 # other
    # gluon or quark tag or something else
    y = torch.tensor([0,0,0])
    y[tmp] = 1
    return y
