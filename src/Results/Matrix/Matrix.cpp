#include "Matrix.h"

const scalar_t& Matrix::operator()(size_t i, size_t j) const
{
	return Elements[i * Columns + j];
}

const Matrix Matrix::operator*(scalar_t scalar) const
{
	Matrix result = *this;

	for (auto& element : result.Elements)
		element *= scalar;

	return result;
}

const Matrix Matrix::operator+(const Matrix& other) const
{
	assert(Rows == other.Rows && Columns == other.Columns);

	Matrix result = *this;

	for (int i = 0; i < Elements.size(); i++)
		result.Elements[i] += other.Elements[i];

	return result;
}

const Matrix Matrix::operator-(const Matrix& other) const
{
	assert(Rows == other.Rows && Columns == other.Columns);

	Matrix result = *this;

	for (int i = 0; i < Elements.size(); i++)
		result.Elements[i] -= other.Elements[i];

	return result;
}

const Matrix Matrix::operator*(const Matrix& other) const
{
	assert(Columns == other.Rows);

	Matrix result;

	result.Rows = Rows;
	result.Columns = other.Columns;

	for (size_t i = 0; i < Rows; i++)
	{
		for (size_t j = 0; j < other.Columns; j++)
		{
			scalar_t sum = 0;

			for (size_t k = 0; k < Columns; k++)
				sum += (*this)(i, k) * other(k, j);

			result.Elements.push_back(sum);
		}
	}

	return result;

}

const Matrix Matrix::operator/(const Matrix& other) const
{
	assert(IsSquare());
	assert(other.IsSquare());

	return *this * other.Inverse();
}

const Matrix Matrix::Transpose() const
{
	Matrix result;

	result.Rows = Columns;
	result.Columns = Rows;

	for (size_t i = 0; i < Columns; i++)
	{
		for (size_t j = 0; j < Rows; j++)
		{
			result.Elements.push_back((*this)(j, i));
		}
	}

	return result;
}

const scalar_t Matrix::Determinant() const
{
	assert(IsSquare());

	if (Rows == 1)
		return Elements[0];

	if (Rows == 2)
		return Elements[0] * Elements[3] - Elements[1] * Elements[2];

	scalar_t result = 0;

	for (size_t i = 0; i < Rows; i++)
	{
		scalar_t sign = (i % 2 == 0) ? 1 : -1;

		Matrix submatrix = GetSubMatrix(0, i);

		result += sign * Elements[i] * submatrix.Determinant();
	}

	return result;	
}

const Matrix Matrix::GetSubMatrix(size_t row, size_t column) const
{
	assert(IsSquare());

	Matrix result;

	result.Rows = Rows - 1;
	result.Columns = Columns - 1;

	for (size_t i = 0; i < Rows; i++)
	{
		if (i == row)
			continue;

		for (size_t j = 0; j < Columns; j++)
		{
			if (j == column)
				continue;

			result.Elements.push_back((*this)(i, j));
		}
	}

	return result;
}

const Matrix Matrix::Inverse() const
{
	assert(IsSquare());
	assert(!IsSingular());

	Matrix result;

	scalar_t determinant = Determinant();

	for (size_t i = 0; i < Rows; i++)
	{
		for (size_t j = 0; j < Columns; j++)
		{
			scalar_t sign = ((i + j) % 2 == 0) ? 1 : -1;

			Matrix submatrix = GetSubMatrix(i, j);

			result.Elements.push_back(sign * submatrix.Determinant() / determinant);
		}
	}

	result = result.Transpose();

	return result;
}

const bool Matrix::IsSquare() const
{
	return Rows == Columns;
}

const bool Matrix::IsIdentity() const
{
	assert(IsSquare());

	for (size_t i = 0; i < Rows; i++)
	{
		for (size_t j = 0; j < Columns; j++)
		{
			if (i == j && (*this)(i, j) != 1)
				return false;

			if (i != j && (*this)(i, j) != 0)
				return false;
		}
	}

	return true;
}

const bool Matrix::IsDiagonal() const
{
	assert(IsSquare());

	for (size_t i = 0; i < Rows; i++)
	{
		for (size_t j = 0; j < Columns; j++)
		{
			if (i != j && (*this)(i, j) != 0)
				return false;
		}
	}

	return true;
}

const bool Matrix::IsSingular() const
{
	assert(IsSquare());

	return Determinant() == 0;
}

const bool Matrix::IsVector() const
{
	return (Rows == 1) != (Columns == 1);
}

const scalar_t Matrix::Dot(const Matrix& other) const
{
	assert(IsVector());
	assert(other.IsVector());
	assert(this->Elements.size() == other.Elements.size());

	scalar_t result = 0;

	for (size_t i = 0; i < Elements.size(); i++)
		result += Elements[i] * other.Elements[i];

	return result;
}

const Matrix Matrix::Cross(const Matrix& other) const
{
	assert(IsVector());
	assert(other.IsVector());
	assert(Elements.size() == 3 && other.Elements.size() == 3);

	Matrix result;

	result.Rows = 1;
	result.Columns = 3;

	result.Elements.push_back(Elements[1] * other.Elements[2] - Elements[2] * other.Elements[1]);
	result.Elements.push_back(Elements[2] * other.Elements[0] - Elements[0] * other.Elements[2]);
	result.Elements.push_back(Elements[0] * other.Elements[1] - Elements[1] * other.Elements[0]);

	return result;
}

const scalar_t Matrix::Norm() const
{
	assert(IsVector());

	scalar_t result = 0;

	for (auto& element : Elements)
		result += element * element;

	return sqrt(result);
}

const Matrix Matrix::Normalize() const
{
	assert(IsVector());

	scalar_t norm = Norm();

	return *this * (1/norm);
}