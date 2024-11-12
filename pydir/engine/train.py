import torch
import torch.nn as nn
from torch_geometric.loader import DataLoader
import yaml
from dotmap import DotMap
import argparse

from pydir.network.model import JetClassifierGNN
from pydir.dataio.JETStreamData import dJETStream


def train(model, dataloader, criterion, optimizer, config,epoch):
  
  total_loss = 0
  for data in dataloader:
    optimizer.zero_grad()
  
    out = model(data)
    loss = criterion(out, data.y)
    total_loss += loss.item()
    
    loss.backward()
    optimizer.step()
  
  if config.training.verbose:
    if epoch % config.training.log_interval == 0:
      print(f'Epoch {epoch+1}, Loss: {total_loss/len(dataloader)}')


  print("Training complete for epoch:", epoch+1)

def test(model, dataloader, criterion):
  model.eval()
  validation_loss = 0
  correct = 0
  with torch.no_grad():
    for data in dataloader:
      out = model(data)
      validation_loss += criterion(out, data.y).item()
      pred = out.argmax(dim=1, keepdim=True)
      correct += pred.eq(data.y.view_as(pred)).sum().item()

  validation_loss /= len(dataloader.dataset)

  print(f'Validation set: Average loss: {validation_loss}, Accuracy: {correct}/{len(dataloader.dataset)} ({100. * correct / len(dataloader.dataset)}%)')

def main(yaml_path):
  config = DotMap(yaml.safe_load(open(yaml_path)))

  # Hyperparameters
  input_dim = 4  # (px, py, pz, energy)
  hidden_dim = config.hyper_params.hidden_dim
  output_dim = 2  # Binary classification (jet or no jet)

  # Model, loss, and optimizer
  model = JetClassifierGNN(input_dim, hidden_dim, output_dim)
  criterion = nn.CrossEntropyLoss()
  optimizer = torch.optim.Adam(model.parameters(), lr=config.hyper_params.learning_rate) 
  dataset_train = dJETStream(config.paths.data_train)
  dataloader_train = DataLoader(dataset_train, batch_size=config.hyper_params.batch_size, shuffle=False);#, collate_fn=pad_collate)

  dataset_valid = dJETStream(config.paths.data_valid)
  dataloader_valid = DataLoader(dataset_valid, batch_size=config.hyper_params.batch_size, shuffle=False);#, collate_fn=pad_collate)

  for epoch in range(int(config.training.epochs)):
    train(model, dataloader_train, criterion, optimizer, config,epoch)
    test(model, dataloader_valid, criterion)

  if config.training.save_model:
    torch.save(model.state_dict(), config.paths.model)

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Train a GNN to classify jets')
  parser.add_argument('--config', type=str, help='Path to config file')
  args = parser.parse_args()
  
  main(args.config)