#ifndef _MATRIXH_INCLUDED
#define _MATRIXH_INCLUDED
#include <type_traits>

template <class ItemType = double>
class Matrix {
public:
	Matrix(int w = 0, int h = 0);
	Matrix(const Matrix<ItemType> &M);
	void copy(const Matrix<ItemType> &M);
	~Matrix();

	ItemType get(int x, int y);
	bool set(int x, int y, ItemType val);
	
	int getHeight();
	int getWidth();

	void print();

	Matrix<ItemType> multiply(const Matrix<ItemType> &M2, bool overwrite = false);
	Matrix<ItemType> multiply(long double scalar, bool overwrite = false);
	Matrix<ItemType> elementMultiply(const Matrix<ItemType> &M2);

	Matrix<ItemType> add(const Matrix<ItemType> &M2, bool overwrite = false);
	Matrix<ItemType> add(long double val);

	Matrix<ItemType> divide(const Matrix<ItemType> &M2);
	Matrix<ItemType> divide(long double scalar);
	Matrix<ItemType> elementDivide(const Matrix<ItemType> &M2);

	Matrix<ItemType> subtract(const Matrix<ItemType> &M2);
	Matrix<ItemType> subtract(long double val);

	Matrix<ItemType> inverse();
	ItemType determinant();

	Matrix<ItemType> transpose();

	Matrix<int> identity(int h, int w);

	Matrix<ItemType> map(ItemType (*function) (ItemType), bool overwrite = false);
	template <class returnType>
	Matrix<returnType> map(returnType (*function) (ItemType));

	Matrix<ItemType> solve(bool overwrite = false);
	Matrix<ItemType> gaussJordan(bool overwrite = false);
	Matrix<ItemType> rowEchelon(bool overwrite = false);
	Matrix<ItemType> reducedRowEchelon(bool overwrite = false);
	
private:
	int width;
	int height;
	ItemType** matrix;
};

#include "../src/Matrix.cpp"
#endif
