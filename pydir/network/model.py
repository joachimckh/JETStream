import torch
import torch.nn as nn
import torch.nn.functional as F
from torch_geometric.nn import GraphConv, global_mean_pool

class JetClassifierGNN(nn.Module):
  def __init__(self, input_dim, hidden_dim, output_dim, n_jets=20):
    super(JetClassifierGNN, self).__init__()
    self.conv1 = GraphConv(input_dim, hidden_dim)
    self.conv2 = GraphConv(hidden_dim, hidden_dim)
    
    self.fc = nn.Linear(hidden_dim, output_dim)
    
    self.fc_jets = nn.Linear(hidden_dim, n_jets * 5)
    self.n_jets = n_jets

  def forward(self, data):
    x, edge_index, batch = data.x, data.edge_index, data.batch
    x = F.relu(self.conv1(x, edge_index))
    x = F.relu(self.conv2(x, edge_index))
    
    x = global_mean_pool(x, batch)
    
    out = self.fc(x)
    
    jets = self.fc_jets(x)
    jets = jets.view(-1, self.n_jets, 5)

    pt = F.relu(jets[:, :, 0]) 
    eta = jets[:, :, 1]
    phi = torch.sigmoid(jets[:, :, 2]) * (2 * torch.pi)
    energy = F.relu(jets[:, :, 3])
    mass = F.relu(jets[:, :, 4])

    jets = torch.stack((pt, eta, phi, energy, mass), dim=-1)
    
    return out, jets
  