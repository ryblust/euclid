#pragma once

namespace euclid {

template<arithmetic Type, std::size_t Row, std::size_t Col>
class Matrix {
public:

	static Matrix identity() noexcept requires (Row == Col) {
		Matrix retMat;
		for (std::size_t i = 0; i < Col; ++i) {
			retMat[i][i] = 1;
		}
		return retMat;
	}

	static Matrix identity(arithmetic auto const val) noexcept requires (Row == Col) {
		Matrix retMat;
		for (std::size_t i = 0; i < Col; ++i) {
			retMat[i][i] = val;
		}
		return retMat;
	}

	Matrix<float, Row, Col> cast() const noexcept {
		Matrix<float, Row, Col> retMat;
		for (std::size_t i = 0; i < Col; ++i) {
			retMat[i] = mat[i].cast<T>();
		}
		return retMat;
	}

	Matrix<Type, Col, Row> transpose() const noexcept {
		Matrix<Type, Col, Row> retMat;
		for (std::size_t col = 0; col < Col; ++col) {
			for (std::size_t row = 0; row < Row; ++row) {
				retMat[row][col] = mat[col][row];
			}
		}
		return retMat;
	}

	Matrix& negative() noexcept {
		for (std::size_t i = 0; i < Col; ++i) {
			mat[i].negative();
		}
		return *this;
	}

	Matrix operator-() const noexcept {
		Matrix retMat = *this;
		return retMat.negative();
	}

	Matrix& operator+=(const Matrix& otherMat) noexcept {
		for (std::size_t i = 0; i < Col; ++i) {
			mat[i] += otherMat[i];
		}
		return *this;
	}

	Matrix& operator-=(const Matrix& otherMat) noexcept {
		for (std::size_t i = 0; i < Col; ++i) {
			mat[i] -= otherMat[i];
		}
		return *this;
	}

	Matrix operator+(const Matrix& otherMat) const noexcept {
		Matrix retMat = *this;
		return retMat += otherMat;
	}

	Matrix operator-(const Matrix& otherMat) const noexcept {
		Matrix retMat = *this;
		return retMat -= otherMat;
	}

	template<arithmetic Mul> requires acceptable_loss<Type, Mul>
	Matrix& operator*=(const Mul scalar) noexcept {
		for (std::size_t i = 0; i < Col; ++i) {
			mat[i] *= scalar;
		}
		return *this;
	}

	template<arithmetic Mul> requires acceptable_loss<Type, Mul>
	Matrix operator*(const Mul scalar) const noexcept {
		Matrix retMat = *this;
		return retMat *= scalar;
	}

	Vector<Type, Col> operator*(const Vector<Type, Col> vector) const noexcept {
		Vector<Type, Row> retVec;
		for (std::size_t i = 0; i < Col; ++i) {
			retVec += mat[i] * vector[i];
		}
		return retVec;
	}

	Point<Type, Col> operator*(const Point<Type, Col> point) const noexcept {
		Point<Type, Row> retPoint;
		for (std::size_t i = 0; i < Col; ++i) {
			const auto v = mat[i] * point[i];
			//retPoint[0] += v[0];
			//retPoint[1] += v[1];
			//retPoint[2] += v[2];
			//retPoint[3] += v[3];
		}
		return retPoint;
	}

	template<std::size_t C>
	Matrix<Type, Row, C> operator*(const Matrix<Type, Col, C>& otherMat) const noexcept {
		Matrix<Type, Row, C> retMat;
		for (std::size_t i = 0; i < C; ++i) {
			retMat[i] = (*this) * otherMat[i];
		}
		return retMat;
	}

	constexpr Vector<Type, Row> operator[](const std::size_t pos) const noexcept {
		return mat[pos];
	}

	constexpr Vector<Type, Row>& operator[](const std::size_t pos) noexcept {
		return mat[pos];
	}

	Vector<Type, Row> mat[Col];
};

template<arithmetic Mul, arithmetic T, std::size_t Row, std::size_t Col>
EUCLID_FORCEINLINE auto operator*(const Mul scalar, const Matrix<T, Row, Col>& mat) noexcept {
	return mat * scalar;
}

template<arithmetic T>
using mat2 = Matrix<T, 2, 2>;

template<arithmetic T>
using mat3 = Matrix<T, 3, 3>;

template<arithmetic T>
using mat4 = Matrix<T, 4, 4>;

using mat2i = Matrix<int, 2, 2>;
using mat3i = Matrix<int, 3, 3>;
using mat4i = Matrix<int, 4, 4>;

using mat2f = Matrix<float, 2, 2>;
using mat3f = Matrix<float, 3, 3>;
using mat4f = Matrix<float, 4, 4>;

}