#ifndef BitCrusher_hpp
#define BitCrusher_hpp

class BitCrusher
{
public:
  void setBits(double bits);
  double crush(double sample, double offset);
private:
  int precision;
  double tolerance;
};

#endif /* BitCrusher_hpp */
