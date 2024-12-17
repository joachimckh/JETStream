#include <torch/torch.h>
#include "JTorchNet.hpp"


int main(int argc, char *argv[]) {

  int nEpochs{100};
  if (argc > 1) {
    nEpochs = std::atoi(argv[1]);
    if (nEpochs <= 0) {
      std::cerr << "Invalid number of epochs specified. Using default: 1\n";
      nEpochs = 1;
    }
    //_ = argv[2];
  }


  jetstream::network::FNet net(4, 5);
  for (const auto& p : net.parameters()) {
    std::cout << p << std::endl;
  }


  // for (int i{0}; i < nEpochs; ++i)
  // {
  //   std::cout << "Epoch: " << i << std::endl;
  //   // Forward pass
  //   auto output = net.forward(torch::randn({2, 4}));
  //   std::cout << output << std::endl;

  //   // Compute loss
  //   auto target = torch::randn({2, 5});
  //   auto loss = torch::mse_loss(output, target);
  //   std::cout << loss << std::endl;

  //   // Zero the gradients before running the backward pass.
  //   net.zero_grad();

  //   // Backward pass
  //   loss.backward();

  //   // Update the weights using gradient descent
  //   torch::optim::Adam optimizer(net.parameters(), /*lr=*/0.01);
  //   optimizer.step();
  // }


}
