#ifndef __Vector4__h__
#define __Vector4__h__

//This class will hold any points or vectors used by models and game
class Vector4 {
public:
	float mV[4];

	Vector4(float x = 0, float y = 0 , float z = 0, float w = 0);
	Vector4 operator+(const Vector4& other) const;
	Vector4& normalize();
};

#endif