#pragma once

#include <torch/torch.h>

namespace jetstream {
namespace network {

struct FNet : torch::nn::Module {

  FNet(int64_t N, int64_t M);

  torch::Tensor forward(torch::Tensor input);

  torch::nn::Linear linear;
  torch::Tensor another_bias;
};

} // namespace network
} // namespace jetstream
