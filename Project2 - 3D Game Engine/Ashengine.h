#pragma once

// Ashengine
#include "Application.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Colour.hpp"
#include "Time.hpp"
#include "FMath.h"

// Std
#include <iostream>
#include <vector>

// File Handling
#include <fstream>
#include <strstream>
#include <algorithm>

using namespace FMath;

struct Triangle
{
	Vector3 Points[3];
	Colour m_Colour;
};

struct Mesh
{
	std::vector<Triangle> Tris;

	bool LoadFromObjectFile(std::string a_FileName);
};

class Ashengine : public Application
{
	// Test
	Mesh Cube;

	Vector3 Camera{0,0,1};
	Vector3 LookDirection;

	float Yaw = 0;

	Colour GetColour(float Lum);

	TimePoint StartTime;

public:

	void OnCreate() override;
	void OnUpdate(float a_DeltaTime) override;
	void OnDestroy() override;

	// Game Window
	Window* GameWindow;
	const char* WindowTitle		= "AshEngine";
	int WindowWidth				= 120;
	int WindowHeight			= 120;
	int PixelWidth				= 16;
	int PixelHeight				= 16;

	// Projection Matrix
	float Near = 0.1f;
	float Far = 1000.0f;
	float Fov = 90.0f;
	float AspectRatio = (float)WindowHeight / (float)WindowWidth;
	float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * PI);

	Matrix4 ProjectionMatrix;

	float Theta = 0.0f;

	int ClipAgainstPlane(Vector3 a_PlanePosition, Vector3 a_PlaneNormal, Triangle& InTri, Triangle& OutTri1, Triangle& OutTri2)
	{
		// Make sure Normal is normalised
		a_PlaneNormal.Normalize();

		// Return signed shortest distance from point to plane, plane normal must be normalised
		auto Distance = [&](Vector3& P)
		{
			Vector3 Normal = P.Normal();
			return (a_PlaneNormal.X * P.X + a_PlaneNormal.Y * P.Y + a_PlaneNormal.Z * P.Z - a_PlaneNormal.DotProduct(a_PlanePosition));
		};

		// Create two temporary storage arrays to classify points either side of plane
		// If distance sign is positive, point lies on "inside" of plane
		Vector3* InsidePoints[3];  int InsidePointCount = 0;
		Vector3* OutsidePoints[3]; int OutsidePointCount = 0;

		// Get signed distance of each point in triangle to plane
		float D0 = Distance(InTri.Points[0]);
		float D1 = Distance(InTri.Points[1]);
		float D2 = Distance(InTri.Points[2]);

		if (D0 >= 0) { InsidePoints[InsidePointCount++] = &InTri.Points[0]; }
		else { OutsidePoints[OutsidePointCount++] = &InTri.Points[0]; }
		if (D1 >= 0) { InsidePoints[InsidePointCount++] = &InTri.Points[1]; }
		else { OutsidePoints[OutsidePointCount++] = &InTri.Points[1]; }
		if (D2 >= 0) { InsidePoints[InsidePointCount++] = &InTri.Points[2]; }
		else { OutsidePoints[OutsidePointCount++] = &InTri.Points[2]; }

		// Classify Triangle Points, and Break Input Tri into Smaller Outputs if Possible

		if (InsidePointCount == 0)
		{
			// All points lie on the outside of plane, so clip whole triangle
			// It ceases to exist

			return 0; // No returned triangles are valid
		}

		if (InsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the triangle to simply pass through
			OutTri1 = InTri;

			return 1; // Just the one returned original triangle is valid
		}

		if (InsidePointCount == 1 && OutsidePointCount == 2)
		{
			OutTri1.m_Colour = InTri.m_Colour;

			// The inside point is valid, so keep
			OutTri1.Points[0] = *InsidePoints[0];

			// The two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			OutTri1.Points[1] = IntersectPlane(a_PlanePosition, a_PlaneNormal, *InsidePoints[0], *OutsidePoints[0]);
			OutTri1.Points[2] = IntersectPlane(a_PlanePosition, a_PlaneNormal, *InsidePoints[0], *OutsidePoints[1]);

			return 1;
		}

		if (InsidePointCount == 2 && OutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			OutTri1.m_Colour = InTri.m_Colour;
			OutTri2.m_Colour = InTri.m_Colour;

			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			OutTri1.Points[0] = *InsidePoints[0];
			OutTri1.Points[1] = *InsidePoints[1];
			OutTri1.Points[2] = IntersectPlane(a_PlanePosition, a_PlaneNormal, *InsidePoints[0], *OutsidePoints[0]);

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			OutTri2.Points[0] = *InsidePoints[1];
			OutTri2.Points[1] = OutTri1.Points[2];
			OutTri2.Points[2] = IntersectPlane(a_PlanePosition, a_PlaneNormal, *InsidePoints[1], *OutsidePoints[0]);

			return 2; // Return two newly formed triangles which form a quad
		}

	}

};

