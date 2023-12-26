#include "FMath.h"

FMath::Matrix4::Matrix4(
	float M00, float M01, float M02,
	float M10, float M11, float M12,
	float M20, float M21, float M22)
{
	M[0][0] = M00; M[0][1] = M01; M[0][2] = M02;
	M[1][0] = M10; M[1][1] = M11; M[1][2] = M12;
	M[2][0] = M20; M[2][1] = M21; M[2][2] = M22;
}

FMath::Matrix4::Matrix4(
	float M00, float M01, float M02, float M03,
	float M10, float M11, float M12, float M13,
	float M20, float M21, float M22, float M23,
	float M30, float M31, float M32, float M33)
{
	M[0][0] = M00; M[0][1] = M01; M[0][2] = M02; M[0][3] = M03;
	M[1][0] = M10; M[1][1] = M11; M[1][2] = M12; M[1][3] = M13;
	M[2][0] = M20; M[2][1] = M21; M[2][2] = M22; M[2][3] = M23;
	M[3][0] = M30; M[3][1] = M31; M[3][2] = M32; M[3][3] = M33;
}

void FMath::MultiplyMatrixVector(Vector3& i, Vector3& o, Matrix4& m)
{
	o.X = i.X * m.M[0][0] + i.Y * m.M[1][0] + i.Z * m.M[2][0] + m.M[3][0];
	o.Y = i.X * m.M[0][1] + i.Y * m.M[1][1] + i.Z * m.M[2][1] + m.M[3][1];
	o.Z = i.X * m.M[0][2] + i.Y * m.M[1][2] + i.Z * m.M[2][2] + m.M[3][2];
	float w = i.X * m.M[0][3] + i.Y * m.M[1][3] + i.Z * m.M[2][3] + m.M[3][3];

	if (w != 0.0f)
	{
		o.X /= w; o.Y /= w; o.Z /= w;
	}
}
