

#ifndef __IvMatrix44__h__
#define __IvMatrix44__h__

/*Defines the class IvMatrix44 as a 4x4 matrix
*/
class IvMatrix44 {
public:
	float mV[4][4];

	IvMatrix44& Identity(void);
	IvMatrix44 operator+(const IvMatrix44& other) const;
	IvMatrix44& operator=(const IvMatrix44& other);
	IvMatrix44 operator*(const IvMatrix44& other) const;
	float Determinant(const IvMatrix44) const;
	float CrossProd(const IvMatrix44& other) const;
};





#endif