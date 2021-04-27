# This test features air flowing through a channel whose cross-sectional area
# shrinks to half its value in the right half. Assuming incompressible flow
# conditions, such as having a low Mach number, the velocity should approximately
# double from inlet to outlet. In this version of the test, the area discontinuity
# is achieved by connecting two flow channels with a junction.

p_outlet = 1e5

[GlobalParams]
  gravity_vector = '0 0 0'

  initial_T = 300
  initial_p = ${p_outlet}

  fp = fp
  closures = simple
  f = 0

  scaling_factor_1phase = '1 1 1e-5'
[]

[FluidProperties]
  [fp]
    type = IdealGasFluidProperties
    gamma = 1.4
  []
[]

[Components]
  [inlet]
    type = InletDensityVelocity1Phase
    input = 'pipe1:in'
    rho = 1.16263315948279 # rho @ (p = 1e5 Pa, T = 300 K)
    vel = 1
  []

  [pipe1]
    type = FlowChannel1Phase
    position = '0 0 0'
    orientation = '1 0 0'
    length = 0.5
    n_elems = 50

    A = 1
    initial_vel = 1
  []

  [junction]
    type = JunctionOneToOne1Phase
    connections = 'pipe1:out pipe2:in'
  []

  [pipe2]
    type = FlowChannel1Phase
    position = '0.5 0 0'
    orientation = '1 0 0'
    length = 0.5
    n_elems = 50

    A = 0.5
    initial_vel = 2
  []

  [outlet]
    type = Outlet1Phase
    input = 'pipe2:out'
    p = ${p_outlet}
  []
[]

[Preconditioning]
  [pc]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  end_time = 10
  [TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.001
    optimal_iterations = 5
    iteration_window = 1
    growth_factor = 1.2
  []

  steady_state_detection = true

  solve_type = PJFNK
  nl_rel_tol = 1e-10
  nl_abs_tol = 1e-8
  nl_max_its = 15

  l_tol = 1e-3
  l_max_its = 10
[]

[Outputs]
  exodus = true
  show = 'A rho vel p'
[]
