#pragma once

#include <vector>

namespace FMath
{
	constexpr double PI = 3.14159265358979323846f;

	struct Vector2
	{
		union
		{
			struct { float X, Y, W; };
			struct { float U, V, W; };
		};

		Vector2()
		{
			X = Y = 0;
			W = 1;
		}
		Vector2(float a_X, float a_Y)
		{
			X = a_X; Y = a_Y; W = 1;
		}
	};

	struct Vector3
	{
		union
		{
			struct { float X, Y, Z, W; };
			struct { float Pitch, Yaw, Roll; };
			float N[3] = { 0, 0, 0 };
		};

		Vector3()
		{
			X = Y = Z = 0;
			W = 1;
		}

		Vector3(float a_X, float a_Y, float a_Z)
		{
			X = a_X; Y = a_Y; Z = a_Z;
		}

		Vector3(float a_X, float a_Y, float a_Z, float a_W)
		{
			X = a_X; Y = a_Y; Z = a_Z; W = a_W;
		}
		/* Uses Square root to give exact length */
		float Length() const
		{
			return sqrtf(X * X + Y * Y + Z * Z);
		}

		/* Returns the Squared Length */
		float SqrLength() const
		{
			return X * X + Y * Y + Z * Z;
		}

		Vector3 Normal()
		{
			return *this / Length();
		}

		void Normalize()
		{
			*this /= Length();
		}

		float DotProduct(const Vector3& b)
		{
			return X * b.X + Y * b.Y + Z * b.Z;
		}

		Vector3 Cross(const Vector3& b)
		{
			return{
				N[1] * b.N[2] - N[2] * b.N[1],
				N[2] * b.N[0] - N[0] * b.N[2],
				N[0] * b.N[1] - N[1] * b.N[0] };
		}

		Vector3& operator+=(const Vector3 rhs)
		{
			this->X += rhs.X;
			this->Y += rhs.Y;
			this->Z += rhs.Z;
			return *this;
		}

		Vector3& operator-=(const Vector3 rhs)
		{
			this->X -= rhs.X;
			this->Y -= rhs.Y;
			this->Z -= rhs.Z;
			return *this;
		}

		Vector3& operator*=(const float rhs)
		{
			this->X *= rhs;
			this->Y *= rhs;
			this->Z *= rhs;
			return *this;
		}

		Vector3& operator/=(const float rhs)
		{
			this->X /= rhs;
			this->Y /= rhs;
			this->Z /= rhs;
			return *this;
		}

		Vector3 operator+(const Vector3 rhs)
		{
			Vector3 Result;
			Result.X = this->X + rhs.X;
			Result.Y = this->Y + rhs.Y;
			Result.Z = this->Z + rhs.Z;
			return Result;
		}

		Vector3 operator-(const Vector3 rhs)
		{
			Vector3 Result;
			Result.X = this->X - rhs.X;
			Result.Y = this->Y - rhs.Y;
			Result.Z = this->Z - rhs.Z;
			return Result;
		}

		Vector3 operator*(const float rhs)
		{
			Vector3 Result;
			Result.X = this->X * rhs;
			Result.Y = this->Y * rhs;
			Result.Z = this->Z * rhs;
			return Result;
		}

		Vector3 operator/(const float rhs)
		{
			Vector3 Result;
			Result.X = this->X / rhs;
			Result.Y = this->Y / rhs;
			Result.Z = this->Z / rhs;
			return Result;
		}

	};

	struct Matrix4
	{
		float M[4][4] = { 0 };

		/* Identity Matrix */
		Matrix4()
		{
			M[0][0] = 1.0f;
			M[1][1] = 1.0f;
			M[2][2] = 1.0f;
			M[3][3] = 1.0f;
		}

		Matrix4(
			float M00, float M01, float M02,
			float M10, float M11, float M12,
			float M20, float M21, float M22)
		{
			M[0][0] = M00; M[0][1] = M01; M[0][2] = M02;
			M[1][0] = M10; M[1][1] = M11; M[1][2] = M12;
			M[2][0] = M20; M[2][1] = M21; M[2][2] = M22;
		}

		Matrix4(
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

		Vector3 operator*(const Vector3 rhs)
		{
			Vector3 Result;
			Result.X = rhs.X * this->M[0][0] + rhs.Y * this->M[1][0] + rhs.Z * this->M[2][0] + rhs.W * this->M[3][0];
			Result.Y = rhs.X * this->M[0][1] + rhs.Y * this->M[1][1] + rhs.Z * this->M[2][1] + rhs.W * this->M[3][1];
			Result.Z = rhs.X * this->M[0][2] + rhs.Y * this->M[1][2] + rhs.Z * this->M[2][2] + rhs.W * this->M[3][2];
			Result.W = rhs.X * this->M[0][3] + rhs.Y * this->M[1][3] + rhs.Z * this->M[2][3] + rhs.W * this->M[3][3];
			return Result;
		}

		Matrix4 operator*(const Matrix4 rhs)
		{
			Matrix4 Matrix;
			for (int c = 0; c < 4; c++)
				for (int r = 0; r < 4; r++)
				{
					Matrix.M[r][c]
						= this->M[r][0] * rhs.M[0][c]
						+ this->M[r][1] * rhs.M[1][c]
						+ this->M[r][2] * rhs.M[2][c]
						+ this->M[r][3] * rhs.M[3][c];
				}
			return Matrix;
		}
	};

#pragma region Matrix

	inline Matrix4 PointAtMatrix(Vector3& Position, Vector3& Target, Vector3& Up)
	{
		// Calculate New Forward Vector
		Vector3 NewForward = Target - Position;
		NewForward.Normalize();

		// Calculate New Up Vector
		Vector3 a = NewForward * Up.DotProduct(NewForward);
		Vector3 NewUp = Up - a;
		NewUp.Normalize();

		// Calculate New Right Vector
		Vector3 NewRight = NewUp.Cross(NewForward);

		// Construct Dimensioning and Translation Matrix	
		Matrix4 Matrix;
		Matrix.M[0][0] = NewRight.X;	Matrix.M[0][1] = NewRight.Y;	Matrix.M[0][2] = NewRight.Z;	Matrix.M[0][3] = 0.0f;
		Matrix.M[1][0] = NewUp.X;		Matrix.M[1][1] = NewUp.Y;		Matrix.M[1][2] = NewUp.Z;		Matrix.M[1][3] = 0.0f;
		Matrix.M[2][0] = NewForward.X;	Matrix.M[2][1] = NewForward.Y;	Matrix.M[2][2] = NewForward.Z;	Matrix.M[2][3] = 0.0f;
		Matrix.M[3][0] = Position.X;	Matrix.M[3][1] = Position.Y;	Matrix.M[3][2] = Position.Z;	Matrix.M[3][3] = 1.0f;
		return Matrix;
	}

	// Only for Rotation/Translation Matrices
	inline Matrix4 QuickInverseMatrix(Matrix4& M)
	{
		Matrix4 Matrix;
		Matrix.M[0][0] = M.M[0][0]; Matrix.M[0][1] = M.M[1][0]; Matrix.M[0][2] = M.M[2][0]; Matrix.M[0][3] = 0.0f;
		Matrix.M[1][0] = M.M[0][1]; Matrix.M[1][1] = M.M[1][1]; Matrix.M[1][2] = M.M[2][1]; Matrix.M[1][3] = 0.0f;
		Matrix.M[2][0] = M.M[0][2]; Matrix.M[2][1] = M.M[1][2]; Matrix.M[2][2] = M.M[2][2]; Matrix.M[2][3] = 0.0f;
		Matrix.M[3][0] = -(M.M[3][0] * Matrix.M[0][0] + M.M[3][1] * Matrix.M[1][0] + M.M[3][2] * Matrix.M[2][0]);
		Matrix.M[3][1] = -(M.M[3][0] * Matrix.M[0][1] + M.M[3][1] * Matrix.M[1][1] + M.M[3][2] * Matrix.M[2][1]);
		Matrix.M[3][2] = -(M.M[3][0] * Matrix.M[0][2] + M.M[3][1] * Matrix.M[1][2] + M.M[3][2] * Matrix.M[2][2]);
		Matrix.M[3][3] = 1.0f;
		return Matrix;
	}

	inline Matrix4 MakeRotationXMatrix(float AngleRad)
	{
		Matrix4 Matrix;
		Matrix.M[0][0] = 1.0f;
		Matrix.M[1][1] = cosf(AngleRad);
		Matrix.M[1][2] = sinf(AngleRad);
		Matrix.M[2][1] = -sinf(AngleRad);
		Matrix.M[2][2] = cosf(AngleRad);
		Matrix.M[3][3] = 1.0f;
		return Matrix;
	}

	inline Matrix4 MakeRotationYMatrix(float AngleRad)
	{
		Matrix4 Matrix;
		Matrix.M[0][0] = cosf(AngleRad);
		Matrix.M[0][2] = sinf(AngleRad);
		Matrix.M[2][0] = -sinf(AngleRad);
		Matrix.M[1][1] = 1.0f;
		Matrix.M[2][2] = cosf(AngleRad);
		Matrix.M[3][3] = 1.0f;
		return Matrix;
	}

	inline Matrix4 MakeRotationZMatrix(float AngleRad)
	{
		Matrix4 Matrix;
		Matrix.M[0][0] = cosf(AngleRad);
		Matrix.M[0][1] = sinf(AngleRad);
		Matrix.M[1][0] = -sinf(AngleRad);
		Matrix.M[1][1] = cosf(AngleRad);
		Matrix.M[2][2] = 1.0f;
		Matrix.M[3][3] = 1.0f;
		return Matrix;
	}

	inline Matrix4 MakeTranslationMatrix(float x, float y, float z)
	{
		Matrix4 matrix;
		matrix.M[0][0] = 1.0f;
		matrix.M[1][1] = 1.0f;
		matrix.M[2][2] = 1.0f;
		matrix.M[3][3] = 1.0f;
		matrix.M[3][0] = x;
		matrix.M[3][1] = y;
		matrix.M[3][2] = z;
		return matrix;
	}

	inline Matrix4 MakeProjectionMatrix(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
	{
		float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
		Matrix4 matrix;
		matrix.M[0][0] = fAspectRatio * fFovRad;
		matrix.M[1][1] = fFovRad;
		matrix.M[2][2] = fFar / (fFar - fNear);
		matrix.M[3][2] = (-fFar * fNear) / (fFar - fNear);
		matrix.M[2][3] = 1.0f;
		matrix.M[3][3] = 0.0f;
		return matrix;
	}

#pragma endregion

	inline Vector3 IntersectPlane(Vector3& a_PlanePosition, Vector3& a_PlaneNormal, Vector3& a_LineStart, Vector3& a_LineEnd, float &T)
	{
		a_PlaneNormal.Normalize();
		float PlaneD = -(a_PlaneNormal.DotProduct(a_PlanePosition));
		float AD = a_LineStart.DotProduct(a_PlaneNormal);
		float BD = a_LineEnd.DotProduct(a_PlaneNormal);
		T = (-PlaneD - AD) / (BD - AD);
		Vector3 LineStartToEnd = a_LineEnd - a_LineStart;
		Vector3 LineToIntersect = LineStartToEnd * T;

		return a_LineStart + LineToIntersect;
	}
};