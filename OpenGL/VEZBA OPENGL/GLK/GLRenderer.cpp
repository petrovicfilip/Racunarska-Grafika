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

void CGLRenderer::SetLightingBase()
{
	float globalAmbient[4] = {0.3, 0.3, 0.3, 1.};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, TRUE); // !!!!!!!!!!! lajt model dve strane

	glEnable(GL_COLOR_MATERIAL); // !!!!!!!!! kolor materijal

	glEnable(GL_LIGHTING);
}

UINT CGLRenderer::LoadTexture(char* file)
{
	UINT texID;
	DImage img;
    // 4 4 1 1
	img.Load(CString(file));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // !!!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // !!!

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // !!!!!!!!!!!!!!!!!!!!!!!

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void DrawAxes()
{
	glBegin(GL_LINES);
	glColor3d(1.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(200.0, 0.0, 0.0);

	glColor3d(0.0, 1.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 200.0, 0.0);

	glColor3d(0.0, 0.0, 1.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 200.0);
	glEnd();
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//SetLightingBase();
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

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity(); // !!!!!!!!!!!

	double eye_x = r * zoom * cos(DEG_TO_RAD(beta)) * sin(DEG_TO_RAD(alpha));
	double eye_y = r * zoom * sin(DEG_TO_RAD(beta));
	double eye_z = r * zoom * cos(DEG_TO_RAD(beta)) * cos(DEG_TO_RAD(alpha));

	gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0, 1, 0);

	// crtanje
	DrawAxes();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0, 0);
	glVertex3d(-0.5, -0.5, -0.5);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3d(0.5, -0.5, -0.5);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3d(0.5, 0.5, -0.5);
	glEnd();


	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
