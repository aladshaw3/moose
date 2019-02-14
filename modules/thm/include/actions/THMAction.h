#ifndef THMACTION_H
#define THMACTION_H

#include "Action.h"
#include "Simulation.h"

class THMApp;
class THMAction;

template <>
InputParameters validParams<THMAction>();

class THMAction : public Action
{
public:
  THMAction(InputParameters params);

protected:
  /// Simulation this action is part of
  Simulation & _simulation;
};

#endif /* THMACTION_H */
