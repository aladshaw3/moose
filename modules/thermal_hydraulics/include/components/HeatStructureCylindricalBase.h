//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "HeatStructureBase.h"

/**
 * Base class for cylindrical heat structure components
 */
class HeatStructureCylindricalBase : public HeatStructureBase
{
public:
  HeatStructureCylindricalBase(const InputParameters & params);

  virtual void setupMesh() override;
  virtual void addMooseObjects() override;
  virtual Real getUnitPerimeter(const HeatStructureSideType & side) const override;

  /**
   * Get the inner radius of the heat structure
   *
   * @returns The inner radius of the heat structure
   */
  virtual Real getInnerRadius() const { return _inner_radius; }

protected:
  /// Inner radius of the heat structure
  Real _inner_radius;

public:
  static InputParameters validParams();
};
