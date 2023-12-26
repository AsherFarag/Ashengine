#include "Ashengine.h"


Colour Ashengine::GetColour(float Lum)
{
	return Colour();
}

void Ashengine::OnCreate()
{
	// Create a window to draw to.
	GameWindow = new Window(WindowTitle, WindowWidth, WindowHeight, PixelWidth, PixelHeight);

	// Set the main window for this application to the one we just made.
	Window* GameWindowPointer = GameWindow;
	Window::SetMainWindow(GameWindowPointer);

	Cube.Tris = {
		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
	};
}

void Ashengine::OnUpdate(float a_DeltaTime)
{
	// Clear the background ready for things to be drawn over the top of it.
	GameWindow->SetBuffer(Colour::BLUE);
	
	// Rotate Cube
	Matrix4 MatRotZ, MatRotX;
	Theta += 5.0f * a_DeltaTime;

	// Rotation Z
	MatRotZ.M[0][0] = cosf(Theta);
	MatRotZ.M[0][1] = sinf(Theta);
	MatRotZ.M[1][0] = -sinf(Theta);
	MatRotZ.M[1][1] = cosf(Theta);
	MatRotZ.M[2][2] = 1;
	MatRotZ.M[3][3] = 1;

	// Rotation X
	MatRotX.M[0][0] = 1;
	MatRotX.M[1][1] = cosf(Theta * 0.5f);
	MatRotX.M[1][2] = sinf(Theta * 0.5f);
	MatRotX.M[2][1] = -sinf(Theta * 0.5f);
	MatRotX.M[2][2] = cosf(Theta * 0.5f);
	MatRotX.M[3][3] = 1;

	// Draw Cube
	for (auto Tri : Cube.Tris)
	{
		Triangle ProjectedTri, TranslatedTri, ZRotatedTri, ZXRotatedTri;

		// Rotate in Z-Axis
		MultiplyMatrixVector(Tri.Points[0], ZRotatedTri.Points[0], MatRotZ);
		MultiplyMatrixVector(Tri.Points[1], ZRotatedTri.Points[1], MatRotZ);
		MultiplyMatrixVector(Tri.Points[2], ZRotatedTri.Points[2], MatRotZ);

		// Rotate in X-Axis
		MultiplyMatrixVector(ZRotatedTri.Points[0], ZXRotatedTri.Points[0], MatRotX);
		MultiplyMatrixVector(ZRotatedTri.Points[1], ZXRotatedTri.Points[1], MatRotX);
		MultiplyMatrixVector(ZRotatedTri.Points[2], ZXRotatedTri.Points[2], MatRotX);

		// Offset into the screen
		TranslatedTri = ZXRotatedTri;
		TranslatedTri.Points[0].Z = ZXRotatedTri.Points[0].Z + 3.0f;
		TranslatedTri.Points[1].Z = ZXRotatedTri.Points[1].Z + 3.0f;
		TranslatedTri.Points[2].Z = ZXRotatedTri.Points[2].Z + 3.0f;

		Vector3 Normal, Line1, Line2;

		Line1.X = TranslatedTri.Points[1].X - TranslatedTri.Points[0].X;
		Line1.Y = TranslatedTri.Points[1].Y - TranslatedTri.Points[0].Y;
		Line1.Z = TranslatedTri.Points[1].Z - TranslatedTri.Points[0].Z;

		Line2.X = TranslatedTri.Points[2].X - TranslatedTri.Points[0].X;
		Line2.Y = TranslatedTri.Points[2].Y - TranslatedTri.Points[0].Y;
		Line2.Z = TranslatedTri.Points[2].Z - TranslatedTri.Points[0].Z;

		Normal.X = Line1.Y * Line2.Z - Line1.Z * Line2.Y;
		Normal.Y = Line1.Z * Line2.X - Line1.X * Line2.Z;
		Normal.Z = Line1.X * Line2.Y - Line1.Y * Line2.X;

		float Length = sqrtf(Normal.X * Normal.X + Normal.Y * Normal.Y + Normal.Z * Normal.Z);
		Normal.X /= Length; Normal.Y /= Length; Normal.Z /= Length;

		if (Normal.X * (TranslatedTri.Points[0].X - Camera.X) +
			Normal.Y * (TranslatedTri.Points[0].Y - Camera.Y) +
			Normal.Z * (TranslatedTri.Points[0].Z - Camera.Z) < 0.0f)
		{
			// Illumination
			Vector3 LightDirection = { 0.0f, 0.0f, -1.0f };
			float Length = sqrtf(LightDirection.X * LightDirection.X + LightDirection.Y * LightDirection.Y + LightDirection.Z * LightDirection.Z);
			LightDirection.X /= Length; LightDirection.Y /= Length; LightDirection.Z /= Length;

			float DotProduct = Normal.X * LightDirection.X + Normal.Y * LightDirection.Y + Normal.Z * LightDirection.Z;

			Colour TriColour(DotProduct, DotProduct, DotProduct, 0.1f);

			// Project triangles from 3D -> 2D
			MultiplyMatrixVector(TranslatedTri.Points[0], ProjectedTri.Points[0], ProjMat);
			MultiplyMatrixVector(TranslatedTri.Points[1], ProjectedTri.Points[1], ProjMat);
			MultiplyMatrixVector(TranslatedTri.Points[2], ProjectedTri.Points[2], ProjMat);

			// Scale into View
			ProjectedTri.Points[0].X += 1.0f; ProjectedTri.Points[0].Y += 1.0f;
			ProjectedTri.Points[1].X += 1.0f; ProjectedTri.Points[1].Y += 1.0f;
			ProjectedTri.Points[2].X += 1.0f; ProjectedTri.Points[2].Y += 1.0f;

			ProjectedTri.Points[0].X *= 0.5f * (float)WindowWidth;
			ProjectedTri.Points[0].Y *= 0.5f * (float)WindowHeight;
			ProjectedTri.Points[1].X *= 0.5f * (float)WindowWidth;
			ProjectedTri.Points[1].Y *= 0.5f * (float)WindowHeight;
			ProjectedTri.Points[2].X *= 0.5f * (float)WindowWidth;
			ProjectedTri.Points[2].Y *= 0.5f * (float)WindowHeight;

			// Draw Triangle
			GameWindow->DrawFillTriangle(
				ProjectedTri.Points[0].X, ProjectedTri.Points[0].Y,
				ProjectedTri.Points[1].X, ProjectedTri.Points[1].Y,
				ProjectedTri.Points[2].X, ProjectedTri.Points[2].Y,
				TriColour);
		}
	}

	// Draw to the window.
	GameWindow->Draw();
}

void Ashengine::OnDestroy()
{
	delete GameWindow;
}
