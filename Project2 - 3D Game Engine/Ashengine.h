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
using namespace std;

struct Triangle
{
	Triangle() {}
	Triangle(Vector3 P1, Vector3 P2, Vector3 P3)
	{
		Points[0] = P1;
		Points[1] = P2;
		Points[2] = P3;
	}

	Triangle(Vector3 P1, Vector3 P2, Vector3 P3, Colour a_Colour)
	{
		Points[0] = P1;
		Points[1] = P2;
		Points[2] = P3;
		m_Colour = a_Colour;
	}

	Triangle(Vector3 P1, Vector3 P2, Vector3 P3, Vector2 T1, Vector2 T2, Vector2 T3, Colour a_Colour)
	{
		Points[0] = P1;
		Points[1] = P2;
		Points[2] = P3;
		TextureCoord[0] = T1;
		TextureCoord[1] = T2;
		TextureCoord[2] = T3;
		m_Colour = a_Colour;
	}

	Vector3 Points[3];
	Vector2 TextureCoord[3];
	Colour m_Colour;
};

struct Mesh
{
	std::vector<Triangle> Tris;

	bool LoadFromObjectFile(std::string a_FileName);
};

struct Sprite
{
public:
	Sprite() {}
	Sprite(int a_Width, int a_Height)
	{
		Create(a_Width, a_Height);
	}
	Sprite(std::wstring sFile)
	{
		//if (!LoadTGA(sFile))
		if (!Load(sFile))
			Create(8, 8);
	}

private:
	int m_Width = 0;
	int m_Height = 0;
	int m_Size = m_Width * m_Height;

	Colour* m_Colours = nullptr;

	void Create(int a_Width, int a_Height)
	{
		m_Width = a_Height;
		m_Height = a_Height;
		m_Size = m_Width * m_Height;
		m_Colours = new Colour[m_Size];

		for (int i = 0; i < m_Size; i++)
		{
			m_Colours[i] = Colour::LIGHT_PURPLE;
		}
	}

public:

	void SetColour(int x, int y, Colour NewColour)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
			return;
		else
			m_Colours[y * m_Width + x] = NewColour;
	}

	Colour GetColour(int x, int y)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
			return Colour::BLACK;
		else
			return m_Colours[y * m_Width + x];
	}

	Colour SampleColour(float x, float y)
	{
		int sx = (int)(x * (float)m_Width);
		int sy = (int)(y * (float)m_Height);
		if (sx < 0 || sx >= m_Width || sy < 0 || sy >= m_Height)
			return Colour::BLACK;
		else
			return m_Colours[sy * m_Width + sx];
	}

	bool Save(std::wstring sFile)
	{
		FILE* f = nullptr;
		_wfopen_s(&f, sFile.c_str(), L"wb");
		if (f == nullptr)
			return false;

		fwrite(&m_Width, sizeof(int), 1, f);
		fwrite(&m_Height, sizeof(int), 1, f);
		fwrite(m_Colours, sizeof(short), m_Width * m_Height, f);

		fclose(f);

		return true;
	}

	//bool Load(std::wstring sFile)
	//{
	//	// Clear existing data
	//	delete[] m_Colours;
	//	m_Width = 0;
	//	m_Height = 0;

	//	// Open file
	//	FILE* f = nullptr;
	//	_wfopen_s(&f, sFile.c_str(), L"rb");
	//	if (f == nullptr)
	//	{
	//		std::wcerr << L"Failed to open file: " << sFile << std::endl;
	//		return false;
	//	}

	//	// Read width and height
	//	std::fread(&m_Width, sizeof(int), 1, f);
	//	std::fread(&m_Height, sizeof(int), 1, f);

	//	// Create color array
	//	m_Size = m_Width * m_Height;
	//	m_Colours = new Colour[m_Size];

	//	// Read color data
	//	for (int i = 1; i <= m_Size; ++i)
	//	{
	//		std::fread(m_Colours, sizeof(Colour), i, f);
	//	}


	//	// Clean up
	//	std::fclose(f);

	//	return true;
	//}

	//bool LoadTGA(const std::wstring& sFile)
	bool Load(std::wstring sFile);
	//{
	//	// Clear existing data
	//	//delete[] m_Colours;

	//	if (m_Colours)
	//	{
	//		stbi_image_free((stbi_uc*)m_Colours);
	//		m_Colours = nullptr;
	//	}


	//	std::string Path(sFile.begin(), sFile.end());
	//	int Channels;
	//	m_Colours = ( Colour* )stbi_load(Path.c_str(), &m_Width, &m_Height, &Channels, 3);

	//	//m_Width = 0;
	//	//m_Height = 0;

	//	// Open file
	//	//std::ifstream file(sFile, std::ios::binary);
	//	//if (!file.is_open())
	//	//{
	//	//	std::wcerr << L"Failed to open file: " << sFile << std::endl;
	//	//	return false;
	//	//}

	//	//// Read TGA header
	//	//TGAHeader header;
	//	//file.read(reinterpret_cast<char*>(&header), sizeof(TGAHeader));

	//	//// Check for uncompressed, 24-bit color TGA
	//	//if (header.imageType != 2 || header.pixelDepth != 24)
	//	//{
	//	//	std::wcerr << L"Unsupported TGA format. Only uncompressed 24-bit color is supported." << std::endl;
	//	//	file.close();
	//	//	return false;
	//	//}

	//	//// Set width and height
	//	//m_Width = header.width;
	//	//m_Height = header.height;
	//	//m_Size = m_Width * m_Height;

	//	//// Create color array
	//	//m_Colours = new Colour[m_Size];

	//	//// Read color data
	//	//file.read(reinterpret_cast<char*>(m_Colours), m_Size * sizeof(Colour));

	//	//// Close file
	//	//file.close();

	//	return true;
	//}
};

class Ashengine : public Application
{
	// Test
	Mesh Cube;
	Sprite* CubeTexture;

	Vector3 Camera{ 0,0,1 };
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
	const char* WindowTitle = "AshEngine";
	int WindowWidth = 80;
	int WindowHeight = 80;
	int PixelWidth = 8;
	int PixelHeight = 8;

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
		// Texture Co-ordinates
		Vector2* TextureInsidePoints[3];  int TextureInsidePointCount = 0;
		Vector2* TextureOutsidePoints[3]; int TextureOutsidePointCount = 0;


		// Get signed distance of each point in triangle to plane
		float D0 = Distance(InTri.Points[0]);
		float D1 = Distance(InTri.Points[1]);
		float D2 = Distance(InTri.Points[2]);

		if (D0 >= 0)
		{
			InsidePoints[InsidePointCount++] = &InTri.Points[0];
			TextureInsidePoints[TextureInsidePointCount++] = &InTri.TextureCoord[0];
		}
		else
		{
			OutsidePoints[OutsidePointCount++] = &InTri.Points[0];
			TextureOutsidePoints[TextureOutsidePointCount++] = &InTri.TextureCoord[0];
		}
		if (D1 >= 0)
		{
			InsidePoints[InsidePointCount++] = &InTri.Points[1];
			TextureInsidePoints[TextureInsidePointCount++] = &InTri.TextureCoord[1];
		}
		else
		{
			OutsidePoints[OutsidePointCount++] = &InTri.Points[1];
			TextureOutsidePoints[TextureOutsidePointCount++] = &InTri.TextureCoord[1];
		}
		if (D2 >= 0)
		{
			InsidePoints[InsidePointCount++] = &InTri.Points[2];
			TextureInsidePoints[TextureInsidePointCount++] = &InTri.TextureCoord[2];
		}
		else
		{
			OutsidePoints[OutsidePointCount++] = &InTri.Points[2];
			TextureOutsidePoints[TextureOutsidePointCount++] = &InTri.TextureCoord[2];
		}

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
			OutTri1.TextureCoord[0] = *TextureInsidePoints[0];

			// The two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			float T;
			OutTri1.Points[1] = IntersectPlane(a_PlanePosition, a_PlaneNormal, *InsidePoints[0], *OutsidePoints[0], T);
			OutTri1.TextureCoord[1].U = T * (TextureOutsidePoints[0]->U - TextureInsidePoints[0]->U) + TextureInsidePoints[0]->U;
			OutTri1.TextureCoord[1].V = T * (TextureOutsidePoints[0]->V - TextureInsidePoints[0]->V) + TextureInsidePoints[0]->V;
			//OutTri1.TextureCoord[1].W = T * (TextureOutsidePoints[0]->W - TextureInsidePoints[0]->W) + TextureInsidePoints[0]->W;

			OutTri1.Points[2] = IntersectPlane(a_PlanePosition, a_PlaneNormal, *InsidePoints[0], *OutsidePoints[1], T);
			OutTri1.TextureCoord[2].U = T * (TextureOutsidePoints[1]->U - TextureInsidePoints[0]->U) + TextureInsidePoints[0]->U;
			OutTri1.TextureCoord[2].V = T * (TextureOutsidePoints[1]->V - TextureInsidePoints[0]->V) + TextureInsidePoints[0]->V;

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
			OutTri1.TextureCoord[0] = *TextureInsidePoints[0];
			OutTri1.TextureCoord[1] = *TextureInsidePoints[1];

			float T;
			OutTri1.Points[2] = IntersectPlane(a_PlanePosition, a_PlaneNormal, *InsidePoints[0], *OutsidePoints[0], T);
			OutTri1.TextureCoord[2].U = T * (TextureOutsidePoints[0]->U - TextureInsidePoints[0]->U) + TextureInsidePoints[0]->U;
			OutTri1.TextureCoord[2].V = T * (TextureOutsidePoints[0]->V - TextureInsidePoints[0]->V) + TextureInsidePoints[0]->V;
			//OutTri1.TextureCoord[2].W = T * (TextureOutsidePoints[0]->W - TextureInsidePoints[0]->W) + TextureInsidePoints[0]->W;

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			OutTri2.Points[0] = *InsidePoints[1];
			OutTri2.Points[0] = *InsidePoints[1];

			OutTri2.Points[1] = OutTri1.Points[2];
			OutTri2.TextureCoord[1] = OutTri1.TextureCoord[2];

			OutTri2.Points[2] = IntersectPlane(a_PlanePosition, a_PlaneNormal, *InsidePoints[1], *OutsidePoints[0], T);
			OutTri2.TextureCoord[2].U = T * (TextureOutsidePoints[0]->U - TextureInsidePoints[1]->U) + TextureInsidePoints[1]->U;
			OutTri2.TextureCoord[2].V = T * (TextureOutsidePoints[0]->V - TextureInsidePoints[1]->V) + TextureInsidePoints[1]->V;
			//OutTri2.TextureCoord[2].W = T * (TextureOutsidePoints[0]->W - TextureInsidePoints[1]->W) + TextureInsidePoints[1]->W;

			return 2; // Return two newly formed triangles which form a quad
		}

	}

	void TexturedTriangle(
		float x1, float y1, float u1, float v1, /*float w1,*/
		float x2, float y2, float u2, float v2,	/*float w2,*/
		float x3, float y3, float u3, float v3,	/*float w3,*/
		Sprite* Texture)
	{
		if (y2 < y1)
		{
			swap(y1, y2);
			swap(x1, x2);
			swap(u1, u2);
			swap(v1, v2);
			// swap(w1, w2);
		}

		if (y3 < y1)
		{
			swap(y1, y3);
			swap(x1, x3);
			swap(u1, u3);
			swap(v1, v3);
			//swap(w1, w3);
		}

		if (y3 < y2)
		{
			swap(y2, y3);
			swap(x2, x3);
			swap(u2, u3);
			swap(v2, v3);
			//swap(w2, w3);
		}

		int dy1 = y2 - y1;
		int dx1 = x2 - x1;
		float dv1 = v2 - v1;
		float du1 = u2 - u1;
		//float dw1 = w2 - w1;

		int dy2 = y3 - y1;
		int dx2 = x3 - x1;
		float dv2 = v3 - v1;
		float du2 = u3 - u1;
		//float dw2 = w3 - w1;

		float tex_u, tex_v;// , tex_w;

		float dax_step = 0, dbx_step = 0,
			du1_step = 0, dv1_step = 0,
			du2_step = 0, dv2_step = 0,
			dw1_step = 0, dw2_step = 0;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		//if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy2) du2_step = du2 / (float)abs(dy2);
		if (dy2) dv2_step = dv2 / (float)abs(dy2);
		//if (dy2) dw2_step = dw2 / (float)abs(dy2);

		if (dy1)
		{
			for (int i = y1; i <= y2; i++)
			{
				int ax = x1 + (float)(i - y1) * dax_step;
				int bx = x1 + (float)(i - y1) * dbx_step;

				float tex_su = u1 + (float)(i - y1) * du1_step;
				float tex_sv = v1 + (float)(i - y1) * dv1_step;
				//float tex_sw = w1 + (float)(i - y1) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				//float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx)
				{
					swap(ax, bx);
					swap(tex_su, tex_eu);
					swap(tex_sv, tex_ev);
					//swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				//tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					//tex_w = (1.0f - t) * tex_sw + t * tex_ew;
					//if (tex_w > pDepthBuffer[i * ScreenWidth() + j])
					//{
					//	Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
					//	pDepthBuffer[i * ScreenWidth() + j] = tex_w;
					//}

					// temp
					GameWindow->SetPixel(j, i, Texture->SampleColour(tex_u, tex_v));

					t += tstep;
				}

			}
		}

		dy1 = y3 - y2;
		dx1 = x3 - x2;
		dv1 = v3 - v2;
		du1 = u3 - u2;
		//dw1 = w3 - w2;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		du1_step = 0, dv1_step = 0;
		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		//if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy1)
		{
			for (int i = y2; i <= y3; i++)
			{
				int ax = x2 + (float)(i - y2) * dax_step;
				int bx = x1 + (float)(i - y1) * dbx_step;

				float tex_su = u2 + (float)(i - y2) * du1_step;
				float tex_sv = v2 + (float)(i - y2) * dv1_step;
				//float tex_sw = w2 + (float)(i - y2) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				//float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx)
				{
					swap(ax, bx);
					swap(tex_su, tex_eu);
					swap(tex_sv, tex_ev);
					//swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				//tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					//tex_w = (1.0f - t) * tex_sw + t * tex_ew;

					//if (tex_w > pDepthBuffer[i * ScreenWidth() + j])
					//{
					//	Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
					//	pDepthBuffer[i * ScreenWidth() + j] = tex_w;
					//}

					// temp
					GameWindow->SetPixel(j, i, Texture->SampleColour(tex_u, tex_v));

					t += tstep;
				}
			}
		}
	}

};

