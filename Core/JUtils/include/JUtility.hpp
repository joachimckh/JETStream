#pragma once

namespace jetstream {
namespace utils {

enum class Composition {
  kGluon,
  kQuark,
  kMixed
};

int getComposition(const int &pdgID) {
  if (pdgID == 21) {
    return 0;
  } else if (pdgID >= 1 &&  pdgID <= 6) {
    return 1;
  } else {
    return 2;
  }
}
    

} // namespace utils
} // namespace jetstream