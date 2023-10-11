//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "SystemBase.h"
// #include "ConstraintWarehouse.h"
#include "MooseObjectWarehouse.h"
#include "MooseObjectTagWarehouse.h"
#include "PerfGraphInterface.h"
// #include "ComputeMortarFunctor.h"
// #include "MooseHashing.h"

#include "libmesh/transient_system.h"
#include "libmesh/linear_implicit_system.h"
#include "libmesh/linear_solver.h"

// Forward declarations
// class FEProblemBase;
// class MoosePreconditioner;
// class JacobianBlock;
// class TimeIntegrator;
// class Predictor;
// class ElementDamper;
// class NodalDamper;
// class GeneralDamper;
// class GeometricSearchData;
// class IntegratedBCBase;
// class NodalBCBase;
// class DirichletBCBase;
// class ADDirichletBCBase;
// class DGKernelBase;
// class InterfaceKernelBase;
// class ScalarKernelBase;
// class DiracKernelBase;
// class NodalKernelBase;
// class Split;
// class KernelBase;
// class BoundaryCondition;
// class ResidualObject;

// libMesh forward declarations
namespace libMesh
{
template <typename T>
class NumericVector;
template <typename T>
class SparseMatrix;
template <typename T>
class DiagonalMatrix;
} // namespace libMesh

/**
 * Linear system to be solved
 */
class LinearSystem : public SystemBase, public PerfGraphInterface
{
public:
  LinearSystem(FEProblemBase & problem, System & sys, const std::string & name);
  virtual ~LinearSystem();

  virtual void init() override;

  virtual void solve() override;
  virtual void restoreSolutions() override;

  /**
   * Quit the current solve as soon as possible.
   */
  virtual void stopSolve() = 0;

  virtual LinearSolver<Number> * linearSolver() = 0;

  virtual KSP getKSP() = 0;

  // Setup Functions ////
  virtual void initialSetup() override;
  virtual void timestepSetup() override;
  virtual void customSetup(const ExecFlagType & exec_type) override;
  virtual void residualSetup() override {}
  virtual void jacobianSetup() override {}

  /**
   * Returns the convergence state
   * @return true if converged, otherwise false
   */
  virtual bool converged() = 0;

  /**
   * Add a time integrator
   * @param type Type of the integrator
   * @param name The name of the integrator
   * @param parameters Integrator params
   */
  void addTimeIntegrator(const std::string & type,
                         const std::string & name,
                         InputParameters & parameters) override;
  using SystemBase::addTimeIntegrator;

  /**
   * Adds a kernel
   * @param kernel_name The type of the kernel
   * @param name The name of the kernel
   * @param parameters Kernel parameters
   */
  virtual void addLinearKernel(const std::string & kernel_name,
                               const std::string & name,
                               InputParameters & parameters);

  /**
   * Adds a boundary condition
   * @param bc_name The type of the boundary condition
   * @param name The name of the boundary condition
   * @param parameters Boundary condition parameters
   */
  void addBoundaryCondition(const std::string & bc_name,
                            const std::string & name,
                            InputParameters & parameters);

  void setInitialSolution();

  /**
   * Computes right hand side for a given tag
   * @param rhs Right hand side is formed in here
   * @param tag_id tag of kernels for which the residual is to be computed.
   */
  void computeRightHandSideTag(NumericVector<Number> & rhs, TagID tag_id);

  /**
   * Form multiple tag-associated right hand side vectors for all the given tags
   */
  void computeRightHandSideTags(const std::set<TagID> & tags);

  void computeLinearSystemTags(const std::set<TagID> & vector_tags,
                               const std::set<TagID> & matrix_tags);

  void computeLinearSystemTagsInternal(const std::set<TagID> & vector_tags,
                                       const std::set<TagID> & matrix_tags);

  /**
   * Form a right hand side vector for a given tag
   */
  void computeRightHandSide(NumericVector<Number> & rhs, TagID tag_id);

  /**
   * Computes multiple (tag associated) system matricese
   */
  void computeSystemMatrixTags(const std::set<TagID> & tags);

  /**
   * Associate jacobian to systemMatrixTag, and then form a matrix for all the tags
   */
  void computeSystemMatrix(SparseMatrix<Number> & matrix, const std::set<TagID> & tags);

  /**
   * Take all tags in the system, and form a matrix for all tags in the system
   */
  void computeSystemMatrix(SparseMatrix<Number> & matrix);

  /**
   * Called at the beginning of the time step
   */
  void onTimestepBegin();

  virtual void setSolution(const NumericVector<Number> & soln);

  /**
   * Update active objects of Warehouses owned by LinearSystem
   */
  void updateActive(THREAD_ID tid);

  /**
   *  Return a numeric vector that is associated with the time tag.
   */
  NumericVector<Number> & getRightHandSideTimeVector();

  /**
   * Return a numeric vector that is associated with the nontime tag.
   */
  NumericVector<Number> & getRightHandSideTimeVector();

  /**
   * Return a right hand side vector that is associated with the residual tag.
   */
  NumericVector<Number> & rightHandSideVector(TagID tag);

  const NumericVector<Number> * const & currentSolution() const override
  {
    return _current_solution;
  }

  virtual void serializeSolution();
  virtual NumericVector<Number> & serializedSolution() override;

  virtual void augmentSparsity(SparsityPattern::Graph & sparsity,
                               std::vector<dof_id_type> & n_nz,
                               std::vector<dof_id_type> & n_oz) override;

  /**
   * Sets a preconditioner
   * @param pc The preconditioner to be set
   */
  void setPreconditioner(std::shared_ptr<MoosePreconditioner> pc);
  MoosePreconditioner const * getPreconditioner() const;

  ///@{
  /// System Integrity Checks
  void checkKernelCoverage(const std::set<SubdomainID> & mesh_subdomains) const;
  bool containsTimeKernel();
  ///@}

  /**
   * Return the number of linear iterations
   */
  unsigned int nLinearIterations() const { return _n_linear_iters; }

  void setPredictor(std::shared_ptr<Predictor> predictor);
  Predictor * getPredictor() { return _predictor.get(); }

  void setPCSide(MooseEnum pcs);

  Moose::PCSideType getPCSide() { return _pc_side; }

  void setMooseKSPNormType(MooseEnum kspnorm);

  Moose::MooseKSPNormType getMooseKSPNormType() { return _ksp_norm; }

  //@{
  /**
   * Access functions to Warehouses from outside LinearSystem
   */
  MooseObjectTagWarehouse<KernelBase> & getKernelWarehouse() { return _kernels; }
  //@}

  virtual System & system() override { return _sys; }
  virtual const System & system() const override { return _sys; }

  TagID timeVectorTag() const override { return _time_tag; }
  TagID nonTimeVectorTag() const override { return _non_time_tag; }
  TagID rightHandSideVectorTag() const override { return _rhs_tag; }
  TagID systemMatrixTag() const override { return _system_mx_tag; }

  FEProblemBase & _fe_problem;
  System & _sys;
  unsigned int _current_l_its;

protected:
  /**
   * Compute the right hand side for a given tag
   * @param tags The tags of kernels for which the residual is to be computed.
   */
  void computeRightHandSideInternal(const std::set<TagID> & tags);

  /**
   * Form multiple matrices for all the tags. Users should not call this func directly.
   */
  void computeSystemMatricesInternal(const std::set<TagID> & tags);

  NumericVector<Number> & solutionInternal() const override { return *_sys.solution; }

  /// solution vector from linear solver
  const NumericVector<Number> * _current_solution;

  /// Tag for time contribution rhs
  TagID _time_rhs_tag;

  /// Vector tags to temporarily store all tags associated with the current system.
  std::set<TagID> _vector_tags;

  /// Matrix tags to temporarily store all tags associated with the current system.
  std::set<TagID> _matrix_tags;

  /// right hand side vector for time contributions
  NumericVector<Number> * _rhs_time;

  /// Tag for non-time contribution rhs
  TagID _non_time_rhs_tag;

  /// residual vector for non-time contributions
  NumericVector<Number> * _rhs_non_time;

  /// Used for the residual vector from PETSc
  TagID _rhs_tag;

  /// Tag for non-time contribution Jacobian
  TagID _system_matrix_non_time_tag;

  /// Tag for every contribution to system matrix
  TagID _system_matrix_system_tag;

  ///@{
  /// Kernel Storage
  MooseObjectTagWarehouse<KernelBase> _kernels;

  ///@}

  ///@{
  /// BoundaryCondition Warhouses
  MooseObjectTagWarehouse<IntegratedBCBase> _integrated_bcs;
  MooseObjectWarehouse<DirichletBCBase> _preset_nodal_bcs;
  ///@}

  /// Preconditioner
  std::shared_ptr<MoosePreconditioner> _preconditioner;
  /// Preconditioning side
  Moose::PCSideType _pc_side;
  /// KSP norm type
  Moose::MooseKSPNormType _ksp_norm;

  unsigned int _n_linear_iters;

  /// If predictor is active, this is non-NULL
  std::shared_ptr<Predictor> _predictor;

private:
  /// The current states of the solution (0 = current, 1 = old, etc)
  std::vector<NumericVector<Number> *> _solution_state;
};
