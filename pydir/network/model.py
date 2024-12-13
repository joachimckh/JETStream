import torch
import torch.nn as nn
import torch.nn.functional as F
from torch_geometric.nn import GraphConv, global_mean_pool

class JetClassifierGNN(nn.Module):
  def __init__(self, input_dim, hidden_dim, n_jets=20):
    super(JetClassifierGNN, self).__init__()
    self.conv1 = GraphConv(input_dim, hidden_dim)
    self.conv2 = GraphConv(hidden_dim, hidden_dim)
    
    self.fc = nn.Linear(hidden_dim, 1)
    
    self.fc_jets = nn.Linear(hidden_dim, n_jets * 5)
    self.n_jets = n_jets
    
    # nn.init.xavier_uniform_(self.fc_count.weight)
    nn.init.uniform_(self.fc_count.weight, -0.1, 0.1)
    nn.init.zeros_(self.fc_count.bias)

  def forward(self, data):
    x, edge_index, batch = data.x, data.edge_index, data.batch
    x = F.relu(self.conv1(x, edge_index))
    x = F.relu(self.conv2(x, edge_index))
    
    x = global_mean_pool(x, batch)
    
    out = self.fc(x)
    out = torch.sigmoid(out)
    
    jets = self.fc_jets(x)
    jets = jets.view(-1, self.n_jets, 5)

    pt = F.relu(jets[:, :, 0]) 
    eta = jets[:, :, 1]
    phi = torch.sigmoid(jets[:, :, 2]) * (2 * torch.pi)
    energy = F.relu(jets[:, :, 3])
    mass = F.relu(jets[:, :, 4])

    jets = torch.stack((pt, eta, phi, energy, mass), dim=-1)
    
    return out, jets
  

class JetPredictorGNN(nn.Module):
  def __init__(self, input_dim, hidden_dim, jet_properties_dim=5):
    super(JetPredictorGNN, self).__init__()
    
    self.conv1 = GraphConv(input_dim, hidden_dim)
    self.conv2 = GraphConv(hidden_dim, hidden_dim)
    
    self.fc_count = nn.Linear(hidden_dim, 1)
    
    self.fc_jet_properties = nn.Linear(hidden_dim, jet_properties_dim)

  def forward(self, data):
    x, edge_index, batch = data.x, data.edge_index, data.batch
    
    x = F.relu(self.conv1(x, edge_index))
    x = F.relu(self.conv2(x, edge_index))
    x = global_mean_pool(x, batch)
    
    # jet_count = torch.relu(self.fc_count(x)).round().squeeze()
    jet_count = torch.clamp(self.fc_count(x), 0, 20).round()
    
    jet_properties = []
    for i in range(int(jet_count.item())):
      jet_properties.append(self.fc_jet_properties(x))
    jet_properties = torch.stack(jet_properties) if jet_properties else torch.empty((0, self.fc_jet_properties.out_features))

    return jet_count, jet_properties.squeeze()
  