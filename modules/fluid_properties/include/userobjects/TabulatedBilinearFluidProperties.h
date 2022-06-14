//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "SinglePhaseFluidProperties.h"
#include "DelimitedFileReader.h"
#include "TabulatedFluidProperties.h"

class SinglePhaseFluidPropertiesPT;
class BilinearInterpolation;
class TabulatedFluidProperties;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"

/**
 * Class for fluid properties read from a file.
 *
 * Property values are read from a CSV file containing property data.
 * Monotonically increasing values of pressure and temperature must be included in
 * the data file, specifying the phase space where tabulated fluid properties will
 * be defined. An error is thrown if either temperature or pressure data is not
 * included or not monotonic, and an error is also thrown if this UserObject is
 * requested to provide a fluid property outside this phase space.
 *
 * This class is intended to be used when complicated formulations for fluid properties
 * (for example, density or enthalpy) are required, which can be computationally expensive.
 * This is particularly the case where the fluid equation of state is based on a
 * Helmholtz free energy that is a function of density and temperature, like that
 * used in CO2FluidProperties. In this case, density must be solved iteratively using
 * pressure and temperature, which increases the computational burden.
 *
 * Using interpolation to calculate these fluid properties can significantly reduce
 * the computational time for these cases.
 *
 * The expected file format for the tabulated fluid properties is now described.
 * The first line must be the header containing the required column
 * names "pressure" and "temperature", and the names of the fluid properties to be
 * read. Available fluid property names are:
 * "density", "enthalpy", "internal_energy", "viscosity", "k" (thermal conductivity),
 * "cp" (isobaric specific heat capacity), "cv" (isochoric specific heat capacity),
 * and "entropy".
 * Note: the order is not important, although having pressure and temperature first
 * makes the data easier for a human to read).
 *
 * The data in the pressure and temperature columns must be monotonically increasing. This file
 * format does require duplication of the pressure and temperature data - each pressure value
 * must be included num_T times, while each temperature value is repeated num_p times, where
 * num_T and num_p are the number of temperature and pressure points, respectively. This class
 * will check that the required number of data points have been entered (num_T * num_p).
 *
 * An example of a valid fluid properties file is provided below:
 *
 * pressure, temperature,   density, enthalpy, internal_energy
 *   200000,         275,   3.90056,   -21487,        -72761.7
 *   200000,         277,   3.86573, -19495.4,        -71232.0
 *   200000,         280,   3.83155, -17499.1,        -69697.3
 *   300000,         275,   6.07273, -22728.3,        -73626.5
 *   300000,         277,   6.01721, -20711.5,        -72079.3
 *   300000,         280,   5.96277, -18691.0,        -70527.7
 *
 * and so on.
 *
 * If no tabulated fluid property data file exists, then data for the fluid supplied
 * by the required FluidProperties UserObject will be generated using the pressure
 * and temperature ranges specified in the input file at the beginning of the simulation.
 * The properties to be tabulated are provided in the "interpolated_properties" input
 * parameter (the properties that can be tabulated are listed above).
 *
 * This tabulated data will be written to file in the correct format,
 * enabling suitable data files to be created for future use. There is an upfront
 * computational expense required for this initial data generation, depending on the
 * required number of pressure and temperature points. However, provided that the
 * number of data points required to generate the tabulated data is smaller than the
 * number of times the property members in the FluidProperties UserObject are used,
 * the initial time to generate the data and the subsequent interpolation time can be much
 * less than using the original FluidProperties UserObject.
 *
 * Properties specified in the data file or listed in the input file (and their derivatives
 * wrt pressure and temperature) will be calculated using bilinear interpolation, while all
 * remaining fluid properties are calculated using the supplied FluidProperties UserObject.
 */
class TabulatedBilinearFluidProperties : public TabulatedFluidProperties
{
public:
  static InputParameters validParams();

  TabulatedBilinearFluidProperties(const InputParameters & parameters);

  virtual void constructInterpolation() override;

protected:
  /**
   * Forms a Column Major Matrix from a single std::vector.
   * @param nrow number of rows in the matrix
   * @param ncol number of columns in the matrix
   * @param vec 1D vector to reshape into a 2D matrix
   * @param[out] Column Major Matrix
   */
  void reshapeData2D(unsigned int nrow,
                     unsigned int ncol,
                     const std::vector<Real> & vec,
                     ColumnMajorMatrix & mat);
};

#pragma GCC diagnostic pop
