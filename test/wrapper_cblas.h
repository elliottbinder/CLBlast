
// =================================================================================================
// This file is part of the CLBlast project. The project is licensed under Apache Version 2.0. This
// project loosely follows the Google C++ styleguide and uses a tab-size of two spaces and a max-
// width of 100 characters per line.
//
// Author(s):
//   Cedric Nugteren <www.cedricnugteren.nl>
//
// This file implements a wrapper around a CPU BLAS library, such that its routines can be called
// in a similar way as the CLBlast routines: using alpha and beta to determine the precision.
//
// =================================================================================================

#ifndef CLBLAST_TEST_WRAPPER_CBLAS_H_
#define CLBLAST_TEST_WRAPPER_CBLAS_H_

extern "C"
{
  #include <cblas.h>
}

#include "internal/utilities.h"

namespace clblast {

// Conversions from CLBlast types
CBLAS_ORDER convertToCBLAS(const Layout v) { return (v == Layout::kRowMajor) ? CblasRowMajor : CblasColMajor; }
CBLAS_TRANSPOSE convertToCBLAS(const Transpose v) { return (v == Transpose::kNo) ? CblasNoTrans : (v == Transpose::kYes) ? CblasTrans : CblasConjTrans; }
CBLAS_UPLO convertToCBLAS(const Triangle v) { return (v == Triangle::kUpper) ? CblasUpper : CblasLower; }
CBLAS_DIAG convertToCBLAS(const Diagonal v) { return (v == Diagonal::kUnit) ? CblasUnit : CblasNonUnit; }
CBLAS_SIDE convertToCBLAS(const Side v) { return (v == Side::kLeft) ? CblasLeft : CblasRight; }

// OpenBLAS is not fully Netlib CBLAS compatible
#ifdef OPENBLAS_VERSION
  using return_pointer_float = openblas_complex_float*;
  using return_pointer_double = openblas_complex_double*;
#else
  using return_pointer_float = void*;
  using return_pointer_double = void*;
#endif

// =================================================================================================
// BLAS level-1 (vector-vector) routines
// =================================================================================================

// Forwards the Netlib BLAS calls for SROTG/DROTG
void cblasXrotg(std::vector<float>& sa_buffer, const size_t sa_offset,
                std::vector<float>& sb_buffer, const size_t sb_offset,
                std::vector<float>& sc_buffer, const size_t sc_offset,
                std::vector<float>& ss_buffer, const size_t ss_offset) {
  cblas_srotg(&sa_buffer[sa_offset],
              &sb_buffer[sb_offset],
              &sc_buffer[sc_offset],
              &ss_buffer[ss_offset]);
}
void cblasXrotg(std::vector<double>& sa_buffer, const size_t sa_offset,
                std::vector<double>& sb_buffer, const size_t sb_offset,
                std::vector<double>& sc_buffer, const size_t sc_offset,
                std::vector<double>& ss_buffer, const size_t ss_offset) {
  cblas_drotg(&sa_buffer[sa_offset],
              &sb_buffer[sb_offset],
              &sc_buffer[sc_offset],
              &ss_buffer[ss_offset]);
}

// Forwards the Netlib BLAS calls for SROTMG/DROTMG
void cblasXrotmg(std::vector<float>& sd1_buffer, const size_t sd1_offset,
                 std::vector<float>& sd2_buffer, const size_t sd2_offset,
                 std::vector<float>& sx1_buffer, const size_t sx1_offset,
                 const std::vector<float>& sy1_buffer, const size_t sy1_offset,
                 std::vector<float>& sparam_buffer, const size_t sparam_offset) {
  cblas_srotmg(&sd1_buffer[sd1_offset],
               &sd2_buffer[sd2_offset],
               &sx1_buffer[sx1_offset],
               sy1_buffer[sy1_offset],
               &sparam_buffer[sparam_offset]);
}
void cblasXrotmg(std::vector<double>& sd1_buffer, const size_t sd1_offset,
                 std::vector<double>& sd2_buffer, const size_t sd2_offset,
                 std::vector<double>& sx1_buffer, const size_t sx1_offset,
                 const std::vector<double>& sy1_buffer, const size_t sy1_offset,
                 std::vector<double>& sparam_buffer, const size_t sparam_offset) {
  cblas_drotmg(&sd1_buffer[sd1_offset],
               &sd2_buffer[sd2_offset],
               &sx1_buffer[sx1_offset],
               sy1_buffer[sy1_offset],
               &sparam_buffer[sparam_offset]);
}

// Forwards the Netlib BLAS calls for SROT/DROT
void cblasXrot(const size_t n,
               std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc,
               const float cos,
               const float sin) {
  cblas_srot(n,
             &x_buffer[x_offset], static_cast<int>(x_inc),
             &y_buffer[y_offset], static_cast<int>(y_inc),
             cos,
             sin);
}
void cblasXrot(const size_t n,
               std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc,
               const double cos,
               const double sin) {
  cblas_drot(n,
             &x_buffer[x_offset], static_cast<int>(x_inc),
             &y_buffer[y_offset], static_cast<int>(y_inc),
             cos,
             sin);
}

// Forwards the Netlib BLAS calls for SROTM/DROTM
void cblasXrotm(const size_t n,
                std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<float>& sparam_buffer, const size_t sparam_offset) {
  cblas_srotm(n,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc),
              &sparam_buffer[sparam_offset]);
}
void cblasXrotm(const size_t n,
                std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<double>& sparam_buffer, const size_t sparam_offset) {
  cblas_drotm(n,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc),
              &sparam_buffer[sparam_offset]);
}

// Forwards the Netlib BLAS calls for SSWAP/DSWAP/CSWAP/ZSWAP
void cblasXswap(const size_t n,
                std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_sswap(n,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXswap(const size_t n,
                std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_dswap(n,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXswap(const size_t n,
                std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_cswap(n,
              reinterpret_cast<float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<float*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}
void cblasXswap(const size_t n,
                std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_zswap(n,
              reinterpret_cast<double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<double*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for SSCAL/DSCAL/CSCAL/ZSCAL
void cblasXscal(const size_t n,
                const float alpha,
                std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_sscal(n,
              alpha,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXscal(const size_t n,
                const double alpha,
                std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_dscal(n,
              alpha,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXscal(const size_t n,
                const float2 alpha,
                std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  cblas_cscal(n,
              alpha_array.data(),
              reinterpret_cast<float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXscal(const size_t n,
                const double2 alpha,
                std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  cblas_zscal(n,
              alpha_array.data(),
              reinterpret_cast<double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// Forwards the Netlib BLAS calls for SCOPY/DCOPY/CCOPY/ZCOPY
void cblasXcopy(const size_t n,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_scopy(n,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXcopy(const size_t n,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_dcopy(n,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXcopy(const size_t n,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_ccopy(n,
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<float*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}
void cblasXcopy(const size_t n,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_zcopy(n,
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<double*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for SAXPY/DAXPY/CAXPY/ZAXPY
void cblasXaxpy(const size_t n,
                const float alpha,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_saxpy(n,
              alpha,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXaxpy(const size_t n,
                const double alpha,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_daxpy(n,
              alpha,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXaxpy(const size_t n,
                const float2 alpha,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  cblas_caxpy(n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<float*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}
void cblasXaxpy(const size_t n,
                const double2 alpha,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  cblas_zaxpy(n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<double*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for SDOT/DDOT
void cblasXdot(const size_t n,
               std::vector<float>& dot_buffer, const size_t dot_offset,
               const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
               const std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc) {
  dot_buffer[dot_offset] = cblas_sdot(n,
                                      &x_buffer[x_offset], static_cast<int>(x_inc),
                                      &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXdot(const size_t n,
               std::vector<double>& dot_buffer, const size_t dot_offset,
               const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
               const std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc) {
  dot_buffer[dot_offset] = cblas_ddot(n,
                                      &x_buffer[x_offset], static_cast<int>(x_inc),
                                      &y_buffer[y_offset], static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for CDOTU/ZDOTU
void cblasXdotu(const size_t n,
                std::vector<float2>& dot_buffer, const size_t dot_offset,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_cdotu_sub(n,
                  reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
                  reinterpret_cast<const float*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
                  reinterpret_cast<return_pointer_float>(&dot_buffer[dot_offset]));
}
void cblasXdotu(const size_t n,
                std::vector<double2>& dot_buffer, const size_t dot_offset,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_zdotu_sub(n,
                  reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
                  reinterpret_cast<const double*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
                  reinterpret_cast<return_pointer_double>(&dot_buffer[dot_offset]));
}

// Forwards the Netlib BLAS calls for CDOTC/ZDOTC
void cblasXdotc(const size_t n,
                std::vector<float2>& dot_buffer, const size_t dot_offset,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_cdotc_sub(n,
                  reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
                  reinterpret_cast<const float*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
                  reinterpret_cast<return_pointer_float>(&dot_buffer[dot_offset]));
}
void cblasXdotc(const size_t n,
                std::vector<double2>& dot_buffer, const size_t dot_offset,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_zdotc_sub(n,
                  reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
                  reinterpret_cast<const double*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
                  reinterpret_cast<return_pointer_double>(&dot_buffer[dot_offset]));
}

// Forwards the Netlib BLAS calls for SNRM2/DNRM2/ScNRM2/DzNRM2
void cblasXnrm2(const size_t n,
                std::vector<float>& nrm2_buffer, const size_t nrm2_offset,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  nrm2_buffer[nrm2_offset] = cblas_snrm2(n,
                                         &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXnrm2(const size_t n,
                std::vector<double>& nrm2_buffer, const size_t nrm2_offset,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  nrm2_buffer[nrm2_offset] = cblas_dnrm2(n,
                                         &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXnrm2(const size_t n,
                std::vector<float2>& nrm2_buffer, const size_t nrm2_offset,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  nrm2_buffer[nrm2_offset] = cblas_scnrm2(n,
                                         reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXnrm2(const size_t n,
                std::vector<double2>& nrm2_buffer, const size_t nrm2_offset,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  nrm2_buffer[nrm2_offset] = cblas_dznrm2(n,
                                         reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// Forwards the Netlib BLAS calls for SASUM/DASUM/ScASUM/DzASUM
void cblasXasum(const size_t n,
                std::vector<float>& asum_buffer, const size_t asum_offset,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  asum_buffer[asum_offset] = cblas_sasum(n,
                                         &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXasum(const size_t n,
                std::vector<double>& asum_buffer, const size_t asum_offset,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  asum_buffer[asum_offset] = cblas_dasum(n,
                                         &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXasum(const size_t n,
                std::vector<float2>& asum_buffer, const size_t asum_offset,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  asum_buffer[asum_offset] = cblas_scasum(n,
                                         reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXasum(const size_t n,
                std::vector<double2>& asum_buffer, const size_t asum_offset,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  asum_buffer[asum_offset] = cblas_dzasum(n,
                                         reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// Forwards the Netlib BLAS calls for iSAMAX/iDAMAX/iCAMAX/iZAMAX
void cblasXamax(const size_t n,
                std::vector<float>& imax_buffer, const size_t imax_offset,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  imax_buffer[imax_offset] = cblas_isamax(n,
                                         &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXamax(const size_t n,
                std::vector<double>& imax_buffer, const size_t imax_offset,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  imax_buffer[imax_offset] = cblas_idamax(n,
                                         &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXamax(const size_t n,
                std::vector<float2>& imax_buffer, const size_t imax_offset,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  imax_buffer[imax_offset] = cblas_icamax(n,
                                         reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXamax(const size_t n,
                std::vector<double2>& imax_buffer, const size_t imax_offset,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  imax_buffer[imax_offset] = cblas_izamax(n,
                                         reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// =================================================================================================
// BLAS level-2 (matrix-vector) routines
// =================================================================================================

// Forwards the Netlib BLAS calls for SGEMV/DGEMV/CGEMV/ZGEMV
void cblasXgemv(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose,
                const size_t m, const size_t n,
                const float alpha,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float beta,
                std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_sgemv(layout, a_transpose,
              m, n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXgemv(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose,
                const size_t m, const size_t n,
                const double alpha,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double beta,
                std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_dgemv(layout, a_transpose,
              m, n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXgemv(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose,
                const size_t m, const size_t n,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float2 beta,
                std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_cgemv(layout, a_transpose,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<float*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}
void cblasXgemv(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose,
                const size_t m, const size_t n,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double2 beta,
                std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zgemv(layout, a_transpose,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<double*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for SGBMV/DGBMV/CGBMV/ZGBMV
void cblasXgbmv(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose,
                const size_t m, const size_t n, const size_t kl, const size_t ku,
                const float alpha,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float beta,
                std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_sgbmv(layout, a_transpose,
              m, n, kl, ku,
              alpha,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXgbmv(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose,
                const size_t m, const size_t n, const size_t kl, const size_t ku,
                const double alpha,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double beta,
                std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_dgbmv(layout, a_transpose,
              m, n, kl, ku,
              alpha,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXgbmv(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose,
                const size_t m, const size_t n, const size_t kl, const size_t ku,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float2 beta,
                std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_cgbmv(layout, a_transpose,
              m, n, kl, ku,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<float*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}
void cblasXgbmv(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose,
                const size_t m, const size_t n, const size_t kl, const size_t ku,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double2 beta,
                std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zgbmv(layout, a_transpose,
              m, n, kl, ku,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<double*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for CHEMV/ZHEMV
void cblasXhemv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float2 beta,
                std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_chemv(layout, triangle,
              n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<float*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}
void cblasXhemv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double2 beta,
                std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zhemv(layout, triangle,
              n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<double*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for CHBMV/ZHBMV
void cblasXhbmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n, const size_t k,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float2 beta,
                std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_chbmv(layout, triangle,
              n, k,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<float*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}
void cblasXhbmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n, const size_t k,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double2 beta,
                std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zhbmv(layout, triangle,
              n, k,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<double*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for CHPMV/ZHPMV
void cblasXhpmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const float2 alpha,
                const std::vector<float2>& ap_buffer, const size_t ap_offset,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float2 beta,
                std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_chpmv(layout, triangle,
              n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&ap_buffer[ap_offset]),
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<float*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}
void cblasXhpmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const double2 alpha,
                const std::vector<double2>& ap_buffer, const size_t ap_offset,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double2 beta,
                std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zhpmv(layout, triangle,
              n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&ap_buffer[ap_offset]),
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              beta_array.data(),
              reinterpret_cast<double*>(&y_buffer[y_offset]), static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for SSYMV/DSYMV
void cblasXsymv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const float alpha,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float beta,
                std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_ssymv(layout, triangle,
              n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXsymv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const double alpha,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double beta,
                std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_dsymv(layout, triangle,
              n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for SSBMV/DSBMV
void cblasXsbmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n, const size_t k,
                const float alpha,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float beta,
                std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_ssbmv(layout, triangle,
              n, k,
              alpha,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXsbmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n, const size_t k,
                const double alpha,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double beta,
                std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_dsbmv(layout, triangle,
              n, k,
              alpha,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for SSPMV/DSPMV
void cblasXspmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const float alpha,
                const std::vector<float>& ap_buffer, const size_t ap_offset,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                const float beta,
                std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_sspmv(layout, triangle,
              n,
              alpha,
              &ap_buffer[ap_offset],
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}
void cblasXspmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const double alpha,
                const std::vector<double>& ap_buffer, const size_t ap_offset,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                const double beta,
                std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc) {
  cblas_dspmv(layout, triangle,
              n,
              alpha,
              &ap_buffer[ap_offset],
              &x_buffer[x_offset], static_cast<int>(x_inc),
              beta,
              &y_buffer[y_offset], static_cast<int>(y_inc));
}

// Forwards the Netlib BLAS calls for STRMV/DTRMV/CTRMV/ZTRMV
void cblasXtrmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_strmv(layout, triangle, a_transpose, diagonal,
              n,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtrmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_dtrmv(layout, triangle, a_transpose, diagonal,
              n,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtrmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ctrmv(layout, triangle, a_transpose, diagonal,
              n,
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXtrmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ztrmv(layout, triangle, a_transpose, diagonal,
              n,
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// Forwards the Netlib BLAS calls for STBMV/DTBMV/CTBMV/ZTBMV
void cblasXtbmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n, const size_t k,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_stbmv(layout, triangle, a_transpose, diagonal,
              n, k,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtbmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n, const size_t k,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_dtbmv(layout, triangle, a_transpose, diagonal,
              n, k,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtbmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n, const size_t k,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ctbmv(layout, triangle, a_transpose, diagonal,
              n, k,
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXtbmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n, const size_t k,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ztbmv(layout, triangle, a_transpose, diagonal,
              n, k,
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// Forwards the Netlib BLAS calls for STPMV/DTPMV/CTPMV/ZTPMV
void cblasXtpmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<float>& ap_buffer, const size_t ap_offset,
                std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_stpmv(layout, triangle, a_transpose, diagonal,
              n,
              &ap_buffer[ap_offset],
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtpmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<double>& ap_buffer, const size_t ap_offset,
                std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_dtpmv(layout, triangle, a_transpose, diagonal,
              n,
              &ap_buffer[ap_offset],
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtpmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<float2>& ap_buffer, const size_t ap_offset,
                std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ctpmv(layout, triangle, a_transpose, diagonal,
              n,
              reinterpret_cast<const float*>(&ap_buffer[ap_offset]),
              reinterpret_cast<float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXtpmv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<double2>& ap_buffer, const size_t ap_offset,
                std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ztpmv(layout, triangle, a_transpose, diagonal,
              n,
              reinterpret_cast<const double*>(&ap_buffer[ap_offset]),
              reinterpret_cast<double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// Forwards the Netlib BLAS calls for STRSV/DTRSV/CTRSV/ZTRSV
void cblasXtrsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_strsv(layout, triangle, a_transpose, diagonal,
              n,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtrsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_dtrsv(layout, triangle, a_transpose, diagonal,
              n,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtrsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ctrsv(layout, triangle, a_transpose, diagonal,
              n,
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXtrsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ztrsv(layout, triangle, a_transpose, diagonal,
              n,
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// Forwards the Netlib BLAS calls for STBSV/DTBSV/CTBSV/ZTBSV
void cblasXtbsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n, const size_t k,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_stbsv(layout, triangle, a_transpose, diagonal,
              n, k,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtbsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n, const size_t k,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_dtbsv(layout, triangle, a_transpose, diagonal,
              n, k,
              &a_buffer[a_offset], a_ld,
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtbsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n, const size_t k,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ctbsv(layout, triangle, a_transpose, diagonal,
              n, k,
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXtbsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n, const size_t k,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ztbsv(layout, triangle, a_transpose, diagonal,
              n, k,
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// Forwards the Netlib BLAS calls for STPSV/DTPSV/CTPSV/ZTPSV
void cblasXtpsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<float>& ap_buffer, const size_t ap_offset,
                std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_stpsv(layout, triangle, a_transpose, diagonal,
              n,
              &ap_buffer[ap_offset],
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtpsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<double>& ap_buffer, const size_t ap_offset,
                std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_dtpsv(layout, triangle, a_transpose, diagonal,
              n,
              &ap_buffer[ap_offset],
              &x_buffer[x_offset], static_cast<int>(x_inc));
}
void cblasXtpsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<float2>& ap_buffer, const size_t ap_offset,
                std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ctpsv(layout, triangle, a_transpose, diagonal,
              n,
              reinterpret_cast<const float*>(&ap_buffer[ap_offset]),
              reinterpret_cast<float*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}
void cblasXtpsv(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t n,
                const std::vector<double2>& ap_buffer, const size_t ap_offset,
                std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc) {
  cblas_ztpsv(layout, triangle, a_transpose, diagonal,
              n,
              reinterpret_cast<const double*>(&ap_buffer[ap_offset]),
              reinterpret_cast<double*>(&x_buffer[x_offset]), static_cast<int>(x_inc));
}

// Forwards the Netlib BLAS calls for SGER/DGER
void cblasXger(const CBLAS_ORDER layout,
               const size_t m, const size_t n,
               const float alpha,
               const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
               const std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc,
               std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld) {
  cblas_sger(layout,
             m, n,
             alpha,
             &x_buffer[x_offset], static_cast<int>(x_inc),
             &y_buffer[y_offset], static_cast<int>(y_inc),
             &a_buffer[a_offset], a_ld);
}
void cblasXger(const CBLAS_ORDER layout,
               const size_t m, const size_t n,
               const double alpha,
               const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
               const std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc,
               std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld) {
  cblas_dger(layout,
             m, n,
             alpha,
             &x_buffer[x_offset], static_cast<int>(x_inc),
             &y_buffer[y_offset], static_cast<int>(y_inc),
             &a_buffer[a_offset], a_ld);
}

// Forwards the Netlib BLAS calls for CGERU/ZGERU
void cblasXgeru(const CBLAS_ORDER layout,
                const size_t m, const size_t n,
                const float2 alpha,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  cblas_cgeru(layout,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<const float*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
              reinterpret_cast<float*>(&a_buffer[a_offset]), a_ld);
}
void cblasXgeru(const CBLAS_ORDER layout,
                const size_t m, const size_t n,
                const double2 alpha,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  cblas_zgeru(layout,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<const double*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
              reinterpret_cast<double*>(&a_buffer[a_offset]), a_ld);
}

// Forwards the Netlib BLAS calls for CGERC/ZGERC
void cblasXgerc(const CBLAS_ORDER layout,
                const size_t m, const size_t n,
                const float2 alpha,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  cblas_cgerc(layout,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<const float*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
              reinterpret_cast<float*>(&a_buffer[a_offset]), a_ld);
}
void cblasXgerc(const CBLAS_ORDER layout,
                const size_t m, const size_t n,
                const double2 alpha,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  cblas_zgerc(layout,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<const double*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
              reinterpret_cast<double*>(&a_buffer[a_offset]), a_ld);
}

// Forwards the Netlib BLAS calls for CHER/ZHER
void cblasXher(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
               const size_t n,
               const float alpha,
               const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld) {
  cblas_cher(layout, triangle,
             n,
             alpha,
             reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
             reinterpret_cast<float*>(&a_buffer[a_offset]), a_ld);
}
void cblasXher(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
               const size_t n,
               const double alpha,
               const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld) {
  cblas_zher(layout, triangle,
             n,
             alpha,
             reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
             reinterpret_cast<double*>(&a_buffer[a_offset]), a_ld);
}

// Forwards the Netlib BLAS calls for CHPR/ZHPR
void cblasXhpr(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
               const size_t n,
               const float alpha,
               const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<float2>& ap_buffer, const size_t ap_offset) {
  cblas_chpr(layout, triangle,
             n,
             alpha,
             reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
             reinterpret_cast<float*>(&ap_buffer[ap_offset]));
}
void cblasXhpr(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
               const size_t n,
               const double alpha,
               const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<double2>& ap_buffer, const size_t ap_offset) {
  cblas_zhpr(layout, triangle,
             n,
             alpha,
             reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
             reinterpret_cast<double*>(&ap_buffer[ap_offset]));
}

// Forwards the Netlib BLAS calls for CHER2/ZHER2
void cblasXher2(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const float2 alpha,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  cblas_cher2(layout, triangle,
              n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<const float*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
              reinterpret_cast<float*>(&a_buffer[a_offset]), a_ld);
}
void cblasXher2(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const double2 alpha,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  cblas_zher2(layout, triangle,
              n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<const double*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
              reinterpret_cast<double*>(&a_buffer[a_offset]), a_ld);
}

// Forwards the Netlib BLAS calls for CHPR2/ZHPR2
void cblasXhpr2(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const float2 alpha,
                const std::vector<float2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<float2>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<float2>& ap_buffer, const size_t ap_offset) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  cblas_chpr2(layout, triangle,
              n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<const float*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
              reinterpret_cast<float*>(&ap_buffer[ap_offset]));
}
void cblasXhpr2(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const double2 alpha,
                const std::vector<double2>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<double2>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<double2>& ap_buffer, const size_t ap_offset) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  cblas_zhpr2(layout, triangle,
              n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&x_buffer[x_offset]), static_cast<int>(x_inc),
              reinterpret_cast<const double*>(&y_buffer[y_offset]), static_cast<int>(y_inc),
              reinterpret_cast<double*>(&ap_buffer[ap_offset]));
}

// Forwards the Netlib BLAS calls for SSYR/DSYR
void cblasXsyr(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
               const size_t n,
               const float alpha,
               const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld) {
  cblas_ssyr(layout, triangle,
             n,
             alpha,
             &x_buffer[x_offset], static_cast<int>(x_inc),
             &a_buffer[a_offset], a_ld);
}
void cblasXsyr(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
               const size_t n,
               const double alpha,
               const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld) {
  cblas_dsyr(layout, triangle,
             n,
             alpha,
             &x_buffer[x_offset], static_cast<int>(x_inc),
             &a_buffer[a_offset], a_ld);
}

// Forwards the Netlib BLAS calls for SSPR/DSPR
void cblasXspr(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
               const size_t n,
               const float alpha,
               const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<float>& ap_buffer, const size_t ap_offset) {
  cblas_sspr(layout, triangle,
             n,
             alpha,
             &x_buffer[x_offset], static_cast<int>(x_inc),
             &ap_buffer[ap_offset]);
}
void cblasXspr(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
               const size_t n,
               const double alpha,
               const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
               std::vector<double>& ap_buffer, const size_t ap_offset) {
  cblas_dspr(layout, triangle,
             n,
             alpha,
             &x_buffer[x_offset], static_cast<int>(x_inc),
             &ap_buffer[ap_offset]);
}

// Forwards the Netlib BLAS calls for SSYR2/DSYR2
void cblasXsyr2(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const float alpha,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld) {
  cblas_ssyr2(layout, triangle,
              n,
              alpha,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc),
              &a_buffer[a_offset], a_ld);
}
void cblasXsyr2(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const double alpha,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld) {
  cblas_dsyr2(layout, triangle,
              n,
              alpha,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc),
              &a_buffer[a_offset], a_ld);
}

// Forwards the Netlib BLAS calls for SSPR2/DSPR2
void cblasXspr2(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const float alpha,
                const std::vector<float>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<float>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<float>& ap_buffer, const size_t ap_offset) {
  cblas_sspr2(layout, triangle,
              n,
              alpha,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc),
              &ap_buffer[ap_offset]);
}
void cblasXspr2(const CBLAS_ORDER layout, const CBLAS_UPLO triangle,
                const size_t n,
                const double alpha,
                const std::vector<double>& x_buffer, const size_t x_offset, const size_t x_inc,
                const std::vector<double>& y_buffer, const size_t y_offset, const size_t y_inc,
                std::vector<double>& ap_buffer, const size_t ap_offset) {
  cblas_dspr2(layout, triangle,
              n,
              alpha,
              &x_buffer[x_offset], static_cast<int>(x_inc),
              &y_buffer[y_offset], static_cast<int>(y_inc),
              &ap_buffer[ap_offset]);
}

// =================================================================================================
// BLAS level-3 (matrix-matrix) routines
// =================================================================================================

// Forwards the Netlib BLAS calls for SGEMM/DGEMM/CGEMM/ZGEMM
void cblasXgemm(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose, const CBLAS_TRANSPOSE b_transpose,
                const size_t m, const size_t n, const size_t k,
                const float alpha,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float>& b_buffer, const size_t b_offset, const size_t b_ld,
                const float beta,
                std::vector<float>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_sgemm(layout, a_transpose, b_transpose,
              m, n, k,
              alpha,
              &a_buffer[a_offset], a_ld,
              &b_buffer[b_offset], b_ld,
              beta,
              &c_buffer[c_offset], c_ld);
}
void cblasXgemm(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose, const CBLAS_TRANSPOSE b_transpose,
                const size_t m, const size_t n, const size_t k,
                const double alpha,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double>& b_buffer, const size_t b_offset, const size_t b_ld,
                const double beta,
                std::vector<double>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_dgemm(layout, a_transpose, b_transpose,
              m, n, k,
              alpha,
              &a_buffer[a_offset], a_ld,
              &b_buffer[b_offset], b_ld,
              beta,
              &c_buffer[c_offset], c_ld);
}
void cblasXgemm(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose, const CBLAS_TRANSPOSE b_transpose,
                const size_t m, const size_t n, const size_t k,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float2>& b_buffer, const size_t b_offset, const size_t b_ld,
                const float2 beta,
                std::vector<float2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_cgemm(layout, a_transpose, b_transpose,
              m, n, k,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const float*>(&b_buffer[b_offset]), b_ld,
              beta_array.data(),
              reinterpret_cast<float*>(&c_buffer[c_offset]), c_ld);
}
void cblasXgemm(const CBLAS_ORDER layout, const CBLAS_TRANSPOSE a_transpose, const CBLAS_TRANSPOSE b_transpose,
                const size_t m, const size_t n, const size_t k,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double2>& b_buffer, const size_t b_offset, const size_t b_ld,
                const double2 beta,
                std::vector<double2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zgemm(layout, a_transpose, b_transpose,
              m, n, k,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const double*>(&b_buffer[b_offset]), b_ld,
              beta_array.data(),
              reinterpret_cast<double*>(&c_buffer[c_offset]), c_ld);
}

// Forwards the Netlib BLAS calls for SSYMM/DSYMM/CSYMM/ZSYMM
void cblasXsymm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle,
                const size_t m, const size_t n,
                const float alpha,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float>& b_buffer, const size_t b_offset, const size_t b_ld,
                const float beta,
                std::vector<float>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_ssymm(layout, side, triangle,
              m, n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &b_buffer[b_offset], b_ld,
              beta,
              &c_buffer[c_offset], c_ld);
}
void cblasXsymm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle,
                const size_t m, const size_t n,
                const double alpha,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double>& b_buffer, const size_t b_offset, const size_t b_ld,
                const double beta,
                std::vector<double>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_dsymm(layout, side, triangle,
              m, n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &b_buffer[b_offset], b_ld,
              beta,
              &c_buffer[c_offset], c_ld);
}
void cblasXsymm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle,
                const size_t m, const size_t n,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float2>& b_buffer, const size_t b_offset, const size_t b_ld,
                const float2 beta,
                std::vector<float2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_csymm(layout, side, triangle,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const float*>(&b_buffer[b_offset]), b_ld,
              beta_array.data(),
              reinterpret_cast<float*>(&c_buffer[c_offset]), c_ld);
}
void cblasXsymm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle,
                const size_t m, const size_t n,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double2>& b_buffer, const size_t b_offset, const size_t b_ld,
                const double2 beta,
                std::vector<double2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zsymm(layout, side, triangle,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const double*>(&b_buffer[b_offset]), b_ld,
              beta_array.data(),
              reinterpret_cast<double*>(&c_buffer[c_offset]), c_ld);
}

// Forwards the Netlib BLAS calls for CHEMM/ZHEMM
void cblasXhemm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle,
                const size_t m, const size_t n,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<float2>& b_buffer, const size_t b_offset, const size_t b_ld,
                const float2 beta,
                std::vector<float2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_chemm(layout, side, triangle,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const float*>(&b_buffer[b_offset]), b_ld,
              beta_array.data(),
              reinterpret_cast<float*>(&c_buffer[c_offset]), c_ld);
}
void cblasXhemm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle,
                const size_t m, const size_t n,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const std::vector<double2>& b_buffer, const size_t b_offset, const size_t b_ld,
                const double2 beta,
                std::vector<double2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zhemm(layout, side, triangle,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<const double*>(&b_buffer[b_offset]), b_ld,
              beta_array.data(),
              reinterpret_cast<double*>(&c_buffer[c_offset]), c_ld);
}

// Forwards the Netlib BLAS calls for SSYRK/DSYRK/CSYRK/ZSYRK
void cblasXsyrk(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose,
                const size_t n, const size_t k,
                const float alpha,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                const float beta,
                std::vector<float>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_ssyrk(layout, triangle, a_transpose,
              n, k,
              alpha,
              &a_buffer[a_offset], a_ld,
              beta,
              &c_buffer[c_offset], c_ld);
}
void cblasXsyrk(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose,
                const size_t n, const size_t k,
                const double alpha,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                const double beta,
                std::vector<double>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_dsyrk(layout, triangle, a_transpose,
              n, k,
              alpha,
              &a_buffer[a_offset], a_ld,
              beta,
              &c_buffer[c_offset], c_ld);
}
void cblasXsyrk(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose,
                const size_t n, const size_t k,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const float2 beta,
                std::vector<float2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_csyrk(layout, triangle, a_transpose,
              n, k,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              beta_array.data(),
              reinterpret_cast<float*>(&c_buffer[c_offset]), c_ld);
}
void cblasXsyrk(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose,
                const size_t n, const size_t k,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const double2 beta,
                std::vector<double2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zsyrk(layout, triangle, a_transpose,
              n, k,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              beta_array.data(),
              reinterpret_cast<double*>(&c_buffer[c_offset]), c_ld);
}

// Forwards the Netlib BLAS calls for CHERK/ZHERK
void cblasXherk(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose,
                const size_t n, const size_t k,
                const float alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const float beta,
                std::vector<float2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_cherk(layout, triangle, a_transpose,
              n, k,
              alpha,
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              beta,
              reinterpret_cast<float*>(&c_buffer[c_offset]), c_ld);
}
void cblasXherk(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose,
                const size_t n, const size_t k,
                const double alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                const double beta,
                std::vector<double2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_zherk(layout, triangle, a_transpose,
              n, k,
              alpha,
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              beta,
              reinterpret_cast<double*>(&c_buffer[c_offset]), c_ld);
}

// Forwards the Netlib BLAS calls for SSYR2K/DSYR2K/CSYR2K/ZSYR2K
void cblasXsyr2k(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE ab_transpose,
                 const size_t n, const size_t k,
                 const float alpha,
                 const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                 const std::vector<float>& b_buffer, const size_t b_offset, const size_t b_ld,
                 const float beta,
                 std::vector<float>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_ssyr2k(layout, triangle, ab_transpose,
               n, k,
               alpha,
               &a_buffer[a_offset], a_ld,
               &b_buffer[b_offset], b_ld,
               beta,
               &c_buffer[c_offset], c_ld);
}
void cblasXsyr2k(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE ab_transpose,
                 const size_t n, const size_t k,
                 const double alpha,
                 const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                 const std::vector<double>& b_buffer, const size_t b_offset, const size_t b_ld,
                 const double beta,
                 std::vector<double>& c_buffer, const size_t c_offset, const size_t c_ld) {
  cblas_dsyr2k(layout, triangle, ab_transpose,
               n, k,
               alpha,
               &a_buffer[a_offset], a_ld,
               &b_buffer[b_offset], b_ld,
               beta,
               &c_buffer[c_offset], c_ld);
}
void cblasXsyr2k(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE ab_transpose,
                 const size_t n, const size_t k,
                 const float2 alpha,
                 const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                 const std::vector<float2>& b_buffer, const size_t b_offset, const size_t b_ld,
                 const float2 beta,
                 std::vector<float2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<float>{beta.real(), beta.imag()};
  cblas_csyr2k(layout, triangle, ab_transpose,
               n, k,
               alpha_array.data(),
               reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
               reinterpret_cast<const float*>(&b_buffer[b_offset]), b_ld,
               beta_array.data(),
               reinterpret_cast<float*>(&c_buffer[c_offset]), c_ld);
}
void cblasXsyr2k(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE ab_transpose,
                 const size_t n, const size_t k,
                 const double2 alpha,
                 const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                 const std::vector<double2>& b_buffer, const size_t b_offset, const size_t b_ld,
                 const double2 beta,
                 std::vector<double2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  const auto beta_array = std::vector<double>{beta.real(), beta.imag()};
  cblas_zsyr2k(layout, triangle, ab_transpose,
               n, k,
               alpha_array.data(),
               reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
               reinterpret_cast<const double*>(&b_buffer[b_offset]), b_ld,
               beta_array.data(),
               reinterpret_cast<double*>(&c_buffer[c_offset]), c_ld);
}

// Forwards the Netlib BLAS calls for CHER2K/ZHER2K
void cblasXher2k(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE ab_transpose,
                 const size_t n, const size_t k,
                 const float2 alpha,
                 const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                 const std::vector<float2>& b_buffer, const size_t b_offset, const size_t b_ld,
                 const float beta,
                 std::vector<float2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  cblas_cher2k(layout, triangle, ab_transpose,
               n, k,
               alpha_array.data(),
               reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
               reinterpret_cast<const float*>(&b_buffer[b_offset]), b_ld,
               beta,
               reinterpret_cast<float*>(&c_buffer[c_offset]), c_ld);
}
void cblasXher2k(const CBLAS_ORDER layout, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE ab_transpose,
                 const size_t n, const size_t k,
                 const double2 alpha,
                 const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                 const std::vector<double2>& b_buffer, const size_t b_offset, const size_t b_ld,
                 const double beta,
                 std::vector<double2>& c_buffer, const size_t c_offset, const size_t c_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  cblas_zher2k(layout, triangle, ab_transpose,
               n, k,
               alpha_array.data(),
               reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
               reinterpret_cast<const double*>(&b_buffer[b_offset]), b_ld,
               beta,
               reinterpret_cast<double*>(&c_buffer[c_offset]), c_ld);
}

// Forwards the Netlib BLAS calls for STRMM/DTRMM/CTRMM/ZTRMM
void cblasXtrmm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t m, const size_t n,
                const float alpha,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float>& b_buffer, const size_t b_offset, const size_t b_ld) {
  cblas_strmm(layout, side, triangle, a_transpose, diagonal,
              m, n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &b_buffer[b_offset], b_ld);
}
void cblasXtrmm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t m, const size_t n,
                const double alpha,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double>& b_buffer, const size_t b_offset, const size_t b_ld) {
  cblas_dtrmm(layout, side, triangle, a_transpose, diagonal,
              m, n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &b_buffer[b_offset], b_ld);
}
void cblasXtrmm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t m, const size_t n,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float2>& b_buffer, const size_t b_offset, const size_t b_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  cblas_ctrmm(layout, side, triangle, a_transpose, diagonal,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<float*>(&b_buffer[b_offset]), b_ld);
}
void cblasXtrmm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t m, const size_t n,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double2>& b_buffer, const size_t b_offset, const size_t b_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  cblas_ztrmm(layout, side, triangle, a_transpose, diagonal,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<double*>(&b_buffer[b_offset]), b_ld);
}

// Forwards the Netlib BLAS calls for STRSM/DTRSM/CTRSM/ZTRSM
void cblasXtrsm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t m, const size_t n,
                const float alpha,
                const std::vector<float>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float>& b_buffer, const size_t b_offset, const size_t b_ld) {
  cblas_strsm(layout, side, triangle, a_transpose, diagonal,
              m, n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &b_buffer[b_offset], b_ld);
}
void cblasXtrsm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t m, const size_t n,
                const double alpha,
                const std::vector<double>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double>& b_buffer, const size_t b_offset, const size_t b_ld) {
  cblas_dtrsm(layout, side, triangle, a_transpose, diagonal,
              m, n,
              alpha,
              &a_buffer[a_offset], a_ld,
              &b_buffer[b_offset], b_ld);
}
void cblasXtrsm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t m, const size_t n,
                const float2 alpha,
                const std::vector<float2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<float2>& b_buffer, const size_t b_offset, const size_t b_ld) {
  const auto alpha_array = std::vector<float>{alpha.real(), alpha.imag()};
  cblas_ctrsm(layout, side, triangle, a_transpose, diagonal,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const float*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<float*>(&b_buffer[b_offset]), b_ld);
}
void cblasXtrsm(const CBLAS_ORDER layout, const CBLAS_SIDE side, const CBLAS_UPLO triangle, const CBLAS_TRANSPOSE a_transpose, const CBLAS_DIAG diagonal,
                const size_t m, const size_t n,
                const double2 alpha,
                const std::vector<double2>& a_buffer, const size_t a_offset, const size_t a_ld,
                std::vector<double2>& b_buffer, const size_t b_offset, const size_t b_ld) {
  const auto alpha_array = std::vector<double>{alpha.real(), alpha.imag()};
  cblas_ztrsm(layout, side, triangle, a_transpose, diagonal,
              m, n,
              alpha_array.data(),
              reinterpret_cast<const double*>(&a_buffer[a_offset]), a_ld,
              reinterpret_cast<double*>(&b_buffer[b_offset]), b_ld);
}

// =================================================================================================
} // namespace clblast

// CLBLAST_TEST_WRAPPER_CBLAS_H_
#endif