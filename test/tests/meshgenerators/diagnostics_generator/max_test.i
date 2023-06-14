[Mesh]
  [elem_maker]
    type = ElementGenerator
    nodal_positions = '0 0 0
                       1 0 0
                       1 1 0
                       0 3 0'
    element_connectivity = '0 1 2 3'
    elem_type = 'QUAD4'
  []
  [diag]
    type = MeshDiagnosticsGenerator
    input = elem_maker
    maximum_element_volumes = 0.1
  []
[]

[Outputs]
  exodus = true
[]

