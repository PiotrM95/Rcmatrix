#include <iostream>
#include "rcmatrix.h"
#include <fstream>
#include<exception>

using namespace std;
int main()
{
	try
	{
	//cout << endl;

	Matrix A1(2,5,1,0);			//Create matrix A1 = [	1.0  0.0  0.0  0.0  0.0
									//			0.0  1.0  0.0  0.0  0.0  ]

		Matrix A2(5, 2, 0.0, 6.3);		//Create matrix A1 = [	0.0  6.3  6.3 
	//									//			6.3  0.0  6.3 
	//									//			6.3  6.3  0.0
	//									//			6.3  6.3  6.3
	//									//			6.3  6.3  6.3  ]
		cout << A1 << endl;
		cout<<endl;
		cout << A2 << endl;
		Matrix S = A1 * A2;			//Multiply A1 by A2
		cout << S << endl;
		//------------------------------------------------------
	fstream f1;
		f1.open("matrix.dat", fstream::in);
		Matrix B(f1);				//Read the matrix data from file matrix.dat
		f1.close();				//First two values in this file specify the matrix dimensions
	//cout << B << endl;
/*
		S = B;						//Assign B to S
		S(0,0) = 1.4;					//Modify S
		cout << "S[0][0]=" << S(0,0) << endl;		//Verify S
		cout << "B[0][0]=" << B(0,0) << endl;		//Verify B
	*/
	}
	catch( IndexOutofRange& w)
	{
		cout << "Index Out of Range" << endl;
	}
	catch( WrongMatrixDimension& w)
	{
		cout << "Wrong Matrix Dimensions" << endl;
	}
	catch( OutofMemory& w)
	{
		cout << "Out of Memory" << endl;
	}
	
		getchar();
	return 0;

}
