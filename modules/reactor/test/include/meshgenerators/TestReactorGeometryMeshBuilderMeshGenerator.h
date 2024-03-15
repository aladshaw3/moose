//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MeshGenerator.h"

class TestReactorGeometryMeshBuilderMeshGenerator : public MeshGenerator
{
public:
  static InputParameters validParams();

  TestReactorGeometryMeshBuilderMeshGenerator(const InputParameters & parameters);

  std::unique_ptr<MeshBase> generate() override;

private:
  /// the input mesh name
  const std::string _input;

  /// The final mesh that is generated by the subgenerators;
  /// This mesh is pointed to by the input. We store it so we can later reset it in `generate`.
  std::unique_ptr<MeshBase> * _input_mesh;

  /// The final mesh that is generated by the subgenerators;
  std::unique_ptr<MeshBase> * _build_mesh;
};
