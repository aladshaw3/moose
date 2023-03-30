//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MooseFunctor.h"

namespace Moose
{
/**
 * This is essentially a forwarding functor that forwards the spatial and temporal evaluation
 * arguments to the parent vector functor and then returns the result indexed at a given component.
 */
template <typename T>
class VectorComponentFunctor : public FunctorBase<T>
{
public:
  using typename FunctorBase<T>::ValueType;
  using typename FunctorBase<T>::GradientType;
  using typename FunctorBase<T>::DotType;
  using VectorArg = typename libMesh::TensorTools::IncrementRank<T>::type;
  using VectorFunctor = FunctorBase<VectorArg>;

  VectorComponentFunctor(const VectorFunctor & vector, const unsigned int component)
    : FunctorBase<T>(vector.functorName() + "_" + std::to_string(component)),
      _vector(vector),
      _component(component)
  {
  }

  bool isExtrapolatedBoundaryFace(const FaceInfo & fi,
                                  const Elem * elem,
                                  const Moose::TimeArg & time) const override;
  bool hasBlocks(SubdomainID sub_id) const override { return _vector.hasBlocks(sub_id); }

private:
  /// The parent vector functor
  const VectorFunctor & _vector;

  /// The component at which we'll index the parent vector functor evaluation result
  const unsigned int _component;

  ValueType evaluate(const ElemArg & elem, const TimeArg & time) const override final
  {
    return _vector(elem, time)(_component);
  }

  ValueType evaluate(const FaceArg & face, const TimeArg & time) const override final
  {
    return _vector(face, time)(_component);
  }

  ValueType evaluate(const ElemQpArg & elem_qp, const TimeArg & time) const override final
  {
    return _vector(elem_qp, time)(_component);
  }

  ValueType evaluate(const ElemSideQpArg & elem_side_qp, const TimeArg & time) const override final
  {
    return _vector(elem_side_qp, time)(_component);
  }

  ValueType evaluate(const ElemPointArg & elem_point, const TimeArg & time) const override final
  {
    return _vector(elem_point, time)(_component);
  }

  using FunctorBase<T>::evaluateGradient;
  GradientType evaluateGradient(const ElemArg & elem_arg, const TimeArg & time) const override final
  {
    return _vector.gradient(elem_arg, time).row(_component);
  }

  GradientType evaluateGradient(const FaceArg & face, const TimeArg & time) const override final
  {
    return _vector.gradient(face, time).row(_component);
  }
};

template <typename T>
bool
VectorComponentFunctor<T>::isExtrapolatedBoundaryFace(const FaceInfo & fi,
                                                      const Elem * elem,
                                                      const Moose::TimeArg & time) const
{
  return _vector.isExtrapolatedBoundaryFace(fi, elem, time);
}
}
