import sys,os

import numpy as np
import pandas as pd

import torch
from torch.utils.data import Dataset,IterableDataset
import torch.nn.functional as F
import operator
import copy

import ROOT
ROOT.gInterpreter.ProcessLine('#include "../../Core/include/PythiaEvent.h"')

class dJETStream(Dataset):
  def __init__(self, file_path="../../tree.root",tree_name="tree", transform=False,np_data=True):

    self.file = ROOT.TFile(file_path)
    self.tree = self.file.Get("%s".format(tree_name))

    self._entries = self.tree.GetEntries()
    # self.X = DataHandler(tracks_path,np_data)
    # self.y = SeparatedDataHandler(mov_path,np_data)['xamP'][:,2:]
    # self.transform = transform

  def _ROOT_Constructor(self):
    self.pt = ROOT.std.vector('float')()

  def __len__(self):
    return len(self.X)

  def __getitem__(self, idx):
    # x = self.X[idx,:]
    # y = self.y[idx,:]

    # if self.transform:
    #   x = self._transform(x)
    #   y = self._transform(y)

    # # x_tensor = torch.tensor(x,dtype=torch.float64)
    # # y_tensor = torch.tensor(y,dtype=torch.float64)
    # x_tensor = torch.from_numpy(x).float()
    # y_tensor = torch.from_numpy(y).float()


    return 0

  # def _transform(self,array):
  #   return (array - array.min())/(array.max()-array.min())

  # def _shape(self,):
  #   return self.X[2,:].shape[0]
  def getEntries(self):
    return self._entries
  
