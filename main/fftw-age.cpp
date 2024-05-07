// Copyright (C) [GPLv3 Licence]
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
 * @file fftw-age.cpp
 * @author Mike S Wang (https://github.com/MikeSWang)
 * @brief Generate multithreaded FFTW wisdom and export to file.
 *
 * @attention Only <em>in-place</em> transforms are supported.
 * @attention Currently, only <em>'measured'</em> or <em>'patient'</em>
 *            <em>complex-to-complex</em> transforms are supported.
 * @attention Currently, there is no wisdom file overwrite protection.
 *
 */

#include <fftw3.h>
#include <omp.h>

#include <cstring>
#include <stdexcept>
#include <string>

#include "gen_fftw_wisdom.hpp"

using namespace fftw_age;

int main(int argc, char* argv[]) {
  // Declare fixed arguments.
  char ARG_TTARR[16] = "i";  // in-place
  char arg_ttype[16] = "c";  // complex-to-complex

  // Parse command line arguments.
  char* arg_tplan = argv[1];
  char* arg_tdir = argv[2];
  long long dimx = atoi(argv[3]);
  long long dimy = atoi(argv[4]);
  long long dimz = atoi(argv[5]);
  char* wisdom_dir = argv[6];

  // Parse environmental variables.
  int nthreads = omp_get_max_threads();

  // Derive FFTW specification.
  TransformType ttype;
  if (std::strcmp(arg_ttype, "c") == 0) {
    ttype = TransformType::C2C;
  } else {
    std::string err_mesg =
      "Unsupported transform type: " + std::string(arg_ttype);
    throw std::invalid_argument(err_mesg.c_str());
  }

  TransformDirection tdir;
  if (std::strcmp(arg_tdir, "f") == 0) {
    tdir = TransformDirection::FORWARD;
  } else
  if (std::strcmp(arg_tdir, "b") == 0) {
    tdir = TransformDirection::BACKWARD;
  } else {
    std::string err_mesg =
      "Invalid transform direction: " + std::string(arg_tdir);
    throw std::invalid_argument(err_mesg.c_str());
  }

  TransformPlanner tplan;
  if (std::strcmp(arg_tplan, "m") == 0) {
    tplan = TransformPlanner::MEASURE;
  } else
  if (std::strcmp(arg_tplan, "p") == 0) {
    tplan = TransformPlanner::PATIENT;
  } else {
    std::string err_mesg =
      "Unsupported transform planner: " + std::string(arg_tplan);
    throw std::invalid_argument(err_mesg.c_str());
  }

  // Derive wisdom file name.
  std::string wfile_prefix = std::string(wisdom_dir) + "/" + "fftw_omp_";
  std::string wfile_suffix = ".wisdom";
  std::string wfile_tag =
    std::string(arg_ttype) + std::string(ARG_TTARR) + std::string(arg_tdir)
    + "_" + std::to_string(dimx)
    + "x" + std::to_string(dimy)
    + "x" + std::to_string(dimz)
    + "_" + std::string(arg_tplan);

  std::string wisdom_file = wfile_prefix + wfile_tag + wfile_suffix;

  // Generate wisdom.
  fftw_init_threads();
  gen_fftw_wisdom(ttype, tdir, tplan, dimx, dimy, dimz, wisdom_file, nthreads);
  fftw_cleanup_threads();

  return 0;
}
