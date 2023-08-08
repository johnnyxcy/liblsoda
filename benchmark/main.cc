#include "fex.hh"
#include "pk.hh"
#include "scipy.hh"

int main() {
  fex_case::run();
  scipy_case::run();
  pk_case::run();
  return (0);
}