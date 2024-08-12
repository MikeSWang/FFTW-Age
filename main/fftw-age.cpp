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

#include <cstring>
#include <stdexcept>
#include <string>

#include <fftw3.h>
#include <omp.h>

#include "argparse.hpp"
#include "gen_fftw_wisdom.hpp"

// UPDATE
#define _FFTW_AGE_NAME "fftw-age"
#define _FFTW_AGE_VERSION "0.1.1"

#define _FFTW_VERSION std::string(fftw_version)
#define _OMP_VERSION std::to_string(_OPENMP)
#define _OMP_NTHREADS std::to_string(omp_get_max_threads())

using namespace fftw_age;

int main(int argc, char* argv[]) {

  std::string PROGNAME = _FFTW_AGE_NAME;

  std::string version_line = PROGNAME + " version " + _FFTW_AGE_VERSION + "\n";
  std::string licence_note =
    "Copyright (C) 2024 Mike S Wang                                        \n"
    "This program comes with ABSOLUTELY NO WARRANTY. This is free software,\n"
    "and you are welcome to redistribute it under certain conditions; see  \n"
    "the source for details.                                               \n";
  std::string lib_info =
    "FFTW version: " + _FFTW_VERSION + "\n"
    + "OpenMP version: " + _OMP_VERSION + "\n"
    + "Thread number: " + _OMP_NTHREADS + "\n";
  std::string VERSION = version_line + licence_note + lib_info;

  // Declare command-line arguments.
  argparse::ArgumentParser program(PROGNAME, VERSION);

  program.add_argument("transform")
    .help(
      "FFTW transform specification: "
      "<ttype><ttarr><tdir>-<dimx>x<dimy>x<dimz>, where "
      "the transform type <ttype> is "
        "'c' (fixed) for complex-to-complex, "
      "the transform array <ttarr> is "
        "'i' (fixed) for in-place, "
      "the transform direction <tdir> is "
        "'f' for forward or "
        "'b' for backward, and "
      "the dimensions of the transform are (<dimx>, <dimy>, <dimz>)"
    );
  program.add_argument("-r", "--rigour")
    .help(
      "FFTW planner flag/rigour: "
        "'m' for measured or "
        "'p' for patient")
    .required()
    .default_value(std::string("m"));
  program.add_argument("-o", "--outdir")
    .help("FFTW wisdom output directory")
    .required()
    .default_value(std::string("."));

  program.parse_args(argc, argv);

  if (program.get<bool>("help")) {
    std::cout << program << std::endl;
    return 0;
  }

  // Parse command-line arguments.
  std::string _transform_str = program.get<std::string>("transform");
  std::size_t _split = _transform_str.find('-');

  std::string _tspec_str = _transform_str.substr(0, _split);
  std::string tdir_str = _tspec_str.substr(_tspec_str.length() - 1);

  std::string _tsize_str = _transform_str.substr(_split + 1);
  std::size_t _split_xy = _tsize_str.find('x');
  std::size_t _split_yz = _tsize_str.find('x', _split_xy + 1);

  long long dimx, dimy, dimz;
  dimx = std::stoll(_tsize_str.substr(0, _split_xy));
  dimy = std::stoll(_tsize_str.substr(_split_xy + 1, _split_yz - _split_xy - 1));
  dimz = std::stoll(_tsize_str.substr(_split_yz + 1));

  std::string tplan_str = program.get<std::string>("rigour");
  std::string wisdom_dir = program.get<std::string>("outdir");

  // Declare fixed arguments.
  char ARG_TTYPE[16] = "c";  // complex-to-complex
  char ARG_TTARR[16] = "i";  // in-place

  // Parse environmental variables.
  int nthreads = omp_get_max_threads();

  // Derive FFTW specification.
  TransformType ttype;
  if (std::strcmp(ARG_TTYPE, "c") == 0) {
    ttype = TransformType::C2C;
  } else {
    std::string err_mesg =
      "Unsupported transform type: " + std::string(ARG_TTYPE);
    throw std::invalid_argument(err_mesg.c_str());
  }

  TransformDirection tdir;
  if (tdir_str == "f") {
    tdir = TransformDirection::FORWARD;
  } else
  if (tdir_str == "b") {
    tdir = TransformDirection::BACKWARD;
  } else {
    std::string err_mesg = "Invalid transform direction: " + tdir_str;
    throw std::invalid_argument(err_mesg.c_str());
  }

  TransformPlanner tplan;
  if (tplan_str == "m") {
    tplan = TransformPlanner::MEASURE;
  } else
  if (tplan_str == "p") {
    tplan = TransformPlanner::PATIENT;
  } else {
    std::string err_mesg = "Unsupported transform planner: " + tplan_str;
    throw std::invalid_argument(err_mesg.c_str());
  }

  // Derive wisdom file name.
  std::string wfile_prefix = std::string(wisdom_dir) + "/" + "fftw_omp_";
  std::string wfile_suffix = ".wisdom";
  std::string wfile_tag =
    std::string(ARG_TTYPE) + std::string(ARG_TTARR) + tdir_str
    + "_" + std::to_string(dimx)
    + "x" + std::to_string(dimy)
    + "x" + std::to_string(dimz)
    + "_" + tplan_str;

  std::string wisdom_file = wfile_prefix + wfile_tag + wfile_suffix;

  // Generate wisdom.
  fftw_init_threads();
  gen_fftw_wisdom(ttype, tdir, tplan, dimx, dimy, dimz, wisdom_file, nthreads);
  fftw_cleanup_threads();

  return 0;
}
