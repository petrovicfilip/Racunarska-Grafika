#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
#include "DImage.h"
#define PI 3.141592
#define DTR(a) (((a) * PI ) / 180.)

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
	glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float ambient[] = { 0.5, 0.5, 0.5, 1 };
	float diffuse[] = { 1, 1, 1, 1 };
	float specular[] = { 1, 1, 1, 1 };
	float position[] = { 0, 1, 1, 0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	tex = LoadTexture("Env.jpg");
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
	gluPerspective(55, (double)w / (double)h, 0.1, 2000);
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

void DrawAxes()
{
	// b r g
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 0, 0);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 50, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 50);

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}
//Napisati funkciju UINT LoadTexture(char* fileName), koja učitava teksturu sa datim
//imenom(fileName) i vraća ID kreirane teksture.Korišćenjem ove funkcije u okviru
//PrepareScene() učitati teksturu Env.jpg.Teksturu obrisati u DistoryScene()

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

//Napisati funkciju void PolarToCartesian(double R, double phi, double theta, double&
//	x, double& y, double& z), koja konvertuje polarne(R, phi theta) koordinate u Dekartove
//	(x, y, z).
void PolarToCartesian(double r, double phi, double theta, double* x, double* y, double* z)
{
	theta = DTR(theta);
	phi = DTR(phi);
	*x = r * cos(theta) * sin(phi);
	*y = r * sin(theta);
	*z = r * cos(theta) * cos(phi);
}

//Napisati funkciju void DrawSphere(float R, int n, int m), koja iscrtava sferu
//poluprečnika R, aproksimiranu sa n koraka duž ekvatora i m koraka duž meridijana.Na sferu
//se primenjuje tekstura Env.jpg(sl.2), centrirana je u odnosu na kameru i na nju ne deluju
//izvori svetlosti.

void CGLRenderer::DrawSphere(float r, int n, int m)
{
	double alphaStep = 180. / (double)m;
	double betaStep = 360. /(double)n;

	double alpha = -90.; // theta
	double beta = 0.; // phi

	// unutra povecam beta i racunam 2 koord i povecam afla na kraj
	// spolja samo beta
	for (int i = 0; i < n; i++)
	{
		glFrontFace(GL_CW);
		glBegin(GL_QUAD_STRIP);
		glColor3f(1, 1, 1);
		alpha = -90.;
		for (int j = 0; j <= m; j++)
		{
			double x, y, z;
			PolarToCartesian(r, beta, alpha, &x, &y, &z);
			glNormal3f(x / r, y / r, z / r);
			double u1 = 0.5 + x / r * 0.3;// beta / 360.0;
			double v1 = 0.5 + z / r * 0.3;//(alpha + 90.0) / 180.0;
			glTexCoord2f(u1, v1);
			glVertex3f(x, y, z);

			double beta2 = beta + betaStep;

			PolarToCartesian(r, beta2, alpha, &x, &y, &z);
			glNormal3f(x / r, y / r, z / r);
			double u2 =  0.5 + x / r * 0.3;
			double v2 =  0.5 + z / r * 0.3;
			glTexCoord2f(u2, v2);
			glVertex3f(x, y, z);

			alpha += alphaStep;
		}
		beta += betaStep;
		glEnd();
		glFrontFace(GL_CCW);
	}
}

//Napisati funkciju void DrawSphFlower(float R, int n, int m, float factor, unsigned
//	char R1, unsigned char G1, unsigned char B1, unsigned char R2, unsigned char G2,
//	unsigned char B2), koja iscrtava sferu koja se otvara u cvet.Sfera je poluprečnika R sa n
//	koraka duž ekvatora i m koraka duž meridijana.Na sferu se primenjuje materijal, čija je
//	difuziona komponenta na južnom polu(R1, G1, B1) boje u RGB modelu sa vrednostima od 0
//	do 255 po kanalu, a boja na severnom polu(R2, G2, B2).Ambijentalna komponenta
//		materijala je 20 % boje na južnom polu, a boja refleksije je bela.Na sferu se ne primenjuju
//		teksture.Pritiskom na taster A sfera se „rasklapa“ po meridijanima(sl.4).Parametar factor
//		množi ugao rotacije, tako da je omogućeno korišćenje iste funkcije za različite „brzine“
//		rasklapanja sfere.Za određivanje ose oko koje se vrši rotacija pojedinačnih segmenata sfere
//		(sl.3) koristiti funkciju CalcRotAxis().
void SetMaterial(unsigned char R1, unsigned char G1, unsigned char B1)
{
	double r = (double)R1 / 255.;
	double g = (double)G1 / 255.;
	double b = (double)B1 / 255.;

	float ambient[4] = {0.2 * r, 0.2 * g, 0.2 * b, 1};
	float diffuse[4] = {r, g, b, 1};
	float specular[4] = {1, 1, 1, 1};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
}

void CalcRotAxis(double x1, double y1, double z1, double x2, double y2, double z2, double& x3, double& y3, double& z3)
{
	x3 = x2 - x1;
	y3 = y2 - y1;
	z3 = z2 - z1;

	double d = sqrt(x3 * x3 + y3 * y3 + z3 * z3);

	x3 /= d;
	y3 /= d;
	z3 /= d;
}
void CGLRenderer::DrawSphFlower(float r, int n, int m, float factor, unsigned char R1, unsigned char G1, unsigned char B1, unsigned char R2, unsigned char G2, unsigned char B2)
{
	double alphaStep = 180. / (double)m;
	double betaStep = 360. / (double)n;

	double alpha = -90.; // theta
	double beta = 0.; // phi

	// unutra povecam beta i racunam 2 koord i povecam afla na kraj
	// spolja samo beta
	for (int i = 0; i < n; i++)
	{
		alpha = -90.;


		double x1, y1, z1;
		PolarToCartesian(r, beta, alpha, &x1, &y1, &z1);

		double beta2 = beta + betaStep;

		double x2, y2, z2;
		PolarToCartesian(r, beta2, alpha, &x2, &y2, &z2);
		glPushMatrix();

		for (int j = 0; j < m; j++)
		{
			if (alpha >= -90 && alpha <= 0)
				SetMaterial(R1, G1, B1);
			else
				SetMaterial(R2, G2, B2);

			double osax, osay, osaz;
			CalcRotAxis(x1, y1, z1, x2, y2, z2, osax, osay, osaz);

			double alpha2 = alpha + alphaStep;

			double x3, y3, z3;
			PolarToCartesian(r, beta, alpha2, &x3, &y3, &z3);

			double x4, y4, z4;
			PolarToCartesian(r, beta2, alpha2, &x4, &y4, &z4);
			//glPushMatrix();

			glTranslated(x1, y1, z1);
			glRotated(rotcvet, osax, osay, osaz);
			glTranslated(-x1, -y1, -z1);

			glBegin(GL_QUADS);

			glNormal3f(x1 / r, y1 / r, z1 / r);
			glVertex3f(x1, y1, z1);
			glNormal3f(x2 / r, y2 / r, z2 / r);
			glVertex3f(x2, y2, z2);
			glNormal3f(x4 / r, y4 / r, z4 / r);
			glVertex3f(x4, y4, z4);
			glNormal3f(x3 / r, y3 / r, z3 / r);
			glVertex3f(x3, y3, z3);

			glEnd();

			x1 = x3;
			y1 = y3;
			z1 = z3;

			x2 = x4;
			y2 = y4;
			z2 = z4;

		//	glPopMatrix();

			alpha += alphaStep;
		}
		glPopMatrix();
		beta += betaStep;
	}
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	// crtanje..


	float ambient[] = { 0.5, 0.5, 0.5, 1 };
	float diffuse[] = { 1, 1, 1, 1 };
	float specular[] = { 1, 1, 1, 1 };
	float position[] = { 0, 1, 1, 0 };

	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glPushMatrix();
	{
		glRotated(-rotx, 1, 0, 0);
		glRotated(-roty, 0, 1, 0);
		glDisable(GL_DEPTH_TEST);
		glColor3f(1., 1., 1.);
		glDisable(GL_LIGHTING);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		DrawSphere(50, 20, 20);
		glDisable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}
	glPopMatrix();

	glTranslated(0, 0, -dist);
	glRotated(-rotx, 1, 0, 0);
	glRotated(-roty, 0, 1, 0);
	DrawAxes();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	//DrawSphere(2, 20, 20);
	glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	DrawSphFlower(3, 30, 30, 1, 0, 255, 0, 255, 0, 0);

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
