// FFTW-Age: FFTW Wisdom Generation
//
// Copyright (C) 2024 Mike S Wang [GPL-3.0]
//
// This file is part of the FFTW-Age program. See the COPYRIGHT
// and LICENCE files at the top-level directory of this distribution
// for details of copyright and licensing.
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

/**
 * @file gen_fftw_wisdom.hpp
 * @author Mike S Wang (https://github.com/MikeSWang)
 * @brief Generate FFTW wisdom.
 */

#ifndef FFTW_AGE_INCLUDE_GEN_FFTW_WISDOM_HPP_INCLUDED_
#define FFTW_AGE_INCLUDE_GEN_FFTW_WISDOM_HPP_INCLUDED_

#include <cstdarg>
#include <cstdio>
#include <stdexcept>
#include <string>

#include <fftw3.h>
#include <omp.h>

namespace fftw_age {

/**
 * @brief FFT transform type.
 *
 */
enum TransformType {
  C2C = 0,  ///< complex-to-complex ("c")
  R2C = 1,  ///< real-to-complex ("f")
  C2R = 2,  ///< complex-to-real ("b")
  R2R = 3,  ///< real-to-real ("r")
};

/**
 * @brief FFT transform direction.
 *
 */
enum TransformDirection {
  FORWARD = FFTW_FORWARD,   ///< forward FFT ("f")
  BACKWARD = FFTW_BACKWARD  ///< backward FFT ("b")
};

/**
 * @brief FFT transform planner flag (rigour).
 *
 * @see <a href="https://www.fftw.org/fftw3_doc/Planner-Flags.html">
 * FFTW Planner Flags</a>
 *
 */
enum TransformPlanner {
  ESTIMATE = FFTW_ESTIMATE,     ///< estimated ("e")
  MEASURE = FFTW_MEASURE,       ///< measured ("m")
  PATIENT = FFTW_PATIENT,       ///< patient ("p")
  EXHAUSTIVE = FFTW_EXHAUSTIVE  ///< exhaustive ("x")
};

/**
 * @brief Exception raised when the transform specification is invalid.
 *
 */
class InvalidTransformError: public std::invalid_argument {
 public:
  std::string err_mesg;  ///< error message

  /**
   * @brief Construct an @ref fftw_age::InvalidTransformError exception.
   *
   * @param fmt_string Error message format string.
   * @param ... An arbitrary number of substitution arguments.
   */
  InvalidTransformError(const char* fmt_string, ...);

  /**
   * @brief Exception string representation.
   *
   * @returns String representation of the exception.
   */
  virtual const char* what() const noexcept;
};

/**
 * @brief Generate FFTW wisdom.
 *
 * @param ttype FFT transform type.
 * @param tdir FFT transform direction.
 * @param tplan FFT transform planner flag.
 * @param dimx, dimy, dimz FFT transform dimensions.
 * @param wisdom_file Output FFTW wisdom file.
 * @param nthreads Number of threads to use (default 1).
 *
 */
void gen_fftw_wisdom(
    TransformType ttype, TransformDirection tdir, TransformPlanner tplan,
    long long dimx, long long dimy, long long dimz,
    std::string wisdom_file,
    int nthreads = 1
);

}  // namespace fftw_age

#endif  // !FFTW_AGE_INCLUDE_GEN_FFTW_WISDOM_HPP_INCLUDED_
