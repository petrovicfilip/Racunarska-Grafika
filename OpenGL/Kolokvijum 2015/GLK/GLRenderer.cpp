#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
#include "DImage.h"
#define PI 3.141592
#define DEG_TO_RAD(a) (((a) * PI) / 180.0)

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
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	e[0] = LoadTexture("TSC0.jpg");
	e[1] = LoadTexture("TSC1.jpg");
	e[2] = LoadTexture("TSC2.jpg");
	e[3] = LoadTexture("TSC3.jpg");
	e[4] = LoadTexture("TSC4.jpg");
	e[5] = LoadTexture("TSC5.jpg");

	m[0] = LoadTexture("M0.jpg");
	m[1] = LoadTexture("M1.jpg");
	m[2] = LoadTexture("M2.jpg");
	m[3] = LoadTexture("M3.jpg");
	m[4] = LoadTexture("M4.jpg");
	m[5] = LoadTexture("M5.jpg");

	s[0] = LoadTexture("S0.jpg");
	s[1] = LoadTexture("S1.jpg");
	s[2] = LoadTexture("S2.jpg");
	s[3] = LoadTexture("S3.jpg");
	s[4] = LoadTexture("S4.jpg");
	s[5] = LoadTexture("S5.jpg");
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40., (double)w / (double)h, 0.1, 200.);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(6, (const GLuint*)e);
	glDeleteTextures(6, (const GLuint*)m);
	glDeleteTextures(6, (const GLuint*)s);

	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

UINT CGLRenderer::LoadTexture(char* file)
{
	UINT texID;
	DImage img;

	img.Load(CString(file));
//	img.Flip();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawPatch(double r, int n)
{
	double inc = 2. / double(n);
	//glColor3f(1, 0, 0);
	for (double y = -1; y < 1; y += inc)
	{
		glFrontFace(GL_CW);
		glBegin(GL_TRIANGLE_STRIP);
		for (double x = -1; x <= 1; x += inc)
		{
			double phi = atan(x);
			double theta = atan(y * cos(phi));

			glTexCoord2d((x + 1.) / 2., -(y + 1.) / 2.);

			// UBICU SE AHAHHAHAHAHAH SMEJANJEEEEEEEEEEEEEE
			double x3 = r * cos(theta) * sin(phi);
			double y3 = r * sin(theta);
			double z3 = r * cos(theta) * cos(phi);

			glNormal3f(x3 / r, y3 / r, z3 / r);
			glVertex3f(x3, y3, z3);

			double y_inc = y + inc;

			theta = atan(y_inc * cos(phi));

			glTexCoord2d((x + 1.) / 2., -(y_inc + 1.) / 2.);
			
			x3 = r * cos(theta) * sin(phi);
			y3 = r * sin(theta);
			z3 = r * cos(theta) * cos(phi);			
			
			glNormal3f(x3 / r, y3 / r, z3 / r);
			glVertex3f(x3, y3, z3);

		}
		glEnd();
		glFrontFace(GL_CCW);
	}
}

void CGLRenderer::DrawEarth(double r, int tes)
{
	glPushMatrix();
	//glRotated(180, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, e[0]);
	DrawPatch(r, tes);
	//glRotated(180, 0, 0, 1);

	glRotated(90, 0, 1, 0);

	//	glRotated(-90, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, e[1]);
	DrawPatch(r, tes);
	//glRotated(90, 0, 0, 1);

	glRotated(90, 0, 1, 0);

	//glRotated(-90, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, e[2]);
	DrawPatch(r, tes);
	//glRotated(90, 0, 0, 1);

	glRotated(90, 0, 1, 0);

	//glRotated(-90, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, e[3]);
	DrawPatch(r, tes);
	//glRotated(90, 0, 0, 1);

	glPopMatrix();

	glPushMatrix();

	glRotated(-90, 1, 0, 0);

	//glRotated(180, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, e[4]);
	DrawPatch(r, tes);
	//glRotated(-180, 0, 0, 1);

	glPopMatrix();

	glPushMatrix();

	glRotated(90, 1, 0, 0);

	glBindTexture(GL_TEXTURE_2D, e[5]);
	DrawPatch(r, tes);

	glPopMatrix();
}

void CGLRenderer::DrawBody(double r, int tes, int* texture)
{
	glPushMatrix();
	//glRotated(180, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	DrawPatch(r, tes);
	//glRotated(180, 0, 0, 1);

	glRotated(90, 0, 1, 0);

	//	glRotated(-90, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	DrawPatch(r, tes);
	//glRotated(90, 0, 0, 1);

	glRotated(90, 0, 1, 0);

	//glRotated(-90, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	DrawPatch(r, tes);
	//glRotated(90, 0, 0, 1);

	glRotated(90, 0, 1, 0);

	//glRotated(-90, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	DrawPatch(r, tes);
	//glRotated(90, 0, 0, 1);

	glPopMatrix();

	glPushMatrix();

	glRotated(-90, 1, 0, 0);

	//glRotated(180, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	DrawPatch(r, tes);
	//glRotated(-180, 0, 0, 1);

	glPopMatrix();

	glPushMatrix();

	glRotated(90, 1, 0, 0);

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	DrawPatch(r, tes);

	glPopMatrix();
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -5);
	glDisable(GL_TEXTURE_2D);
	// ... 
	//glBegin(GL_TRIANGLES);

	//glColor3f(1, 0, 0);
	//glVertex3f(-.5, -.5, -.5);
	//glColor3f(1, 0, 0);
	//glVertex3f(.5, -.5, -.5);
	//glColor3f(1, 0, 0);
	//glVertex3f(.5, .5, -.5);

	//glEnd();
	double ralpha = DEG_TO_RAD(alpha);
	double rbeta = DEG_TO_RAD(beta);

	double eyex = r * cos(rbeta) * sin(ralpha);
	double eyey = r * sin(rbeta);
	double eyez = r * cos(rbeta) * cos(ralpha);
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0);

	glEnable(GL_TEXTURE_2D);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	DrawBody(50, 10, s);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//glPushMatrix();

	DrawEarth(2, 10);

	glTranslated(0, 0, 50);
	glRotated(angle, 0, 1, 0);
	DrawBody(1, 10, m);

	//glPopMatrix();



	glFlush();
	SwapBuffers(pDC->m_hDC);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
