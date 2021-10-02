[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables/T]
  initial_condition = 300
[]

[Kernels]
  [time]
    type = ADTimeDerivative
    variable = T
  []
  [diff]
    type = ADMatDiffusion
    variable = T
    diffusivity = diffusivity
  []
  [source]
    type = ADBodyForce
    variable = T
    value = 100
    function = 1
  []
[]

[BCs]
  [left]
    type = ADDirichletBC
    variable = T
    boundary = left
    value = 300
  []
  [right]
    type = ADNeumannBC
    variable = T
    boundary = right
    value = -100
  []
[]

[Materials/constant]
  type = ADGenericConstantMaterial
  prop_names = 'diffusivity'
  prop_values = 1
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  num_steps = 4
  dt = 0.25
[]

[Postprocessors]
  [T_avg]
    type = ElementAverageValue
    variable = T
    execute_on = 'initial timestep_end'
  []
  [q_left]
    type = ADSideDiffusiveFluxAverage
    variable = T
    boundary = left
    diffusivity = diffusivity
    execute_on = 'initial timestep_end'
  []
[]

[Reporters]
  [acc]
    type = AccumulateReporter
    reporters = 'T_avg/value q_left/value'
  []
[]

[Controls/stochastic]
  type = SamplerReceiver
[]

[Outputs]
[]
