#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void DrawAxes();
	void Reshape(CDC* pDC, int w, int h);
	void DrawEnvCube(double a, double tx, double ty, double tz);
	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);
	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);
	void DrawLegSegment(double r, double h, int nSeg);
	void DrawLeg();
	void DrawSpiderBody();
	void DrawSpider();
	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	UINT LoadTexture(char* fileName);
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

public:
	double r;
	double zoom;
	double alpha;
	double beta;
	int last_x;
	int last_y;
	bool pressed = false;
	double eye_x;
	double eye_y;
	double eye_z;

	double last_zoom;

	int front;
	int back;
	int left;
	int right;
	int top;
	int bot;
	int spider;

	double y_ang = 0.;
	double x_ang = 0.;
	double dist = 10;
};
