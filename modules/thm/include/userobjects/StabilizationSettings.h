#pragma once

#include "GeneralUserObject.h"
#include "FlowModel.h"
#include "NamingInterface.h"

class StabilizationSettings;
class Simulation;

template <>
InputParameters validParams<StabilizationSettings>();

class StabilizationSettings : public GeneralUserObject, public NamingInterface
{
public:
  StabilizationSettings(const InputParameters & parameters);

  virtual void execute();
  virtual void initialize();
  virtual void finalize();

  virtual void addVariables(FlowModel & fm, const SubdomainName & subdomain_name) const = 0;
  virtual void initMooseObjects(FlowModel & fm) = 0;
  virtual void addMooseObjects(FlowModel & fm, InputParameters & pars) const = 0;

protected:
  Simulation & _m_sim;
  /// The application this is associated with
  THMApp & _m_app;
  /// The Factory associated with the MooseApp
  Factory & _m_factory;
};
