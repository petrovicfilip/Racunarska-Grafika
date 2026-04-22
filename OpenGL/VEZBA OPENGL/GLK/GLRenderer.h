#pragma once

class CGLRenderer
{

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void SetLightingBase();
	void SetLocalLighting(int lignt);
	UINT LoadTexture(char* file);
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);
	void DrawCube(double a, Color color);
	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void SetMaterial(float ambient[4], float diffuse[4], float specular[4], float emission[4], int shininess);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

public:
	bool pressed = false;
	double r = 5;
	double zoom = 1.0;
	double alpha = 0.0;
	double beta = 0.0;
	double last_x = 0.0;
	double last_y = 0.0;

	int texWall;
	int texCustom;

};
