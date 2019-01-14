template <class T, int r, int c>
class Matrix {
public:
	Matrix();
	~Matrix();
	T& operator() (int x, int y);
	T& operator() (int i);
	template <class E>
	Matrix<T,r,c>& operator+=(Matrix<E,r,c> mat) {
		for (int i = 0; i < r*c; i++) {
			data[i] += mat(i);
		}
	}
	template <class E>
	Matrix<T,r,c>& operator-=(Matrix<E,r,c> mat) {
		for (int i = 0; i < r*c; i++) {
			data[i] -= mat(i);
		}
	}
	template <class E>
	Matrix<T,r,c>& operator*=(Matrix<E,c,c> mat) {
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
	}
	template<class E>
	Matrix<T,r,c>& operator*=(E val) { 
		for (int i = 0; i < r*c; i++) { 
			data[i] *= val; 
		} 
		return *this;
	}
private:
	T data[r*c];

};

template <class T, int r, int c>
class MatInitList {
public:
	MatInitList(Matrix<T,r,c>& m);
	~MatInitList();
	MatInitList<T,r,c>& operator,(T i);
private:
	Matrix<T,r,c> *mat;
	int pos;
};

template <class T, int r, int c>
MatInitList<T,r,c>::MatInitList(Matrix<T,r,c>& m) : mat(&m), pos(1) {
}

template<class T, int r, int c>
MatInitList<T,r,c>::~MatInitList() { }

template<class T, int r, int c>
MatInitList<T, r, c>& MatInitList<T,r,c>::operator,(T i) {
	(*mat)(pos) = i;
	pos++;
	return *this;
}

template <class T, int r, int c>
Matrix<T,r,c>::Matrix() {
	for (int i = 0; i < r*c; i++) {
		data[i] = T();
	}
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

template <class T, int r, int c>
MatInitList<T,r,c> operator<<(Matrix<T,r,c>& mat, T i) {
	mat(0) = i;
	MatInitList<T, r, c> initList(mat);
	return initList;
}

template <class T, int r, int c, class E>
Matrix<T, r, c> operator+(Matrix<T, r, c> mat1, Matrix<E,r,c> mat2) {
	Matrix<T, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)+mat2(i,j);
		}
	}
	return mat;
}

template <class T, int r, int c, class E>
Matrix<T, r, c> operator-(Matrix<T, r, c> mat1, Matrix<E, r, c> mat2) {
	Matrix<T, r, c> mat;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			mat(i,j) = mat1(i,j)-mat2(i,j);
		}
	}
	return mat;
}

template <class T, int i, int j, int k, class E>
Matrix<T, i, k> operator*(Matrix<T, i, j> mat1, Matrix<E, j, k> mat2) {
	Matrix<T, i, k> mat;
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

template <class T, int r, int c, class E>
Matrix<T, r, c> operator*(Matrix<T, r, c> mat, E scal) {
	Matrix<T,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = scal*mat(i);
	}
	return mat2;
}

template <class T, int r, int c, class E>
Matrix<T, r, c> operator*(E scal, Matrix<T, r, c> mat) {
	Matrix<T,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = scal*mat(i);
	}
	return mat2;
}

template <class T, int r, int c, class E>
Matrix<T, r, c> operator/(Matrix<T, r, c> mat, E scal) {
	Matrix<T,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = mat(i)/scal;
	}
	return mat2;
}

template <class T, int r, int c, class E>
Matrix<T, r, c> operator/(E scal, Matrix<T, r, c> mat) {
	Matrix<T,r,c> mat2;
	for (int i = 0; i < r*c; i++) {
		mat2(i) = mat(i)/scal;
	}
	return mat2;
}


template <class T, int r>
using colVector = Matrix<T, r, 1>;

template <class T, int c>
using rowVector = Matrix<T, 1, c>;

template<class T, int n>
Matrix<T, n, n> Identity() {
	Matrix<T, n, n> mat;
	for (int i = 0; i < n; i++) {
		mat(i,i) = 1;
	}
	return mat;
}

template <class T, int r, int c>
Matrix<T,c,r> trans(Matrix<T,r,c> mat) {
	Matrix<T,c,r> mat2;
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < r; j++) {
			mat2(i,j) = mat(j,i);
		}
	}
	return mat2;
}

template <class T>
colVector<T,3> operator^(colVector<T,3> vec1, colVector<T,3> vec2) {
	colVector<T,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}

template <class T>
rowVector<T,3> operator^(rowVector<T,3> vec1, rowVector<T,3> vec2) {
	rowVector<T,3> vec;
	vec(0) = vec1(1)*vec2(2)-vec1(2)*vec2(1);
	vec(1) = vec1(2)*vec2(0)-vec1(0)*vec2(2);
	vec(2) = vec1(0)*vec2(1)-vec1(1)*vec2(0);
	return vec;
}
