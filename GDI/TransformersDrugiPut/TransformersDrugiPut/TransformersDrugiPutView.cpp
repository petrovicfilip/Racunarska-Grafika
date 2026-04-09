
// TransformersDrugiPutView.cpp : implementation of the CTransformersDrugiPutView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TransformersDrugiPut.h"
#endif

#include "TransformersDrugiPutDoc.h"
#include "TransformersDrugiPutView.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592
#define DEG_TO_RAD(a) ((a) * PI / 180.0)


// CTransformersDrugiPutView

IMPLEMENT_DYNCREATE(CTransformersDrugiPutView, CView)

BEGIN_MESSAGE_MAP(CTransformersDrugiPutView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CTransformersDrugiPutView construction/destruction

CTransformersDrugiPutView::CTransformersDrugiPutView() noexcept
{
	arm1 = new DImage();
	arm2 = new DImage();
	leg1 = new DImage();
	leg2 = new DImage();
	body = new DImage();
	background = new DImage();
	
	arm1->Load(CString("arm1.png"));
	arm2->Load(CString("arm2.png"));
	leg1->Load(CString("leg1.png"));
	leg2->Load(CString("leg2.png"));
	body->Load(CString("body1.png"));
	background->Load(CString("background.jpg"));

}

CTransformersDrugiPutView::~CTransformersDrugiPutView()
{
	delete arm1;
	delete arm2;
	delete leg1;
	delete leg2;
	delete body;
	delete background;
}
BOOL CTransformersDrugiPutView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTransformersDrugiPutView drawing
void CTransformersDrugiPutView::DrawBackground(CDC* pdc, CRect rc)
{
	CRect bkrect(0, 0, background->Width(), background->Height());
	background->Draw(pdc, bkrect, rc);
}

void CTransformersDrugiPutView::DrawImgTransparent(CDC* pdc, DImage* pImage)
{
	unsigned char* bits = pImage->GetDIBBits();
	pImage->DrawTransparent(pdc, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()), RGB(bits[0], bits[1], bits[2]));
}

void CTransformersDrugiPutView::Translate(CDC* pdc, float dX, float dY, bool rightMultiply)
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
void CTransformersDrugiPutView::Rotate(CDC* pdc, float angle, bool rightMultiply)
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
void CTransformersDrugiPutView::DrawArm1(CDC* pdc)
{
	// oko 34 31
	// posle drugi deo oko 210 102
	Translate(pdc, 34, 31, false);
	Rotate(pdc, arm1rot, false);
	Translate(pdc, -34, -31, false);
	DrawImgTransparent(pdc, arm1);

}

void CTransformersDrugiPutView::DrawArm2(CDC* pdc)
{
	// oko 23 61
	//Translate(pdc, 23, 61, false);
	Rotate(pdc, arm2rot + 180, false);
	Translate(pdc, -23, -61, false);
	DrawImgTransparent(pdc, arm2);

}
void CTransformersDrugiPutView::DrawLeg1(CDC* pdc)
{
	// 30 125
	Translate(pdc, 30, 125, false);
	Rotate(pdc, leg1rot, false);
	Translate(pdc, -30, -125, false);
	DrawImgTransparent(pdc, leg1);
}

void CTransformersDrugiPutView::DrawLeg2(CDC* pdc)
{
	// 35 60
	Translate(pdc, 35, 60, false);
	Rotate(pdc, leg2rot, false);
	Translate(pdc, -35, -60, false);
	DrawImgTransparent(pdc, leg2);

}

void CTransformersDrugiPutView::DrawBody1(CDC* pdc)
{
	// 26 133
	// 210 84 je druga tacka oko koje ide ruka1 prva tacka
	Translate(pdc, 26, 133, false);
	Rotate(pdc, bodyrot, false);
	Translate(pdc, -26, -133, false);
	DrawImgTransparent(pdc, body);

}

void CTransformersDrugiPutView::DrawTransformer(CDC* memdc)
{
	Translate(memdc, 300, 500, false);

	DrawLeg2(memdc);
	Translate(memdc, 35 - 30, 60 - 125, false);
	DrawLeg1(memdc);

	Translate(memdc, 237 - 26, 125 - 131, false);
	DrawBody1(memdc);
	Translate(memdc, 210 - 34, 84 - 31, false);
	//sad se pozicioniram za ruku1
	XFORM zaarm1;
	memdc->GetWorldTransform(&zaarm1);
	//DrawArm1(&memdc);
	Translate(memdc, 34, 31, false);
	Rotate(memdc, arm1rot, false);
	Translate(memdc, -34, -31, false);
	Translate(memdc, 210, 102, false);
	// sad sam u centar za ruku 2
	//Translate(&memdc, -23, -61, false);
	DrawArm2(memdc);
	memdc->SetWorldTransform(&zaarm1);
	DrawArm1(memdc);
}

void CTransformersDrugiPutView::OnDraw(CDC* pdc)
{
	CTransformersDrugiPutDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(client);
	CDC memdc;
	CBitmap bmp;
	XFORM oldtrans;
	memdc.CreateCompatibleDC(pdc);
	memdc.GetWorldTransform(&oldtrans); // ovo na kraj ideeee
	bmp.CreateCompatibleBitmap(pdc, client.Width(), client.Height());
	memdc.SetGraphicsMode(GM_ADVANCED);
	CBitmap* oldBmp = memdc.SelectObject(&bmp);
	//memdc.FillSolidRect(client, RGB(255, 255, 255));
	// CRTANJE
	DrawBackground(&memdc, client);
	DrawTransformer(&memdc);

	memdc.SetWorldTransform(&oldtrans);
	pdc->BitBlt(0, 0, client.Width(), client.Height(), &memdc, 0, 0, SRCCOPY);
	memdc.SelectObject(oldBmp);
}

// CTransformersDrugiPutView printing

BOOL CTransformersDrugiPutView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTransformersDrugiPutView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTransformersDrugiPutView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTransformersDrugiPutView diagnostics

#ifdef _DEBUG
void CTransformersDrugiPutView::AssertValid() const
{
	CView::AssertValid();
}

void CTransformersDrugiPutView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTransformersDrugiPutDoc* CTransformersDrugiPutView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTransformersDrugiPutDoc)));
	return (CTransformersDrugiPutDoc*)m_pDocument;
}
#endif //_DEBUG


// CTransformersDrugiPutView message handlers

void CTransformersDrugiPutView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case 'Q':
	{
		arm1rot += 2;
		leg1rot += 2;
		Invalidate(FALSE);
		break;
	}
	case 'A':
	{

		arm1rot -= 2;
		leg1rot -= 2;
		Invalidate(FALSE);
		break;
	}
	case 'T':
	{
		arm2rot += 2;
		Invalidate(FALSE);
		break;
	}
	case 'G':
	{
		arm2rot -= 2;
		Invalidate(FALSE);
		break;
	}
	case 'R':
	{
		leg2rot += 2;
		Invalidate(FALSE);
		break;
	}
	case 'F':
	{
		leg2rot -= 2;
		Invalidate(FALSE);
		break;
	}
	case 'W':
	{
		bodyrot += 2;
		Invalidate(FALSE);
		break;
	}
	case 'S':
	{
		bodyrot -= 2;
		Invalidate(FALSE);
		break;
	}
	case VK_LEFT:
	{
		if (arm1rot < 0)
			arm1rot += 10;
		if (arm2rot < 0)
			arm2rot += 10;
		if (leg1rot < 0)
			leg1rot += 15;
		if (leg2rot > 0)
			leg2rot -= 10;
		if (bodyrot > 0)
			bodyrot -= 5;
		Invalidate(FALSE);
		break;
	}
	case VK_RIGHT:
	{
		if (arm1rot >= -270)
			arm1rot -= 10;
		if (arm2rot >= -260)
			arm2rot -= 10;
		if (leg1rot >= -205)
			leg1rot -= 15;
		if (leg2rot <= 90)
			leg2rot += 10;
		if (bodyrot <= 20)
			bodyrot += 5;
		Invalidate(FALSE);
		break;
	}
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
