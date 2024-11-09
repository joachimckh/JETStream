import torch
import torch.nn as nn

from network.model import JetClassifierGNN

# Hyperparameters
input_dim = 4  # (px, py, pz, energy)
hidden_dim = 32
output_dim = 2  # Binary classification (jet or no jet) PUT IN CONFIG

# Model, loss, and optimizer
model = JetClassifierGNN(input_dim, hidden_dim, output_dim)
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

# # Training loop
# for epoch in range(10):
#     total_loss = 0
#     for data in dataloader:
#         optimizer.zero_grad()
        
#         # Forward pass
#         out = model(data)
        
#         # Compute loss
#         loss = criterion(out, data.y)
#         total_loss += loss.item()
        
#         # Backward pass and optimize
#         loss.backward()
#         optimizer.step()
    
#     print(f'Epoch {epoch+1}, Loss: {total_loss/len(dataloader)}')