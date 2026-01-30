
// Kolokvijum 2023View.cpp : implementation of the CKolokvijum2023View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kolokvijum 2023.h"
#endif

#include "Kolokvijum 2023Doc.h"
#include "Kolokvijum 2023View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592
#define DEG_TO_RAD(a) ((a) * PI / 180.0)


// CKolokvijum2023View

IMPLEMENT_DYNCREATE(CKolokvijum2023View, CView)

BEGIN_MESSAGE_MAP(CKolokvijum2023View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKolokvijum2023View construction/destruction

CKolokvijum2023View::CKolokvijum2023View() noexcept
{
	/*glava.png,
		nadkolenica.png, nadlaktica.png, podkolenica.png, podlaktica.png, saka.png,
		stopalo.png, telo.png, pozadina.jpg*/
	glava = new DImage();
	nadkolenica = new DImage();
	nadlaktica = new DImage();
	podkolenica = new DImage();
	podlaktica = new DImage();
	saka = new DImage();
	stopalo = new DImage();
	telo = new DImage();
	pozadina = new DImage();

	glava->Load(CString("glava.png"));
	nadkolenica->Load(CString("nadkolenica.png"));
	nadlaktica->Load(CString("nadlaktica.png"));
	podkolenica->Load(CString("podkolenica.png"));
	podlaktica->Load(CString("podlaktica.png"));
	saka->Load(CString("saka.png"));
	stopalo->Load(CString("stopalo.png"));
	telo->Load(CString("telo.png"));
	pozadina->Load(CString("pozadina.jpg"));
}

CKolokvijum2023View::~CKolokvijum2023View()
{
	delete glava;
	delete nadkolenica;
	delete nadlaktica;
	delete podkolenica;
	delete podlaktica;
	delete saka;
	delete stopalo;
	delete telo;
	delete pozadina;
}

BOOL CKolokvijum2023View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKolokvijum2023View drawing

void CKolokvijum2023View::DrawBackground(CDC* pdc)
{
	pozadina->Draw(pdc, CRect(0, 0, pozadina->Width(), pozadina->Height()), CRect(0, 0, pozadina->Width(), pozadina->Height()));
}

void CKolokvijum2023View::DrawImgTransparent(CDC* pdc, DImage* pImage)
{
	unsigned char* bits = pImage->GetDIBBits();
	pImage->DrawTransparent(pdc, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()), RGB(bits[0], bits[1], bits[2]));
	// nikako delete bits!!!!!!
}

void CKolokvijum2023View::Translate(CDC* pdc, float dX, float dY, bool rightMultiply = false)
{
	XFORM mat;
	mat.eDx = dX;
	mat.eDy = dY;
	mat.eM11 = 1.0;
	mat.eM12 = 0.0;
	mat.eM21 = 0.0;
	mat.eM22 = 1.0;
	pdc->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}
void CKolokvijum2023View::Rotate(CDC* pdc, float angle, bool rightMultiply = false)
{
	XFORM mat;
	angle = DEG_TO_RAD(angle);
	mat.eDx = 0.0;
	mat.eDy = 0.0;
	mat.eM11 = cos(angle);
	mat.eM12 = sin(angle);
	mat.eM21 = -sin(angle);
	mat.eM22 = cos(angle);
	pdc->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CKolokvijum2023View::Scale(CDC* pdc, float sX, float sY, bool rightMultiply = false)
{
	XFORM mat;
	mat.eDx = 0.0;
	mat.eDy = 0.0;
	mat.eM11 = sX;
	mat.eM12 = 0.0;
	mat.eM21 = 0.0;
	mat.eM22 = sY;
	pdc->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolokvijum2023View::DrawHalf(CDC* pdc)
{
	//Napisati funkciju void DrawHalf(CDC * pDC), koja iscrtava levu polovinu robota, bez
	//	glave.Nadlaktica se rotira oko tačke(35, 35), podlaktica oko tačke(30, 33), a šaka oko tačke
	//	(25, 3).Ugao za šaku se menja na pritisak tastera A i S(minimalna vrednost –10,
	//		maksmalna 30), ugao za podlakticu na pritisak tastera D i F(minimalna vrednost –10,
	//			maksmalna 80), a ugao za nadlakticu na pritisak tastera G i H(minimalna vrednost –10,
	//				maksmalna 90).Koordinate za nadovezivanje date su u tabeli 1. Ograničiti minimalne i
	//	maksimalne uglove rotacije
	XFORM zatelo;
	pdc->GetWorldTransform(&zatelo);
	DrawImgTransparent(pdc, telo);
//	Telo(25, 65) Nadlaktica(35, 35)
	Translate(pdc, 25, 65);
	Rotate(pdc, nadlakticarot);
	Translate(pdc, -35, -35);
	DrawImgTransparent(pdc, nadlaktica);
	//Nadlaktica(22, 167) Podlaktica(30, 33)
	Translate(pdc, 22, 167, false);
	Rotate(pdc, podlakticarot);
	Translate(pdc, -30, -33);
	DrawImgTransparent(pdc, podlaktica);
	//Podlaktica (30,140) Saka (25,3) 
	Translate(pdc, 30, 140, false);
	Rotate(pdc, sakarot);
	Translate(pdc, -25, -3);
	DrawImgTransparent(pdc, saka);
	pdc->SetWorldTransform(&zatelo);
	//Telo (61,262) Nadkolenica (29,20)
	Translate(pdc, 61 - 29, 262 - 20, false);
	DrawImgTransparent(pdc, nadkolenica);
	//Nadkolenica (30,184) Podkolenica (25,37) 
	Translate(pdc, 30 - 25, 184 - 37, false);
	DrawImgTransparent(pdc, podkolenica);
	//Podkolenica(25, 248) Stopalo(20, 16)
	Translate(pdc, 25 - 20, 248- 16, false);
	DrawImgTransparent(pdc, stopalo);
	pdc->SetWorldTransform(&zatelo);
}

void  CKolokvijum2023View::DrawHead(CDC* pdc)
{
	DrawImgTransparent(pdc, glava);
}

void CKolokvijum2023View::DrawRobot(CDC* pdc)
{
	XFORM old;
	Translate(pdc, pozadina->Width() / 2, pozadina->Height() / 2);
	Translate(pdc, -telo->Width(), -telo->Height());
	Rotate(pdc, robotrot);
	Scale(pdc, factor, factor);

	pdc->GetWorldTransform(&old);
	Translate(pdc, telo->Width(), 0);
	Translate(pdc, -glava->Width() / 2, -glava->Height() * 0.8);
	DrawImgTransparent(pdc, glava);
	pdc->SetWorldTransform(&old);

	DrawHalf(pdc);
	Translate(pdc, 2 * telo->Width(), 0);
	Scale(pdc, -1, 1);
	DrawHalf(pdc);
}

void CKolokvijum2023View::OnDraw(CDC* pdc)
{
	CKolokvijum2023Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(client);
	CDC memdc;
	CBitmap bmp;
	XFORM oldtrans;
	memdc.CreateCompatibleDC(pdc);
	bmp.CreateCompatibleBitmap(pdc, client.Width(), client.Height());
	memdc.SelectObject(&bmp);
	memdc.GetWorldTransform(&oldtrans);
	memdc.SetGraphicsMode(GM_ADVANCED);
	memdc.FillSolidRect(client, RGB(255, 255, 255));
	// crtanje
	DrawBackground(&memdc);
	//Translate(&memdc, 100, 100);
	DrawRobot(&memdc);
	//DrawHalf(&memdc);
	memdc.SetWorldTransform(&oldtrans);
	pdc->BitBlt(0, 0, client.Width(), client.Height(), &memdc, 0, 0, SRCCOPY);
	// TODO: add draw code for native data here
}

// CKolokvijum2023View printing

BOOL CKolokvijum2023View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKolokvijum2023View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKolokvijum2023View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKolokvijum2023View diagnostics

#ifdef _DEBUG
void CKolokvijum2023View::AssertValid() const
{
	CView::AssertValid();
}

void CKolokvijum2023View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKolokvijum2023Doc* CKolokvijum2023View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKolokvijum2023Doc)));
	return (CKolokvijum2023Doc*)m_pDocument;
}
#endif //_DEBUG


// CKolokvijum2023View message handlers

void CKolokvijum2023View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case 'A':
		{
			if (sakarot < 30)
				sakarot += 4;
			Invalidate(FALSE);
			break;
		}
		case 'S':
		{
			if (sakarot > -10)
				sakarot -= 4;
			Invalidate(FALSE);
			break;
		}
		case 'D':
		{
			if (podlakticarot < 80)
				podlakticarot += 4;
			Invalidate(FALSE);
			break;
		}
		case 'F':
		{
			if (podlakticarot > -10)
				podlakticarot -= 4;
			Invalidate(FALSE);
			break;
		}
		case 'G':
		{
			if (nadlakticarot < 90)
				nadlakticarot += 4;
			Invalidate(FALSE);
			break;
		}
		case 'H':
		{
			if (nadlakticarot > -10)
				nadlakticarot -= 4;
			Invalidate(FALSE);
			break;
		}
		case '1':
		{
			factor *= 0.9;
			robotrot += 5;
			Invalidate(FALSE);
			break;
		}
		case '2':
		{
			factor *= 1.1;
			robotrot -= 5;
			Invalidate(FALSE);
			break;
		}
		default:
			break;

	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
