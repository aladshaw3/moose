#ifndef ENUMS_H
#define ENUMS_H

namespace RELAP7
{
  /// Type of the stabilization
  enum EStabilizationType
  {
    STABILIZATION_NONE,
    STABILIZATION_SUPG,
    STABILIZATION_LAPIDUS,
    STABILIZATION_RUSANOV,
    STABILIZATION_PRESSURE,
    STABILIZATION_ENTROPY_VISCOSITY
  };

  ///< Type of the end
  enum EEndType
  {
    IN,                 ///< inlet
    OUT,                ///< outlet
    PRIMARY_IN,         ///< heat exchanger primary loop inlet
    PRIMARY_OUT,        ///< heat exchanger primary loop outlet
    SECONDARY_IN,       ///< heat exchanger secondary loop inlet
    SECONDARY_OUT       ///< heat exchanger secondary loop outlet
  };

  enum EValveStatusType
  {
    VALVE_CLOSE = 0,    ///< valve is closed
    VALVE_OPEN = 1,     ///< valve is opened
    VALVE_CLOSING = -1, ///< valve is being closed
    VALVE_OPENING = 2   ///< valve is being opened
  };

  enum EValveActionType
  {
    VALVE_NO_ACTION = 0,    ///< maintaining current status
    VALVE_TURNING_ON = 1,   ///< turning on the valve
    VALVE_TURNING_OFF = -1  ///< turning off the valve
  };

  enum ETHCouplingType
  {
    MOD_DENSITY, ///< moderator density
    MOD_TEMP,    ///< moderator temperature
    FUEL_TEMP    ///< fuel temperature
  };

  /// The type of an equation
  enum EFlowEquationType
  {
    CONTINUITY = 0,
    MOMENTUM = 1,
    ENERGY = 2,
    VOIDFRACTION = 3,
    INVALID = 4
  };

  /// Type of fuel
  enum EHeatStructureType
  {
    HS_TYPE_INVALID,
    HS_TYPE_PLATE,
    HS_TYPE_CYLINDER
  };

  /**
   * Heat flux partitioning model
   */
  enum EHeatFluxPartitioningModelType
  {
    HFPM_LINEAR = 0,
    HFPM_SIMPLE = 1
  };

}

#endif // ENUMS_H

