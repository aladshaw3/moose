//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

// MOOSE includes
#include "UserObject.h"
#include "BoundaryRestrictableRequired.h"
#include "MaterialPropertyInterface.h"
#include "Coupleable.h"
#include "MooseVariableDependencyInterface.h"
#include "TransientInterface.h"
#include "ElementIDInterface.h"

/**
 * Base class for user objects executed one or more sidesets, which may be
 * on the outer boundary of the mesh, or be internal to mesh, blocks etc
 */
class SideUserObject : public UserObject,
                       public BoundaryRestrictableRequired,
                       public MaterialPropertyInterface,
                       public Coupleable,
                       public MooseVariableDependencyInterface,
                       public TransientInterface,
                       public ElementIDInterface
{
public:
  static InputParameters validParams();

  SideUserObject(const InputParameters & parameters);

protected:
  MooseMesh & _mesh;

  const MooseArray<Point> & _q_point;
  const QBase * const & _qrule;
  const MooseArray<Real> & _JxW;
  const MooseArray<Real> & _coord;
  const MooseArray<Point> & _normals;

  const Elem * const & _current_elem;
  /// current side of the current element
  const unsigned int & _current_side;

  const Elem * const & _current_side_elem;
  const Real & _current_side_volume;

  const BoundaryID & _current_boundary_id;

  /**
   * Returns the local FaceInfo to use in functor arguments and interpolations.
   * Note this face info could hold the element from the other side of the
   * sideset. Sidesets are oriented!
   */
void
getFaceInfo()
{
  _face_infos.clear();

  // Either the element or the neighbor is a valid argument to get a face info
  const Elem * equal_level_neighbor = _current_elem->neighbor_ptr(_current_side);
  _candidate_neighbors = {equal_level_neighbor};

  mooseAssert(_current_elem, "We should have an element");
  mooseAssert(_current_elem->active(), "The current element should be active");

  // No neighbor means we are at a boundary, a FaceInfo exists in the mesh
  // If a neighbor exists, the face info may only be defined on that side
  if (equal_level_neighbor)
  {
    // neighbor is not active and we cant just pick an arbitrary refined element in neighbor
    // no FaceInfo in mesh will satisfy us, let's error out
    if (!equal_level_neighbor->active())
      equal_level_neighbor->active_family_tree_by_neighbor(_candidate_neighbors, _current_elem);

    for (const Elem * neighbor : _candidate_neighbors)
    {
      const Elem * element = _current_elem;
      auto side = _current_side;

      // First check refinement level
      if (_current_elem->level() < neighbor->level())
      {
        element = neighbor;
        side = neighbor->which_neighbor_am_i(_current_elem);
      }
      // Then check ids
      else if (_current_elem->id() > neighbor->id())
      {
        element = neighbor;
        side = neighbor->which_neighbor_am_i(_current_elem);
      }
      _face_infos.push_back(_mesh.faceInfo(element, side));
    }
  }
  else
    _face_infos.push_back(_mesh.faceInfo(_current_elem, _current_side));
}
};
