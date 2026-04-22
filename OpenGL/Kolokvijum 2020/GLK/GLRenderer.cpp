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
	r = 5;
	alpha = 0.0;
	beta = 0.0;
	zoom = 1.0;
	last_x = 0.0;
	last_y = 0.0;
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
	glClearColor(1, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);

	front = LoadTexture("front.jpg");
	back= LoadTexture("back.jpg");
	left = LoadTexture("left.jpg");
	right = LoadTexture("right.jpg");
	top = LoadTexture("top.jpg");
	bot = LoadTexture("bot.jpg");
	spider = LoadTexture("spider.png");

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DrawAxes()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	
	glColor3f(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(50, 0, 0);

	glColor3f(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 50, 0);

	glColor3f(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 50);

	glColor3f(1, 1, 1);
	
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / h, 0.1, 2000);
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

UINT CGLRenderer::LoadTexture(char* file)
{
	UINT texID;
	DImage img;

	img.Load(CString(file));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

//Napisati funkciju void CGLRenderer::DrawEnvCube(double a), koja iscrtava kocku
//stranice dužine a, kojom se uokvirava scena(centrirana na poziciji kamere).Kocka se
//uvek vidi samo sa unutrašnje strane i na njenim stranicama su „nalepljene“ teksture :
//front.jpg, left.jpg, right.jpg, back.jpg, top.jpg i bot.jpg(vidi sl.2).U DrawScene
//funkciji nacrtati ovu kocku, stranice 100 jedinica.
void CGLRenderer::DrawEnvCube(double a, double tx, double ty, double tz)
{
	double half_a = a / 2.;
	glTranslatef(tx, ty, tz);
	glCullFace(GL_FRONT);

	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);
	// ... 
	glTexCoord2f(1, 1);
	glVertex3d(-half_a, -half_a, half_a);
	glTexCoord2f(0, 1);
	glVertex3d(half_a, -half_a, half_a);
	glTexCoord2f(0, 0);
	glVertex3d(half_a, half_a, half_a);
	glTexCoord2f(1, 0);
	glVertex3d(-half_a, half_a, half_a);

	glEnd();


	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);
	glVertex3d(half_a, -half_a, half_a);
	glTexCoord2f(0, 1);
	glVertex3d(half_a, -half_a, -half_a);
	glTexCoord2f(0, 0);
	glVertex3d(half_a, half_a, -half_a);
	glTexCoord2f(1, 0);
	glVertex3d(half_a, half_a, half_a);

	glEnd();
	

	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);
	glVertex3d(half_a, -half_a, -half_a);
	glTexCoord2f(0, 1);
	glVertex3d(-half_a, -half_a, -half_a);
	glTexCoord2f(0, 0);
	glVertex3d(-half_a, half_a, -half_a);
	glTexCoord2f(1, 0);
	glVertex3d(half_a, half_a, -half_a);

	glEnd();


	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);
	glVertex3d(-half_a, -half_a, -half_a);
	glTexCoord2f(0, 1);
	glVertex3d(-half_a, -half_a, half_a);
	glTexCoord2f(0, 0);
	glVertex3d(-half_a, half_a, half_a);
	glTexCoord2f(1, 0);
	glVertex3d(-half_a, half_a, -half_a);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);
	glVertex3d(-half_a, half_a, half_a);
	glTexCoord2f(0, 1);
	glVertex3d(half_a, half_a, half_a);
	glTexCoord2f(0, 0);
	glVertex3d(half_a, half_a, -half_a);
	glTexCoord2f(1, 0);
	glVertex3d(-half_a, half_a, -half_a);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, bot);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);
	glVertex3d(half_a, -half_a, half_a);
	glTexCoord2f(0, 1);
	glVertex3d(-half_a, -half_a, half_a);
	glTexCoord2f(0, 0);
	glVertex3d(-half_a, -half_a, -half_a);
	glTexCoord2f(1, 0);
	glVertex3d(half_a, -half_a, -half_a);

	glEnd();

	glCullFace(GL_BACK);

	glTranslatef(-tx, -ty, -tz);
		
}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU,
	double texV, double texR)
{
	double alpha = 0.;
	double beta = 0.;
	double alpha_inc = 90.0 / (double)nSeg;
	double beta_inc = 360. / (double)nSeg;
	glColor3f(1, 0, 0);

	for (int i = 0; i <= nSeg; i++)
	{
		alpha = 0.;

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= nSeg; j++)
		{
			double alpha_rad = DEG_TO_RAD(alpha);
			double beta_rad = DEG_TO_RAD(beta);

			glTexCoord2f(texU + texR * cos(beta_rad) * (alpha / 90.), texV + texR * sin(beta_rad) * (alpha / 90.));
			glVertex3d(r * sin(alpha_rad) * cos(beta_rad), r * cos(alpha_rad), r * sin(alpha_rad) * sin(beta_rad));

			beta_rad = DEG_TO_RAD(beta + beta_inc);

			glTexCoord2f(texU + texR * cos(beta_rad) * (alpha / 90.), texV + texR * sin(beta_rad) * (alpha / 90.));
			glVertex3d(r * sin(alpha_rad) * cos(beta_rad), r * cos(alpha_rad), r * sin(alpha_rad) * sin(beta_rad));

			alpha += alpha_inc;

		}
		beta += beta_inc;
		glEnd();
	}

	glFrontFace(GL_CW);
	for (int i = 0; i <= nSeg; i++)
	{
		alpha = 0.;
		glBegin(GL_QUAD_STRIP);

		for (int j = 0; j <= nSeg; j++)
		{
			double alpha_rad = DEG_TO_RAD(alpha);
			double beta_rad = DEG_TO_RAD(beta);


			glTexCoord2f(texU + texR * cos(beta_rad) * (alpha / 90.), texV + texR * sin(beta_rad) * (alpha / 90.));
			glVertex3d(r * sin(alpha_rad) * cos(beta_rad), -r * cos(alpha_rad), r * sin(alpha_rad) * sin(beta_rad));

			beta_rad = DEG_TO_RAD(beta + beta_inc);

			glTexCoord2f(texU + texR * cos(beta_rad) * (alpha / 90.), texV + texR * sin(beta_rad) * (alpha / 90.));
			glVertex3d(r * sin(alpha_rad) * cos(beta_rad), -r * cos(alpha_rad), r * sin(alpha_rad) * sin(beta_rad));
			

	/*		glTexCoord2f(texU + texR * cos(beta_rad) * (alpha / 90.), texV + texR * sin(beta_rad) * (alpha / 90.));
			glVertex3f(r * sin(alpha_rad) * sin(beta_rad), -r * cos(alpha_rad), r * sin(alpha_rad) * cos(beta_rad));

			beta_rad = DEG_TO_RAD(beta + beta_inc);

			glTexCoord2f(texU + texR * cos(beta_rad) * (alpha / 90.), texV + texR * sin(beta_rad) * (alpha / 90.));
			glVertex3f(r * sin(alpha_rad) * sin(beta_rad), -r * cos(alpha_rad), r * sin(alpha_rad) * cos(beta_rad));*/

			alpha += alpha_inc;
		}
		beta += beta_inc;
		glEnd();
	}
	glFrontFace(GL_CCW);
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double
	texU, double texV, double texR)
{
	glColor3f(1, 0, 0);//
//	glDisable(GL_CULL_FACE);

	glBegin(GL_TRIANGLE_FAN);

	glVertex3d(0, 0, 0);

	double alpha = 0.;
	double alpha_inc = 360. / (double)nSeg;
	for (int i = 0; i <= nSeg; i++)
	{
		glVertex3d(r * cos(DEG_TO_RAD(alpha)), 0, r * sin(DEG_TO_RAD(alpha)));

		alpha += alpha_inc;
	}
	glEnd();


	glBegin(GL_TRIANGLE_FAN);

	glTexCoord2f(texU, texV);
	glVertex3d(0, h, 0);

	alpha = 0.;
	for (int i = 0; i <= nSeg; i++)
	{

		glTexCoord2f(texU + texR * cos(DEG_TO_RAD(alpha)), texV + texR * sin(DEG_TO_RAD(alpha)));
		glVertex3d(r * cos(DEG_TO_RAD(alpha)), 0, -r * sin(DEG_TO_RAD(alpha)));

		alpha += alpha_inc;
	}
	glEnd();


}

//Napisati funkciju void CGLRenderer::DrawLegSegment(double r, double h, int nSeg),
//koja iscrtava jedan segment noge pauka, pozivom prethodno definisanih funkcija
//DrawSphere i DrawCone.Poluprečnici sfere i kupe su r, visina kupe h, a broj
//segmenata sfere i kupe su 2 * nSeg i nSeg, respektivno.Ostale parametre funkcija za
//crtanje sfere i kupe uskladiti sa teksturom spider koja je priložena(vidi sl.3)

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	DrawSphere(r, 2 * nSeg, 0.25, 0.25, 0.25);
	glTranslated(0, r, 0);
	DrawCone(r, h, nSeg, 0.75, 0.75, 0.25);
}

void CGLRenderer::DrawLeg()
{
	//glRotated(95. / 2., 0, 0, 1);
	DrawLegSegment(1, 10, 5);
	glTranslated(0, 10, 0);
	glTranslated(0, 1, 0); // za r lopte
	glRotated(85, 0, 0, 1);
	DrawLegSegment(1, 10, 5);
}

//Napisati funkciju void CGLRenderer::DrawSpiderBody(), koja iscrtava telo pauka,
//sastavljenu od tri sfere(DrawSphere) : glave, grudnog dela i stomaka, poluprečnika 2, 3
//i 5, respektivno.Broj segmenata za sve tri sfere postaviti na 10, a teksturne koordinate
//tako da se grudni deo i stomak mapiraju na gornju levu četvrtinu, a glava na gornju
//desnu četvrtinu teksture spider.Glava i grudni deo su „spljošteni“ na 50 %, a stomak
//na 80 % po jednoj od osa

void CGLRenderer::DrawSpiderBody()
{
	// grudni deo
	glPushMatrix();
	glScaled(1, 0.5, 1);
	DrawSphere(3, 20, 0.25, 0.25, 0.25);
	//glPopMatrix();

	// glava
	//glPushMatrix();
	glTranslated(0, 0, 2 + 3);
	glRotated(90, 0, 1, 0);
	DrawSphere(2, 20, 0.75, 0.25, 0.25);

	glPopMatrix();

	// stomace
	glPushMatrix();
	glTranslated(0, 0, -3 - 5);
	glScaled(1, 0.8, 1);
	DrawSphere(5, 20, 0.25, 0.25, 0.25);
	glPopMatrix();
}

//Napisati funkciju void CGLRenderer::DrawSpider(), koja iscrtava celog pauka,
//sastavljenog od tela i 8 nogu.Noge polaze iz sredine grudnog dela.Prvi segmenti su
//zarotirani za 45 u odnosu na grudni deo i 30 jedna u odnosu na drugu

void CGLRenderer::DrawSpider()
{
	DrawSpiderBody();
	glPushMatrix();
	// ... 
	glRotated(-45, 0, 1, 0);

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glRotated(95. / 2., 0, 0, 1);
		//glRotated(180., 0, 0, 1);
		DrawLeg();
		glPopMatrix();
		glRotated(30, 0, 1, 0);
	}

	glPopMatrix();


	glPushMatrix();
	// ... 
	glRotated(135, 0, 1, 0);

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glRotated(95. / 2., 0, 0, 1);
		//glRotated(180., 0, 0, 1);
		DrawLeg();
		glPopMatrix();
		glRotated(30, 0, 1, 0);
	}

	glPopMatrix();

}

void Normalize(float a[3], float b[3], float out[3])
{
	out[0] = a[1] * b[2] - a[2] * b[1];
	out[1] = a[2] * b[0] - a[0] * b[2];
	out[2] = a[0] * b[1] - a[1] * b[0];

	float d = sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);

	out[0] /= d;
	out[1] /= d;
	out[2] /= d;
}
void SetMaterial(float ambient[4], float diffuse[4], float specular[4], float emmision[4], int shininess)
{
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmision);
	//glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	//float ambient[4] = {0.3, 0.3, 0.3, 1};
	//float diffuse[4] = {0.8, 0.8, 0.8, 1};
	//float specular[4] = {1, 1, 1, 1};

	//float position[4] = { 1, 1, 1, 0 };

	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, ambient);
	//glLightfv(GL_LIGHT0, GL_POSITION, position);
	////glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.02);


	//float direction[3] = { 0, 0, -1 };
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.);

	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.);

	//glEnable(GL_LIGHT0);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//glTranslated(0, -45, 0);

	//eye_x = r * zoom * cos(DEG_TO_RAD(beta)) * sin(DEG_TO_RAD(alpha));
	//eye_y = r * zoom * sin(DEG_TO_RAD(beta));
	//eye_z = r * zoom * cos(DEG_TO_RAD(beta)) * cos(DEG_TO_RAD(alpha));

	//gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0, 1, 0);
	// ...
	//glTranslatef(0, -10, 0);
	//glPushMatrix();
	////glTranslatef(0, 10, 0);
	//glRotated(-y_ang, 0, 1, 0);
	//glRotated(-x_ang, 1, 0, 0);
	////glDisable(GL_DEPTH_TEST);
	//DrawEnvCube(100., 0, 0, 0);
	//glEnable(GL_DEPTH_TEST);
	//glPopMatrix();
	//glTranslated(0, 0, -dist);
	//glRotated(-y_ang, 0, 1, 0);
	//glRotated(-x_ang, 1, 0, 0);

	// OKEJ

	//glTranslatef(0, 0, -dist);
	//glRotatef(-x_ang, 1, 0, 0);
	//glRotatef(-y_ang, 0, 1, 0);

	//glTranslatef(0, -10, 0);
	//glPushMatrix();

	//DrawAxes();
	//glEnable(GL_LIGHTING);
	//glBindTexture(GL_TEXTURE_2D, spider);
	//glDisable(GL_CULL_FACE);
	//DrawSpider();

	//glPopMatrix();
	////glTranslatef(0, -10, 0);

	//glTranslatef(0, 45, 0);
	//DrawEnvCube(100., 0, 0, 0);

	// OKEJ
	glTranslatef(0, 0, -dist);
	glRotated(-x_ang, 1, 0, 0);
	glRotated(-y_ang, 0, 1, 0);
	glPushMatrix();

	glTranslated(0, 40, 0);

	DrawEnvCube(100., 0, 0, 0);

	glPopMatrix();
	glTranslated(0, -5, 0);

	DrawAxes();
	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, spider);
	glDisable(GL_CULL_FACE);
	DrawSpider();

	//glPopMatrix();

	glFlush();
	SwapBuffers(pDC->m_hDC);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}