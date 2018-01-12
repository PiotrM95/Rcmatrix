#include"rcmatrix.h"
#include<stdexcept>
#include<exception>
#include <fstream>

using namespace std;

Matrix::Matrix()
{
	data = new Rcmatrix(1);
}


Matrix::Matrix(size_t w, size_t k, float diagonal, float background)
{
	data = new Rcmatrix(1);
	try
	{
		data->matrix = new  float*[w];
	}
	catch (std::bad_alloc)
	{
		delete data;
		
		throw OutofMemory();
	}

	for (size_t i = 0; i<w; i++)
	{
		try
		{
			data->matrix[i] = new float[k];
		}
		catch (std::bad_alloc)
		{
			for (size_t z = i; z >= 0; z--)
				delete data->matrix[z];
			
			delete 	[] data->matrix;
			delete data;
			
			throw OutofMemory();
		}
			for (size_t j = 0; j<k; j++)
			{
				if (j == i)
					data->matrix[i][j] = diagonal;
				else
					data->matrix[i][j] = background;
			}

	}
	data->w = w;
	data->k = k;
}

Matrix::Matrix(fstream& plik)
{
size_t w,k;
plik>> w>>k;
*this(w,k,0,0);
for(size_t i=0;i<w;i++)
	for(size_t j=0;j<k;j++)
			{ 
			float z;
			plik>>z;
			*this(i,j) = z;
			cout << *this(i,j)<<" ";
			}
}
Matrix::~Matrix()
{
	if (data != nullptr)
	{

		if (data->n>1)
			data->n--;
		else
			delete data;
	}
}

Matrix::Matrix(const Matrix & m)
{
	data = m.data;
	m.data->n++;
}

Matrix & Matrix::operator=(Matrix & m)
{
	if (this->data->matrix == nullptr)
	{
		this->data = m.data;
		m.data->n++;
		return *this;
	}
	else
	{
		Matrix tmp(m.data->w, m.data->k, 0, 0);
		for (size_t i = 0; i<m.data->w; i++)
			for (size_t j = 0; j<m.data->k; j++)
				tmp.data->matrix[i][j] = m.data->matrix[i][j];
		this->clear();
		this->data = tmp.data;
		return *this;
	}

}




bool Matrix::operator==(Matrix & m)
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

bool Matrix::operator!=(Matrix & m)
{
	bool result = (*this == m);
	if (result == true)
		return false;
	else
		return true;
}
float& Matrix::operator()(size_t i, size_t j)
{
	if (i >= data->w || j >= data->k)
	{
		  
		throw IndexOutofRange();
	}
	return data->matrix[i][j];
}

float Matrix::operator()(size_t i, size_t j) const
{
	if (i >= data->w || j >= data->k)
	{
		throw IndexOutofRange();
	}
	return data->matrix[i][j];
}

Matrix Matrix::operator*(Matrix & m)
{

	if (this->data->w != m.data->k)
	{
		throw WrongMatrixDimension();
	}
	Matrix result(data->w, m.data->k, 0, 0);
/*	for (size_t i = 0; i<data->w; i++)
		for (size_t i = 0; i < m.data->w; i++) {
			for (size_t j = 0; j < m.data->k; j++) {
				for (size_t k = 0; k < m.data->k; k++) {
					result(i, j) += this->data->matrix[i][k] * m(k, j);
				}
			}
		}
	*/	
			
	 for(size_t i=0; i<data->w; i++)
 {
   for(size_t j=0; j<data->w; j++)
     {
         for(size_t k=0; k<data->w; k++)
          {
       result(i,j) += this->data->matrix[i][k] * m(k,j);
       
       }
       }
       }
	
	return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix & m)
{
	for (size_t i = 0; i<m.data->w; i++)
	{ 
		for (size_t j = 0; j<m.data->k; j++)
			os << m(i,j) << '\t';
		os << '\n';
	}
		
	return os;
}
/*
std::fstream& operator>>(std::fstream& is, Matrix & m)
{
	for (size_t i = 0; i<m.data->w; i++)
		for (size_t j = 0; j<m.data->k; j++)
			is >> m.data->matrix[i][j];
			
			size_t w,k;
			is >> w >> k;
			Matrix z(w,k,0,0);
			for (size_t i = 0; i<z.data->w; i++)
		for (size_t j = 0; j<z.data->k; j++)
			is >> z.data->matrix(i,j);
	return is;
}

*/

void Matrix::clear()
{
	if (data != nullptr)
	{
		if (data->n == 1)
		{
			if (data != nullptr)
				delete data;
		}
		else
		{
			data->n--;
			data = nullptr;
			data = new Rcmatrix(1);

		}
	}
}


/*


char & Matrix::operator[](size_t index)
{
return data->string[index];
}

const char & Matrix::operator[](size_t index) const
{
return data->string[index];
}

char & Matrix::at(size_t index)
{
if (index > data->string_length)
{
throw std::out_of_range("out_of_range");
}
return data->string[index];
}




int Matrix::atoi()
{
bool character = false;
int tmp = 0;
size_t i = 0;
if (data->string[0] == '-')
{
i++;
character = true;
}
while (i<size())
{
tmp = 10 * tmp + data->string[i] - 48;
i++;
}
return character ? -tmp : tmp;
}



const char & Matrix::at(size_t index) const
{
if (index > data->string_length)
{
throw std::out_of_range("out_of_range");
}
return data->string[index];
}

Matrix &Matrix::operator=(const Matrix & str)
{
delete data;
data = str.data;
data->n++;
return *this;
}

Matrix &Matrix::operator=(const char* str)
{

data->alocated = strl(str) + 1;
data->string_length = strl(str);
data->string = new char[capacity()];
for (size_t i = 0; i < size(); i++)
data->string[i] = str[i];
data->string[size()] = '\0';
return *this;
}

Matrix &Matrix::operator=( char* str)
{

data->alocated = strl(str) + 1;
data->string_length = strl(str);
data->string = new char[capacity()];
for (size_t i = 0; i < size(); i++)
data->string[i] = str[i];
data->string[size()] = '\0';
return *this;
}



Matrix &Matrix::operator=(char str)
{

data->alocated = 2;
data->string_length = 1;
data->string = new char[2];
data->string[0] = str;
data->string[1] = '\0';
return *this;
}

Matrix & Matrix::operator+=(Matrix & str)
{
size_t tmp = size();
size_t i = 0;
this->resize(size() + str.size());
while (str[tmp] != '\0')
data->string[tmp] = str.data->string[i], tmp++, i++;
return *this;
}

Matrix & Matrix::operator+=(const char* s)
{
size_t size = strl(s);
size_t z = this->size();
size_t i = 0;
this->resize(this->size() + size);
while (s[i] != '\0')
data->string[z] = s[i], s++, i++;
return *this;
}

Matrix & Matrix::operator+=(char c)
{

this->resize(this->size() + 1);
this->data->string[size() - 1] = c;
return *this;
}

void Matrix::swap(Matrix & str)
{
Matrix s1;
s1 = str;
str = *this;
*this = s1;
}
int Matrix::compare(const Matrix & str) const
{
for (size_t i = 0; i<data->string_length && i<str.size(); i++)
{
if (data->string[i] > str.data->string[i])
{
return  1;
break;
}
if (data->string[i] < str.data->string[i])
{
return -1;
break;
}
}
if (data->string_length == str.size())
return 0;
else if (data->string_length > str.size())
return -1;
else return 1;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
Matrix s1;
s1.resize(lhs.data->string_length + rhs.data->string_length);
int i = 0, j = 0;
while (lhs.data->string[i] != '\0')
s1.data->string[i] = lhs.data->string[i], i++;
while (rhs.data->string[j] != '\0')
s1.data->string[i] = rhs.data->string[j], j++;
return s1;
}

Matrix operator+(const Matrix& lhs, const char* rhs)

{
Matrix s1;
s1.resize(lhs.data->string_length + s1.strl(rhs));
int i = 0, j = 0;
while (lhs.data->string[i] != '\0')
s1.data->string[i] = lhs.data->string[i], i++;
while (rhs[j] != '\0')
s1.data->string[i] = rhs[j], j++;
return s1;
}

Matrix operator+(const char* lhs, const Matrix& rhs)
{
Matrix s1;
s1.resize(s1.strl(lhs) + rhs.data->string_length);
int i = 0, j = 0;
while (lhs[i] != '\0')
s1.data->string[i] = lhs[i], i++;
while (rhs.data->string[j] != '\0')
s1.data->string[i] = rhs[j], j++;
return s1;
}

Matrix operator+(const Matrix& lhs, char rhs)
{
Matrix s1;
s1.resize(lhs.data->string_length + 1);
for (size_t i = 0; i < s1.data->string_length - 1; i++)
s1.data->string[i] = lhs.data->string[i];
s1.data->string[s1.data->string_length - 1] = rhs;
return s1;
}

Matrix operator+(char lhs, const Matrix& rhs)
{
Matrix s1;
s1.resize(rhs.data->string_length + 1);
s1.data->string[0] = lhs;
int j = 0;
for (size_t i = 1; i < s1.data->string_length; i++)
s1.data->string[i] = rhs.data->string[j], j++;
return s1;
}

bool operator==(const Matrix & lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result != 0)
return false;
return true;

}

bool operator==(const char* lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result != 0)
return false;
return true;
}

bool operator==(const Matrix & lhs, const char* rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result != 0)
return false;
return true;
}


bool operator!=(const Matrix & lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result != 0)
return true;
return false;
}

bool operator!=(const char* lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result != 0)
return true;
return false;
}

bool operator!=(const Matrix & lhs, const char* rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result != 0)
return true;
return false;
}


bool operator<(const Matrix & lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result < 0)
return true;
return false;
}

bool operator<(const char* lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result < 0)
return true;
return false;
}

bool operator<(const Matrix & lhs, const char* rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result < 0)
return true;
return false;
}


bool operator<=(const Matrix & lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result <= 0)
return true;
return false;
}

bool operator<=(const char* lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result <= 0)
return true;
return false;
}

bool operator<=(const Matrix & lhs, const char* rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result <= 0)
return true;
return false;
}


bool operator>(const Matrix & lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result > 0)
return true;
return false;
}

bool operator>(const char* lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result > 0)
return true;
return false;
}

bool operator>(const Matrix & lhs, const char* rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result > 0)
return true;
return false;
}


bool operator>=(const Matrix & lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result >= 0)
return true;
return false;
}

bool operator>=(const char* lhs, const Matrix & rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result >= 0)
return true;
return false;
}

bool operator>=(const Matrix & lhs, const char* rhs)
{
Matrix s1 = lhs;
int result = s1.compare(rhs);
if (result >= 0)
return true;
return false;
}

std::istream& operator>>(std::istream& is, Matrix & str)
{
is >> str.data->string;
return is;
}

std::ostream& operator<<(std::ostream& os, const Matrix& str)
{
os << str.data->string;
return os;
}
*/
