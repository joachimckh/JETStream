import torch.nn as nn
import torch.nn.functional as F
from torch_geometric.nn import GraphConv, global_mean_pool

class JetClassifierGNN(nn.Module):
  def __init__(self, input_dim, hidden_dim, output_dim):
    super(JetClassifierGNN, self).__init__()
    self.conv1 = GraphConv(input_dim, hidden_dim)
    self.conv2 = GraphConv(hidden_dim, hidden_dim)
    self.fc = nn.Linear(hidden_dim, output_dim)
  
  def forward(self, data):
    x, edge_index, batch = data.x, data.edge_index, data.batch
    x = F.relu(self.conv1(x, edge_index))
    x = F.relu(self.conv2(x, edge_index))
    
    x = global_mean_pool(x, batch)
    
    out = self.fc(x)
    
    return out
  
