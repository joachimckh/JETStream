import torch
import torch.nn as nn
import torch.nn.functional as F
from torch_geometric.nn import GraphConv, GCNConv, global_mean_pool

class JetClassifierGNN(nn.Module):
  def __init__(self, input_dim, hidden_dim):
    super(JetClassifierGNN, self).__init__()
    self.conv1 = GraphConv(input_dim, hidden_dim)
    self.conv2 = GraphConv(hidden_dim, hidden_dim)
    
    
    self.softmax = nn.Softmax(dim=1)
    
    self.final_layer_size = 3 # guess the mother 
    self.fc = nn.Linear(hidden_dim, self.final_layer_size)
    
    # nn.init.xavier_uniform_(self.fc_count.weight)
    # nn.init.uniform_(self.fc_count.weight, -0.1, 0.1)
    # nn.init.zeros_(self.fc_count.bias)

  def forward(self, data):
    x, edge_index, batch = data.x, data.edge_index, data.batch
    x = F.relu(self.conv1(x, edge_index))
    x = F.relu(self.conv2(x, edge_index))
    
    x = global_mean_pool(x, batch)
    
    out = self.fc(x)
    
    out = self.softmax(out)

    return out
  


class JetClassifierGNN(nn.Module):
  def __init__(self, input_dim, hidden_dim):
    super(JetClassifierGNN, self).__init__()
    self.conv1 = GCNConv(input_dim, hidden_dim)
    self.conv2 = GCNConv(hidden_dim, hidden_dim)
    self.softmax = nn.Softmax(dim=1)
    
    self.final_layer_size = 3
    self.fc = nn.Linear(hidden_dim, self.final_layer_size)

  def forward(self, data):
    x, edge_index, edge_weight, batch = data.x, data.edge_index, data.edge_attr, data.batch

    x = F.relu(self.conv1(x, edge_index, edge_weight))
    x = F.relu(self.conv2(x, edge_index, edge_weight))
    x = global_mean_pool(x, batch)
    out = self.fc(x)
    out = self.softmax(out)
    return out