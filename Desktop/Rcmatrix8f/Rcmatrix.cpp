#include"Rcmatrix.h"
#include<stdexcept>
#include<exception>
#include <fstream>

using namespace std;

Matrix::Matrix(fstream& plik)
{
	size_t w, k;
	plik >> w >> k;
	data = new Rcmatrix(w,k,0,0);	
		for(size_t i =0;i<w;i++)
		      for(size_t j = 0; j<k; j++)
		          plik>>  this->data->matrix[i][j];
}
Matrix::~Matrix()
{
	if (data != nullptr)
	{
		if (data->n>1)
			data->n--;
		else
		{	
		      delete data;
		}
	}
}

Matrix::Matrix(const Matrix & m)
{
	data = m.data;
	m.data->n++;
}

Matrix & Matrix::operator=(const Matrix & m)
{
	m.data->n++;
	if (--this->data->n == 0)
		delete this->data;
	this->data = m.data;
	return *this;
}

Matrix Matrix::operator+(const Matrix & m)const
{
	if (this->data->w == m.data->w && this->data->k == m.data->k)
	{
		
		Matrix tmp(data->w, data->k,0,0);
			for (size_t i = 0; i < data->w; i++)
				for (size_t j = 0; j < data->k; j++)
					tmp.data->matrix[i][j] = data->matrix[i][j] + m.data->matrix[i][j];
			return tmp;
	}
	else
		throw WrongMatrixDimension();
}

Matrix & Matrix::operator+=(const Matrix & m)
{
	if (this->data->w == m.data->w && this->data->k == m.data->k)
	{
		
		Rcmatrix* tab;
		tab = new Rcmatrix(data->w, data->k,0,0);
		for (size_t i = 0; i < data->w; i++)
			for (size_t j = 0; j < data->k; j++)
				tab->matrix[i][j] = data->matrix[i][j] + m.data->matrix[i][j];
		
		this->data = this->data->detach();
		this->data = tab;
		return *this;
	}
	else
		throw WrongMatrixDimension();
}

Matrix Matrix::operator-(const Matrix & m)const
{
	if (this->data->w == m.data->w && this->data->k == m.data->k)
	{
		Matrix tmp(data->w, data->k,0,0);
			for (size_t i = 0; i < data->w; i++)
				for (size_t j = 0; j < data->k; j++)
					tmp.data->matrix[i][j] = data->matrix[i][j] - m.data->matrix[i][j];
			return tmp;
	}
	else
		throw WrongMatrixDimension();
}

Matrix & Matrix::operator-=(const Matrix & m)
{
	if (this->data->w == m.data->w && this->data->k == m.data->k)
	{
		Rcmatrix* tab;
		tab = new Rcmatrix(data->w, data->k,0,0);
		for (size_t i = 0; i < data->w; i++)
			for (size_t j = 0; j < data->k; j++)
				tab->matrix[i][j] = data->matrix[i][j] - m.data->matrix[i][j];
		delete this->data;
		this->data = tab;
		return *this;
	}
	else
		throw WrongMatrixDimension();
}

bool Matrix::operator==(const Matrix & m)const
{
	if (this->data->w != m.data->w || this->data->k != m.data->k)
		return false;
	else
	{
		for (size_t i = 0; i<m.data->w; i++)
			for (size_t j = 0; j<m.data->k; j++)
				if (this->data->matrix[i][j] != m.data->matrix[i][j])
					return false;
		return true;
	}
}

bool Matrix::operator!=(const Matrix & m)const
{
	bool result = (*this == m);
	if (result == true)
		return false;
	else
		return true;
}

Matrix Matrix::operator*(const Matrix & m)const
{
	if (this->data->w != m.data->k)
		throw WrongMatrixDimension();
	Matrix result(data->w, m.data->k, 0, 0);
	for (size_t i = 0; i<data->w; i++)
		for (size_t j = 0; j<data->w; j++)
			for (size_t k = 0; k<data->w; k++)
				result.data->matrix[i][j] += this->data->matrix[i][k] * m.data->matrix[k][j];
	return result;
}



std::ostream& operator<<(std::ostream& os, const Matrix & m)
{
	for (size_t i = 0; i<m.data->w; i++)
	{
		for (size_t j = 0; j<m.data->k; j++)
			os << m.data->matrix[i][j] << '\t';
		os << '\n';
	}
	return os;
}

float Matrix::operator[](size_t j)const
{
	if (j > this->data->k)
		throw IndexOutofRange();
	return *(data->matrix[j]);
}

Matrix::Cref Matrix::operator[](size_t i)
{
	if (i > this->data->k)
		throw IndexOutofRange();
		
	return Cref(*this, i);
}
