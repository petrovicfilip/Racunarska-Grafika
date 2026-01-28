
// PacmanView.cpp : implementation of the CPacmanView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Pacman.h"
#endif

#include "PacmanDoc.h"
#include "PacmanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592
#define DEG_TO_RAD(a) (((a) * PI) / 180.0)


// CPacmanView

IMPLEMENT_DYNCREATE(CPacmanView, CView)

BEGIN_MESSAGE_MAP(CPacmanView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CPacmanView construction/destruction

CPacmanView::CPacmanView() noexcept
{
	background = new DImage();
	background->Load(CString("G:\\Grafika ispit\\I kolokvijum 2015 - resursi\\blue.png"));
}

CPacmanView::~CPacmanView()
{
	delete background;
}

BOOL CPacmanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
bool InCircle(double x, double y, double cx, double cy, double r)
{
	double dx = x - cx;
	double dy = y - cy;
	return dx * dx + dy * dy <= r * r;
}
bool Collision(double cx1, double cy1, double r1,
	double cx2, double cy2, double r2)
{
	double dx = cx1 - cx2;
	double dy = cy1 - cy2;
	double sum = r1 + r2;
	return dx * dx + dy * dy <= sum * sum;
}
// CPacmanView drawing
void CPacmanView::Translate(CDC* pdc, double x, double y, bool rightmul = false)
{
	XFORM mat;
	mat.eDx = x;
	mat.eDy = y;
	mat.eM11 = 1.0;
	mat.eM12 = 0.0;
	mat.eM21 = 0.0;
	mat.eM22 = 1.0;
	pdc->ModifyWorldTransform(&mat, rightmul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CPacmanView::DrawPacman(CDC* pdc, CRect rect, float angle)
{
	double top = rect.top;
	double bottom = rect.bottom;
	double left = rect.left;
	double right = rect.right;
	if (!open_mouth)
		angle = 0.0;
	CBrush brush(RGB(255, 255, 0));
	CBrush* old = pdc->SelectObject(&brush);
	double centerx = (left + right) / 2;
	double centery = (top + bottom) / 2;
	double r = (top + bottom) / 2 - top;
	// NE PITUJ!!!
	if(last_dir == VK_RIGHT)
	{
		double angrad = DEG_TO_RAD(angle);
		double x1 = r * cos(angrad) + centerx;
		double y1 = r * sin(angrad) + centery;
		double y2 = -r * sin(angrad) + centery;
		Pie(pdc->m_hDC, rect.left, rect.top, rect.right, rect.bottom, x1, y2, x1, y1);
		double xeye = r * 0.5 * cos(DEG_TO_RAD(angle + 20)) + centerx;
		double yeye = -r * sin(DEG_TO_RAD(angle + 20)) + centery;
		CBrush eyebrush(RGB(0, 0, 0));
		CBrush* yellow = pdc->SelectObject(&eyebrush);
		Ellipse(pdc->m_hDC, xeye, yeye, xeye + 10, yeye + 10);
	}
	else if(last_dir == VK_LEFT)
	{
		double angrad = DEG_TO_RAD(angle);
		double x1 = r * cos(DEG_TO_RAD(180.0 - angle)) + centerx;
		double y1 = r * sin(angrad) + centery;
		double y2 = -r * sin(angrad) + centery;
		Pie(pdc->m_hDC, rect.left, rect.top, rect.right, rect.bottom, x1, y1, x1, y2);
		double xeye = r * 0.5 * cos(DEG_TO_RAD(180.0 - angle - 20)) + centerx;
		double yeye = -r * sin(DEG_TO_RAD(180.0 - angle - 20)) + centery;
		CBrush eyebrush(RGB(0, 0, 0));
		CBrush* yellow = pdc->SelectObject(&eyebrush);
		Ellipse(pdc->m_hDC, xeye, yeye, xeye + 10, yeye + 10);
	}
	else if(last_dir == VK_UP)
	{
		double angrad = DEG_TO_RAD(angle - 90.0);
		double x1 = r * cos(angrad) + centerx;
		double y1 = r * sin(angrad) + centery;
		double x2 = -r * cos(angrad) + centerx;
		Pie(pdc->m_hDC, rect.left, rect.top, rect.right, rect.bottom, x2, y1, x1, y1);
		double xeye = r * 0.5 * cos(DEG_TO_RAD(90 - angle - 20)) + centerx;
		double yeye = -r * 0.5 * sin(DEG_TO_RAD(90 - angle - 20)) + centery;
		Ellipse(pdc->m_hDC, xeye, yeye, xeye + 10, yeye + 10);
		CBrush eyebrush(RGB(0, 0, 0));
		CBrush* yellow = pdc->SelectObject(&eyebrush);
		Ellipse(pdc->m_hDC, xeye, yeye, xeye + 10, yeye + 10);
	}
	else if (last_dir == VK_DOWN)
	{
		double angrad = DEG_TO_RAD(angle - 90.0);
		double x1 = r * cos(angrad) + centerx;
		double y1 = -r * sin(angrad) + centery;
		double x2 = -r * cos(angrad) + centerx;
		Pie(pdc->m_hDC, rect.left, rect.top, rect.right, rect.bottom, x1, y1, x2, y1);
		double xeye = r * 0.5 * cos(DEG_TO_RAD(270 + angle + 20)) + centerx;
		double yeye = -r * 0.5 * sin(DEG_TO_RAD(270 + angle + 20)) + centery;
		Ellipse(pdc->m_hDC, xeye, yeye, xeye + 10, yeye + 10);
		CBrush eyebrush(RGB(0, 0, 0));
		CBrush* yellow = pdc->SelectObject(&eyebrush);
		Ellipse(pdc->m_hDC, xeye, yeye, xeye + 10, yeye + 10);
	}

	pdc->SelectObject(old);
}
void DrawEye(CDC* pdc, double x, double y, double r)
{
	CRect rect(x, y, x + 2 * r, y + 2 * r);
	CBrush wbrush(RGB(255, 255, 255));
	CBrush* oldbrush = pdc->SelectObject(&wbrush);
	pdc->Ellipse(rect);
	CBrush bbrush(RGB(0, 0, 255));
	pdc->SelectObject(&bbrush);
	rect.OffsetRect(r / 4, 0);
	rect.DeflateRect(r / 2, r / 2);
	pdc->Ellipse(rect);
	pdc->SelectObject(oldbrush);
}
void CPacmanView::DrawGhost(CDC* pdc, CRect rect) 
{
	CBrush brush(RGB(255, 0, 0));
	//CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CBrush* old = pdc->SelectObject(&brush);
	//Ellipse(pdc->m_hDC, rect.left, rect.top, rect.right, rect.bottom);
	//CPen* oldpen = pdc->SelectObject(&pen);
	pdc->BeginPath();
	pdc->MoveTo(CPoint(rect.left, rect.bottom));
	pdc->LineTo(CPoint(rect.left, (rect.bottom + rect.top) / 2));
	CRect rect2(rect.left, rect.top, rect.right, (rect.bottom + rect.top) / 2);
	SetArcDirection(pdc->m_hDC, AD_CLOCKWISE);
	pdc->ArcTo(rect.left, rect.top, rect.right, rect.bottom, rect.left, (rect.bottom + rect.top) / 2, rect.right, (rect.bottom + rect.top) / 2);
	pdc->LineTo(CPoint(rect.right, rect.bottom));
	pdc->MoveTo(CPoint(rect.left, rect.bottom));
	double top = (rect.bottom + (rect.bottom + ((rect.top + rect.bottom) / 2)) / 2) / 2;
	double bottom = rect.bottom + (96 / 8);
	double left = rect.left; // menja se za 25 kroz iteracije
	double right = (rect.left + ((rect.left + rect.right) / 2)) / 2; // isto za 25
	for (int i = 0; i < 4; i++)
	{
		CRect rect(left, top, right, bottom);
		CPoint druga((left + right) / 2, top);
		pdc->ArcTo(rect, CPoint(left, (bottom + top) / 2), CPoint(right, (bottom + top) / 2));
		left += 24;
		right += 24;
	}
	double centerx = (rect.right + rect.left) / 2;
	double centery = (rect.top + rect.bottom) / 2;
	pdc->EndPath();
	pdc->FillPath();
	DrawEye(pdc, centerx - 10, centery - 10, 10);
	DrawEye(pdc, centerx + 20, centery - 10, 10);
}

void CPacmanView::OnDraw(CDC* pdc)
{
	CPacmanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect pacman(x, y, x + 100, y + 100);
	CRect ghost(100, 120, 196, 216);
	CRect client;
	GetClientRect(client);
	CDC memdc;
	XFORM oldmode;
	memdc.CreateCompatibleDC(pdc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pdc, client.Width(), client.Height());
	memdc.SelectObject(&bmp);
	memdc.GetWorldTransform(&oldmode);
	memdc.SetGraphicsMode(GM_ADVANCED);
	if(!bkdrawn)
	{
		CRect bkrect(0, 0, background->Width(), background->Height());
		background->Draw(&memdc, bkrect, client);
	}
	DrawPacman(&memdc, pacman, angle);
	DrawGhost(&memdc, ghost);

	memdc.SetBkMode(TRANSPARENT);
	memdc.SetTextColor(RGB(255, 255, 255));
	CFont font;
	font.CreateFont(72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
	memdc.SelectObject(&font);
	double cwx = client.Width() / 2;
	double cwy = client.Height() / 2;
	if (Collision(x + 50, y + 50, 50, 100 + 48, 120 + 48, 48)) 
	{
		end = true;
		CRect textrect(cwx - 200, cwy - 36, cwx + 200, cwy + 36);
		DrawText(memdc.m_hDC, L"THE END", 7, textrect, DT_CENTER);
	}
	memdc.SetWorldTransform(&oldmode);
	pdc->BitBlt(0, 0, client.Width(), client.Height(), &memdc, 0, 0, SRCCOPY);
}


// CPacmanView printing

BOOL CPacmanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPacmanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPacmanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CPacmanView diagnostics

#ifdef _DEBUG
void CPacmanView::AssertValid() const
{
	CView::AssertValid();
}

void CPacmanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPacmanDoc* CPacmanView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPacmanDoc)));
	return (CPacmanDoc*)m_pDocument;
}
#endif //_DEBUG


// CPacmanView message handlers

void CPacmanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(!end)
	{
		switch (nChar)
		{
		case VK_LEFT:
			x -= 10;
			Invalidate(FALSE);
			last_dir = VK_LEFT;
			break;
		case VK_RIGHT:
			x += 10;
			Invalidate(FALSE);
			last_dir = VK_RIGHT;
			break;
		case VK_UP:
			y -= 10;
			Invalidate(FALSE);
			last_dir = VK_UP;
			break;
		case VK_DOWN:
			y += 10;
			Invalidate(FALSE);
			last_dir = VK_DOWN;
			break;
		}
		open_mouth = !open_mouth;

	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
