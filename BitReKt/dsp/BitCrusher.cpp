#include "BitCrusher.hpp"

double BitCrusher::crush(double sample, double offset)
{
  double expanded = sample * precision;
  int crushed = expanded;

  // Round value
  if (expanded - ((double) crushed) >= 0.5)
    crushed++;
  else if (expanded - ((double) crushed) <= -0.5)
    crushed--;

  return ((double) crushed) / ((double) precision);
}

void BitCrusher::setBits(double bits)
{
  this->precision = std::pow(2, bits) / 2.0;
}
