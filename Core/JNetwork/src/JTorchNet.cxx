#include "JTorchNet.hpp"

namespace jetstream {
namespace network {

FNet::FNet(int64_t N, int64_t M) : 
inputLayer(register_module("fc1", torch::nn::Linear(N, 128))),
layer1(register_module("fc2", torch::nn::Linear(128, 64))),
output(register_module("fc3", torch::nn::Linear(64, M)))
{
  bias = register_parameter("b", torch::randn(M));
}

torch::Tensor FNet::forward(torch::Tensor x) {
  x = torch::relu(inputLayer->forward(x));
  x = torch::relu(layer1->forward(x));
  x = output->forward(x);
  return x + bias;
}


} // namespace network
} // namespace jetstream