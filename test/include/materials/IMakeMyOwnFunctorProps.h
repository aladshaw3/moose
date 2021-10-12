//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "FunctorMaterial.h"

/**
 * A material that optionally couples both a finite element and finite volume variable (strictly
 * speaking they don't have to be one or the other). This class also uses a specialization of
 * FunctorMaterialProperty that overrides the \p evaluate methods instead of using lambdas
 */
class IMakeMyOwnFunctorProps : public FunctorMaterial
{
public:
  static InputParameters validParams();

  IMakeMyOwnFunctorProps(const InputParameters & parameters);

protected:
  const Moose::Functor<ADReal> & _fe_var;
  const Moose::Functor<ADReal> & _fv_var;
  const Moose::Functor<ADReal> * const _retrieved_prop;
  FunctorMaterialProperty<ADReal> * const _fe_prop;
  FunctorMaterialProperty<ADReal> * const _fv_prop;
};
