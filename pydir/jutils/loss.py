import torch
import torch.nn.functional as F

def JetLoss(predicted_jets, primary_loss, penaltyfactor=0.1):
  pt, _, phi, energy, mass = predicted_jets[..., 0], predicted_jets[..., 1], predicted_jets[..., 2], predicted_jets[..., 3], predicted_jets[..., 4]

  pt_penalty = torch.mean(F.relu(-pt))
  energy_penalty = torch.mean(F.relu(-energy))
  mass_penalty = torch.mean(F.relu(-mass))

  phi_penalty = torch.mean(F.relu(phi - 2 * torch.pi) + F.relu(-phi))

  penalty = pt_penalty + energy_penalty + mass_penalty + phi_penalty
  return primary_loss + penalty * penaltyfactor 

# def MultiPredLoss(predicted_count, predicted_properties, true_count, true_properties):
#   count_loss = F.mse_loss(predicted_count, true_count.float())
  
#   properties_loss = 0
#   for i in range(int(true_count.item())): # true count predictive count?? 
#     properties_loss += F.mse_loss(predicted_properties[i], true_properties[i])

#   total_loss = count_loss + properties_loss
#   return total_loss


def MultiPredLoss(predicted_count, predicted_properties, true_count, true_properties):
  print("predicted_count", predicted_count)
  print("true_count", true_count)
  count_loss = F.mse_loss(predicted_count.float(), true_count.unsqueeze(0).float())
  min_count = min(int(predicted_count.item()), int(true_count.item()))
  
  properties_loss = 0
  for i in range(min_count):
    pred_prop = predicted_properties[i].squeeze()
    true_prop = true_properties[i].squeeze()
    properties_loss += F.mse_loss(pred_prop,true_prop)
  
  if int(predicted_count.item()) != int(true_count.item()):
    count_mismatch_penalty = abs(predicted_count.item() - true_count.item()) * 0.5
    properties_loss += count_mismatch_penalty

  total_loss = count_loss + properties_loss
  return total_loss

if __name__=="__main__":

  #loss tests
  predicted_count = torch.rand(1)
  true_count = torch.tensor([0])

  predicted_properties = torch.rand(1,5)
  true_properties = torch.rand(true_count,5)

  loss = MultiPredLoss(predicted_count,predicted_properties, true_count, true_properties)
  print("loss", loss)