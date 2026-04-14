#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);
	void SetLightingBase();
	UINT LoadTexture(char* file);
	void SetMaterial(float ambient[4], float diffuse[4], float specular[4], float emission[4], int shininess);
	// inicijalizuje scenu,
	void DrawCylinder(CDC* pDC, double r, double h,  double angle_step, double red, double green, double blue);
	void DrawCylinderOpt(CDC* pDC, double r, double h, double angle_step, double red, double green, double blue);
	void DrawCone(CDC* pDC, double r, double h, double angle_step);
	void DrawSphere(CDC* pDC, double r, double alpha_step, double beta_step, double red, double green, double blue);
	void DrawGround(CDC* pDC, double h, double dim1, double dim2);
	void DrawAxes();
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
public:
	double angle_x = 0.0;
	double angle_z = 0.0;

	double yaw = 0.0;
	double pitch = 0.0;

	bool is_pressed = false;

	int last_x = 0, last_y = 0;
	double r_cam = 80;
	double zoom = 1.0;

	bool grid = true;
};
