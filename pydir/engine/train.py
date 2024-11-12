import torch
import torch.nn as nn
from torch_geometric.loader import DataLoader
import yaml
from dotmap import DotMap
import argparse

from pydir.network.model import JetClassifierGNN
from pydir.dataio.JETStreamData import dJETStream


def trainer(yaml_path):
  config = DotMap(yaml.safe_load(open(yaml_path)))

  # Hyperparameters
  input_dim = 4  # (px, py, pz, energy)
  hidden_dim = config.hyper_params.hidden_dim
  output_dim = 2  # Binary classification (jet or no jet)

  # Model, loss, and optimizer
  model = JetClassifierGNN(input_dim, hidden_dim, output_dim)
  criterion = nn.CrossEntropyLoss()
  optimizer = torch.optim.Adam(model.parameters(), lr=config.hyper_params.learning_rate) 
  dataset = dJETStream()
  dataloader = DataLoader(dataset, batch_size=config.hyper_params.batch_size, shuffle=False);#, collate_fn=pad_collate)

  
  for epoch in range(int(config.training.epochs)):
    total_loss = 0
    for data in dataloader:
      optimizer.zero_grad()
    
      out = model(data)
      loss = criterion(out, data.y)
      total_loss += loss.item()
      
      loss.backward()
      optimizer.step()
      
    print(f'Epoch {epoch+1}, Loss: {total_loss/len(dataloader)}')


  print("Training complete")

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Train a GNN to classify jets')
  parser.add_argument('--config', type=str, help='Path to config file')
  args = parser.parse_args()
  
  trainer(args.config)