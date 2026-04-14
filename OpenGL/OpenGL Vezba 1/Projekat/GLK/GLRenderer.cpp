#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include <iostream>
using namespace std;
#include <vector>;
//#pragma comment(lib, "GL\\glut32.lib")
#include "DImage.h"
#define PI 3.141592
#define DEG_TO_RAD(a) (((a) * PI) / 180.0)

struct Point3
{
	double x, y, z;
};

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);
	////glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glShadeModel(GL_SMOOTH);
	//float globalAmbient[] = { 0.3,0.3,0.3,1 };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	SetLightingBase();

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::SetLightingBase()
{
	float globalAmbient[4] = {0.3, 0.3, 0.3, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, TRUE);
	
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

UINT CGLRenderer::LoadTexture(char* file)
{
	UINT texID;
	DImage img;

	img.Load(CString(file));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}


void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void NormCrossProd(double v1[3], double v2[3], double out[3])
{
	// v1 x v2 pa podelim sa d da normalizujem
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];

	double d = sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);

	out[0] /= d;
	out[1] /= d;
	out[2] /= d;
}

void CGLRenderer::SetMaterial(float ambient[4], float diffuse[4], float specular[4], float emission[4], int shininess)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void CGLRenderer::DrawCylinder(CDC* pDC, double r, double h, double angle_step, double red, double green, double blue)
{
	//glMatrixMode(GL_MODELVIEW);
	//osova trianglefan, omotac quadstrip
	double x = 0, y = 0, z = 0;
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, -1, 0);
		glVertex3f(x, y, z);
		for (int i = 0; i <= 360; i += angle_step)
		{
			x = r * cos(DEG_TO_RAD(i));
			z = r * sin(DEG_TO_RAD(i));

			glVertex3f(x, y, z);
		}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, 1, 0);
		glVertex3f(0, y + h, 0);
		for (int i = 360; i >= 0; i -= angle_step)
		{
			x = r * cos(DEG_TO_RAD(i));
			z = r * sin(DEG_TO_RAD(i));

			glVertex3f(x, y + h, z);
		}
	glEnd();
	x = r, y = 0, z = 0;
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1, 0, 0);
		glVertex3f(x, y, z);
		glVertex3f(x, y + h, z);
		for (int i = 0; i <= 360; i += angle_step)
		{
			double cosi = cos(DEG_TO_RAD(i));
			double sini = sin(DEG_TO_RAD(i));
			x = r * cosi;
			z = r * sini;
			glNormal3f(cosi, 0, sini);
			glVertex3f(x, y, z);
			glVertex3f(x, y + h, z);
		}
	glEnd();
}

void CGLRenderer::DrawCylinderOpt(CDC* pDC, double r, double h, double angle_step, double red, double green, double blue)
{
	double x = 0, y = 0, z = 0;
	vector<double> vertices1;
	vector<double> colors1;
	vector<int> indices1;
	vertices1.push_back(0.0);
	vertices1.push_back(0.0);
	vertices1.push_back(0.0);

	colors1.push_back(red);
	colors1.push_back(green);
	colors1.push_back(blue);

	for (int i = 0; i < 360; i += angle_step)
	{
		x = r * cos(DEG_TO_RAD(i));
		z = r * sin(DEG_TO_RAD(i));

		vertices1.push_back(x);
		vertices1.push_back(0.0);
		vertices1.push_back(z);

		colors1.push_back(red);
		colors1.push_back(green);
		colors1.push_back(blue);
	}

	int n = (int)(360 / angle_step);
	for(int i = 1; i < n; i++)
	{
		indices1.push_back(0);
		indices1.push_back(i);
		indices1.push_back(i + 1);
	}
	indices1.push_back(0);
	indices1.push_back(n);
	indices1.push_back(1);

	glVertexPointer(3, GL_DOUBLE, 0, vertices1.data());
	glColorPointer(3, GL_DOUBLE, 0, colors1.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawElements(GL_TRIANGLE_FAN, indices1.size(), GL_UNSIGNED_INT, indices1.data());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, y + h, 0);
	for (int i = 360; i >= 0; i -= angle_step)
	{
		x = r * cos(DEG_TO_RAD(i));
		z = r * sin(DEG_TO_RAD(i));
		glVertex3f(x, y + h, z);
	}
	glEnd();

	x = r, y = 0, z = 0;
	glBegin(GL_QUAD_STRIP);
	glVertex3f(x, y, z);
	glVertex3f(x, y + h, z);
	for (int i = 0; i <= 360; i += angle_step)
	{
		x = r * cos(DEG_TO_RAD(i));
		z = r * sin(DEG_TO_RAD(i));
		glVertex3f(x, y, z);
		glVertex3f(x, y + h, z);
	}
	glEnd();
}

void CGLRenderer::DrawCone(CDC* pDC, double r, double h, double angle_step)
{
	//glMatrixMode(GL_MODELVIEW);
	//osova trianglefan, omotac quadstrip
	double x = 0, y = 0, z = 0;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0, 0.5, 0.5);
	glVertex3f(x, y, z);
	for (int i = 0; i <= 360; i += angle_step)
	{
		x = r * cos(DEG_TO_RAD(i));
		z = r * sin(DEG_TO_RAD(i));
		glNormal3f(0, -1, 0);
		glVertex3f(x, y, z);
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	x = 0, z = 0;
	// zbog cull face
	glNormal3f(0, 1, 0);
	glVertex3f(x, y + h, z);
	for (int i = 180; i >= 0; i -= angle_step)
	{
		double cosi = cos(DEG_TO_RAD(i));
		double sini = sin(DEG_TO_RAD(i));
		x = r * cosi;
		z = r * sini;
		glNormal3f(cosi, 0, sini);
		glVertex3f(x, y, z);
	}
	for (int i = 360; i >= 180; i -= angle_step)
	{
		double cosi = cos(DEG_TO_RAD(i));
		double sini = sin(DEG_TO_RAD(i));
		x = r * cosi;
		z = r * sini;
		glNormal3f(cosi, 0, sini);
		glVertex3f(x, y, z);
	}
	glEnd();
}

void CGLRenderer::DrawSphere(CDC* pDC, double r, double alpha_step, double beta_step, double red, double green, double blue)
{
	glBegin(GL_QUAD_STRIP);
	glColor3f(red, green, blue);
	double x = 0, y = 0, z = 0;
	for (int beta = 0; beta <= 360; beta += beta_step)
	{
		for (int alpha = 90; alpha >= -90; alpha -= alpha_step)
		{
			double ar = DEG_TO_RAD(alpha);
			double br = DEG_TO_RAD(beta);

			double cos_ar = cos(ar);
			double sin_ar = sin(ar);
			double cos_br = cos(br);
			double sin_br = sin(br);
			x = r * cos_ar * cos_br;
			y = r * sin_ar;
			z = r * cos_ar * sin_br;
			glNormal3f(cos_ar * cos_br, sin_ar, cos_ar * sin_br);
			glVertex3f(x, y, z);
			br = DEG_TO_RAD(beta + beta_step);
			x = r * cos(ar) * cos(br);
			z = r * cos(ar) * sin(br);
			glNormal3f(cos_ar * cos(br), sin_ar, cos_ar * sin(br));
			glVertex3f(x, y, z);
		}
	}
	glEnd();
}


void CGLRenderer::DrawGround(CDC* pDC, double h, double dim1, double dim2)
{
	glColor3f(0, 0.8, 0.6);
	Point3 upper_layer[4] = { {-dim1 / 2, 0, dim2 / 2}, {dim1 / 2, 0, dim2 / 2}, {dim1 / 2, 0, -dim2 / 2}, {-dim1 / 2, 0, -dim2 / 2} };
	Point3 lower_layer[4] = { {-dim1 / 2, -h, dim2 / 2}, {dim1 / 2, -h, dim2 / 2}, {dim1 / 2, -h, -dim2 / 2}, {-dim1 / 2, -h, -dim2 / 2} };
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(upper_layer[0].x, upper_layer[0].y, upper_layer[0].z);
	glNormal3f(0, 1, 0);
	glVertex3f(upper_layer[1].x, upper_layer[1].y, upper_layer[1].z);
	glNormal3f(0, 1, 0);
	glVertex3f(upper_layer[2].x, upper_layer[2].y, upper_layer[2].z);
	glNormal3f(0, 1, 0);
	glVertex3f(upper_layer[3].x, upper_layer[3].y, upper_layer[3].z);

	glNormal3f(0, -1, 0);
	glVertex3f(lower_layer[0].x, lower_layer[0].y, lower_layer[0].z);
	glNormal3f(0, -1, 0);
	glVertex3f(lower_layer[3].x, lower_layer[3].y, lower_layer[3].z);
	glNormal3f(0, -1, 0);
	glVertex3f(lower_layer[2].x, lower_layer[2].y, lower_layer[2].z);
	glNormal3f(0, -1, 0);
	glVertex3f(lower_layer[1].x, lower_layer[1].y, lower_layer[1].z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glNormal3f(0, 0, 1);
	glVertex3f(upper_layer[0].x, upper_layer[0].y, upper_layer[0].z);
	glNormal3f(0, 0, 1);
	glVertex3f(lower_layer[0].x, lower_layer[0].y, lower_layer[0].z);

	glNormal3f(1, 0, 0);
	glVertex3f(upper_layer[1].x, upper_layer[1].y, upper_layer[1].z);
	glNormal3f(1, 0, 0);
	glVertex3f(lower_layer[1].x, lower_layer[1].y, lower_layer[1].z);

	glNormal3f(0, 0, -1);
	glVertex3f(upper_layer[2].x, upper_layer[2].y, upper_layer[2].z);
	glNormal3f(0, 0, -1);
	glVertex3f(lower_layer[2].x, lower_layer[2].y, lower_layer[2].z);

	glNormal3f(0, 0, -1);
	glVertex3f(upper_layer[3].x, upper_layer[3].y, upper_layer[3].z);
	glNormal3f(0, 0, -1);
	glVertex3f(lower_layer[3].x, lower_layer[3].y, lower_layer[3].z);

	glNormal3f(-1, 0, 0);
	glVertex3f(upper_layer[0].x, upper_layer[0].y, upper_layer[0].z);
	glNormal3f(-1, 0, 0);
	glVertex3f(lower_layer[0].x, lower_layer[0].y, lower_layer[0].z);
	glEnd();
}

void CGLRenderer::DrawAxes()
{
	if (!grid) return;

	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(200, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 200, 0);

	glColor3f(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 200);
	glEnd();
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	double rad_pitch = DEG_TO_RAD(pitch);
	double rad_yaw = DEG_TO_RAD(yaw);

	double eyex = zoom * r_cam * cos(rad_pitch) * sin(rad_yaw);
	double eyey = zoom * r_cam * sin(rad_pitch);
	double eyez = zoom * r_cam * cos(rad_pitch) * cos(rad_yaw);

	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0);

	float ambient[] = { 0.3,0.3,0.3,1 };
	float diffuse[] = { 0.8,0.8,0.8,1 };
	float specular[] = { 1.0,1.0,1.0,1 };

	// sunce
	float position[] = { 1.0, 1.0, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHT0);


	//gluLookAt(50, 0, 0, 0, 0, 0, 0, 1, 0);
	DrawAxes();
	DrawGround(pDC, 5, 100, 50);
	DrawCone(pDC, 10, 20, 1);

	glTranslatef(0, 20, 0);
	glPushMatrix();
	glRotatef(-60, 0, 0, 1);
	DrawCylinder(pDC, 7, 20, 10, 0, 0.5, 0.5);
	glTranslatef(0, 20 + 7, 0);
	DrawSphere(pDC, 7, 2, 2, 0.5, 0, 0);
	glPopMatrix();

	glRotatef(angle_x, 1, 0, 0);
	glRotatef(angle_z, 0, 0, 1);
	DrawCylinder(pDC, 7, 20, 10, 1, 0.0, 0.5);
	glTranslatef(0, 20 + 7, 0);
	DrawSphere(pDC, 7, 2, 2, 0.5, 0, 0);
	glTranslatef(0, -20 - 7, 0);

	glRotatef(-angle_z, 0, 0, 1);
	glRotatef(-angle_x, 1, 0, 0);
	glRotatef(60, 0, 0, 1);
	DrawCylinder(pDC, 7, 20, 1, 1, 0.5, 0.0);
	glTranslatef(0, 20 + 7, 0);
	DrawSphere(pDC, 7, 10, 10, 0.5, 0, 0);

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}