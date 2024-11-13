import torch
import torch.nn as nn
from torch.optim.lr_scheduler import StepLR
from torch.utils.tensorboard import SummaryWriter

from torch_geometric.loader import DataLoader
import yaml
from dotmap import DotMap
import argparse
from tqdm import tqdm

from pydir.network.model import JetClassifierGNN
from pydir.dataio.JETStreamData import dJETStream
from pydir.jutils.loss import JetLoss


def train(model, dataloader, criterion, optimizer, config, writer, epoch):
  
  total_loss = 0
  for data in tqdm(dataloader):
    optimizer.zero_grad()
    out, jets = model(data)
    loss = criterion[0](out, data.y)
    loss_jet = criterion[1](jets, data.jets)
    loss_jet = JetLoss(jets, loss_jet, penaltyfactor=config.hyper_params.jet_penaltyfactor)
    total_loss += loss.item() + loss_jet.item()
    loss.backward()
    optimizer.step()
  
  if config.training.verbose:
    if epoch % config.training.log_interval == 0:
      print(f'Epoch {epoch+1}, Loss: {total_loss/len(dataloader)}')

  writer.add_scalar("Loss/train", total_loss, epoch)
  writer.add_scalar("Loss/train_jet", loss_jet, epoch)
  writer.add_scalar("Loss/train_classifier", loss, epoch)

  print("Training complete for epoch:", epoch+1)

def test(model, dataloader, criterion, writer, epoch):
  model.eval()
  validation_loss = 0
  valid_loss_jets = 0
  correct = 0
  with torch.no_grad():
    for data in dataloader:
      out, jets = model(data)
      validation_loss += criterion[0](out, data.y).item()
      loss_jet = criterion[1](jets, data.jets).item() 
      valid_loss_jets += JetLoss(jets, loss_jet, penaltyfactor=config.hyper_params.jet_penaltyfactor).item() 
      pred = out.argmax(dim=1, keepdim=True)
      correct += pred.eq(data.y.view_as(pred)).sum().item()

  validation_loss /= len(dataloader)
  valid_loss_jets /= len(dataloader)

  print(f'Validation set: Average loss: {validation_loss}, Accuracy: {correct}/{len(dataloader.dataset)} ({100. * correct / len(dataloader.dataset)}%)')

  print(f'Validation set: Average loss jets: {valid_loss_jets}')

  writer.add_scalar("Loss/Validation", validation_loss, epoch)
  writer.add_scalar("Loss/Validation_Jets", valid_loss_jets, epoch)
  writer.add_scalar("Accuracy/Validation", 100. * correct / len(dataloader.dataset), epoch)

  return validation_loss

def main(yaml_path):
  config = DotMap(yaml.safe_load(open(yaml_path)))
  writer = SummaryWriter(log_dir=config.paths.log_dir)

  # Hyperparameters
  input_dim = 4  # (px, py, pz, energy)
  hidden_dim = config.hyper_params.hidden_dim
  output_dim = 2  # Binary classification (jet or no jet)

  # Model, loss, and optimizer
  model = JetClassifierGNN(input_dim, hidden_dim, output_dim,n_jets=config.analysis.n_jets)
  criterion_Classifier = nn.CrossEntropyLoss()
  criterion_Jet = nn.MSELoss()
  criterion = [criterion_Classifier, criterion_Jet]
  optimizer = torch.optim.Adam(model.parameters(), lr=config.hyper_params.learning_rate) 
  dataset_train = dJETStream(config.paths.data_train, n_jets=config.analysis.n_jets)
  dataloader_train = DataLoader(dataset_train, batch_size=config.hyper_params.batch_size, shuffle=False)

  dataset_valid = dJETStream(config.paths.data_valid, n_jets=config.analysis.n_jets)
  dataloader_valid = DataLoader(dataset_valid, batch_size=config.hyper_params.batch_size, shuffle=False)

  scheduler = StepLR(optimizer, step_size=1, gamma=config.hyper_params.gamma)

  loss_hist = 0
  best_epoch = 0
  print("#" * 25)
  print("\n" * 2)
  print("\t\t\t\t\t\t TRAINING %i EPOCHS" % config.training.epochs)
  print("\n" * 2)
  print("#" * 25)
  for epoch in range(int(config.training.epochs)):
    train(model, dataloader_train, criterion, optimizer, config, writer, epoch)
    loss = test(model, dataloader_valid, criterion, writer, epoch)
    if (loss < loss_hist) or (epoch == 0):
      loss_hist = loss
      best_epoch = epoch
      if config.training.save_model:
        torch.save(model.state_dict(), config.paths.model)

    if epoch % config.hyper_params.epoch_lr_step == 0:
      scheduler.step()


  print(f'Best model found at epoch {best_epoch+1} with loss {loss_hist}')
  writer.flush()
  writer.close()

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Train a GNN to classify jets')
  parser.add_argument('--config', type=str, help='Path to config file')
  args = parser.parse_args()
  
  main(args.config)