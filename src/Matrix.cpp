#ifndef _MATRIX_INCLUDED
#define _MATRIX_INCLUDED

#include "../includes/Matrix.h"
#include <iostream>

#pragma region Constructors and Destructor
template <class ItemType>
Matrix<ItemType>::Matrix(int h, int w) {
	width = w;
	height = h;
	matrix = new ItemType*[height];
	for (int i = 0; i < height; i++) {
		matrix[i] = new ItemType[width];
	}
}

template <class ItemType>
Matrix<ItemType>::Matrix(const Matrix<ItemType> &M) {
	height = M.height;
	width = M.width;
	matrix = new ItemType*[height];
	for (int i = 0; i < height; i++) {
		matrix[i] = new ItemType[width];
		for (int j = 0; j < width; j++) {
			matrix[i][j] = M.matrix[i][j];
		}
	}
}

template <class ItemType>
void Matrix<ItemType>::copy(const Matrix<ItemType> &M) {
	for (int i = 0; i < height; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
	height = M.height;
	width = M.width;
	matrix = new ItemType*[height];
	for (int i = 0; i < height; i++) {
		matrix[i] = new ItemType[width];
		for (int j = 0; j < width; j++) {
			matrix[i][j] = M.matrix[i][j];
		}
	}
}

template <class ItemType>
Matrix<ItemType>::~Matrix() {
	for (int i = 0; i < height; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}
#pragma endregion

#pragma region Get Set and Info Stuffs
template <class ItemType>
ItemType Matrix<ItemType>::get(int row, int col) {
	return matrix[row][col];
}

template <class ItemType>
bool Matrix<ItemType>::set(int row, int col, ItemType val) {
	matrix[row][col] = ItemType(val);
	return true;
}

template <class ItemType>
int Matrix<ItemType>::getHeight() {
	return height;
}

template  <class ItemType>
int Matrix<ItemType>::getWidth() {
	return width;
}

template <class ItemType>
void Matrix<ItemType>::print() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			std::cout << matrix[i][j] << '\t';
		}
		std::cout << std::endl;
	}
}
#pragma endregion


#pragma region Basic Matrix Math Ops
template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::multiply(const Matrix<ItemType> &M, bool overwrite) {
	if (width != M.height) {
		return Matrix<ItemType>(0, 0);
	}
	if (!overwrite) {
		Matrix<ItemType> result(height, M.width);
		for (int i = 0; i < result.height; i++) {
			for (int j = 0; j < result.width; j++) {
				ItemType sum = 0;
				for (int k = 0; k < width; k++) {
					sum += matrix[i][k] * M.matrix[k][j];
				}
				result.matrix[i][j] = sum;
			}
		}
		return result;
	}
	int newHeight = height;
	int newWidth = M.width;
	ItemType** newMatrix = new ItemType*[height];
	for (int i = 0; i < height; i++) {
		newMatrix[i] = new ItemType[width];
	}

	for (int i = 0; i < newHeight; i++) {
		for (int j = 0; j < newWidth; j++) {
			ItemType sum = 0;
			for (int k = 0; k < width; k++) {
				sum += matrix[i][k] * M.matrix[k][j];
			}
			newMatrix[i][j] = sum;
		}
	}

	for (int i = 0; i < height; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;

	height = newHeight;
	width = newWidth;
	matrix = newMatrix;

	return *this;
}

template <class ItemType>
Matrix<ItemType>  Matrix<ItemType>::multiply(long double scalar, bool overwrite) {
	if (!overwrite) {
		Matrix<ItemType> M(height, width);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				M.matrix[i][j] = matrix[i][j] * scalar;
			}
		}
		return M;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			matrix[i][j] *= scalar;
		}
	}
	return *this;
}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::elementMultiply(const Matrix<ItemType> &M2) {
	if (M2.height != height || M2.width != width) {
		return Matrix<ItemType>(0, 0);
	}
	Matrix<ItemType> result(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result.matrix[i][j] = matrix[i][j] * M2.matrix[i][j];
		}
	}
	return result;
}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::divide(const Matrix<ItemType> &M2) {
	if (M2.height != M2.width || width != M2.height) {
		return Matrix<ItemType>(0, 0);
	}
	Matrix<ItemType> divisor(M2.inverse());
	return multiply(divisor);
}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::divide(long double scalar) {
	Matrix result(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result.matrix[i][j] = matrix[i][j] / scalar;
		}
	}
	return result;
}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::elementDivide(const Matrix<ItemType> &M2) {
	if (M2.height != height || M2.width != width) {
		return Matrix<ItemType>(0, 0);
	}
	Matrix<ItemType> result(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result.matrix[i][j] = matrix[i][j] / M2.matrix[i][j];
		}
	}
	return result;
}


template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::add(const Matrix<ItemType> &M2, bool overwrite) {
	if (M2.width != width || M2.height != height) {
		return Matrix<ItemType>(0, 0);//there will never be an itemless matrix, therefore checking for this condition will inform something has gone wrong
	}
	if (!overwrite) {
		Matrix<ItemType> result(height, width);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				result.matrix[i][j] = matrix[i][j] + M2.matrix[i][j];
			}
		}
		return M2;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			matrix[i][j] += M2.matrix[i][j];
		}
	}
	return *this;
}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::add(long double num) {
	Matrix<ItemType> M(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			M.matrix[i][j] = matrix[i][j] + num;
		}
	}
	return M;
}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::subtract(const Matrix<ItemType> &M2) {
	if (M2.height != height || M2.width != width) {
		return Matrix<ItemType>(0, 0);
	}
	Matrix<ItemType> result(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result.matrix[i][j] = matrix[i][j] - M2.matrix[i][j];
		}
	}
	return result;
}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::subtract(long double val) {//still need to decide on this lol
	Matrix<ItemType> M(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			M.matrix[i][j] = matrix[i][j] - val;
		}
	}
	return M;
}

//use gauss-jordan elimination for any matrix larger than 2x2? there may be a size threshold where i'd want to try to do an in-place inversion that takes longer instead of abusing memory
template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::inverse() {
	if (height != width) {
		return Matrix<ItemType>(0, 0);
	}
	Matrix<ItemType> result(height, height);
	if (height == 1) {
		result.matrix[0][0] = 1 / matrix[0][0];
		return result;
	}
	if (height == 2) {
		ItemType det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		result.matrix[0][0] = matrix[1][1];
		result.matrix[1][1] = matrix[0][0];
		result.matrix[0][1] = matrix[0][1];
		result.matrix[1][0] = matrix[1][0];
		return result.divide(det);
	}
	Matrix augmented(height, height * 2);
	for (int i = 0; i < height; i++) {
		int j;
		for (j = 0; j < height; j++) {
			augmented.matrix[i][j] = matrix[i][j];
		}
		for (j = width; j < augmented.width; j++) {
			augmented.matrix[i][j] = 0;
			if (width + i == j) {
				augmented.matrix[i][j] = 1;
			}
		}
	}
	augmented.solve(true);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result.matrix[i][j] = augmented.matrix[i][j + width];
		}
	}
	return result;
}

template <class ItemType>
ItemType Matrix<ItemType>::determinant() {
	if (height != width || height <= 1) {
		return ItemType(0);
	}
	if (height == 2) {
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	}
	Matrix<ItemType> temp(*this);
	temp.rowEchelon(true);
	ItemType det = temp.matrix[0][0];
	for (int i = 1; i < height; i++) {
		det *= temp.matrix[i][i];
	}
	return det;
}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::transpose() {
	Matrix<ItemType> M(width, height);
	for (int i = 0; i < M.height; i++) {
		for (int j = 0; j < M.width; j++) {
			M.matrix[i][j] = matrix[j][i];
		}
	}
	return M;
}

template <class ItemType>
Matrix<int> Matrix<ItemType>::identity(int h, int w) {
	Matrix<int> M(h, w);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			M.set(i, j, i == j ? 1 : 0);
		}
	}
	return M;
}


#pragma endregion

#pragma region Map Functions
template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::map(ItemType (*function) (ItemType), bool overwrite) {
	if (!overwrite) {
		Matrix M(height, width);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				M.matrix[i][j] = function(matrix[i][j]);
			}
		}
		return M;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			matrix[i][j] = function(matrix[i][j]);
		}
	}
	return *this;
}

template <class ItemType>
template <class returnType>
Matrix<returnType> Matrix<ItemType>::map(returnType (*function) (ItemType)) {
	Matrix<returnType> M(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			M.set(i, j, function(matrix[i][j]));
		}
	}
	return M;
}
#pragma endregion

#pragma region Advanced Matrix Math (rref)
template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::gaussJordan(bool overwrite) {
	if (!overwrite) {
		Matrix<ItemType> result(*this);
		return result.solve(1);
	}

	return this->solve(1);
}

//solve system of linear equations [A|b] where A is the matrix of equations and b is the vector of solutions. returns 
//eliminated matrix, from which values can be extrapolated
template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::solve(bool overwrite) {
	if (!overwrite) {
		Matrix<ItemType> result(*this);

		bool pivotFound = false;
		bool nonZeroExists = false;

		int numPivots = height > width ? width : height;
		int pivotColumn = 0, pivotRow = 0;
		ItemType temp;
		ItemType scalar;

		for (int i = 0; i < numPivots; i++, pivotColumn++, pivotRow++) {
			if (pivotRow >= height || pivotColumn >= width - 1) {
				break;//break if pivot location is outside matrix, or in the last column
			}

			//find or create the pivot
			pivotFound = false;
			nonZeroExists = false;
			for (int j = pivotRow; j < height; j++) {
				if (result.matrix[j][pivotColumn] == 1) {
					pivotFound = true;
					nonZeroExists = true;
					if (j != pivotRow) {
						//swap current row with pivot row
						ItemType* tempPtr = matrix[pivotRow];
						matrix[pivotRow] = matrix[j];
						matrix[j] = tempPtr;
						tempPtr = NULL;
					}
				}
				if (result.matrix[j][pivotColumn] != 0) {
					nonZeroExists = true;
				}
			}
			if (!pivotFound) {
				if (!nonZeroExists) {
					//advance pivot column. go to start of loop
					pivotRow--;
					numPivots++;
					continue;
				} else {
					//find the next nonzero row
					//swap, then divid the row by pivot
					for (int j = pivotRow; j < height; j++) {
						if (result.matrix[j][pivotColumn] != 0) {
							pivotFound = true;
							if (j != pivotRow) {
								//swap pivot row w/ this row
								ItemType* tempPtr = matrix[pivotRow];
								matrix[pivotRow] = matrix[j];
								matrix[j] = tempPtr;
								tempPtr = NULL;
							}
							//divide the current row by the pivot
							//break back to outer loop
							scalar = result.matrix[pivotRow][pivotColumn];
							for (int k = pivotColumn; k < width; k++) {
								result.matrix[pivotRow][k] /= scalar;
							}
							break;
						}
					}
				}
			}
			//if there's still no pivot after this, or is outside the scope of the matrix, break the loop
			if (!pivotFound) {
				break;
			}

			//eliminate above pivot
			for (int j = pivotRow - 1; j >= 0; j--) {
				scalar = result.matrix[j][pivotColumn];
				for (int k = pivotColumn; k < width; k++) {
					result.matrix[j][k] -= result.matrix[pivotRow][k] * scalar;
				}
			}

			//eliminate below pivot
			for (int j = pivotRow + 1; j < height; j++) {
				scalar = result.matrix[j][pivotColumn];
				for (int k = pivotColumn; k < width; k++) {
					result.matrix[j][k] -= result.matrix[pivotRow][k] * scalar;
				}
			}
		}

		return result;
	} 

	bool pivotFound = false;
	bool nonZeroExists = false;

	int numPivots = height > width ? width : height;
	int pivotColumn = 0, pivotRow = 0;
	ItemType temp;
	ItemType scalar;

	for (int i = 0; i < numPivots; i++, pivotColumn++, pivotRow++) {
		if (pivotRow >= height || pivotColumn >= width - 1) {
			break;//break if pivot location is outside matrix, or in the last column
		}

		//find or create the pivot
		pivotFound = false;
		nonZeroExists = false;
		for (int j = pivotRow; j < height; j++) {
			if (matrix[j][pivotColumn] == 1) {
				pivotFound = true;
				nonZeroExists = true;
				if (j != pivotRow) {
					//swap current row with pivot row
					ItemType* tempPtr = matrix[pivotRow];
					matrix[pivotRow] = matrix[j];
					matrix[j] = tempPtr;
					tempPtr = NULL;
				}
			}
			if (matrix[j][pivotColumn] != 0) {
				nonZeroExists = true;
			}
		}
		if (!pivotFound) {
			if (!nonZeroExists) {
				//advance pivot column. go to start of loop
				pivotRow--;
				numPivots++;
				continue;
			} else {
				//find the next nonzero row
				//swap, then divid the row by pivot
				for (int j = pivotRow; j < height; j++) {
					if (matrix[j][pivotColumn] != 0) {
						pivotFound = true;
						if (j != pivotRow) {
							//swap pivot row w/ this row
							ItemType* tempPtr = matrix[pivotRow];
							matrix[pivotRow] = matrix[j];
							matrix[j] = tempPtr;
							tempPtr = NULL;
						}
						//divide the current row by the pivot
						//break back to outer loop
						scalar = matrix[pivotRow][pivotColumn];
						for (int k = pivotColumn; k < width; k++) {
							matrix[pivotRow][k] /= scalar;
						}
						break;
					}
				}
			}
		}
		//if there's still no pivot after this, or is outside the scope of the matrix, break the loop
		if (!pivotFound) {
			break;
		}

		//eliminate above pivot
		for (int j = pivotRow - 1; j >= 0; j--) {
			scalar = matrix[j][pivotColumn];
			for (int k = pivotColumn; k < width; k++) {
				matrix[j][k] -= matrix[pivotRow][k] * scalar;
			}
		}

		//eliminate below pivot
		for (int j = pivotRow + 1; j < height; j++) {
			scalar = matrix[j][pivotColumn];
			for (int k = pivotColumn; k < width; k++) {
				matrix[j][k] -= matrix[pivotRow][k] * scalar;
			}
		}
	}

	return *this;

}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::reducedRowEchelon(bool overwrite) {
	if (!overwrite) {
		Matrix<ItemType> result(height, width);
		result.rowEchelon(true);
		ItemType scalar;
		for (int i = 0, j = 0; i < height && j < width; i++, j++) {
			if (result.matrix[i][j] != 0) {
				scalar = result.matrix[i][j];
				for (int k = j; k < width; k++) {
					result.matrix[i][k] /= scalar;
				}
			} else {
				j++;
			}
		}
		return result;
	}

	this->rowEchelon(true);
	ItemType scalar;
	for (int i = 0, j = 0; i < height && j < width; i++, j++) {
		if (matrix[i][j] != 0) {
			scalar = matrix[i][j];
			for (int k = j; k < width; k++) {
				matrix[i][k] /= scalar;
			}
		} else {
			j++;
		}
	}

	return *this;
}

template <class ItemType>
Matrix<ItemType> Matrix<ItemType>::rowEchelon(bool overwrite) {
	if (!overwrite) {
		Matrix<ItemType> result(*this);

		bool pivotFound = false;
		bool nonZeroExists = false;

		int numPivots = height > width ? width : height;
		int pivotColumn = 0, pivotRow = 0;
		ItemType temp;
		ItemType scalar;

		//get that shit to reduced row echelon form 
		for (int i = 0; i < numPivots; i++, pivotColumn++, pivotRow++) {
			if (pivotRow > height - 1 || pivotColumn > width - 1) {
				break;//break if pivot location is outside matrix, or in the last column
			}

			//find or create the pivot
			pivotFound = true;
			if (result.matrix[pivotRow][pivotColumn] == 0) {
				pivotFound = false;
				for (int j = pivotRow; j < height; j++) {
					if (result.matrix[j][pivotColumn] != 0) {
						pivotFound = true;
						if (j != pivotRow) {
							//swap current row with pivot row
							ItemType* tempPtr = matrix[pivotRow];
							matrix[pivotRow] = matrix[j];
							matrix[j] = tempPtr;
							tempPtr = NULL;
						}
					}
				}
			}
			if (!pivotFound) {
				//advance pivot column. go to start of loop
				pivotRow--;
				continue;
			}

			//eliminate below pivot
			for (int j = pivotRow + 1; j < height; j++) {
				scalar = result.matrix[j][pivotColumn];
				for (int k = pivotColumn; k < width; k++) {
					result.matrix[j][k] -= result.matrix[pivotRow][k] * scalar;
				}
			}
		}

		return result;
	}

	bool pivotFound = false;
	bool nonZeroExists = false;

	int numPivots = height > width ? width : height;
	int pivotColumn = 0, pivotRow = 0;
	ItemType temp;
	ItemType scalar;

	//get that shit to reduced row echelon form 
	for (int i = 0; i < numPivots; i++, pivotColumn++, pivotRow++) {
		if (pivotRow > height - 1 || pivotColumn > width - 1) {
			break;//break if pivot location is outside matrix, or in the last column
		}

		//find or create the pivot
		pivotFound = true;
		if (matrix[pivotRow][pivotColumn] == 0) {
			pivotFound = false;
			for (int j = pivotRow; j < height; j++) {
				if (matrix[j][pivotColumn] != 0) {
					pivotFound = true;
					if (j != pivotRow) {
						//swap current row with pivot row
						ItemType* tempPtr = matrix[pivotRow];
						matrix[pivotRow] = matrix[j];
						matrix[j] = pivotRow;
						pivotRow = NULL;
					}
				}
			}
		}
		if (!pivotFound) {
			//advance pivot column. go to start of loop
			pivotRow--;
			continue;
		}

		//eliminate below pivot
		for (int j = pivotRow + 1; j < height; j++) {
			scalar = matrix[j][pivotColumn] / matrix[pivotRow][pivotColumn];
			for (int k = pivotColumn; k < width; k++) {
				matrix[j][k] -= matrix[pivotRow][k] * scalar;
			}
		}
	}

	return *this;

}
#pragma endregion

#pragma region Utility

#pragma endregion

#pragma region Operator Overloads
#pragma endregion

#endif
