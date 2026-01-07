
// klk2017najjaceView.cpp : implementation of the Cklk2017najjaceView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "klk2017najjace.h"
#endif

#include "klk2017najjaceDoc.h"
#include "klk2017najjaceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define PI 3.14159
#define DEG_TO_RAD(a) ((a) * (PI) / (180.0))


// Cklk2017najjaceView

IMPLEMENT_DYNCREATE(Cklk2017najjaceView, CView)

BEGIN_MESSAGE_MAP(Cklk2017najjaceView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Cklk2017najjaceView construction/destruction

Cklk2017najjaceView::Cklk2017najjaceView() noexcept
{
	leg1.Load(CString("C:\\Grafika\\2017\\Leg1.png"));
	leg2.Load(CString("C:\\Grafika\\2017\\Leg2.png"));
	leg3.Load(CString("C:\\Grafika\\2017\\Leg3.png"));
	back2.Load(CString("C:\\Grafika\\2017\\Back2.jpg"));
	body.Load(CString("C:\\Grafika\\2017\\Body.png"));
}

Cklk2017najjaceView::~Cklk2017najjaceView()
{
}

BOOL Cklk2017najjaceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cklk2017najjaceView drawing

void Cklk2017najjaceView::DrawTransparent(CDC* pdc, DImage* img) {

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

void Cklk2017najjaceView::DrawScene(CDC* pdc, CRect rect)
{
	CRect rc(0, 0, back2.Width(), back2.Height());
	back2.Draw(pdc, rc, rect);
	Translate(pdc, 100 + stepX, 100, false);
	Scale(pdc, factor, factor, false);
	DrawTransparent(pdc, &body);
	/*Gornji levi ugao okvirnog pravougaonika tela je inicijalno na poziciji
	(100, 100).Sve noge su supuštene u odnosu na telo za 168 piksela.Zadnje noge su
		pomerene u odnosu na telo duž X - ose za 72, a prednje za 270 piksela, respektivno.*/
	//Translate(pdc, 52, 168, false); // zadnje noge
	// zadnje
	DrawLeg(pdc, angBack, 52, 168);
	DrawLeg(pdc, -angBack, 52, 168);
	// prednje
	DrawLeg(pdc, angFront, 270, 168);
	DrawLeg(pdc, -angFront, 270, 168);

}
void Cklk2017najjaceView::DrawLeg(CDC* pdc, double alpha, double dx, double dy)
{
	XFORM trans;
	pdc->GetWorldTransform(&trans);
	Translate(pdc, dx + 28, dy + 28, false);
	Rotate(pdc, alpha, false);
	Translate(pdc, -28, -28, false);
	DrawTransparent(pdc, &leg1);
	Translate(pdc, 0, 140, false); // 140 pomeraj na ipsilon za drugi deo
	Translate(pdc, 29, 29, false);
	Rotate(pdc, -alpha, false);
	Translate(pdc, -29, -29, false);
	DrawTransparent(pdc, &leg2);
	Translate(pdc, -33, 100, false); // za treci deo
	DrawTransparent(pdc, &leg3);
	pdc->SetWorldTransform(&trans);
}
void Cklk2017najjaceView::OnDraw(CDC* pdc)
{
	Cklk2017najjaceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(&client);
	//DrawScene(pdc, client);

	CDC memdc;
	memdc.CreateCompatibleDC(pdc);
	CBitmap bmp;
	XFORM oldT;
	bmp.CreateCompatibleBitmap(pdc, client.Width(), client.Height());
	memdc.GetWorldTransform(&oldT);
	memdc.SetGraphicsMode(GM_ADVANCED);

	memdc.SelectObject(&bmp); // !
	memdc.FillSolidRect(client, RGB(255, 255, 255)); // !

	DrawScene(&memdc, client);
	int width = client.Width();
	int height = client.Height();

	CString text;
	double a = 100.0 + stepX;
	text.Format(_T("Client width: %d, height: %d, 100 + incX: %f"), width, height, a);

	memdc.SetWorldTransform(&oldT); // !
	memdc.TextOutW(10, 10, text);
	pdc->BitBlt(0, 0, client.Width(), client.Height(), &memdc, 0, 0, SRCCOPY);



}
void Cklk2017najjaceView::Translate(CDC* pdc, float dX, float dY, bool rightMultiply = false)
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
void Cklk2017najjaceView::Rotate(CDC* pdc, float angle, bool rightMultiply = false)
{
	angle = DEG_TO_RAD(angle);
	XFORM mat;
	mat.eDx = 0.0;
	mat.eDy = 0.0;
	mat.eM11 = cos(angle);
	mat.eM12 = sin(angle);
	mat.eM21 = -sin(angle);
	mat.eM22 = cos(angle);
	pdc->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void Cklk2017najjaceView::Scale(CDC* pdc, float sX, float sY, bool rightMultiply = false)
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
// Cklk2017najjaceView printing

BOOL Cklk2017najjaceView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cklk2017najjaceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cklk2017najjaceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cklk2017najjaceView diagnostics

#ifdef _DEBUG
void Cklk2017najjaceView::AssertValid() const
{
	CView::AssertValid();
}

void Cklk2017najjaceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cklk2017najjaceDoc* Cklk2017najjaceView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cklk2017najjaceDoc)));
	return (Cklk2017najjaceDoc*)m_pDocument;
}
#endif //_DEBUG


// Cklk2017najjaceView message handlers


void Cklk2017najjaceView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CRect client;
	GetClientRect(&client);
	switch (nChar)
	{
	case VK_RIGHT:
	{
		if (incBack)
		{
			angBack += 10;
			if (angBack == 20)
				incBack = !incBack;
		}
		else
		{
			angBack -= 10;
			if (angBack == -20)
				incBack = !incBack;
		}
		if (incFront)
		{
			angFront += 10;
			if (angFront == 20)
				incFront = !incFront;
		}
		else
		{
			angFront -= 10;
			if (angFront == -20)
				incFront = !incFront;
		}

		stepX += 20;
		if (stepX + 100 >= client.Width())
			stepX = -100 - body.Width() * factor;
		Invalidate(FALSE);
		break;
	}
	case VK_UP:
		factor = 1.1 * factor;
		Invalidate(FALSE);
		break;
	case VK_DOWN:
		factor = 0.9 * factor;
		Invalidate(FALSE);
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
