import torch
import torch.nn as nn
from torch.optim.lr_scheduler import StepLR
from torch.utils.tensorboard import SummaryWriter

from torch_geometric.loader import DataLoader
import yaml
from dotmap import DotMap
import argparse
from tqdm import tqdm

from pydir.network.model import JetClassifierGNN, JetPredictorGNN
from pydir.dataio.JETStreamData import dJETStream
from pydir.jutils.loss import JetLoss, MultiPredLoss


def train(model, dataloader, optimizer, config, writer, epoch):
  
  total_loss = 0
  for ii,data in enumerate(tqdm(dataloader)):
    optimizer.zero_grad()
    njets, jet_properties = model(data)
    loss = MultiPredLoss(njets, jet_properties, data.njets, data.jets)
    
    loss.backward()
    optimizer.step()

    # print("loss", loss)
    # print("true count", data.njets)
    # print("true props shape", data.jets.shape)
    # print("pred count", njets)
    # print("pred props shape", jet_properties.shape)
    # if ii>50:
    #   break

    # total_loss += loss.item()
    
  # if config.training.verbose:
  #   if epoch % config.training.log_interval == 0:
  #     print(f'Epoch {epoch+1}, Loss: {total_loss/len(dataloader)}')
  #     # print(f'Epoch {epoch+1}, Loss: {total_loss}')

  # writer.add_scalar("Loss/train", total_loss, epoch)
  # writer.add_scalar("Loss/train_jet", loss, epoch)

  print("Training complete for epoch:", epoch+1)

def test(model, dataloader, writer, epoch):
  model.eval()
  validation_loss = 0
  valid_loss_jets = 0
  correct = 0
  with torch.no_grad():
    for data in dataloader:
      njets, jet_properties = model(data)
      validation_loss = MultiPredLoss(njets, jet_properties, data.njets, data.jets)
      # validation_loss += criterion[1](njets, data.njets.unsqueeze(0)).item()
      # valid_loss_jets += criterion[1](jet_properties, data.jets).item() 

  #     correct += pred.eq(data.y.view_as(pred)).sum().item()

  validation_loss /= len(dataloader)
  # valid_loss_jets /= len(dataloader)

  print(f'Validation set: Average loss: {validation_loss}, Accuracy: {correct}/{len(dataloader.dataset)} ({100. * correct / len(dataloader.dataset)}%)')

  # print(f'Validation set: Average loss jets: {valid_loss_jets}')

  # writer.add_scalar("Loss/Validation", validation_loss, epoch)
  # writer.add_scalar("Loss/Validation_Jets", valid_loss_jets, epoch)
  # writer.add_scalar("Accuracy/Validation", 100. * correct / len(dataloader.dataset), epoch)

  return validation_loss,valid_loss_jets

def main(yaml_path):
  config = DotMap(yaml.safe_load(open(yaml_path)))
  writer = SummaryWriter(log_dir=config.paths.log_dir)

  # Hyperparameters
  input_dim = 4  # (px, py, pz, energy)
  hidden_dim = config.hyper_params.hidden_dim

  # Model, loss, and optimizer
  # model = JetClassifierGNN(input_dim, hidden_dim, n_jets=config.analysis.n_jets)
  model = JetPredictorGNN(input_dim, hidden_dim, jet_properties_dim=5)
  optimizer = torch.optim.Adam(model.parameters(), lr=config.hyper_params.learning_rate) 
  dataset_train = dJETStream(config.paths.data_train)
  dataloader_train = DataLoader(dataset_train, batch_size=config.hyper_params.batch_size, shuffle=False)

  dataset_valid = dJETStream(config.paths.data_valid)
  dataloader_valid = DataLoader(dataset_valid, batch_size=config.hyper_params.batch_size, shuffle=False)

  scheduler = StepLR(optimizer, step_size=1, gamma=config.hyper_params.gamma)

  
  print("#" * 25)
  print("\n" * 1)
  print("\t\t\t\t\t\t TRAINING %i EPOCHS" % config.training.epochs)
  print("\n" * 1)
  print("#" * 25)
  loss_hist = 0
  best_epoch = 0
  n_patience = 0
  for epoch in range(int(config.training.epochs)):
    train(model, dataloader_train, optimizer, config, writer, epoch)
    # closs,jloss = test(model, dataloader_valid, writer, epoch)
    # loss = 0.2 * closs + 0.8 * jloss / (0.2 + 0.8)
    # if (loss < loss_hist) or (epoch == 0):
    #   loss_hist = loss
    #   best_epoch = epoch
    #   n_patience = 0
    #   if config.training.save_model:
    #     torch.save(model.state_dict(), config.paths.model)
    # else:
    #   n_patience += 1
    #   if n_patience == config.training.patience:
    #     print(f'Early stopping at epoch {epoch+1}')
    #     break
    # if epoch % config.hyper_params.epoch_lr_step == 0:
    #   scheduler.step()


  print(f'Best model found at epoch {best_epoch+1} with loss {loss_hist}')
  writer.flush()
  writer.close()

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Train a GNN to classify jets')
  parser.add_argument('--config', type=str, help='Path to config file')
  args = parser.parse_args()
  
  main(args.config)