#pragma once

#include <torch/torch.h>

namespace jetstream {
namespace network {

struct FNet : torch::nn::Module {

  FNet(int64_t N, int64_t M);

  torch::Tensor forward(torch::Tensor input);

  torch::nn::Linear inputLayer;
  torch::nn::Linear layer1;
  // torch::nn::Linear layer2;
  torch::nn::Linear output;

  torch::Tensor bias;
};

} // namespace network
} // namespace jetstream
