

#ifndef __Transform44__h__
#define __Transform44__h__
#include "IvMatrix44.h"
/*Defines the class Transform which is made from a 4x4 Matrix
	This class Will Define:
		Translations
		Scaling
		Rotations
		World2View
		Perspective

	Why does this Exist?
		Adds a layer of abstraction away from the 4x4 matrix data type, making code easier to read
		Also allows us to define matrices for specific purposes
*/
class Transform {
public:
	IvMatrix44 data;

	Transform(const Transform& other);
	Transform(int type = 0, float x = 0, float y = 0, float z = 0);
	Transform operator*(const Transform& other) const;
	Transform& operator=(const Transform& other);
private:
	Transform& Translate(float x, float y, float z);
	Transform& Scale(float x, float y, float z);
	Transform& Rotate(float x, float y, float z);

};





#endif