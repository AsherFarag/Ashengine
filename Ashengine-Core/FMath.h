#pragma once

#include <vector>

namespace FMath
{
	# define PI 3.14159265358979323846

	struct Vector3
	{
		float X, Y, Z;
	};

	struct Matrix4
	{
		float M[4][4] = { 0 };

		Matrix4() {};

		Matrix4(
			float M00, float M01, float M02,
			float M10, float M11, float M12,
			float M20, float M21, float M22);

		Matrix4(
			float M00, float M01, float M02, float M03,
			float M10, float M11, float M12, float M13,
			float M20, float M21, float M22, float M23,
			float M30, float M31, float M32, float M33);
	};

	void MultiplyMatrixVector(Vector3& i, Vector3& o, Matrix4& m);
};