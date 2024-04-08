/*************************************************/
/*           DO NOT MODIFY THIS HEADER           */
/*                                               */
/*                     MOOSE                     */
/*                                               */
/*    (c) 2015 Battelle Energy Alliance, LLC     */
/*            ALL RIGHTS RESERVED                */
/*                                               */
/*   Prepared by Battelle Energy Alliance, LLC   */
/*     Under Contract No. DE-AC07-05ID14517      */
/*     With the U. S. Department of Energy       */
/*                                               */
/*     See COPYRIGHT for full restrictions       */
/*************************************************/

#pragma once

namespace PhysicalConstants
{
// Physical constants
// Avogadro's number [at/mol]
// https://physics.nist.gov/cgi-bin/cuu/Value?na|search_for=Avogadro
const auto avogadro_number = 6.02214076e23;
// Boltzmann constant [J/K]
// https://physics.nist.gov/cgi-bin/cuu/Value?k|search_for=Boltzmann
const auto boltzmann_constant = 1.380649e-23;
// Conversion coefficient from calories to Joules
// https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication1038.pdf
// Table 5.2.12: Energy, work, heat
const Real cal_to_J = 4.184;
// Conversion coefficient from eV to Joules
// https://physics.nist.gov/cgi-bin/cuu/Value?Revj|search_for=joules
const auto eV_to_J = 1.602176634e-19;
// Ideal gas constant [J/(mol-K)]
// https://physics.nist.gov/cgi-bin/cuu/Value?eqr
const auto ideal_gas_constant = 8.31446261815324;
// Stefan-Boltzmann constant [W/(m2-K^4)]
// https://physics.nist.gov/cgi-bin/cuu/Value?sigma|search_for=Stefan-Boltzmann
const auto stefan_boltzmann_constant = 5.670374419e-8;
} // namespace PhysicalConstants
