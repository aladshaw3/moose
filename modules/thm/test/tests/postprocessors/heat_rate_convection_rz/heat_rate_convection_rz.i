# Tests the HeatRateConvectionRZ post-processor.

R_o = 0.2
thickness = 0.05
R_i = ${fparse R_o - thickness}

L = 3.0
S = ${fparse 2 * pi * R_o * L}

Q = 5000
T = 300
T_ambient = 350
htc = ${fparse Q / (S * (T_ambient - T))}

[HeatStructureMaterials]
  [./region1-mat]
    type = SolidMaterialProperties
    k = 1
    Cp = 1
    rho = 1
  [../]
[]

[Components]
  [./heat_structure]
    type = HeatStructureCylindrical

    position = '1 2 3'
    orientation = '1 1 1'
    inner_radius = ${R_i}
    length = ${L}
    n_elems = 50

    names = 'region1'
    materials = 'region1-mat'
    widths = '${thickness}'
    n_part_elems = '5'

    initial_T = ${T}
  [../]
[]

[Postprocessors]
  [./Q_pp]
    type = HeatRateConvectionRZ
    boundary = heat_structure:outer
    axis_point = '1 2 3'
    axis_dir = '1 1 1'
    offset = ${R_i}
    htc = ${htc}
    T = T_solid
    T_ambient = ${T_ambient}
    execute_on = 'initial'
  [../]
[]

[Problem]
  solve = false
[]

[Executioner]
  type = Transient
  num_steps = 1
[]

[Outputs]
  file_base = 'heat_rate_convection_rz'
  [./csv]
    type = CSV
    precision = 15
    execute_on = 'initial'
  [../]
[]
