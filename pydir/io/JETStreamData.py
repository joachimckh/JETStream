import sys,os

import numpy as np

import torch
from torch.utils.data import Dataset
import torch.nn.functional as F

import ROOT
ROOT.gSystem.Load("../../libPythiaEvent.so")

class dJETStream(Dataset):
  def __init__(self, file_path="../../tmp.root",tree_name="tree", transform=False,np_data=True):

    self.file = ROOT.TFile(file_path)
    self.tree = self.file.Get(tree_name)

    self._entries = self.tree.GetEntries()

  def __len__(self):
    return len(self._entries)

  def __getitem__(self, idx):
    self.tree.GetEntry(idx)
    # event = self.tree.event
    input = np.array([self.tree.px, self.tree.py, self.tree.pz, self.tree.energy])

    # x = self.X[idx,:]
    # y = self.y[idx,:]

    # if self.transform:
    #   x = self._transform(x)
    #   y = self._transform(y)

    # # x_tensor = torch.tensor(x,dtype=torch.float64)
    # # y_tensor = torch.tensor(y,dtype=torch.float64)
    # x_tensor = torch.from_numpy(x).float()
    # y_tensor = torch.from_numpy(y).float()


    return input

  # def _transform(self,array):
  #   return (array - array.min())/(array.max()-array.min())

  # def _shape(self,):
  #   return self.X[2,:].shape[0]
  


if __name__=="__main__":
  dataset = dJETStream()
  print(dataset.getEntries())
  
  tmp = dataset.__getitem__(3)
  print("#"*10)
  print("data: ",tmp)
  print("-"*10)
