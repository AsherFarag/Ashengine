#include "Ashengine.h"

#include <string>
#include <list>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Sprite::Load(std::wstring sFile)
{
	// Clear existing data
	//delete[] m_Colours;

	if (m_Colours)
	{
		stbi_image_free((stbi_uc*)m_Colours);
		m_Colours = nullptr;
	}


	std::string Path(sFile.begin(), sFile.end());
	int Channels;
	m_Colours = (Colour*)stbi_load(Path.c_str(), &m_Width, &m_Height, &Channels, 4);

	if (m_Colours)
	{
		m_Size = m_Width * m_Height;
	}
	else
	{
		m_Size = 0;
		m_Width = 0;
		m_Height = 0;
		m_Colours = nullptr;
	}

	//m_Width = 0;
	//m_Height = 0;

	// Open file
	//std::ifstream file(sFile, std::ios::binary);
	//if (!file.is_open())
	//{
	//	std::wcerr << L"Failed to open file: " << sFile << std::endl;
	//	return false;
	//}

	//// Read TGA header
	//TGAHeader header;
	//file.read(reinterpret_cast<char*>(&header), sizeof(TGAHeader));

	//// Check for uncompressed, 24-bit color TGA
	//if (header.imageType != 2 || header.pixelDepth != 24)
	//{
	//	std::wcerr << L"Unsupported TGA format. Only uncompressed 24-bit color is supported." << std::endl;
	//	file.close();
	//	return false;
	//}

	//// Set width and height
	//m_Width = header.width;
	//m_Height = header.height;
	//m_Size = m_Width * m_Height;

	//// Create color array
	//m_Colours = new Colour[m_Size];

	//// Read color data
	//file.read(reinterpret_cast<char*>(m_Colours), m_Size * sizeof(Colour));

	//// Close file
	//file.close();

	return m_Colours != nullptr;
}

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

	//Cube.LoadFromObjectFile("Resources/teapot.obj");
	Cube.Tris = {
		// SOUTH
		Triangle ( Vector3 {0.0f, 0.0f, 0.0f, 1.0f},    Vector3 {0.0f, 1.0f, 0.0f, 1.0f},    Vector3 {1.0f, 1.0f, 0.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {0.0f, 1.0f},    Vector2 {1.0f, 1.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {0.0f, 0.0f, 0.0f, 1.0f},    Vector3 {1.0f, 1.0f, 0.0f, 1.0f},    Vector3 {1.0f, 0.0f, 0.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {1.0f, 1.0f},    Vector2 {1.0f, 0.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {1.0f, 0.0f, 0.0f, 1.0f},    Vector3 {1.0f, 1.0f, 0.0f, 1.0f},    Vector3 {1.0f, 1.0f, 1.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {0.0f, 1.0f},    Vector2 {1.0f, 1.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {1.0f, 0.0f, 0.0f, 1.0f},    Vector3 {1.0f, 1.0f, 1.0f, 1.0f},    Vector3 {1.0f, 0.0f, 1.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {1.0f, 1.0f},    Vector2 {1.0f, 0.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {1.0f, 0.0f, 1.0f, 1.0f},    Vector3 {1.0f, 1.0f, 1.0f, 1.0f},    Vector3 {0.0f, 1.0f, 1.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {0.0f, 1.0f},    Vector2 {1.0f, 1.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {1.0f, 0.0f, 1.0f, 1.0f},    Vector3 {0.0f, 1.0f, 1.0f, 1.0f},    Vector3 {0.0f, 0.0f, 1.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {1.0f, 1.0f},    Vector2 {1.0f, 0.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {0.0f, 0.0f, 1.0f, 1.0f},    Vector3 {0.0f, 1.0f, 1.0f, 1.0f},    Vector3 {0.0f, 1.0f, 0.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {0.0f, 1.0f},    Vector2 {1.0f, 1.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {0.0f, 0.0f, 1.0f, 1.0f},    Vector3 {0.0f, 1.0f, 0.0f, 1.0f},    Vector3 {0.0f, 0.0f, 0.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {1.0f, 1.0f},    Vector2 {1.0f, 0.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {0.0f, 1.0f, 0.0f, 1.0f},    Vector3 {0.0f, 1.0f, 1.0f, 1.0f},    Vector3 {1.0f, 1.0f, 1.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {0.0f, 1.0f},    Vector2 {1.0f, 1.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {0.0f, 1.0f, 0.0f, 1.0f},    Vector3 {1.0f, 1.0f, 1.0f, 1.0f},    Vector3 {1.0f, 1.0f, 0.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {1.0f, 1.0f},    Vector2 {1.0f, 0.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {1.0f, 0.0f, 1.0f, 1.0f},    Vector3 {0.0f, 0.0f, 1.0f, 1.0f},    Vector3 {0.0f, 0.0f, 0.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {0.0f, 1.0f},    Vector2 {1.0f, 1.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f}),
		Triangle ( Vector3 {1.0f, 0.0f, 1.0f, 1.0f},    Vector3 {0.0f, 0.0f, 0.0f, 1.0f},    Vector3 {1.0f, 0.0f, 0.0f, 1.0f},	Vector2 {0.0f, 0.0f},    Vector2 {1.0f, 1.0f},    Vector2 {1.0f, 0.0f},	Colour {0.0f, 1.0f, 1.0f, 1.0f})
	};

	CubeTexture = new Sprite(L"Resources/ore.jpg");
}

void Ashengine::OnUpdate(float a_DeltaTime)
{
	// Input
	float CameraSpeed = 1.0f;

	if (Input::IsKeyDown(KeyCode::LeftShift))
		CameraSpeed = 5.0f;

	if (Input::IsKeyDown(KeyCode::Up))
		Camera.Y += CameraSpeed * 10.0f * a_DeltaTime;
	if (Input::IsKeyDown(KeyCode::Down))
		Camera.Y -= CameraSpeed * 10.0f * a_DeltaTime;
	if (Input::IsKeyDown(KeyCode::Left))
		Yaw -= CameraSpeed * 2.0f * a_DeltaTime;
	if (Input::IsKeyDown(KeyCode::Right))
		Yaw += CameraSpeed * 2.0f * a_DeltaTime;

	Vector3 Forward = LookDirection * (CameraSpeed * 8.0f * a_DeltaTime);

	// Camera
	Vector3 Up = { 0, 1, 0 };
	Vector3 Target = { 0, 0, 1 };
	Matrix4 CameraRotation = FMath::MakeRotationYMatrix(Yaw);
	LookDirection = CameraRotation * Target;
	Target = Camera + LookDirection;

	Matrix4 CameraMatrix = PointAtMatrix(Camera, Target, Up);

	Vector3 Right = CameraMatrix * Vector3{ 1, 0, 0 };

	if (Input::IsKeyDown(KeyCode::W))
		Camera = Camera + Forward;
	if (Input::IsKeyDown(KeyCode::S))
		Camera = Camera - Forward;
	if (Input::IsKeyDown(KeyCode::A))
		Camera += Right * (CameraSpeed * 5.0f * a_DeltaTime);
	if (Input::IsKeyDown(KeyCode::D))
		Camera -= Right * (CameraSpeed * 5.0f * a_DeltaTime);

	// Fps Counter
	float FPS = 1.f / a_DeltaTime;
	std::string FPSTitle = WindowTitle + std::string(" FPS: ") + std::to_string(FPS);
	GameWindow->SetTitle(FPSTitle.c_str());

	// Clear the background ready for things to be drawn over the top of it.
	GameWindow->SetBuffer(Colour::BLUE);
	
	// Rotate Mesh
	Theta += 0.0f * a_DeltaTime;

	Matrix4 MatRotX = MakeRotationXMatrix(Theta * 1.0f);
	Matrix4 MatRotY = MakeRotationYMatrix(Theta * 0.5f);
	Matrix4 MatRotZ = MakeRotationZMatrix(Theta * 0.f);

	Matrix4 TranslationMatrix = FMath::MakeTranslationMatrix(0, 10.f, 20.0f);

	Matrix4 WorldMatrix;
	WorldMatrix = Matrix4(); // Identity
	WorldMatrix = MatRotZ * MatRotY; // Rotation
	WorldMatrix = WorldMatrix * TranslationMatrix; // Translation

	Matrix4 ViewMatrix = QuickInverseMatrix(CameraMatrix);

	std::vector<Triangle> TrianglesToRaster;

	// Draw Triangles
	for (auto& Tri : Cube.Tris)
	{
		Triangle ProjectedTri, TransformedTri, ViewedTri;
		// Verticies
		TransformedTri.Points[0] = WorldMatrix * Tri.Points[0];
		TransformedTri.Points[1] = WorldMatrix * Tri.Points[1];
		TransformedTri.Points[2] = WorldMatrix * Tri.Points[2];
		// Texture Co-ordinates
		TransformedTri.TextureCoord[0] = Tri.TextureCoord[0];
		TransformedTri.TextureCoord[1] = Tri.TextureCoord[1];
		TransformedTri.TextureCoord[2] = Tri.TextureCoord[2];

		// Calculate Triangle Normal
		Vector3 Normal, Line1, Line2;
		// Get the Lines either side of Triangle
		Line1 = TransformedTri.Points[1] - TransformedTri.Points[0];
		Line2 = TransformedTri.Points[2] - TransformedTri.Points[0];
		// Take Cross-Product of Lines to get Normal to Triangle Surface
		Normal = Line1.Cross(Line2);

		Normal.Normalize();

		// Get Ray from triangle to camera
		Vector3 CameraRay = TransformedTri.Points[0] - Camera;

		// Drawn Tris
		if (Normal.DotProduct(CameraRay) < 0.0f)
		{
			// Illumination
			Vector3 LightDirection = { 0.0f, 5.0f, -12.5f };
			LightDirection.Normalize();

			float DotProduct = /*LightDirection.DotProduct(Normal);*/ (CameraRay.Normal() * -1).DotProduct(Normal);
			DotProduct = max(DotProduct, 0.0f);
			//ProjectedTri.m_Colour = Colour(DotProduct, DotProduct, DotProduct, 1.0f);
			ProjectedTri.m_Colour = Colour(DotProduct, DotProduct * 0.5f, DotProduct * 0.5f + pow(1.0f - DotProduct, 2.0f) * 1.2f, 0.1f);

			

			// Convert World Space -> View Space
			ViewedTri.Points[0] = ViewMatrix * TransformedTri.Points[0];
			ViewedTri.Points[1] = ViewMatrix * TransformedTri.Points[1];
			ViewedTri.Points[2] = ViewMatrix * TransformedTri.Points[2];
			ViewedTri.TextureCoord[0] = TransformedTri.TextureCoord[0];
			ViewedTri.TextureCoord[1] = TransformedTri.TextureCoord[1];
			ViewedTri.TextureCoord[2] = TransformedTri.TextureCoord[2];

			// Clip Viewed Triangles against Near Plane
			int ClippedTris = 0;
			Triangle Clipped[2];
			Vector3 PlanePosition = { 0.0f, 0.0f, 0.1f }; Vector3 PlaneNormal = { 0.0f, 0.0f, 1.0f };
			ClippedTris = ClipAgainstPlane(PlanePosition, PlaneNormal,ViewedTri, Clipped[0], Clipped[1]);

			for (int n = 0; n < ClippedTris; n++)
			{
				// Project triangles from 3D -> 2D
				ProjectedTri.Points[0] = ProjectionMatrix * Clipped[n].Points[0];
				ProjectedTri.Points[1] = ProjectionMatrix * Clipped[n].Points[1];
				ProjectedTri.Points[2] = ProjectionMatrix * Clipped[n].Points[2];
				ProjectedTri.TextureCoord[0] = Clipped[n].TextureCoord[0];
				ProjectedTri.TextureCoord[1] = Clipped[n].TextureCoord[1];
				ProjectedTri.TextureCoord[2] = Clipped[n].TextureCoord[2];

				// Scale into View
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

				// Translate into View
				Vector3 OffsetView = { 1, 1, 0 };
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
	}

	std::sort(TrianglesToRaster.begin(), TrianglesToRaster.end(), [](Triangle& T1, Triangle& T2)
	{
		float Z1 = (T1.Points[0].Z + T1.Points[1].Z + T1.Points[2].Z) / 3.0f;
		float Z2 = (T2.Points[0].Z + T2.Points[1].Z + T2.Points[2].Z) / 3.0f;
		return Z1 > Z2;
	});

	for (auto& TriToRaster : TrianglesToRaster)
	{
		// Clip Triangles against Screen Edges
		Triangle Clipped[2];
		std::list<Triangle> Triangles;

		// Add Initial Tri
		Triangles.push_back(TriToRaster);
		int NewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int TrisToAdd = 0;
			while (NewTriangles > 0)
			{
				// Take triangle from front of queue
				Triangle Test = Triangles.front();
				Triangles.pop_front();
				NewTriangles--;

				// Clip it against a plane. We only need to test each 
				// subsequent plane, against subsequent new triangles
				// as all triangles after a plane clip are guaranteed
				// to lie on the inside of the plane. I like how this
				// comment is almost completely and utterly justified
				switch (p)
				{
				case 0:	
					TrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, Test, Clipped[0], Clipped[1]);
					break;
				case 1:	
					TrisToAdd = ClipAgainstPlane({ 0.0f, (float)WindowHeight - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, Test, Clipped[0], Clipped[1]);
					break;
				case 2:	
					TrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, Test, Clipped[0], Clipped[1]);
					break;
				case 3:	
					TrisToAdd = ClipAgainstPlane({ (float)WindowWidth - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, Test, Clipped[0], Clipped[1]);
					break;
				}

				for (int w = 0; w < TrisToAdd; w++)
					Triangles.push_back(Clipped[w]);
			}

			NewTriangles = Triangles.size();
		}

		// Draw Triangle
		for (auto& TriToDraw : Triangles)
		{
			//GameWindow->DrawFillTriangle(
			//	TriToDraw.Points[0].X, TriToDraw.Points[0].Y,
			//	TriToDraw.Points[1].X, TriToDraw.Points[1].Y,
			//	TriToDraw.Points[2].X, TriToDraw.Points[2].Y,
			//	TriToDraw.m_Colour);

			TexturedTriangle(
				TriToDraw.Points[0].X, TriToDraw.Points[0].Y, TriToDraw.TextureCoord[0].X, TriToDraw.TextureCoord[0].Y,
				TriToDraw.Points[1].X, TriToDraw.Points[1].Y, TriToDraw.TextureCoord[1].X, TriToDraw.TextureCoord[1].Y,
				TriToDraw.Points[2].X, TriToDraw.Points[2].Y, TriToDraw.TextureCoord[2].X, TriToDraw.TextureCoord[2].Y, CubeTexture);



			//GameWindow->DrawTriangle(
			//	TriToDraw.Points[0].X, TriToDraw.Points[0].Y,
			//	TriToDraw.Points[1].X, TriToDraw.Points[1].Y,
			//	TriToDraw.Points[2].X, TriToDraw.Points[2].Y,
			//	Colour::WHITE);
		}
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
