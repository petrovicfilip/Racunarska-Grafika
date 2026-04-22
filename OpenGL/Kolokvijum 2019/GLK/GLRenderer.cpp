#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
#include "DImage.h"
#define PI 3.141592
#define DTR(a) (((a) * PI) / 180.)

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

void SetLightingBase()
{
	float ambient[4] = {0.3, 0.3, 0.3, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, TRUE); // GL_TRUE MOZDA BOLJE!!!
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, TRUE); // GL_TRUE MOZDA BOLJE!!!

	glEnable(GL_LIGHTING);
}

void SetLocalLighting(int light)
{
	float ambient[4] = { 0.3, 0.3, 0.3, 1 };
	float diffuse[4] = { 0.8, 0.8, 0.8, 1 };
	float specular[4] = { 1, 1, 1, 1 };
	float position[4] = {0., 30., 0., 1}; // zaboravio sam bio KECA !!!!!!!!!!

	glLightfv(light, GL_AMBIENT,ambient);
	glLightfv(light, GL_DIFFUSE, diffuse);
	glLightfv(light, GL_SPECULAR, specular);
	glLightfv(light, GL_POSITION, position);

	glLightf(light, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(light, GL_LINEAR_ATTENUATION, 0.01);
	glLightf(light, GL_QUADRATIC_ATTENUATION, 0.002);

	float direction[3] = { 0, -1, 0 };

	glLightfv(light, GL_SPOT_DIRECTION, direction);
	glLightf(light, GL_SPOT_CUTOFF, 30.);
	glLightf(light, GL_SPOT_EXPONENT, 2.);

	glEnable(light);
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1, 1, 1, 1);
	// depth test cull itd...
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST); // ALOOOOOOOOOOOOOOOOOOOO PAZIII SEEEEEEEEEEEEE ZABRAVIO SI GAAAAAAAAAAAAAAAAAAAAA !!!!!!!!!!!!!!!!!!!!!!
	glEnable(GL_NORMALIZE); // dodati!!!

	top = LoadTexture("top.jpg");
	left = LoadTexture("left.jpg");
	right = LoadTexture("right.jpg");
	back = LoadTexture("back.jpg");
	front = LoadTexture("front.jpg");
	bot = LoadTexture("bot.jpg");
	excavator = LoadTexture("excavator.png");

	SetLightingBase();

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
	gluPerspective(55, (double)w / (double)h, 0.1, 200);
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
	glDisable(GL_TEXTURE_2D);

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
}


//Napisati funkciju UINT CGLRenderer::LoadTexture(char* fileName), koja
//učitava teksturu sa datim imenom(fileName) i vraća ID kreirane teksture.
//Korišćenjem ove funkcije u okviru PrepareScene() učitati teksture : excavator.png,
//front.jpg, left.jpg, right.jpg, back.jpg, top.jpg i bot.jpg

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

//Napisati funkciju void CGLRenderer::DrawPolygon(POINTF* points, POINTF*
//	texCoords, int n), koja iscrtava n - to strani poligon na osnovu niza zadatih temena
//	(points) i njihovih teksturnih koordinata(texCoords).Temena poligona zadatu su
//	svojim X i Y koordinatama.Preporuka je koristiti GL_TRIANGLE_FAN za
//	iscrtavanje poligona.

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	// prvo ide je centar pa onda temena
	//glDisable(GL_TEXTURE_2D);
//	glColor3f(1, 0, 0);

	glBegin(GL_TRIANGLE_FAN);

	glColor3f(1, 1, 1);

	glTexCoord2f(texCoords[0].x, texCoords[0].y);
	glVertex3f(0, 0, 0);

	for (int i = 0; i < n; i++)
	{
		glTexCoord2f(texCoords[i + 1].x, texCoords[i + 1].y);
		glVertex3f(points[i].x, points[i].y, 0);
	}

	glTexCoord2f(texCoords[1].x, texCoords[1].y);
	glVertex3f(points[0].x, points[0].y, 0);
	glEnd();
}

//Napisati funkciju void CGLRenderer::DrawExtrudedPolygon(POINTF* points,
//	POINTF* texCoords, int n, float zh, float r, float g, float b), koja
//	iscrtava n - to stranu prizmu, sa osnovicama definisanim funkcijama DrawPolygon().
//	Parametri points i texCoords koriste se u pozivu funkcije DrawPolygon, za kreiranje
//	osnovica.Parametar zh određuje visinu prizme u pravcu Z - ose, dok su parametri(r,
//		g, b) odgovarajuće komponente boje, koja se primenjuje na bočne stranice poligona.
void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	// 2 * n
	DrawPolygon(points, texCoords, n);
	glTranslatef(0, 0, zh);
	glFrontFace(GL_CW);
	DrawPolygon(points, texCoords, n);
	glFrontFace(GL_CCW);
	glTranslatef(0, 0, -zh);

	glColor3f(r, g, b);
	
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < n; i++)
	{
		glVertex3f(points[i].x, points[i].y, 0);
		glVertex3f(points[i].x, points[i].y, zh);

	}
	glVertex3f(points[0].x, points[0].y, 0);
	glVertex3f(points[0].x, points[0].y, zh);

	glEnd();
}

//Napisati funkciju void CGLRenderer::DrawBase(), koja iscrtava gusenice bagera.
//Prostorne i teksturne koordinate temena treba odrediti na osnovu sl. 3, pri čemu se
//može smatrati da je jedan podeok na slici 0.5 prostornih jedinica.Visina prizme je 5.
//Bočne stranice prizme su crne boje.

POINTF* TexCoord(POINTF* points, int n, double texS, double texT)
{
	POINTF* tex = new POINTF[n + 1];

	//texT = 1. - texT;
	tex[0].x = texS;
	tex[0].y = 1 - texT;

	for (int i = 1; i <= n; i++)
	{
		tex[i].x = texS + points[i - 1].x / 16.;
		tex[i].y = texT + points[i - 1].y / 16.;
		tex[i].y = 1 - tex[i].y;
	}

	return tex;
}

void CGLRenderer::DrawBase()
{
	glPushMatrix();

	glTranslated(0, 2, 0);

	POINTF points[8] = { {-8., -1}, {-7., -2}, {7., -2}, {8., -1}, {8, 1},  {7, 2}, {-7, 2}, {-8, 1} };
	POINTF* tex = TexCoord(points, 8, 8. / 16., 2. / 16.);

	glBindTexture(GL_TEXTURE_2D, excavator);
	DrawExtrudedPolygon(points, tex, 8, 5, 0, 0, 0);

	glPopMatrix();

}

//Napisati funkciju void CGLRenderer::DrawBody(), koja iscrtava kabinu bagera.
//Prostorne i teksturne koordinate temena treba odrediti na osnovu sl. 3. Bočne stranice
//prizme su narandžaste boje(0.96, 0.5, 0.12).Visina prizme je 4.

void CGLRenderer::DrawBody()
{
	glTranslated(0, 2, 0);
	glTranslated(0, 4, 0);

	glTranslated(0, 0, 2);
	glRotated(bodyrot, 0, 1, 0);
	glPushMatrix();
	glTranslated(0, 0, -2);

	POINTF points[8] = { {-4., 4.}, {0., 4}, {4., 0}, {4., -4}, {-4, -4} };
	POINTF* tex = TexCoord(points, 5, 12. / 16., 12. / 16.);

	glBindTexture(GL_TEXTURE_2D, excavator);
	DrawExtrudedPolygon(points, tex, 5, 4, 0.96, 0.5, 0.12);

	glPopMatrix();
}

//. Napisati funkciju void CGLRenderer::DrawArm(double zh), koja iscrtava pokretni
//deo ruke bagera.Parametar zh je širina ruke.Prostorne i teksturne koordinate temena
//treba odrediti na osnovu sl. 3. Bočne stranice prizme su narandžaste boje(0.96, 0.5,
//	0.12).Visina prizme je zh.

void CGLRenderer::DrawArm(double zh)
{
	glPushMatrix();
		
	//glTranslated();

	POINTF points[6] = { {-8., 1.}, {-7., 2}, {8., 1}, {8., -1}, {-7, -2}, {-8, -1} };
	POINTF* tex = TexCoord(points, 6, 0.5, 6 / 16.);

	glBindTexture(GL_TEXTURE_2D, excavator);
	DrawExtrudedPolygon(points, tex, 6, zh, 0.96, 0.5, 0.12);

	glPopMatrix();
}


void CGLRenderer::DrawFork()
{
	glPushMatrix();

	POINTF points[6] = { {-4., 2.}, {-3., 3}, {3., 3}, {4., 2}, {4, -3}, {-4, -3} };
	POINTF* tex = TexCoord(points, 6, 4/16., 13 / 16.);

	glTranslated(-2, -1, 0);
	glRotated(rotfork, 0, 0, 1);
	glTranslated(2, 1, 0);

	glBindTexture(GL_TEXTURE_2D, excavator);
	DrawExtrudedPolygon(points, tex, 6, 1, 0.7, 0.7, 0.7);

	glPopMatrix();
}

void CGLRenderer::DrawExcavator()
{
	DrawBase();
	DrawBody();

	glTranslated(0, 2, 0);
	glTranslated(0, 4, 0);
	glTranslated(0, 0, 2);

	glTranslated(0, -3, 0);
	glTranslated(3, 0, 0);
	glTranslated(6, 0, 0);

	glTranslated(-6, 0, 0);
	glRotated(rotarm1, 0, 0, 1);
	glTranslated(6, 0, 0);

	DrawArm(1);

	glTranslated(7, 0, 0);
	glRotated(rotarm2, 0, 0, 1);
	glTranslated(6, 0, 0);

	DrawArm(1.5);

	glTranslated(7, 0, 0);

	DrawFork();
}

void CGLRenderer::DrawEnvCube(double a)
{
	double halfa = a / 2.;


	int t[4] = { front, right, back, left };
	glPushMatrix();
	//
	for (int i = 0; i < 4; i++)
	{
		glBindTexture(GL_TEXTURE_2D, t[i]);

		glBegin(GL_QUADS);

		glTexCoord2f(0, 1);
		glVertex3f(-halfa, -halfa, halfa);
		glTexCoord2f(1, 1);
		glVertex3f(halfa, -halfa, halfa);
		glTexCoord2f(1, 0);
		glVertex3f(halfa, halfa, halfa);
		glTexCoord2f(0, 0);
		glVertex3f(-halfa, halfa, halfa);

		glEnd();

		glRotated(90, 0, 1, 0);
	}
	glPopMatrix();

	glRotated(-90, 1, 0, 0);

	glBindTexture(GL_TEXTURE_2D, top);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-halfa, -halfa, halfa);
	glTexCoord2f(1, 1);
	glVertex3f(halfa, -halfa, halfa);
	glTexCoord2f(1, 0);
	glVertex3f(halfa, halfa, halfa);
	glTexCoord2f(0, 0);
	glVertex3f(-halfa, halfa, halfa);

	glEnd();

	glRotated(180, 1, 0, 0);

	glBindTexture(GL_TEXTURE_2D, bot);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-halfa, -halfa, halfa);
	glTexCoord2f(1, 1);
	glVertex3f(halfa, -halfa, halfa);
	glTexCoord2f(1, 0);
	glVertex3f(halfa, halfa, halfa);
	glTexCoord2f(0, 0);
	glVertex3f(-halfa, halfa, halfa);

	glEnd();

}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	double eyex = r * zoom * cos(DTR(beta)) * sin(DTR(alpha));
	double eyey = r * zoom * sin(DTR(beta));
	double eyez = r * zoom * cos(DTR(beta)) * cos(DTR(alpha));

	gluLookAt(eyex, eyey, eyez, 0, 5, 0, 0, 1, 0);

	glPushMatrix();
	DrawAxes();

	POINTF points[5] = { {-4., 4}, {0, 4}, {4, 0}, {4, -4}, {-4, -4} };
	//DrawExtrudedPolygon(points, points, 5, 5, 1, 0, 0);
	SetLocalLighting(GL_LIGHT0);
	DrawExcavator();
	glPopMatrix();

	glTranslated(0, 50, 0);

	DrawEnvCube(100.);


	glFlush();
	SwapBuffers(pDC->m_hDC);
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
