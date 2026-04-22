#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	UINT LoadTexture(char* fileName);

	void DrawPatch(double R, int n);

	void DrawEarth(double r, int tes);

	void DrawBody(double r, int tes, int* texture);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context

public:
	int e[6], m[6], s[6];


	double r = 3;
	double alpha = 0.;
	double beta = 0.;
	int last_x = 0.;
	int last_y = 0.;
	bool pressed = false;

	double angle = 0.0;
};
