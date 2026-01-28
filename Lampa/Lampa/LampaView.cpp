
// LampaView.cpp : implementation of the CLampaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lampa.h"
#endif

#include "LampaDoc.h"
#include "LampaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLampaView

IMPLEMENT_DYNCREATE(CLampaView, CView)

BEGIN_MESSAGE_MAP(CLampaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLampaView construction/destruction

CLampaView::CLampaView() noexcept
{
	pozadina.Load(CString("G:\\Grafika ispit\\GDI2022\\pozadina.jpg"));
	arm1.Load(CString("G:\\Grafika ispit\\GDI2022\\arm1.png"));
	arm2.Load(CString("G:\\Grafika ispit\\GDI2022\\arm2.png"));
	base.Load(CString("G:\\Grafika ispit\\GDI2022\\base.png"));
	head.Load(CString("G:\\Grafika ispit\\GDI2022\\head.png"));
	arm1_shadow.Load(CString("G:\\Grafika ispit\\GDI2022\\arm1_shadow.png"));
	arm2_shadow.Load(CString("G:\\Grafika ispit\\GDI2022\\arm2_shadow.png"));
	base_shadow.Load(CString("G:\\Grafika ispit\\GDI2022\\base_shadow.png"));
	head_shadow.Load(CString("G:\\Grafika ispit\\GDI2022\\head_shadow.png"));
}

CLampaView::~CLampaView()
{
}

BOOL CLampaView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
void CLampaView::DrawTransparent(CDC* pdc, DImage* img) {

	CBitmap bmpMask;

	bmpMask.CreateBitmap(img->Width(), img->Height(), 1, 1, NULL);

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pdc);
	CDC* destDC = new CDC();
	destDC->CreateCompatibleDC(pdc);

	CBitmap* oldSrcBmp = srcDC->SelectObject(img->GetBitmap());
	CBitmap* oldDestBmp = destDC->SelectObject(&bmpMask);

	COLORREF clrTopLeft = srcDC->GetPixel(0, 0);
	COLORREF oldBkColor = srcDC->SetBkColor(clrTopLeft);
	destDC->BitBlt(0, 0, img->Width(), img->Height(), srcDC, 0, 0, SRCCOPY);

	COLORREF oldSrcTextColor = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->BitBlt(0, 0, img->Width(), img->Height(), destDC, 0, 0, SRCAND);


	srcDC->SetTextColor(oldSrcTextColor);
	srcDC->SetBkColor(oldBkColor);

	srcDC->SelectObject(oldSrcBmp);

	pdc->BitBlt(0, 0, img->Width(), img->Height(), destDC, 0, 0, SRCAND);

	destDC->SelectObject(img->GetBitmap());
	pdc->BitBlt(0, 0, img->Width(), img->Height(), destDC, 0, 0, SRCPAINT);
	destDC->SelectObject(oldDestBmp);

	srcDC->DeleteDC();
	delete srcDC;

	destDC->DeleteDC();
	delete destDC;
}
void CLampaView::DrawImgTransparent(CDC* pdc, DImage* pImage)
{
	DrawTransparent(pdc, pImage);
}
void CLampaView::DrawLampBase(CDC* pdc, bool bIsShadow)
{
	Translate(pdc, 700, 450, false);
	if (bIsShadow)
	{
		Translate(pdc, 50, 120, false); // zato sto sam ga prebacio u ovu fju ovde ovaj prvi translate, pa da ga nastelujem
		Scale(pdc, 1, 0.25, false);
		Rotate(pdc, -90, false);
		DrawTransparent(pdc, &base_shadow);
	}
	else
		DrawTransparent(pdc, &base);
	Translate(pdc, 167, 39, false);
	DrawLampArm1(pdc, bIsShadow); // namerno ostavljeno ovde, da se zbuni citalac
}
void CLampaView::DrawLampArm1(CDC* pdc, bool bIsShadow)
{
	Rotate(pdc, -45 + arm1rot, false);
	Translate(pdc, -58, -61, false);
	if (bIsShadow)
	{
		DrawTransparent(pdc, &arm1_shadow);
	}
	else
		DrawTransparent(pdc, &arm1);

}

void CLampaView::DrawLampArm2(CDC* pdc, bool bIsShadow)
{
	Translate(pdc, 309, 61, false);
	Rotate(pdc, -90 + arm2rot, false);
	Translate(pdc, -36, -40, false);
	if (bIsShadow)
		DrawTransparent(pdc, &arm2_shadow);
	else
		DrawTransparent(pdc, &arm2);
}
void CLampaView::DrawLampHead(CDC* pdc, bool bIsShadow)
{
	Translate(pdc, 272, 40, false);
	Rotate(pdc, 180 + headrot, false);
	Translate(pdc, -178, -100, false);
	if (bIsShadow)
		DrawTransparent(pdc, &head_shadow);
	else
		DrawTransparent(pdc, &head);
}

void CLampaView::DrawLamp(CDC* pdc, bool bIsShadow)
{
	DrawLampBase(pdc, bIsShadow);
	DrawLampArm2(pdc, bIsShadow);
	DrawLampHead(pdc, bIsShadow);
}
void CLampaView::Translate(CDC* pdc, float dx, float dy, bool rightMultiply)
{
	XFORM mat;
	mat.eDx = dx;
	mat.eDy = dy;
	mat.eM11 = 1.0;
	mat.eM12 = 0.0;
	mat.eM21 = 0.0;
	mat.eM22 = 1.0;
	pdc->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLampaView::Scale(CDC* pdc, float sX, float sY, bool rightMultiply)
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

void CLampaView::Rotate(CDC* pdc, float angle, bool rightMultiply)
{
	XFORM mat;
	angle = angle * 3.14159 / 180.0;
	mat.eDx = 0.0;
	mat.eDy = 0.0;
	mat.eM11 = cos(angle);
	mat.eM12 = sin(angle);
	mat.eM21 = -sin(angle);
	mat.eM22 = cos(angle);
	pdc->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CLampaView::DrawBackground(CDC* pdc)
{
	CRect client;
	CRect rect(0, 0, pozadina.Width(), pozadina.Height());
	GetClientRect(client);
	pozadina.Draw(pdc, rect, client);
}
// pdc->GetDeviceCaps(
void CLampaView::OnDraw(CDC* pdc)
{
	CLampaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(client);
	CDC memdc;
	CBitmap bmp;
	XFORM oldtrans; // !!!
	bmp.CreateCompatibleBitmap(pdc, client.Width(), client.Height());
	memdc.CreateCompatibleDC(pdc);
	memdc.SetGraphicsMode(GM_ADVANCED);
	memdc.GetWorldTransform(&oldtrans);
	CBitmap* oldbmp = memdc.SelectObject(&bmp);
	memdc.FillSolidRect(client, RGB(255, 255, 255));
	DrawBackground(&memdc);
	XFORM old;
	memdc.GetWorldTransform(&old);
	DrawLamp(&memdc, true);
	memdc.SetWorldTransform(&old);
	DrawLamp(&memdc, false);

	memdc.SetWorldTransform(&oldtrans);
	pdc->BitBlt(0, 0, client.Width(), client.Height(), &memdc, 0, 0, SRCCOPY);
}


// CLampaView printing

BOOL CLampaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLampaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLampaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLampaView diagnostics

#ifdef _DEBUG
void CLampaView::AssertValid() const
{
	CView::AssertValid();
}

void CLampaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLampaDoc* CLampaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLampaDoc)));
	return (CLampaDoc*)m_pDocument;
}
#endif //_DEBUG


// CLampaView message handlers

void CLampaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case '1':
		{
			arm1rot += 10;
			Invalidate(FALSE);
			break;
		}
		case '2':
		{
			arm1rot -= 10;
			Invalidate(FALSE);
			break;
		}
		case '3':
		{
			arm2rot += 10;
			Invalidate(FALSE);
			break;
		}
		case '4':
		{
			arm2rot -= 10;
			Invalidate(FALSE);
			break;
		}
		case '5':
		{
			headrot += 10;
			Invalidate(FALSE);
			break;
		}
		case '6':
		{
			headrot -= 10;
			Invalidate(FALSE);
			break;
		}
		default:
			break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
