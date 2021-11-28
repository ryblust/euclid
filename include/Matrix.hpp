#pragma once
#ifndef _WEEKNDMATRIX_HPP
#define _WEEKNDMATRIX_HPP

#include "Point.hpp"
#include <initializer_list>

namespace euclid {

template<arithmetic Type, size_t Row, size_t Col>
class Matrix {
public:
	using value_type     = Type;
	using this_type		 = Matrix;
	using this_ref		 = Matrix&;
	using transposed_mat = Matrix<Type, Col, Row>;
	using vec_type		 = Vector<Type, Row>;
	using vec_ref		 = Vector<Type, Row>&;

	consteval Matrix() noexcept {}

	constexpr Matrix(std::initializer_list<value_type> list) noexcept {
		for (size_t row = 0; row < Row; ++row) {
			for (size_t col = 0; col < Col; ++col) {
				mat[col][row] = *(list.begin() + row * Col + col);
			}
		}
	}

	static constexpr this_type identity() noexcept requires (Row == Col) {
		Matrix retMat{};
		for (size_t i = 0; i < Col; ++i) {
			retMat[i][i] = 1;
		}
		return retMat;
	}

	template<arithmetic T>
	constexpr Matrix<T, Row, Col> cast() const noexcept {
		Matrix<T, Row, Col> retMat;
		for (size_t i = 0; i < Col; ++i) {
			retMat[i] = mat[i].cast<T>();
		}
		return retMat;
	}

	constexpr transposed_mat transpose() const noexcept {
		transposed_mat retMat;
		for (size_t col = 0; col < Col; ++col) {
			for (size_t row = 0; row < Row; ++row) {
				retMat[row][col] = mat[col][row];
			}
		}
		return retMat;
	}

	constexpr void print() const noexcept {
		#ifdef _IOSTREAM_
			for (size_t row = 0; row < Row; ++row) {
				for (size_t col = 0; col < Col; ++col) {
					if (col != Col - 1) [[likely]] {
						std::cout << mat[col][row] << ", ";
					} else {
						std::cout << mat[col][row] << '\n';
					}
				}
			}
		#endif // _IOSTREAM_
	}

	constexpr this_ref negative() noexcept {
		for (size_t i = 0; i < Col; ++i) {
			mat[i].negative();
		}
		return *this;
	}

	constexpr this_type operator-() const noexcept {
		this_type retMat = *this;
		return retMat.negative();
	}

	template<arithmetic T> requires acceptable_loss<Type, T>
	constexpr this_ref operator+=(const Matrix<T, Row, Col>& otherMat) noexcept {
		for (size_t i = 0; i < Col; ++i) {
			mat[i] += otherMat[i];
		}
		return *this;
	}

	template<arithmetic T> requires acceptable_loss<Type, T>
	constexpr this_ref operator-=(const Matrix<T, Row, Col>& otherMat) noexcept {
		for (size_t i = 0; i < Col; ++i) {
			mat[i] -= otherMat[i];
		}
		return *this;
	}

	template<arithmetic T>
	constexpr auto operator+(const Matrix<T, Row, Col>& otherMat) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Matrix<type, Row, Col> retMat;
		for (size_t i = 0; i < Col; ++i) {
			retMat[i] = mat[i] + otherMat[i];
		}
		return retMat;
	}

	template<arithmetic T>
	constexpr auto operator-(const Matrix<T, Row, Col>& otherMat) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Matrix<type, Row, Col> retMat;
		for (size_t i = 0; i < Col; ++i) {
			retMat[i] = mat[i] - otherMat[i];
		}
		return retMat;
	}

	template<arithmetic T> requires acceptable_loss<Type, T>
	constexpr this_ref operator*= (const Scalar<T> scalar) noexcept {
		for (size_t i = 0; i < Col; ++i) {
			mat[i] *= scalar;
		}
		return *this;
	}

	template<arithmetic T> requires acceptable_loss<Type, T>
	constexpr this_ref operator*=(const T scalar) noexcept {
		return (*this) *= Scalar(scalar);
	}

	template<arithmetic T>
	constexpr auto operator*(const Scalar<T> scalar) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Matrix<type, Row, Col> retMat;
		for (size_t i = 0; i < Col; ++i) {
			retMat[i] = mat[i] * scalar;
		}
		return retMat;
	}

	template<arithmetic T>
	constexpr auto operator*(const T scalar) const noexcept {
		return (*this) * Scalar(scalar);
	}

	template<arithmetic T>
	constexpr auto operator*(const Vector<T, Col>& vec) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Vector<type, Col> retVec;
		for (size_t i = 0; i < Col; ++i) {
			retVec += mat[i] * vec[i];
		}
		return retVec;
	}

	template<arithmetic T>
	constexpr auto operator*(const Point<T, Col>& point) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Point<type, Col> retPoint;
		// to-do
		return retPoint;
	}

	template<arithmetic T, size_t C>
	constexpr auto operator*(const Matrix<T, Col, C>& otherMat) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Matrix<type, Row, C> retMat;
		for (size_t i = 0; i < C; ++i) {
			retMat[i] = (*this) * otherMat[i];
		}
		return retMat;
	}

	constexpr vec_ref operator[](const size_t pos) noexcept {
		return mat[pos];
	}

	constexpr vec_type operator[](const size_t pos) const noexcept {
		return mat[pos];
	}
public:
	Vector<Type, Row> mat[Col];
};

template<arithmetic Ty1, arithmetic Ty2, size_t Row, size_t Col> inline
constexpr auto operator*(const Scalar<Ty1> scalar, const Matrix<Ty2, Row, Col>& mat) noexcept {
	return mat * scalar;
}

template<arithmetic Ty1, arithmetic Ty2, size_t Row, size_t Col> inline
constexpr auto operator*(const Ty1 scalar, const Matrix<Ty2, Row, Col>& mat) noexcept {
	return mat * scalar;
}

#pragma region TypeDefineForMatrix
// for 2d Matrix
template<arithmetic T>
using mat2 = Matrix<T, 2, 2>;
// for 3d Matrix
template<arithmetic T>
using mat3 = Matrix<T, 3, 3>;
// for 4d Matrix
template<arithmetic T>
using mat4 = Matrix<T, 4, 4>;

// for integer Matrix
using mat2i = Matrix<int, 2, 2>;
using mat3i = Matrix<int, 3, 3>;
using mat4i = Matrix<int, 4, 4>;
// for unsigned Matrix
using mat2u = Matrix<unsigned, 2, 2>;
using mat3u = Matrix<unsigned, 3, 3>;
using mat4u = Matrix<unsigned, 4, 4>;
// for float Matrix
using mat2f = Matrix<float, 2, 2>;
using mat3f = Matrix<float, 3, 3>;
using mat4f = Matrix<float, 4, 4>;
// for double Matrix
using mat2d = Matrix<double, 2, 2>;
using mat3d = Matrix<double, 3, 3>;
using mat4d = Matrix<double, 4, 4>;
#pragma endregion

}

#endif // _WEEKNDMATRIX_HPP