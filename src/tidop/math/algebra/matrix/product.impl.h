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

template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
inline MatrixProduct<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3>::MatrixProduct(const Matrix<T, _rows1, _cols1> *A, const Matrix<T, _rows2, _cols2> *B, const Matrix<T, _rows3, _cols3> *C)
  : A(A),
    B(B),
    C(C)
{
}

template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
inline MatrixProductSIMD<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3>::MatrixProductSIMD(const Matrix<T, _rows1, _cols1> *A, const Matrix<T, _rows2, _cols2> *B, const Matrix<T, _rows3, _cols3> *C)
    : MatrixProduct(A, B, C)
{
}

template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
inline void MatrixProductSIMD<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3>::eval() const
{
    TL_ASSERT(A->cols() == B->rows(), "A columns != B rows");
    TL_ASSERT(A->rows() == C->rows(), "C rows != A rows");
    TL_ASSERT(B->cols() == C->cols(), "B columns != C columns");

    size_t rows = A->rows();
    size_t dim = A->cols();
    size_t cols = B->cols();

    Packed<T> packed_a;
    Packed<T> packed_b;
    Packed<T> packed_c;
    Packed<T> packed_a1;
    Packed<T> packed_a2;
    Packed<T> packed_a3;
    Packed<T> packed_a4;
    Packed<T> packed_a5;
    Packed<T> packed_a6;
    Packed<T> packed_a7;
    Packed<T> packed_a8;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = cols - cols % packed_size;
    size_t iter = rows - rows % 8;

    T b{};

    for (size_t r = 0; r < iter; r += 8) {
        for (size_t i = 0; i < dim; i++) {

            packed_a1.setScalar(matrix1(r, i));
            packed_a2.setScalar(matrix1(r + 1, i));
            packed_a3.setScalar(matrix1(r + 2, i));
            packed_a4.setScalar(matrix1(r + 3, i));
            packed_a5.setScalar(matrix1(r + 4, i));
            packed_a6.setScalar(matrix1(r + 5, i));
            packed_a7.setScalar(matrix1(r + 6, i));
            packed_a8.setScalar(matrix1(r + 7, i));

            for (size_t c = 0; c < max_vector; c += packed_size) {

                packed_b.loadUnaligned(&matrix2(i, c));

                packed_c.loadUnaligned(&matrix(r, c));
                packed_c += packed_a1 * packed_b;
                packed_c.storeUnaligned(&matrix(r, c));

                packed_c.loadUnaligned(&matrix(r + 1, c));
                packed_c += packed_a2 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 1, c));

                packed_c.loadUnaligned(&matrix(r + 2, c));
                packed_c += packed_a3 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 2, c));

                packed_c.loadUnaligned(&matrix(r + 3, c));
                packed_c += packed_a4 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 3, c));

                packed_c.loadUnaligned(&matrix(r + 4, c));
                packed_c += packed_a5 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 4, c));

                packed_c.loadUnaligned(&matrix(r + 5, c));
                packed_c += packed_a6 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 5, c));

                packed_c.loadUnaligned(&matrix(r + 6, c));
                packed_c += packed_a7 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 6, c));

                packed_c.loadUnaligned(&matrix(r + 7, c));
                packed_c += packed_a8 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 7, c));

            }

            for (size_t c = max_vector; c < cols; c++) {

                b = matrix2(i, c);
                matrix(r, c) += matrix1(r, i) * b;
                matrix(r + 1, c) += matrix1(r + 1, i) * b;
                matrix(r + 2, c) += matrix1(r + 2, i) * b;
                matrix(r + 3, c) += matrix1(r + 3, i) * b;
                matrix(r + 4, c) += matrix1(r + 4, i) * b;
                matrix(r + 5, c) += matrix1(r + 5, i) * b;
                matrix(r + 6, c) += matrix1(r + 6, i) * b;
                matrix(r + 7, c) += matrix1(r + 7, i) * b;
            }

        }
    }

    for (size_t r = iter; r < rows; r++) {
        for (size_t i = 0; i < dim; i++) {

            T a = matrix1(r, i);
            packed_a.setScalar(a);

            for (size_t c = 0; c < max_vector; c += packed_size) {

                packed_b.loadUnaligned(&matrix2(i, c));

                packed_c.loadUnaligned(&matrix(r, c));
                packed_c += packed_a * packed_b;
                packed_c.storeUnaligned(&matrix(r, c));
            }

            for (size_t c = max_vector; c < cols; c++) {
                matrix(r, c) += a * matrix2(i, c);
            }

        }
    }
}


template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
inline MatrixProductBLAS<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3>::MatrixProductBLAS(const Matrix<T, _rows1, _cols1> *A, const Matrix<T, _rows2, _cols2> *B, const Matrix<T, _rows3, _cols3> *C)
    : MatrixProduct(A, B, C)
{
}

template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
inline void MatrixProductBLAS<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3>::eval() const
{
    blas::gemm(static_cast<int>(A->rows()),
               static_cast<int>(A->cols()),
               static_cast<int>(B->cols()),
               A->data(), B->data(), C->data());
}


template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
inline MatrixProductCuBLAS<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3>::MatrixProductCuBLAS(const Matrix<T, _rows1, _cols1> *A, const Matrix<T, _rows2, _cols2> *B, const Matrix<T, _rows3, _cols3> *C)
    : MatrixProduct(A, B, C)
{
}

template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
inline void MatrixProductCuBLAS<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3>::eval() const
{
    ;
}