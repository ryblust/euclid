#pragma once

namespace euclid {

template<arithmetic Type, std::size_t Row, std::size_t Col>
class Matrix {
public:
	consteval Matrix() noexcept {}

	constexpr Matrix(const std::initializer_list<Type> list) noexcept {
		const auto begin = list.begin();
		for (std::size_t row = 0; row < Row; ++row) {
			for (std::size_t col = 0; col < Col; ++col) {
				mat[col][row] = *(begin + row * Col + col);
			}
		}
	}

	static constexpr Matrix identity() noexcept requires (Row == Col) {
		Matrix retMat;
		for (std::size_t i = 0; i < Col; ++i) {
			retMat[i][i] = 1;
		}
		return retMat;
	}

	static constexpr Matrix identity(arithmetic auto const val) noexcept requires (Row == Col) {
		Matrix retMat;
		for (std::size_t i = 0; i < Col; ++i) {
			retMat[i][i] = val;
		}
		return retMat;
	}

	template<arithmetic T>
	constexpr Matrix<T, Row, Col> cast() const noexcept {
		Matrix<T, Row, Col> retMat;
		for (std::size_t i = 0; i < Col; ++i) {
			retMat[i] = mat[i].cast<T>();
		}
		return retMat;
	}

	constexpr Matrix<Type, Col, Row> transpose() const noexcept {
		Matrix<Type, Col, Row> retMat;
		for (std::size_t col = 0; col < Col; ++col) {
			for (std::size_t row = 0; row < Row; ++row) {
				retMat[row][col] = mat[col][row];
			}
		}
		return retMat;
	}
	
#ifdef _has_Euclid_io
	constexpr void print() const noexcept {
		for (size_t row = 0; row < Row; ++row) {
			for (size_t col = 0; col < Col; ++col) {
				if (col != Col - 1) [[likely]] {
					std::printf(io::detail::format_traits<Type>::start, mat[col][row]);
				} else [[unlikely]] {
					std::printf(io::detail::format_traits<Type>::end, mat[col][row]);
				}
			}
			std::printf("\n");
		}
	}
#endif // _has_Euclid_io

	constexpr Matrix& negative() noexcept {
		for (std::size_t i = 0; i < Col; ++i) {
			mat[i].negative();
		}
		return *this;
	}

	constexpr Matrix operator-() const noexcept {
		Matrix retMat = *this;
		return retMat.negative();
	}

	template<arithmetic T> requires acceptable_loss<Type, T>
	constexpr Matrix& operator+=(const Matrix<T, Row, Col>& otherMat) noexcept {
		for (std::size_t i = 0; i < Col; ++i) {
			mat[i] += otherMat[i];
		}
		return *this;
	}

	template<arithmetic T> requires acceptable_loss<Type, T>
	constexpr Matrix& operator-=(const Matrix<T, Row, Col>& otherMat) noexcept {
		for (std::size_t i = 0; i < Col; ++i) {
			mat[i] -= otherMat[i];
		}
		return *this;
	}

	template<arithmetic T>
	constexpr auto operator+(const Matrix<T, Row, Col>& otherMat) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Matrix<type, Row, Col> retMat;
		for (std::size_t i = 0; i < Col; ++i) {
			retMat[i] = mat[i] + otherMat[i];
		}
		return retMat;
	}

	template<arithmetic T>
	constexpr auto operator-(const Matrix<T, Row, Col>& otherMat) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Matrix<type, Row, Col> retMat;
		for (std::size_t i = 0; i < Col; ++i) {
			retMat[i] = mat[i] - otherMat[i];
		}
		return retMat;
	}

	template<arithmetic T> requires acceptable_loss<Type, T>
	constexpr Matrix& operator*=(const T scalar) noexcept {
		for (std::size_t i = 0; i < Col; ++i) {
			mat[i] *= scalar;
		}
		return *this;
	}

	template<arithmetic T>
	constexpr auto operator*(const T scalar) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Matrix<type, Row, Col> retMat;
		for (std::size_t i = 0; i < Col; ++i) {
			retMat[i] = mat[i] * scalar;
		}
		return retMat;
	}

	template<arithmetic T>
	constexpr auto operator*(const Vector<T, Col>& vector) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Vector<type, Row> retVec;
		for (std::size_t i = 0; i < Col; ++i) {
			retVec += mat[i] * vector[i];
		}
		return retVec;
	}

	template<arithmetic T>
	constexpr auto operator*(const Point<T, Col>& point) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Point<type, Row> retPoint;
		for (std::size_t i = 0; i < Col; ++i) {
			const auto v = mat[i] * point[i];
			retPoint[0] += v[0];
			retPoint[1] += v[1];
			retPoint[2] += v[2];
			retPoint[3] += v[3];
		}
		return retPoint;
	}

	template<arithmetic T, std::size_t C>
	constexpr auto operator*(const Matrix<T, Col, C>& otherMat) const noexcept {
		using type = arithmetic_promotion_t<Type, T>;
		Matrix<type, Row, C> retMat;
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

template<arithmetic Ty1, arithmetic Ty2, std::size_t Row, std::size_t Col> inline
constexpr auto operator*(const Ty1 scalar, const Matrix<Ty2, Row, Col>& mat) noexcept {
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

using mat2u = Matrix<unsigned, 2, 2>;
using mat3u = Matrix<unsigned, 3, 3>;
using mat4u = Matrix<unsigned, 4, 4>;

using mat2f = Matrix<float, 2, 2>;
using mat3f = Matrix<float, 3, 3>;
using mat4f = Matrix<float, 4, 4>;

using mat2d = Matrix<double, 2, 2>;
using mat3d = Matrix<double, 3, 3>;
using mat4d = Matrix<double, 4, 4>;

}