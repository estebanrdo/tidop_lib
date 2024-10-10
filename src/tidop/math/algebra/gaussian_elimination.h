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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 

template<typename T>
class GaussianElimination;

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
class LuDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

protected:

  Matrix<T, _rows, _cols> A;
  
};

//template<typename T, size_t _rows, size_t _cols> inline
//Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::gaussianElimination(T *determinant) const
//{
//  T d{1};
//  size_t rows = this->rows();
//  size_t cols = this->cols();
//
//  Matrix<T, _rows, _cols> matrix(*this);
//
//  for (size_t i = 0; i < rows; ++i) {
//
//    size_t pivot_row = findMax(matrix, i);
//    T pivotElement = matrix.at(pivot_row, i);
//
//    if (pivotElement == static_cast<T>(0)) {
//      d = static_cast<T>(0);
//      // Matriz singular
//      break;
//    }
//
//    if (pivot_row != i) {
//      matrix.swapRows(i, pivot_row);
//      d *= -static_cast<T>(1);
//    }
//
//    d *= pivotElement;
//
//    for (size_t r = i + 1; r < rows; ++r) {
//      T scale_row_coeff = matrix.at(r, i) / matrix.at(i, i);
//      matrix.at(r, i) = 0;
//      for (size_t c = i + 1; c < cols; ++c) {
//        matrix.at(r, c) -= scale_row_coeff * matrix.at(i, c);
//      }
//    }
//  }
//
//  if (determinant) {
//    *determinant = d;
//  }
//
//  return matrix;
//}
//
//template<typename T, size_t _rows, size_t _cols>
//inline size_t Matrix<T, _rows, _cols>::findMax(const Matrix<T, _rows, _cols>& matrix, size_t index) const
//{
//  T pivotElement = matrix.at(index, index);
//  size_t pivotRow = index;
//  for (size_t r = index + 1; r < this->rows(); ++r) {
//    if (std::abs(matrix.at(r, index)) > std::abs(pivotElement)) {
//      pivotElement = matrix.at(r, index);
//      pivotRow = r;
//    }
//  }
//  return pivotRow;
//}