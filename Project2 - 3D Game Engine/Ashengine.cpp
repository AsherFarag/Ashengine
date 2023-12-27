#include "Ashengine.h"

#include <string>


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

	ProjectionMatrix = FMath::MakeProjectionMatrix(Fov, AspectRatio, Near, Far);

	Cube.LoadFromObjectFile("Resources/teapot.obj");
}

void Ashengine::OnUpdate(float a_DeltaTime)
{
	// Input

	if (Input::IsKeyDown(KeyCode::Up))
		Camera.Y += 10.0f * a_DeltaTime;
	if (Input::IsKeyDown(KeyCode::Down))
		Camera.Y -= 10.0f * a_DeltaTime;
	if (Input::IsKeyDown(KeyCode::Left))
		Yaw -= 2.0f * a_DeltaTime;
	if (Input::IsKeyDown(KeyCode::Right))
		Yaw += 2.0f * a_DeltaTime;

	Vector3 Forward = LookDirection * (8.0f * a_DeltaTime);

	if (Input::IsKeyDown(KeyCode::W))
		Camera += Forward;
	if (Input::IsKeyDown(KeyCode::S))
		Camera -= Forward;
	if (Input::IsKeyDown(KeyCode::A))
		Camera.X += 10.0f * a_DeltaTime;
	if (Input::IsKeyDown(KeyCode::D))
		Camera.X -= 10.0f * a_DeltaTime;


	float FPS = 1.f / a_DeltaTime;
	std::string FPSTitle = WindowTitle + std::string(" FPS: ") + std::to_string(FPS);
	GameWindow->SetTitle(FPSTitle.c_str());

	// Clear the background ready for things to be drawn over the top of it.
	GameWindow->SetBuffer(Colour::BLUE);
	
	// Rotate Mesh
	Theta += 0.0f * a_DeltaTime;

	Matrix4 MatRotX = MakeRotationXMatrix(Theta * 0.0f);
	Matrix4 MatRotY = MakeRotationYMatrix(Theta * 1.0f);
	Matrix4 MatRotZ = MakeRotationZMatrix(Theta * 0.f);

	Matrix4 TranslationMatrix = FMath::MakeTranslationMatrix(0.f, 0.f, 8);

	Matrix4 WorldMatrix;
	WorldMatrix = Matrix4(); // Identity
	WorldMatrix = MatRotZ * MatRotY; // Rotation
	WorldMatrix = WorldMatrix * TranslationMatrix; // Translation

	// Camera
	Vector3 Up = { 0, 1, 0 };
	Vector3 Target = {0, 0, 1};
	Matrix4 CameraRotation = FMath::MakeRotationYMatrix(Yaw);
	LookDirection = CameraRotation * Target;
	Target = Camera + LookDirection;

	Matrix4 CameraMatrix = PointAtMatrix(Camera, Target, Up);
	Matrix4 ViewMatrix = QuickInverseMatrix(CameraMatrix);

	std::vector<Triangle> TrianglesToRaster;

	// Draw Triangles
	for (auto Tri : Cube.Tris)
	{
		Triangle ProjectedTri, TransformedTri, ViewedTri;

		TransformedTri.Points[0] = WorldMatrix * Tri.Points[0];
		TransformedTri.Points[1] = WorldMatrix * Tri.Points[1];
		TransformedTri.Points[2] = WorldMatrix * Tri.Points[2];

		// Calculate Triangle Normal
		Vector3 Normal, Line1, Line2;
		// Get the Lines either side of Triangle
		Line1 = TransformedTri.Points[1] - TransformedTri.Points[0];
		Line2 = TransformedTri.Points[2] - TransformedTri.Points[0];
		// Take Cross-Product of Lines to get Normal to Triangle Surface
		Normal = Line1.Cross(Line2);

		Normal.Normalize();

		float Length = sqrtf(Normal.X * Normal.X + Normal.Y * Normal.Y + Normal.Z * Normal.Z);
		Normal /= Length;

		// Drawn Tris
		if (Normal.DotProduct(Camera) < 0.0f)
		{
			// Illumination
			Vector3 LightDirection = { 0.0f, 0.0f, -1.0f };
			LightDirection.Normalize();

			float DotProduct = LightDirection.DotProduct(Normal);

			ProjectedTri.m_Colour = Colour(DotProduct, DotProduct * 0.5f, DotProduct * 0.5f + pow(1.0f - DotProduct, 2.0f) * 1.2f, 0.1f);

			// Convert World Space -> View Space
			ViewedTri.Points[0] = ViewMatrix * TransformedTri.Points[0];
			ViewedTri.Points[1] = ViewMatrix * TransformedTri.Points[1];
			ViewedTri.Points[2] = ViewMatrix * TransformedTri.Points[2];

			// Project triangles from 3D -> 2D
			ProjectedTri.Points[0] = ProjectionMatrix * ViewedTri.Points[0];
			ProjectedTri.Points[1] = ProjectionMatrix * ViewedTri.Points[1];
			ProjectedTri.Points[2] = ProjectionMatrix * ViewedTri.Points[2];

			ProjectedTri.Points[0] /= ProjectedTri.Points[0].W;
			ProjectedTri.Points[1] /= ProjectedTri.Points[1].W;
			ProjectedTri.Points[2] /= ProjectedTri.Points[2].W;


			// X/Y are inverted so put them back
			ProjectedTri.Points[0].X *= -1.0f;
			ProjectedTri.Points[1].X *= -1.0f;
			ProjectedTri.Points[2].X *= -1.0f;
			ProjectedTri.Points[0].Y *= -1.0f;
			ProjectedTri.Points[1].Y *= -1.0f;
			ProjectedTri.Points[2].Y *= -1.0f;

			// Scale into View
			Vector3 OffsetView = { 1, 1, 1};
			ProjectedTri.Points[0] += OffsetView;
			ProjectedTri.Points[1] += OffsetView;
			ProjectedTri.Points[2] += OffsetView;

			ProjectedTri.Points[0].X *= 0.5f * (float)WindowWidth;
			ProjectedTri.Points[0].Y *= 0.5f * (float)WindowHeight;
			ProjectedTri.Points[1].X *= 0.5f * (float)WindowWidth;
			ProjectedTri.Points[1].Y *= 0.5f * (float)WindowHeight;
			ProjectedTri.Points[2].X *= 0.5f * (float)WindowWidth;
			ProjectedTri.Points[2].Y *= 0.5f * (float)WindowHeight;

			TrianglesToRaster.push_back(ProjectedTri);
		}
	}

	std::sort(TrianglesToRaster.begin(), TrianglesToRaster.end(), [](Triangle& T1, Triangle& T2)
	{
		float Z1 = (T1.Points[0].Z + T1.Points[1].Z + T1.Points[2].Z) / 3.0f;
		float Z2 = (T2.Points[0].Z + T2.Points[1].Z + T2.Points[2].Z) / 3.0f;
		return Z1 > Z2;
	});

	for (auto& ProjectedTri : TrianglesToRaster)
	{
		// Draw Triangle
		GameWindow->DrawFillTriangle(
			ProjectedTri.Points[0].X, ProjectedTri.Points[0].Y,
			ProjectedTri.Points[1].X, ProjectedTri.Points[1].Y,
			ProjectedTri.Points[2].X, ProjectedTri.Points[2].Y,
			ProjectedTri.m_Colour);
	}

	// Draw to the window.
	GameWindow->Draw();
}

void Ashengine::OnDestroy()
{
	delete GameWindow;
}

bool Mesh::LoadFromObjectFile(std::string a_FileName)
{
	std::ifstream File(a_FileName);
	if (!File.is_open())
		return false;

	// Local Cache of Verticies
	std::vector<Vector3> Verticies;

	while (!File.eof())
	{
		char Line[128];
		File.getline(Line, 128);

		std::strstream Stream;
		Stream << Line;

		char JunkChar;

		// Vertex Reading
		if (Line[0] == 'v' && Line[1] == ' ')
		{
			Vector3 Vertex;
			Stream >> JunkChar >> Vertex.X >> Vertex.Y >> Vertex.Z;
			Verticies.push_back(Vertex);
		}

		if (Line[0] == 'f' && Line[1] == ' ')
		{
			int Face[3];
			Stream >> JunkChar >> Face[0] >> Face[1] >> Face[2];
			Tris.push_back({ Verticies[Face[0] - 1], Verticies[Face[1] - 1], Verticies[Face[2] - 1] });
		}
	}
	return true;
}
