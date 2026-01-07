
// TransformersGDIView.cpp : implementation of the CTransformersGDIView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TransformersGDI.h"
#endif

#include "TransformersGDIDoc.h"
#include "TransformersGDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14159265359
#define DEG_TO_RAD(a) ((a) * (PI) / 180.0)

// CTransformersGDIView

IMPLEMENT_DYNCREATE(CTransformersGDIView, CView)

BEGIN_MESSAGE_MAP(CTransformersGDIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	//ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CTransformersGDIView construction/destruction

CTransformersGDIView::CTransformersGDIView() noexcept
{
	// oko 450KB, namerno ga guram na stek jer mi se moze
	arm1.Load(CString("C:\\Grafika\\2019\\arm1.png"));
	arm2.Load(CString("C:\\Grafika\\2019\\arm2.png"));
	leg1.Load(CString("C:\\Grafika\\2019\\leg1.png"));
	leg2.Load(CString("C:\\Grafika\\2019\\leg2.png"));
	body.Load(CString("C:\\Grafika\\2019\\body1.png"));
	background.Load(CString("C:\\Grafika\\2019\\background.jpg"));
	/*CString msg;
	msg.Format(L"Sirina: %d  Visina: %d", arm1.Width(), arm1.Height());
	AfxMessageBox(msg);*/
}

CTransformersGDIView::~CTransformersGDIView()
{

}

BOOL CTransformersGDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTransformersGDIView drawing

void CTransformersGDIView::DrawTransparent(CDC* pdc, DImage* img) {

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

void CTransformersGDIView::DrawBackground(CDC* pdc, CRect rc) 
{
	CRect rect(0, 0, background.Width(), background.Height());
	background.Draw(pdc, rect, rc);
}

void Translate(CDC* pdc, float dX, float dY, bool rightMultiply = false)
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

void Rotate(CDC* pdc, float angle, bool rightMultiply = false)
{
	XFORM mat;
	double ang = angle * PI / 180.0;
	mat.eM11 = cos(ang);
	mat.eM12 = sin(ang);
	mat.eM21 = -sin(ang);
	mat.eM22 = cos(ang);
	mat.eDx = 0.0;// ovo nisam bio inicijalizovao i nije mi radelo ahahahahahahhahsHAJDLJKSQAHBDLhjwsdarfpiu ;WEGHFLKWE	gbfoluil
	mat.eDy = 0.0;// pa na sta je default ako nije nula mamu vam u picku jebem
	pdc->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CTransformersGDIView::DrawArm1(CDC* pdc)
{
	// 34 31
	// 210 102
	Translate(pdc, 34, 31, false);
	Rotate(pdc, arm1rot, false);
	Translate(pdc, -34, -31, false);
	DrawTransparent(pdc, &arm1);
}

void CTransformersGDIView::DrawArm2(CDC* pdc)
{
	// 23 61
	// 23 61 poklopiti sa 210 102
	Translate(pdc, 23, 61, false);
	Rotate(pdc, arm2rot + 180 , false);
	Translate(pdc, -23, -61, false);
	DrawTransparent(pdc, &arm2);
}

void CTransformersGDIView::DrawLeg2(CDC* pdc)
{
	Translate(pdc, 35, 60, false);
	Rotate(pdc, leg2rot, false);
	Translate(pdc, -35, -60, false);
	DrawTransparent(pdc, &leg2);
}

void CTransformersGDIView::DrawLeg1(CDC* pdc)
{
	Translate(pdc, 30, 125, false);
	Rotate(pdc, leg1rot, false);
	Translate(pdc, -30, -125, false);
	DrawTransparent(pdc, &leg1);
}

void CTransformersGDIView::DrawBody(CDC* pdc)
{
	Translate(pdc, 26, 133, false);
	Rotate(pdc, bodyrot, false);
	Translate(pdc, -26, -133, false);
	DrawTransparent(pdc, &body);
}

void CTransformersGDIView::OnDraw(CDC* pdc)
{
	CTransformersGDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC memdc;
	XFORM oldXf;
	memdc.CreateCompatibleDC(pdc);
	memdc.SetGraphicsMode(GM_ADVANCED);
	memdc.GetWorldTransform(&oldXf);
	//pdc->SetGraphicsMode(GM_ADVANCED);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pdc, rect.Width(), rect.Height());

	CBitmap* oldbmp = memdc.SelectObject(&bmp);

	memdc.FillSolidRect(rect, RGB(255, 255, 255));

	// crtanje
	// ...
	DrawBackground(&memdc, rect);
	Translate(&memdc, 300, 400, false);
	XFORM prenoge2;
	memdc.GetWorldTransform(&prenoge2);
	DrawLeg2(&memdc);
	memdc.SetWorldTransform(&prenoge2);
	Translate(&memdc, 5, 60 - 125, false);
	DrawLeg1(&memdc);
	// 237 125
	// 26 133
	Translate(&memdc, 212, -6, false);
	XFORM za_telo;
	memdc.GetWorldTransform(&prenoge2);
	DrawBody(&memdc);
	// 212 85 tacka na telu za arm1 poklapanje 34 31
	Translate(&memdc, 212 - 34, 85 - 31, false);
	XFORM za_ruku1;
	memdc.GetWorldTransform(&za_ruku1);
	//DrawArm1(&memdc);
	// prvo crtam ruku 2, pa onda 1
	// 210 102 i 23 61
	Translate(&memdc, 34, 31);
	Rotate(&memdc, arm1rot);
	Translate(&memdc, -34, -31);
	Translate(&memdc, 210 - 23, 102 - 61, false);
	DrawArm2(&memdc);
	memdc.SetWorldTransform(&za_ruku1);
	DrawArm1(&memdc);

	memdc.SetWorldTransform(&oldXf);

	pdc->BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
}

void CTransformersGDIView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch (nChar) 
    {
        case VK_LEFT:
            leg2rot += 5;
            leg1rot -= 5;
            bodyrot += 1;
            arm1rot -= 10;
            arm2rot -= 5;
            Invalidate(FALSE);  // OVDE
            break;

        case VK_RIGHT:
			leg2rot -= 5;
			leg1rot += 5;
			bodyrot -= 1;
			arm1rot += 10;
			arm2rot += 5;
			Invalidate(FALSE);  // OVDE
			break;

        default:
            CView::OnKeyDown(nChar, nRepCnt, nFlags);
            break;
    }
}


//BOOL CTransformersGDIView::OnEraseBkgnd(CDC* pDC)
//{
//	//return TRUE;
//}

// CTransformersGDIView printing

BOOL CTransformersGDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTransformersGDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTransformersGDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTransformersGDIView diagnostics

#ifdef _DEBUG
void CTransformersGDIView::AssertValid() const
{
	CView::AssertValid();
}

void CTransformersGDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTransformersGDIDoc* CTransformersGDIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTransformersGDIDoc)));
	return (CTransformersGDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CTransformersGDIView message handlers
