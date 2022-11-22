#include <Euclid/Euclid.hpp>

// In Euclid, we recommended to create Vec or Mat by using setxxx function
int main() {
  /* constexpr opt */ auto v1 = euclid::setVec4(1, 2, 3, 0);
  // /* constexpr opt */ auto v2 = euclid::setVec3(1, 2, 3);
  /* constexpr opt */ auto m1 = euclid::setMat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  euclid::print(v1, m1);
}