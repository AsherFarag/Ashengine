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
	Vector3 Camera {0, 0, 1};

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
	int PixelWidth				= 8;
	int PixelHeight				= 8;

	// Projection Matrix
	float Near = 0.1f;
	float Far = 1000.0f;
	float Fov = 90.0f;
	float AspectRatio = (float)WindowHeight / (float)WindowWidth;
	float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * PI);

	Matrix4 ProjectionMatrix;

	float Theta = 0.0f;

};

