#ifndef LINEAR_ALGEBRA_H_
#define LINEAR_ALGEBRA_H_
#include <cmath>
#include <type_traits>
#include <algorithm>
#include <utility>

template <class T, int r, int c>
class Matrix {
public:
	Matrix();
	Matrix(const Matrix<T,r,c>& m);
	~Matrix();
	const T& operator[] (int i) const;
	T& operator() (int x, int y);
	T& operator() (int i);
	template <class E>
	Matrix<T,r,c>& operator+=(Matrix<E,r,c>& mat) {
		for (int i = 0; i < r*c; i++) {
			data[i] += mat(i);
		}
		return *this;
	}
	template <class E>
	Matrix<T,r,c>& operator+=(Matrix<E,r,c>&& mat) {
		for (int i = 0; i < r*c; i++) {
			data[i] += mat(i);
		}
		return *this;
	}
	template <class E>
	Matrix<T,r,c>& operator-=(Matrix<E,r,c>& mat) {
		for (int i = 0; i < r*c; i++) {
			data[i] -= mat(i);
		}
		return *this;
	}
	template <class E>
	Matrix<T,r,c>& operator-=(Matrix<E,r,c>&& mat) {
		for (int i = 0; i < r*c; i++) {
			data[i] -= mat(i);
		}
		return *this;
	}
	template <class E>
	Matrix<T,r,c>& operator*=(Matrix<E,c,c>& mat) {
		T data2[r*c];
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				T sum = T();
				for (int k = 0; k < c; k++) {
					sum += (*this)(i,k)*mat(k,j);
				}
				data2[i*c+j] = sum;
			}
		}
		for (int i = 0; i < r*c; i++) {
			data[i] = data2[i];
		}
		return *this;
	}
	template <class E>
	Matrix<T,r,c>& operator*=(Matrix<E,c,c>&& mat) {
		T data2[r*c];
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				T sum = T();
				for (int k = 0; k < c; k++) {
					sum += (*this)(i,k)*mat(k,j);
				}
				data2[i*c+j] = sum;
			}
		}
		for (int i = 0; i < r*c; i++) {
			data[i] = data2[i];
		}
		return *this;
	}
	template<class E>
	Matrix<T,r,c>& operator*=(E&& val) { 
		for (int i = 0; i < r*c; i++) { 
			data[i] *= val; 
		} 
		return *this;
	}
	template<class E>
	Matrix<T,r,c>& operator/=(E&& val) {
		for (int i = 0; i < r*c; i++) {
			data[i] /= val;
		}
		return *this;
	}
private:
	T data[r*c];

};


template <class T, int r, int c>
const T& Matrix<T,r,c>::operator[](int i) const {
	return data[i];
}


template <class T, int r, int c>
class MatInitList {
public:
	MatInitList(Matrix<T,r,c>& m);
	~MatInitList();
	template <class E>
	MatInitList<T,r,c>& operator,(E i) {
		(*mat)(pos) = i;
		pos++;
		return *this;
	}
private:
	Matrix<T,r,c> *mat;
	int pos;
};

template <class T, int r, int c>
MatInitList<T,r,c>::MatInitList(Matrix<T,r,c>& m) : mat(&m), pos(1) {
}

template<class T, int r, int c>
MatInitList<T,r,c>::~MatInitList() { }


template <class T, int r, int c>
Matrix<T,r,c>::Matrix() {
	for (int i = 0; i < r*c; i++) {
		data[i] = T();
	}
}

template<class T, int r, int c>
Matrix<T,r,c>::Matrix(const Matrix<T,r,c>& m) {
	std::copy(m.data, m.data+(r*c), data);
}

template <class T, int r, int c>
Matrix<T,r,c>::~Matrix() { }

template <class T, int r, int c>
T& Matrix<T,r,c>::operator() (int x, int y) {
	return data[x*c+y];
}

template <class T, int r, int c>
T& Matrix<T,r,c>::operator() (int i) {
	return data[i];
}

template <class T, int r, int c, class E>
MatInitList<T,r,c> operator<<(Matrix<T,r,c>& mat, E i) {
	mat(0) = i;
	MatInitList<T, r, c> initList(mat);
	return initList;
}

template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator+(Matrix<T, r, c>& mat1, Matrix<E,r,c>& mat2) {
	Matrix<typename std::common_type_t<T, E>, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)+mat2(i,j);
		}
	}
	return mat;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator+(Matrix<T, r, c>& mat1, Matrix<E,r,c>&& mat2) {
	Matrix<typename std::common_type_t<T, E>, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)+mat2(i,j);
		}
	}
	return mat;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator+(Matrix<T, r, c>&& mat1, Matrix<E,r,c>& mat2) {
	Matrix<typename std::common_type_t<T, E>, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)+mat2(i,j);
		}
	}
	return mat;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator+(Matrix<T, r, c>&& mat1, Matrix<E,r,c>&& mat2) {
	Matrix<typename std::common_type_t<T, E>, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)+mat2(i,j);
		}
	}
	return mat;
}



template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator-(Matrix<T, r, c>& mat1, Matrix<E, r, c>& mat2) {
	Matrix<typename std::common_type_t<T, E>, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)-mat2(i,j);
		}
	}
	return mat;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator-(Matrix<T, r, c>& mat1, Matrix<E, r, c>&& mat2) {
	Matrix<typename std::common_type_t<T, E>, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)-mat2(i,j);
		}
	}
	return mat;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator-(Matrix<T, r, c>&& mat1, Matrix<E, r, c>& mat2) {
	Matrix<typename std::common_type_t<T, E>, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)-mat2(i,j);
		}
	}
	return mat;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator-(Matrix<T, r, c>&& mat1, Matrix<E, r, c>&& mat2) {
	Matrix<typename std::common_type_t<T, E>, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)-mat2(i,j);
		}
	}
	return mat;
}



template <class T, int i, int j, int k, class E>
Matrix<typename std::common_type_t<T, E>, i, k> operator*(Matrix<T, i, j>& mat1, Matrix<E, j, k>& mat2) {
	Matrix<typename std::common_type_t<T, E>, i, k> mat;
	T sum = T();
	for (int r1 = 0; r1 < i; r1++) {
		for (int c1 = 0; c1 < k; c1++) {
			sum = T();
			for (int l = 0; l < j; l++) {
					sum += mat1(r1,l)*mat2(l,c1);
			}
			mat(r1,c1) = sum;
		}
	}
	return mat;
}
template <class T, int i, int j, int k, class E>
Matrix<typename std::common_type_t<T, E>, i, k> operator*(Matrix<T, i, j>& mat1, Matrix<E, j, k>&& mat2) {
	Matrix<typename std::common_type_t<T, E>, i, k> mat;
	T sum = T();
	for (int r1 = 0; r1 < i; r1++) {
		for (int c1 = 0; c1 < k; c1++) {
			sum = T();
			for (int l = 0; l < j; l++) {
					sum += mat1(r1,l)*mat2(l,c1);
			}
			mat(r1,c1) = sum;
		}
	}
	return mat;
}
template <class T, int i, int j, int k, class E>
Matrix<typename std::common_type_t<T, E>, i, k> operator*(Matrix<T, i, j>&& mat1, Matrix<E, j, k>& mat2) {
	Matrix<typename std::common_type_t<T, E>, i, k> mat;
	T sum = T();
	for (int r1 = 0; r1 < i; r1++) {
		for (int c1 = 0; c1 < k; c1++) {
			sum = T();
			for (int l = 0; l < j; l++) {
					sum += mat1(r1,l)*mat2(l,c1);
			}
			mat(r1,c1) = sum;
		}
	}
	return mat;
}
template <class T, int i, int j, int k, class E>
Matrix<typename std::common_type_t<T, E>, i, k> operator*(Matrix<T, i, j>&& mat1, Matrix<E, j, k>&& mat2) {
	Matrix<typename std::common_type_t<T, E>, i, k> mat;
	T sum = T();
	for (int r1 = 0; r1 < i; r1++) {
		for (int c1 = 0; c1 < k; c1++) {
			sum = T();
			for (int l = 0; l < j; l++) {
					sum += mat1(r1,l)*mat2(l,c1);
			}
			mat(r1,c1) = sum;
		}
	}
	return mat;
}

template <class T, class E, int i>
typename std::common_type_t<T, E> operator*(Matrix<E, 1, i>& mat1, Matrix<T, i, 1>& mat2) {
	typename std::common_type_t<E, T> sum = typename std::common_type_t<E, T>();
	for (int j = 0; j < i; j++) {
		sum += mat1(j)*mat2(j);
	}
	return sum;
}
template <class T, class E, int i>
typename std::common_type_t<T, E> operator*(Matrix<E, 1, i>& mat1, Matrix<T, i, 1>&& mat2) {
	typename std::common_type_t<E, T> sum = typename std::common_type_t<E, T>();
	for (int j = 0; j < i; j++) {
		sum += mat1(j)*mat2(j);
	}
	return sum;
}
template <class T, class E, int i>
typename std::common_type_t<T, E> operator*(Matrix<E, 1, i>&& mat1, Matrix<T, i, 1>& mat2) {
	typename std::common_type_t<E, T> sum = typename std::common_type_t<E, T>();
	for (int j = 0; j < i; j++) {
		sum += mat1(j)*mat2(j);
	}
	return sum;
}
template <class T, class E, int i>
typename std::common_type_t<T, E> operator*(Matrix<E, 1, i>&& mat1, Matrix<T, i, 1>&& mat2) {
	typename std::common_type_t<E, T> sum = typename std::common_type_t<E, T>();
	for (int j = 0; j < i; j++) {
		sum += mat1(j)*mat2(j);
	}
	return sum;
}




template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator*(Matrix<T, r, c>& mat, E&& scal) {
	Matrix<typename std::common_type_t<T, E>,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = scal*mat(i);
	}
	return mat2;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator*(Matrix<T, r, c>&& mat, E&& scal) {
	Matrix<typename std::common_type_t<T, E>,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = scal*mat(i);
	}
	return mat2;
}

template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator*(E&& scal, Matrix<T, r, c>& mat) {
	Matrix<std::common_type_t<T, E>,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = scal*mat(i);
	}
	return mat2;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator*(E&& scal, Matrix<T, r, c>&& mat) {
	Matrix<std::common_type_t<T, E>,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = scal*mat(i);
	}
	return mat2;
}

template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator/(Matrix<T, r, c>& mat, E&& scal) {
	Matrix<typename std::common_type_t<T, E>,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = mat(i)/scal;
	}
	return mat2;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator/(Matrix<T, r, c>&& mat, E&& scal) {
	Matrix<typename std::common_type_t<T, E>,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = mat(i)/scal;
	}
	return mat2;
}

template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator/(E&& scal, Matrix<T, r, c>& mat) {
	Matrix<typename std::common_type_t<T, E>,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = mat(i)/scal;
	}
	return mat2;
}
template <class T, int r, int c, class E>
Matrix<typename std::common_type_t<T, E>, r, c> operator/(E&& scal, Matrix<T, r, c>&& mat) {
	Matrix<typename std::common_type_t<T, E>,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = mat(i)/scal;
	}
	return mat2;
}


template <class T, int r>
using colVector = Matrix<T, r, 1>;

template <class T, int c>
using rowVector = Matrix<T, 1, c>;

template <class T, class E, int i>
typename std::common_type_t<T,E> operator*(const colVector<T, i> &vec1, const colVector<E, i>& vec2) {
	typename std::common_type_t<T,E> result = typename std::common_type_t<T,E>();
	for (int j = 0; j < i; j++) {
		result += vec1[j]*vec2[j];
	}
	return result;
}

template <class T, class E, int i>
typename std::common_type_t<T,E> operator*(const rowVector<T, i> &vec1, const rowVector<E, i>& vec2) {
	typename std::common_type_t<T,E> result = typename std::common_type_t<T,E>();
	for (int j = 0; j < i; j++) {
		result += vec1[j]*vec2[j];
	}
	return result;
}

template<class T, int n>
Matrix<T, n, n> Identity() {
	Matrix<T, n, n> mat;
	for (int i = 0; i < n; i++) {
		mat(i,i) = 1;
	}
	return mat;
}

template <class T, int r, int c>
Matrix<T,c,r> trans(Matrix<T,r,c>& mat) {
	Matrix<T,c,r> mat2;
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < r; j++) {
			mat2(i,j) = mat(j,i);
		}
	}
	return mat2;
}
template <class T, int r, int c>
Matrix<T,c,r> trans(Matrix<T,r,c>&& mat) {
	Matrix<T,c,r> mat2;
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < r; j++) {
			mat2(i,j) = mat(j,i);
		}
	}
	return mat2;
}



template <class T, class E>
colVector<typename std::common_type_t<T, E>, 3> operator^(colVector<T, 3>& vec1, colVector<E, 3>& vec2) {
	colVector<typename std::common_type_t<T,E> ,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}


template <class T, class E>
colVector<typename std::common_type_t<T, E>, 3> operator^(colVector<T,3>& vec1, colVector<E,3>&& vec2) {
	colVector<typename std::common_type_t<T,E> ,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}
template <class T, class E>
colVector<typename std::common_type_t<T, E>, 3> operator^(colVector<T,3>&& vec1, colVector<E,3>& vec2) {
	colVector<typename std::common_type_t<T,E> ,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}
template <class T, class E>
colVector<typename std::common_type_t<T, E>, 3> operator^(colVector<T,3>&& vec1, colVector<E,3>&& vec2) {
	colVector<typename std::common_type_t<T,E> ,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}


template <class E, class F>
rowVector<typename std::common_type_t<F, E> ,3> operator^(rowVector<E,3>& vec1, rowVector<F,3>& vec2) {
	rowVector<typename std::common_type_t<F, E>,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}
template <class E, class F>
rowVector<typename std::common_type_t<F, E> ,3> operator^(rowVector<E,3>& vec1, rowVector<F,3>&& vec2) {
	rowVector<typename std::common_type_t<F, E>,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}
template <class E, class F>
rowVector<typename std::common_type_t<F, E> ,3> operator^(rowVector<E,3>&& vec1, rowVector<F,3>& vec2) {
	rowVector<typename std::common_type_t<F, E>,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}
template <class E, class F>
rowVector<typename std::common_type_t<F, E> ,3> operator^(rowVector<E,3>&& vec1, rowVector<F,3>&& vec2) {
	rowVector<typename std::common_type_t<F, E>,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}

template <class E, class F, class G>
Matrix<float, 4, 4> translate(E&& x, F&& y, G&& z) {
	auto mat = Identity<float, 4>();
	mat(0, 3) = x;
	mat(1, 3) = y;
	mat(2, 3) = z;
	return mat;
}

template <class E>
Matrix<float, 4, 4> rotationX(E&& angle) {
	auto mat = Identity<float, 4>();
	mat(1,1) = cos(angle);
	mat(1,2) = -1*sin(angle);
	mat(2,1) = sin(angle);
	mat(2,2) = cos(angle);
	return mat;
}

template <class E>
Matrix<float, 4, 4> rotationY(E&& angle) {
	auto mat = Identity<float, 4>();
	mat(0,0) = cos(angle);
	mat(0,2) = sin(angle);
	mat(2,0) = -1*sin(angle);
	mat(2,2) = cos(angle);
	return mat;
}

template <class E>
Matrix<float, 4, 4> rotationZ(E&& angle) {
	auto mat = Identity<float, 4>();
	mat(0,0) = cos(angle);
	mat(0,1) = -1*sin(angle);
	mat(1,0) = sin(angle);
	mat(1,1) = cos(angle);
	return mat;
}

template <class E, class F, class G>
Matrix<float, 4, 4> scale(E&& x, F&& y, G&& z) {
	auto mat = Identity<float, 4>();
	mat(0,0) = x;
	mat(1,1) = y;
	mat(2,2) = z;
	return mat;
}

template <class T, int r, int c>
Matrix<T,r,c> normalize(Matrix<T,r,c>& mat) {
	Matrix<T,r,c> mat2;
	T sum = T();
	for (int i = 0; i < r*c; i++) {
		sum += mat(i)*mat(i);
	}
	for (int i = 0; i < r*c; i++) {
		mat2(i) = mat(i)/sum;
	}
	return mat2;
}
template <class T, int r, int c>
Matrix<T,r,c> normalize(Matrix<T,r,c>&& mat) {
	Matrix<T,r,c> mat2;
	T sum = T();
	for (int i = 0; i < r*c; i++) {
		sum += mat(i)*mat(i);
	}
	for (int i = 0; i < r*c; i++) {
		mat2(i) = mat(i)/sum;
	}
	return mat2;
}

template <class T>
T det(Matrix<T, 1, 1>& mat) {
	return mat(0);
}
template <class T>
T det(Matrix<T, 1, 1>&& mat) {
	return mat(0);
}

template <class T, int n>
T det(Matrix<T, n, n>& mat) {
	T sum = T();
	for (int i = 0; i < n; i++) {
		Matrix<T, n-1, n-1> mat2;
		for (int j = 0; j < n; j++) {
			if (j == i) continue;
			int j2 = j;
			if (j2 > i) j2--;
			for (int k = 0; k < n; k++) {
				if (k == i) continue;
				int k2 = k;
				if (k2 > i) k2--;
				mat2(j2, k2) = mat(j,k);
			}
		}
		if (i % 2 == 0) {
			sum += mat(i)*det(mat2);
		}
		else {
			sum -= mat(i)*det(mat2);
		}
	}
	return sum;
}
template <class T, int n>
T det(Matrix<T, n, n>&& mat) {
	T sum = T();
	for (int i = 0; i < n; i++) {
		Matrix<T, n-1, n-1> mat2;
		for (int j = 0; j < n; j++) {
			if (j == i) continue;
			int j2 = j;
			if (j2 > i) j2--;
			for (int k = 0; k < n; k++) {
				if (k == i) continue;
				int k2 = k;
				if (k2 > i) k2--;
				mat2(j2, k2) = mat(j,k);
			}
		}
		if (i % 2 == 0) {
			sum += mat(i)*det(mat2);
		}
		else {
			sum -= mat(i)*det(mat2);
		}
	}
	return sum;
}

template <class E, class F, class G>
Matrix<float, 4, 4> lookAt(colVector<E, 3> cameraPos, colVector<F, 3> target, colVector<G, 3> up) {
	Matrix <float, 4, 4> mat;
	auto Z = normalize(cameraPos-target);
	auto X = noramlize(up^Z);
	auto Y = Z^X;
	mat << X(0),X(1),X(2),-1*X*cameraPos,
	    Y(0),Y(1),Y(2),-1*Y*cameraPos,
	    Z(0),Z(1),Z(2),-1*Z*cameraPos,
	    0,0,0,1;
	return mat;
}

template <class T, class E, class F, class G>
Matrix<float, 4, 4> projection(T&& fovy, E&& aspect, F&& zNear, G&& zFar) {
	Matrix<float, 4, 4> mat;
	auto tanHalfFovy = tan(fovy / 2.0);
	mat(0,0) = 1.0 / (aspect * tanHalfFovy);
	mat(1,1) = 1.0 / tanHalfFovy;
	mat(2,2) = (zFar + zNear) / (zFar - zNear);
	mat(2,3) = 1.0;
	mat(3,2) = - (2.0 * zNear * zFar) / (zFar - zNear);
	return mat;
}

template <class T>
using quaternion = colVector<T, 4>;

template <class T, class E>
quaternion<float> genQuaternion(const colVector<T, 3> &axis, E theta) {
	quaternion<float> quat;
	float sinTh = sin(theta/2);
	quat << sinTh*axis[0],sinTh*axis[1],sinTh*axis[2],cos(theta/2);
	return quat;
}

template <class T, class E>
quaternion<typename std::common_type_t<T, E>> quatMult(const quaternion<T> &quat1,const quaternion<T> &quat2) {
	quaternion<typename std::common_type_t<T, E>> quat;
	quat << quat2[3]*quat1[0]+quat1[3]*quat2[0]+quat1[1]*quat2[2]-quat1[2]*quat2[1], quat2[3]*quat1[1]+quat1[3]*quat2[1]+quat1[2]*quat2[0]-quat2[2]*quat1[0], quat2[3]*quat1[2]+quat1[3]*quat2[2]+quat1[0]*quat2[1]-quat1[1]*quat2[0], quat1[3]*quat2[3]-quat1[0]*quat2[0]-quat1[1]*quat2[1]-quat1[2]*quat2[2];
	return quat;
}

template <class T>
quaternion<T> invert(const quaternion<T> &quat) {
	quaternion<T> nquat;
	nquat << -1*quat[0],-1*quat[1],-1*quat[2],quat[3];
	return nquat;
}

template <class T, class E>
colVector<typename std::common_type_t<T, E>,3> rotate(const colVector<T, 3> &vec,const quaternion<E> &quat) {
	quaternion<E> quat2;
	quat2 << vec[0],vec[1],vec[2],0;
	auto result = quatMult( quatMult( quat, quat2), invert(quat));
	colVector<typename std::common_type_t<T, E>,3> vec2;
	vec2 << result(0),result(1),result(2);
	return vec2;
}

#endif
