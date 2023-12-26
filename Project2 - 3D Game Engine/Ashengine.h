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

using namespace FMath;

struct Triangle
{
	Vector3 Points[3];
};

struct Mesh
{
	std::vector<Triangle> Tris;
};

class Ashengine : public Application
{
	// Test
	Mesh Cube;
	Vector3 Camera;

	Colour GetColour(float Lum);

	TimePoint StartTime;

public:

	void OnCreate() override;
	void OnUpdate(float a_DeltaTime) override;
	void OnDestroy() override;

	// Game Window
	Window* GameWindow;
	const char* WindowTitle	 = "AshEngine";
	const int WindowWidth	 = 120;
	const int WindowHeight	 = 120;
	const int PixelWidth	 = 8;
	const int PixelHeight	 = 8;

	// Projection Matrix
	float Near = 0.1f;
	float Far = 1000.0f;
	float Fov = 90.0f;
	float AspectRatio = (float)WindowHeight / (float)WindowWidth;
	float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * PI);

	Matrix4 ProjMat = Matrix4{
		AspectRatio * FovRad, 0, 0, 0,
		0, FovRad, 0, 0,
		0, 0, Far / (Far - Near), 1,
		0, 0, (-Far * Near) / (Far - Near), 0
	};

	float Theta = 0.0f;

};

