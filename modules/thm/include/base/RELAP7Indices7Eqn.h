#ifndef RELAP7INDICES7EQN_H
#define RELAP7INDICES7EQN_H

namespace RELAP77Eqn
{
/// Number of conservative solution variables, plus cross-sectional area
static const unsigned int N_CONS_VAR = 8;

/// Indices for conservative solution variables, plus cross-sectional area
enum ConservativeVariableIndex
{
  CONS_VAR_BETA = 0,
  CONS_VAR_ARHOA_LIQUID = 1,
  CONS_VAR_ARHOUA_LIQUID = 2,
  CONS_VAR_ARHOEA_LIQUID = 3,
  CONS_VAR_ARHOA_VAPOR = 4,
  CONS_VAR_ARHOUA_VAPOR = 5,
  CONS_VAR_ARHOEA_VAPOR = 6,
  CONS_VAR_AREA = 7
};

/// Number of primitive solution variables in a phase
static const unsigned int N_LOCAL_PRIM_VAR = 5;

/// Indices for primitive solution variables in a phase
enum LocalPrimitiveVariableIndex
{
  LOCAL_PRIM_VAR_ALPHA = 0,
  LOCAL_PRIM_VAR_DENSITY = 1,
  LOCAL_PRIM_VAR_VELOCITY = 2,
  LOCAL_PRIM_VAR_SPECIFIC_TOTAL_ENERGY = 3,
  LOCAL_PRIM_VAR_PRESSURE = 4
};

/// Number of equations
static const unsigned int N_EQ = 7;

/// Indices for equations
enum EquationIndex
{
  EQ_VF = 0,
  EQ_MASS_LIQUID = 1,
  EQ_MOMENTUM_LIQUID = 2,
  EQ_ENERGY_LIQUID = 3,
  EQ_MASS_VAPOR = 4,
  EQ_MOMENTUM_VAPOR = 5,
  EQ_ENERGY_VAPOR = 6
};

/// Number of local equations (per phase)
static const unsigned int N_LOCAL_EQ = 4;

/// Local indices (arbitrary phase) for equations
enum LocalEquationIndex
{
  LOCAL_EQ_VF = 0,
  LOCAL_EQ_MASS = 1,
  LOCAL_EQ_MOMENTUM = 2,
  LOCAL_EQ_ENERGY = 3
};

/// Total number of slopes
static const unsigned int N_SLOPES = 8;
/// Number of slopes that are limited
static const unsigned int N_LIMITED_SLOPES = 7;

/// Indices for slope array
enum SlopeIndex
{
  SLOPE_ALPHA_LIQUID = 0,
  SLOPE_PRESSURE_LIQUID = 1,
  SLOPE_VELOCITY_LIQUID = 2,
  SLOPE_TEMPERATURE_LIQUID = 3,
  SLOPE_PRESSURE_VAPOR = 4,
  SLOPE_VELOCITY_VAPOR = 5,
  SLOPE_TEMPERATURE_VAPOR = 6,
  SLOPE_ALPHA_LIQUID_UNLIMITED = 7
};

/// Number of phases
static const unsigned int N_PHASE = 2;

/// Phase indices
enum PhaseIndex
{
  PHASE_LIQUID = 0,
  PHASE_VAPOR = 1
};
}

#endif // RELAP7INDICES7EQN_H
