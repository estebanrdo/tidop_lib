/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once


#include "tidop/math/algebra/matrix/operations.h"


namespace tl
{

namespace internal
{

/* Determinant 4x4 */

template<typename T, size_t _rows, size_t _col>
auto determinant4x4(const Matrix<T, _rows, _col> &matrix) -> enableIfFloat<T, T>
{

#if defined(TL_HAVE_SIMD_INTRINSICS) && defined(TL_HAVE_SSE2)

    __m128 row0 = _mm_load_ps(&matrix(0, 0));
    __m128 row1 = _mm_load_ps(&matrix(1, 0));
    __m128 row2 = _mm_load_ps(&matrix(2, 0));
    __m128 row3 = _mm_load_ps(&matrix(3, 0));

    // Calcular cofactors de la primera fila
    __m128 fac0, fac1, fac2, fac3;
    {
        __m128 tmp0 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 tmp1 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(1, 1, 1, 1));
        __m128 tmp2 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 tmp3 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(3, 3, 3, 3));

        __m128 tmp4 = _mm_shuffle_ps(row1, row1, _MM_SHUFFLE(3, 2, 1, 0));
        __m128 tmp5 = _mm_shuffle_ps(row1, row1, _MM_SHUFFLE(3, 2, 0, 1));

        fac0 = _mm_sub_ps(_mm_mul_ps(tmp4, tmp3), _mm_mul_ps(tmp5, tmp2));
        fac1 = _mm_sub_ps(_mm_mul_ps(tmp5, tmp1), _mm_mul_ps(tmp4, tmp0));
        fac2 = _mm_sub_ps(_mm_mul_ps(tmp4, tmp2), _mm_mul_ps(tmp5, tmp3));
        fac3 = _mm_sub_ps(_mm_mul_ps(tmp5, tmp0), _mm_mul_ps(tmp4, tmp1));
    }

    // Determinante
    __m128 mul0 = _mm_mul_ps(row0, fac0);
    __m128 mul1 = _mm_mul_ps(_mm_shuffle_ps(row0, row0, _MM_SHUFFLE(3, 2, 1, 0)), fac1);
    __m128 mul2 = _mm_mul_ps(_mm_shuffle_ps(row0, row0, _MM_SHUFFLE(3, 0, 2, 1)), fac2);
    __m128 mul3 = _mm_mul_ps(_mm_shuffle_ps(row0, row0, _MM_SHUFFLE(3, 1, 0, 2)), fac3);

    __m128 add0 = _mm_add_ps(mul0, _mm_sub_ps(mul1, mul2));
    __m128 add1 = _mm_add_ps(add0, mul3);

    float result[4];
    _mm_store_ps(result, add1);

    return result[0] - result[1] + result[2] - result[3];

#else
    
    __m256i row0 = _mm256_loadu_si256((__m256i*)&matrix(0, 0));
    __m256i row1 = _mm256_loadu_si256((__m256i*)&matrix(1, 0));
    __m256i row2 = _mm256_loadu_si256((__m256i*)&matrix(2, 0));
    __m256i row3 = _mm256_loadu_si256((__m256i*)&matrix(3, 0));

    __m256i tmp1, tmp2, tmp3, tmp4, minor0, minor1, minor2, minor3;

    // Calcular los cofactors de la primera fila
    tmp1 = _mm256_shuffle_epi32(row2, _MM_SHUFFLE(0, 0, 3, 3));
    tmp2 = _mm256_shuffle_epi32(row3, _MM_SHUFFLE(1, 1, 2, 2));
    tmp3 = _mm256_shuffle_epi32(row2, _MM_SHUFFLE(1, 1, 2, 2));
    tmp4 = _mm256_shuffle_epi32(row3, _MM_SHUFFLE(0, 0, 3, 3));
    
    minor0 = _mm256_sub_epi32(_mm256_mullo_epi32(_mm256_shuffle_epi32(row1, _MM_SHUFFLE(0, 0, 0, 0)), tmp1),
                              _mm256_mullo_epi32(_mm256_shuffle_epi32(row1, _MM_SHUFFLE(3, 3, 3, 3)), tmp2));
    minor1 = _mm256_sub_epi32(_mm256_mullo_epi32(_mm256_shuffle_epi32(row1, _MM_SHUFFLE(1, 1, 1, 1)), tmp3),
                              _mm256_mullo_epi32(_mm256_shuffle_epi32(row1, _MM_SHUFFLE(2, 2, 2, 2)), tmp4));
    
    tmp1 = _mm256_shuffle_epi32(row2, _MM_SHUFFLE(2, 2, 1, 1));
    tmp2 = _mm256_shuffle_epi32(row3, _MM_SHUFFLE(3, 3, 0, 0));
    tmp3 = _mm256_shuffle_epi32(row2, _MM_SHUFFLE(3, 3, 0, 0));
    tmp4 = _mm256_shuffle_epi32(row3, _MM_SHUFFLE(2, 2, 1, 1));
    
    minor2 = _mm256_sub_epi32(_mm256_mullo_epi32(_mm256_shuffle_epi32(row1, _MM_SHUFFLE(2, 2, 2, 2)), tmp1),
                              _mm256_mullo_epi32(_mm256_shuffle_epi32(row1, _MM_SHUFFLE(1, 1, 1, 1)), tmp2));
    minor3 = _mm256_sub_epi32(_mm256_mullo_epi32(_mm256_shuffle_epi32(row1, _MM_SHUFFLE(3, 3, 3, 3)), tmp3),
                              _mm256_mullo_epi32(_mm256_shuffle_epi32(row1, _MM_SHUFFLE(0, 0, 0, 0)), tmp4));

    // Determinante
    __m256i mul0 = _mm256_mullo_epi32(row0, minor0);
    __m256i mul1 = _mm256_mullo_epi32(_mm256_shuffle_epi32(row0, _MM_SHUFFLE(2, 2, 1, 1)), minor1);
    __m256i mul2 = _mm256_mullo_epi32(_mm256_shuffle_epi32(row0, _MM_SHUFFLE(1, 1, 0, 0)), minor2);
    __m256i mul3 = _mm256_mullo_epi32(_mm256_shuffle_epi32(row0, _MM_SHUFFLE(3, 3, 2, 2)), minor3);

    __m256i add0 = _mm256_sub_epi32(mul0, mul1);
    __m256i add1 = _mm256_add_epi32(add0, mul2);
    __m256i add2 = _mm256_sub_epi32(add1, mul3);

    int result[8];
    _mm256_storeu_si256((__m256i*)result, add2);

    return result[0] - result[1] + result[2] - result[3];

#endif // TL_HAVE_SIMD_INTRINSICS

}




template<typename T, size_t _rows, size_t _col>
auto determinant4x4(const Matrix<T, _rows, _col> &matrix) -> enableIfDouble<T, T>
{

#if defined(TL_HAVE_SIMD_INTRINSICS) && defined(TL_HAVE_AVX)

    __m256d row0 = _mm256_load_pd(matrix[0][0]);
    __m256d row1 = _mm256_load_pd(matrix[1][0]);
    __m256d row2 = _mm256_load_pd(matrix[2][0]);
    __m256d row3 = _mm256_load_pd(matrix[3][0]);

    // Calcular cofactors de la primera fila
    __m256d fac0, fac1, fac2, fac3;
    {
        __m256d tmp0 = _mm256_shuffle_pd(row2, row3, 0x0);
        __m256d tmp1 = _mm256_shuffle_pd(row2, row3, 0x5);
        __m256d tmp2 = _mm256_shuffle_pd(row2, row3, 0xA);
        __m256d tmp3 = _mm256_shuffle_pd(row2, row3, 0xF);
        fac0 = _mm256_sub_pd(_mm256_mul_pd(_mm256_shuffle_pd(row1, row1, 0x0), tmp3),
                             _mm256_mul_pd(_mm256_shuffle_pd(row1, row1, 0x5), tmp2));
        fac1 = _mm256_sub_pd(_mm256_mul_pd(_mm256_shuffle_pd(row1, row1, 0x5), tmp1),
                             _mm256_mul_pd(_mm256_shuffle_pd(row1, row1, 0x0), tmp0));
        fac2 = _mm256_sub_pd(_mm256_mul_pd(_mm256_shuffle_pd(row1, row1, 0x0), tmp2),
                             _mm256_mul_pd(_mm256_shuffle_pd(row1, row1, 0x5), tmp3));
        fac3 = _mm256_sub_pd(_mm256_mul_pd(_mm256_shuffle_pd(row1, row1, 0x5), tmp0),
                             _mm256_mul_pd(_mm256_shuffle_pd(row1, row1, 0x0), tmp1));
    }

    // Determinante
    __m256d mul0 = _mm256_mul_pd(row0, fac0);
    __m256d mul1 = _mm256_mul_pd(_mm256_shuffle_pd(row0, row0, 0x5), fac1);
    __m256d mul2 = _mm256_mul_pd(_mm256_shuffle_pd(row0, row0, 0xA), fac2);
    __m256d mul3 = _mm256_mul_pd(_mm256_shuffle_pd(row0, row0, 0xF), fac3);

    __m256d add0 = _mm256_add_pd(mul0, _mm256_sub_pd(mul1, mul2));
    __m256d add1 = _mm256_add_pd(add0, mul3);

    double result[4];
    _mm256_store_pd(result, add1);

    return result[0] - result[1] + result[2] - result[3];

#elif defined(TL_HAVE_SIMD_INTRINSICS) && defined(TL_HAVE_SSE2)

    __m128d row0_lo = _mm_loadu_pd(&matrix[0][0]);
    __m128d row0_hi = _mm_loadu_pd(&matrix[0][2]);
    __m128d row1_lo = _mm_loadu_pd(&matrix[1][0]);
    __m128d row1_hi = _mm_loadu_pd(&matrix[1][2]);
    __m128d row2_lo = _mm_loadu_pd(&matrix[2][0]);
    __m128d row2_hi = _mm_loadu_pd(&matrix[2][2]);
    __m128d row3_lo = _mm_loadu_pd(&matrix[3][0]);
    __m128d row3_hi = _mm_loadu_pd(&matrix[3][2]);

    __m128d minor0, minor1, minor2, minor3;
    __m128d tmp1, tmp2, tmp3, tmp4;

    // Shuffle rows to align for subdeterminants
    tmp1 = _mm_shuffle_pd(row2_lo, row3_lo, _MM_SHUFFLE2(0, 0));
    tmp2 = _mm_shuffle_pd(row2_hi, row3_hi, _MM_SHUFFLE2(0, 0));
    tmp3 = _mm_shuffle_pd(row2_lo, row3_lo, _MM_SHUFFLE2(1, 1));
    tmp4 = _mm_shuffle_pd(row2_hi, row3_hi, _MM_SHUFFLE2(1, 1));

    // Compute cofactors of the first row
    minor0 = _mm_sub_pd(_mm_mul_pd(_mm_shuffle_pd(row1_lo, row1_lo, _MM_SHUFFLE2(1, 1)), tmp4),
                        _mm_mul_pd(_mm_shuffle_pd(row1_lo, row1_lo, _MM_SHUFFLE2(0, 0)), tmp2));
    minor1 = _mm_sub_pd(_mm_mul_pd(_mm_shuffle_pd(row1_hi, row1_hi, _MM_SHUFFLE2(1, 1)), tmp3),
                        _mm_mul_pd(_mm_shuffle_pd(row1_hi, row1_hi, _MM_SHUFFLE2(0, 0)), tmp1));
    minor2 = _mm_sub_pd(_mm_mul_pd(_mm_shuffle_pd(row1_lo, row1_lo, _MM_SHUFFLE2(1, 1)), tmp2),
                        _mm_mul_pd(_mm_shuffle_pd(row1_lo, row1_lo, _MM_SHUFFLE2(0, 0)), tmp4));
    minor3 = _mm_sub_pd(_mm_mul_pd(_mm_shuffle_pd(row1_hi, row1_hi, _MM_SHUFFLE2(1, 1)), tmp1),
                        _mm_mul_pd(_mm_shuffle_pd(row1_hi, row1_hi, _MM_SHUFFLE2(0, 0)), tmp3));

    // Combine results
    __m128d det0 = _mm_add_pd(_mm_shuffle_pd(minor0, minor1, _MM_SHUFFLE2(0, 0)),
                              _mm_shuffle_pd(minor0, minor1, _MM_SHUFFLE2(1, 1)));
    __m128d det1 = _mm_add_pd(_mm_shuffle_pd(minor2, minor3, _MM_SHUFFLE2(0, 0)),
                              _mm_shuffle_pd(minor2, minor3, _MM_SHUFFLE2(1, 1)));

    // Multiply first row by cofactors
    __m128d row0 = _mm_add_pd(row0_lo, row0_hi);
    __m128d det = _mm_mul_pd(row0, _mm_add_pd(det0, det1));

    // Sum the components
    double result[2];
    _mm_storeu_pd(result, det);

    return result[0] - result[1];

#else
    
    ///...

#endif

}


template<typename T, size_t _rows, size_t _col>
auto determinant4x4(const Matrix<T, _rows, _col> &matrix) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    int>
{

#if defined(TL_HAVE_SIMD_INTRINSICS) && defined(TL_HAVE_AVX)

#elif defined(TL_HAVE_SIMD_INTRINSICS) && defined(TL_HAVE_SSE2)
    __m128i row0 = _mm_loadu_si128((__m128i*)&matrix[0][0]);
    __m128i row1 = _mm_loadu_si128((__m128i*)&matrix[1][0]);
    __m128i row2 = _mm_loadu_si128((__m128i*)&matrix[2][0]);
    __m128i row3 = _mm_loadu_si128((__m128i*)&matrix[3][0]);

    __m128i tmp1, tmp2, tmp3, tmp4, minor0, minor1, minor2, minor3;

    // Calcular los cofactors de la primera fila
    tmp1 = _mm_shuffle_epi32(row2, _MM_SHUFFLE(0, 0, 3, 3));
    tmp2 = _mm_shuffle_epi32(row3, _MM_SHUFFLE(1, 1, 2, 2));
    tmp3 = _mm_shuffle_epi32(row2, _MM_SHUFFLE(1, 1, 2, 2));
    tmp4 = _mm_shuffle_epi32(row3, _MM_SHUFFLE(0, 0, 3, 3));
    
    minor0 = _mm_sub_epi32(_mm_mullo_epi32(_mm_shuffle_epi32(row1, _MM_SHUFFLE(0, 0, 0, 0)), tmp1),
                           _mm_mullo_epi32(_mm_shuffle_epi32(row1, _MM_SHUFFLE(3, 3, 3, 3)), tmp2));
    minor1 = _mm_sub_epi32(_mm_mullo_epi32(_mm_shuffle_epi32(row1, _MM_SHUFFLE(1, 1, 1, 1)), tmp3),
                           _mm_mullo_epi32(_mm_shuffle_epi32(row1, _MM_SHUFFLE(2, 2, 2, 2)), tmp4));
    
    tmp1 = _mm_shuffle_epi32(row2, _MM_SHUFFLE(2, 2, 1, 1));
    tmp2 = _mm_shuffle_epi32(row3, _MM_SHUFFLE(3, 3, 0, 0));
    tmp3 = _mm_shuffle_epi32(row2, _MM_SHUFFLE(3, 3, 0, 0));
    tmp4 = _mm_shuffle_epi32(row3, _MM_SHUFFLE(2, 2, 1, 1));
    
    minor2 = _mm_sub_epi32(_mm_mullo_epi32(_mm_shuffle_epi32(row1, _MM_SHUFFLE(2, 2, 2, 2)), tmp1),
                           _mm_mullo_epi32(_mm_shuffle_epi32(row1, _MM_SHUFFLE(1, 1, 1, 1)), tmp2));
    minor3 = _mm_sub_epi32(_mm_mullo_epi32(_mm_shuffle_epi32(row1, _MM_SHUFFLE(3, 3, 3, 3)), tmp3),
                           _mm_mullo_epi32(_mm_shuffle_epi32(row1, _MM_SHUFFLE(0, 0, 0, 0)), tmp4));

    // Determinante
    __m128i mul0 = _mm_mullo_epi32(row0, minor0);
    __m128i mul1 = _mm_mullo_epi32(_mm_shuffle_epi32(row0, _MM_SHUFFLE(2, 2, 1, 1)), minor1);
    __m128i mul2 = _mm_mullo_epi32(_mm_shuffle_epi32(row0, _MM_SHUFFLE(1, 1, 0, 0)), minor2);
    __m128i mul3 = _mm_mullo_epi32(_mm_shuffle_epi32(row0, _MM_SHUFFLE(3, 3, 2, 2)), minor3);

    __m128i add0 = _mm_sub_epi32(mul0, mul1);
    __m128i add1 = _mm_add_epi32(add0, mul2);
    __m128i add2 = _mm_sub_epi32(add1, mul3);

    int result[4];
    _mm_storeu_si128((__m128i*)result, add2);

    return result[0] - result[1] + result[2] - result[3];

#endif

}

#include "tidop/math/algebra/matrix/determinant.impl.h"

} // End namespace internal

} // End namespace tl