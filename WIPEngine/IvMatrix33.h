

#ifndef __IvMatrix33__h__
#define __IvMatrix33__h__

/*Defines the class IvMatrix33 as a 3x3 matrix
*/
class IvMatrix33 {
public:
	float mV[3][3];
	//float& operator()(unsigned int row, unsigned int col);
	IvMatrix33 operator+(const IvMatrix33& other) const;
	IvMatrix33& operator=(const IvMatrix33& other);
	IvMatrix33 operator*(const IvMatrix33& other) const;
	float Determinant(const IvMatrix33) const;
	float CrossProd(const IvMatrix33& other) const;
};

#endif