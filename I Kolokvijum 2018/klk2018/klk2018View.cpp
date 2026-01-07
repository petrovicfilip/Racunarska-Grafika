
// klk2018View.cpp : implementation of the Cklk2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "klk2018.h"
#endif

#include "klk2018Doc.h"
#include "klk2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14159265359
#define DEG_TO_RAD(a) ((a) * (PI) / 180.0)

// Cklk2018View

IMPLEMENT_DYNCREATE(Cklk2018View, CView)

BEGIN_MESSAGE_MAP(Cklk2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Cklk2018View construction/destruction

Cklk2018View::Cklk2018View() noexcept
{
	arm1 = new DImage();
	arm2 = new DImage();
	excavator = new DImage();
	background = new DImage();

	arm1->Load(CString("C:\\Grafika\\2018\\arm1.png"));
	arm2->Load(CString("C:\\Grafika\\2018\\arm2.png"));
	excavator->Load(CString("C:\\Grafika\\2018\\bager.png"));
	background->Load(CString("C:\\Grafika\\2018\\pozadina.png"));
	fork = GetEnhMetaFile(CString("C:\\Grafika\\2018\\viljuska.emf")); // !!!!
	GetEnhMetaFileHeader(fork, sizeof(header), &header);
}

Cklk2018View::~Cklk2018View()
{
	delete arm1;
	delete arm2;
	delete excavator;
	delete background;
	DeleteEnhMetaFile(fork);
}

BOOL Cklk2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cklk2018View drawing
void Cklk2018View::DrawTransparent(CDC* pdc, DImage* img) {

	CBitmap bmpMask;

	bmpMask.CreateBitmap(img->Width(), img->Height(), 1, 1, NULL);

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pdc);
	CDC* destDC = new CDC();
	destDC->CreateCompatibleDC(pdc);

	CBitmap* oldSrcBmp = srcDC->SelectObject(img->GetBitmap());
	CBitmap* oldDestBmp = destDC->SelectObject(&bmpMask);

	COLORREF clrTopLeft = srcDC->GetPixel(1, 1);
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

void Cklk2018View::Translate(CDC* pdc, float dX, float dY, bool rightMultiply)
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
void Cklk2018View::Rotate(CDC* pdc, float angle, bool rightMultiply)
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
void Cklk2018View::Scale(CDC* pdc, float sX, float sY, bool rightMultiply)
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
void Cklk2018View::DrawBackground(CDC* pdc)
{
	CRect client;
	GetClientRect(client);
	CRect rcimg(0, 0, background->Width(), background->Height());
	background->Draw(pdc, rcimg, client);
}
void Cklk2018View::DrawImgTransparent(CDC* pdc, DImage* img)
{
	DrawTransparent(pdc, img);
}
void Cklk2018View::DrawBody(CDC* pdc, CRect client)
{
	// 57 182 za ruku otp
	DrawImgTransparent(pdc, excavator);
	Translate(pdc, 0, 180 - 71, false);

}
void Cklk2018View::DrawArm1(CDC* pdc)
{
	Translate(pdc, 58, 61, false);
	Rotate(pdc, arm1_rot - 90, false);
	Translate(pdc, -58, -61, false);
	DrawImgTransparent(pdc, arm1);	
	// 309, 61 manji krug
}

void Cklk2018View::DrawArm2(CDC* pdc)
{
	Translate(pdc, 309, 61, false); // u centar sam, tu sammm, evo me!!!!
	Rotate(pdc, arm2_rot - 90, false);
	Translate(pdc, -36, -40, false);
	DrawImgTransparent(pdc, arm2);
	// 309, 61 manji krug
}

void Cklk2018View::DrawFork(CDC* pdc)
{

	// rot oko 14 20
	CRect frameMM(
		header.rclFrame.left,
		header.rclFrame.top,
		header.rclFrame.right,
		header.rclFrame.bottom
	);
	int widthPx = MulDiv(
		frameMM.Width(),
		pdc->GetDeviceCaps(LOGPIXELSX),
		2540
	);

	int heightPx = MulDiv(
		frameMM.Height(),
		pdc->GetDeviceCaps(LOGPIXELSY),
		2540
	);
	CRect rect(0, 0, widthPx, heightPx);
	Translate(pdc, 272, 40, false); // u centar sam, tu sammm, evo me!!!!
	Rotate(pdc, fork_rot, false);
	Scale(pdc, 2.5, 2.5, false);
	Translate(pdc, -14, -20, false);
	PlayEnhMetaFile(pdc->m_hDC, fork, rect);
}
void Cklk2018View::OnDraw(CDC* pdc)
{
	Cklk2018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	XFORM old;
	GetClientRect(client);
	CDC memdc;
	memdc.CreateCompatibleDC(pdc);
	memdc.GetWorldTransform(&old);
	memdc.SetGraphicsMode(GM_ADVANCED);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pdc, client.Width(), client.Height()); // ovde pdc!!!
	memdc.SelectObject(&bmp);
	memdc.FillSolidRect(client, RGB(255, 255, 255));

	//...

	DrawBackground(&memdc);
	Translate(&memdc, client.Width() - excavator->Width() + x_offset, client.Height() - excavator->Height(), false);
	DrawBody(&memdc, client);
	DrawArm1(&memdc);
	DrawArm2(&memdc);
	DrawFork(&memdc);

	memdc.SetWorldTransform(&old);
	pdc->BitBlt(0, 0, client.Width(), client.Height(), &memdc, 0, 0, SRCCOPY);
}


// Cklk2018View printing

BOOL Cklk2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cklk2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cklk2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cklk2018View diagnostics

#ifdef _DEBUG
void Cklk2018View::AssertValid() const
{
	CView::AssertValid();
}

void Cklk2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cklk2018Doc* Cklk2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cklk2018Doc)));
	return (Cklk2018Doc*)m_pDocument;
}
#endif //_DEBUG


// Cklk2018View message handlers


void Cklk2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
		case '1':
		{
			arm1_rot += 10;
			Invalidate(FALSE);
			break;
		}
		case '2':
		{
			arm1_rot -= 10;
			Invalidate(FALSE);
			break;
		}
		case '3':
		{
			arm2_rot += 10;
			Invalidate(FALSE);
			break;
		}
		case '4':
		{
			arm2_rot -= 10;
			Invalidate(FALSE);
			break;
		}
		case '5':
		{
			fork_rot += 10;
			Invalidate(FALSE);
			break;
		}
		case '6':
		{
			fork_rot -= 10;
			Invalidate(FALSE);
			break;
		}
		case VK_LEFT:
		{
			x_offset -= 18;
			Invalidate(FALSE);
			break;
		}
		case VK_RIGHT:
		{
			x_offset += 18;
			Invalidate(FALSE);
			break;
		}
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
