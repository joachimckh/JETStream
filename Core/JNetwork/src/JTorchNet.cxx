#include "JTorchNet.hpp"

namespace jetstream{
namespace network {

FNet::FNet(int64_t N, int64_t M)
    : linear(register_module("linear", torch::nn::Linear(N, M))) {
  another_bias = register_parameter("b", torch::randn(M));
}

torch::Tensor FNet::forward(torch::Tensor input) {
  return linear(input) + another_bias;
}

}
}