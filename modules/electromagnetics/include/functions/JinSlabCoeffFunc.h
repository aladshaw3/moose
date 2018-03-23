#ifndef JINSLABCOEFFFUNC_H
#define JINSLABCOEFFFUNC_H

#include "Function.h"
#include "FunctionInterface.h"

class JinSlabCoeffFunc;

template <>
InputParameters validParams<JinSlabCoeffFunc>();

/**
 *    Function for coefficient in JinSlab1D case.
 */
class JinSlabCoeffFunc : public Function, public FunctionInterface
{
public:
  JinSlabCoeffFunc(const InputParameters & parameters);

  virtual Real value(Real t, const Point & p) override;

protected:
  Function & _epsR_real;
  Function & _epsR_imag;

  Real _muR_real;
  Real _muR_imag;

  Real _k;

  Real _theta;

  MooseEnum _component;
};

#endif // JINSLABCOEFFFUNC_H
