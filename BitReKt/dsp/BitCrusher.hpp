#pragma once

#include <cmath>

class BitCrusher
{
public:
  void setBits(double bits);
  double crush(double sample, double offset);
private:
  int precision;
  double tolerance;
};
