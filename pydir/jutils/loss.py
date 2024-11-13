import torch
import torch.nn.functional as F

def JetLoss(predicted_jets, primary_loss, penaltyfactor=0.1):
  pt, _, phi, energy, mass = predicted_jets[..., 0], predicted_jets[..., 1], predicted_jets[..., 2], predicted_jets[..., 3], predicted_jets[..., 4]

  pt_penalty = torch.mean(F.relu(-pt))
  energy_penalty = torch.mean(F.relu(-energy))
  mass_penalty = torch.mean(F.relu(-mass))

  phi_penalty = torch.mean(F.relu(phi - 2 * torch.pi) + F.relu(-phi))

  penalty = pt_penalty + energy_penalty + mass_penalty + phi_penalty

  return primary_loss + penalty * penaltyfactor  # Scale the penalty term