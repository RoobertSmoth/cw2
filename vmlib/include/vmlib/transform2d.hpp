#ifndef TRANSFORM2D_HPP_6A31CA41_7EDF_4BBD_BE8D_02F8ECD4A79D
#define TRANSFORM2D_HPP_6A31CA41_7EDF_4BBD_BE8D_02F8ECD4A79D

#include "stdtypes.hpp"

// For simplicity, these are defined only for Mat22f, i.e., the 2x2 float
// matrix type.
inline 
Mat22f make_scaling_2d( float aX, float aY ) noexcept
{	
	Mat22f result{};
	result[0,0] = aX;
	result[0,1] = 0;
	result[1,0] = 0;
	result[1,1] = aY;

	return result; //TODO: implement me, done
}

inline 
Mat22f make_scaling_2d( float aUniform ) noexcept
{	
	Mat22f result{};
	result[0,0] = aUniform;
	result[0,1] = 0;
	result[1,0] = 0;
	result[1,1] = aUniform;
	return result; //TODO: implement me, done
}

inline
Mat22f make_rotation_2d( float aAngleInRadians ) noexcept
{	
	Mat22f result{};
	result[0,0] = std::cos(aAngleInRadians);
	result[0,1] = -std::sin(aAngleInRadians);
	result[1,0] = std::sin(aAngleInRadians);
	result[1,1] = std::cos(aAngleInRadians);
	return result; //TODO: implement me, done
}

#endif // TRANSFORM2D_HPP_6A31CA41_7EDF_4BBD_BE8D_02F8ECD4A79D
