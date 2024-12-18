#pragma once

namespace jetstream {
namespace utils {

enum class Composition {
  kGluon,
  kQuark,
  kMixed
};

Composition getComposition(const int &pdgID) {
  if (pdgID == 21) {
    return Composition::kGluon;
  } else if (pdgID >= 1 &&  pdgID <= 6) {
    return Composition::kQuark;
  } else {
    return Composition::kMixed;
  }
}
    

} // namespace utils
} // namespace jetstream