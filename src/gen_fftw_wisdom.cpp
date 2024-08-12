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
 * @file gen_fftw_wisdom.cpp
 * @author Mike S Wang (https://github.com/MikeSWang)
 */

#include "gen_fftw_wisdom.hpp"

namespace fftw_age {

InvalidTransformError::InvalidTransformError(const char* fmt_string, ...):
std::invalid_argument("Invalid transform error.") {
  std::va_list args;
  va_start(args, fmt_string);
  char err_mesg_buf[4096];
  std::vsnprintf(err_mesg_buf, sizeof(err_mesg_buf), fmt_string, args);
  va_end(args);

  this->err_mesg = std::string(err_mesg_buf);
}

const char* InvalidTransformError::what() const noexcept {
  return this->err_mesg.c_str();
}

void gen_fftw_wisdom(
    TransformType ttype, TransformDirection tdir, TransformPlanner tplan,
    long long dimx, long long dimy, long long dimz,
    std::string wisdom_file,
    int nthreads
) {
  // Check transform specifications.
  if (ttype < 0 || ttype > 3) {
    throw InvalidTransformError(
        "Invalid transform type: `ttype` = %d", ttype
    );
  }
  if (ttype != 0) {
    throw InvalidTransformError(
        "Only complex-to-complex transforms are "
        "currently supported: `ttype` = %d", ttype
    );
  }

  if (tdir != FORWARD && tdir != BACKWARD) {
    throw InvalidTransformError(
        "Invalid transform direction: `tdir` = %d", tdir
    );
  }

  if (tplan != MEASURE && tplan != PATIENT) {
    throw InvalidTransformError(
        "Only `FFTW_MEASURE` and `FFTW_PATIENT` planners "
        "are currently supported: `tplan` = %s", tplan
    );
  }

  // Plan FFTW.
  bool flag_plan_gen = false;

  fftw_complex* cgrid = nullptr;
  double* rgrid = nullptr;

  fftw_plan_with_nthreads(nthreads);
  fftw_plan transform;

  if (ttype == TransformType::C2C) {
    cgrid = fftw_alloc_complex(dimx * dimy * dimz);
    transform = fftw_plan_dft_3d(dimx, dimy, dimz, cgrid, cgrid, tdir, tplan);
    flag_plan_gen = true;
  } else {
    throw InvalidTransformError(
        "Unsupported transform type: `ttype` = %d", ttype
    );
  }

  if (!flag_plan_gen) {
    throw std::runtime_error("No FFTW plan has been generated");
  }

  // Export FFTW wisdom.
  std::FILE* wisdom_file_ptr = std::fopen(wisdom_file.c_str(), "w");
  if (wisdom_file_ptr == nullptr) {
    std::string err_mesg = "Failed to open FFTW wisdom file: " + wisdom_file;
    throw std::runtime_error(err_mesg.c_str());
  }
  fftw_export_wisdom_to_filename(wisdom_file.c_str());

  std::printf(
    "FFTW wisdom file has been exported: %s\n", wisdom_file.c_str()
  );

  // Clean up.
  if (wisdom_file_ptr != nullptr) {std::fclose(wisdom_file_ptr);}
  if (flag_plan_gen) {fftw_destroy_plan(transform);}
  if (cgrid != nullptr) {fftw_free(cgrid);}
  if (rgrid != nullptr) {free(rgrid);}
}

}  // namespace fftw_age
