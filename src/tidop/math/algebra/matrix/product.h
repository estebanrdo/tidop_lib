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

template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
class MatrixProduct
  : public MatrixOperations<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3> 
{

public:

    MatrixProduct(const Matrix<T, _rows1, _cols1> *A, 
                  const Matrix<T, _rows2, _cols2> *B,
				  const Matrix<T, _rows3, _cols3> *C);

protected:

    const Matrix<T, _rows1, _cols1> *A;
    const Matrix<T, _rows2, _cols2> *B;
    Matrix<T, _rows3, _cols3> *C;
};


template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
class MatrixProductSIMD
  : public MatrixProduct<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3> 
{

public:

    MatrixProductSIMD(const Matrix<T, _rows1, _cols1> *A, 
                      const Matrix<T, _rows2, _cols2> *B,
				      const Matrix<T, _rows3, _cols3> *C);
	  
	  
    void eval() const override;
};


template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
class MatrixProductBLAS
  : public MatrixProduct<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3> 
{

public:

    MatrixProductBLAS(const Matrix<T, _rows1, _cols1> *A, 
                      const Matrix<T, _rows2, _cols2> *B,
				      const Matrix<T, _rows3, _cols3> *C);
	  
	  
    void eval() const override;
};


template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
class MatrixProductCuBLAS
  : public MatrixProduct<T, _rows1, _cols1, _rows2, _cols2, _rows3, _cols3> 
{

public:

    MatrixProductCuBLAS(const Matrix<T, _rows1, _cols1> *A, 
                        const Matrix<T, _rows2, _cols2> *B,
				        const Matrix<T, _rows3, _cols3> *C);
	  
	  
    void eval() const override;
};


#include "tidop/math/algebra/matrix/product.impl.h"

} // End namespace tl