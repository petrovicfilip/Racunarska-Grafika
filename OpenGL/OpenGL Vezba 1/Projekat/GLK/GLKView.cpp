
// GLKView.cpp : implementation of the CGLKView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GLK.h"
#endif

#include "GLKDoc.h"
#include "GLKView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLKView

IMPLEMENT_DYNCREATE(CGLKView, CView)

BEGIN_MESSAGE_MAP(CGLKView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CGLKView construction/destruction

CGLKView::CGLKView()
{
	// TODO: add construction code here

}

CGLKView::~CGLKView()
{
}

BOOL CGLKView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGLKView drawing

void CGLKView::OnDraw(CDC* pDC)
{
	CGLKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CGLKView printing

BOOL CGLKView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLKView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLKView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGLKView diagnostics

#ifdef _DEBUG
void CGLKView::AssertValid() const
{
	CView::AssertValid();
}

void CGLKView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLKDoc* CGLKView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLKDoc)));
	return (CGLKDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLKView message handlers


int CGLKView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CGLKView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CGLKView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);	
}


BOOL CGLKView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CGLKView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}

void CGLKView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case 'G':
		{
			m_glRenderer.grid = !m_glRenderer.grid;
			Invalidate();
			break;
		}
		case VK_UP:
		{
			m_glRenderer.angle_x -= 2.0;
			Invalidate();
			break;
		}
		case VK_DOWN:
		{
			m_glRenderer.angle_x += 2.0;
			Invalidate();
			break;
		}
		case VK_LEFT:
		{
			m_glRenderer.angle_z += 2.0;
			Invalidate();
			break;
		}
		case VK_RIGHT:
		{
			m_glRenderer.angle_z -= 2.0;
			Invalidate();
			break;
		}
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGLKView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_glRenderer.is_pressed = true;
	m_glRenderer.last_x = point.x;
	m_glRenderer.last_y = point.y;
	CView::OnLButtonDown(nFlags, point);
}

void CGLKView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_glRenderer.is_pressed = false;
	CView::OnLButtonUp(nFlags, point);
}

void CGLKView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_glRenderer.is_pressed)
	{
		int dx = point.x - m_glRenderer.last_x;
		int dy = point.y - m_glRenderer.last_y;

		m_glRenderer.yaw -= dx * 0.2;
		m_glRenderer.pitch += dy * 0.2;

		m_glRenderer.last_x = point.x;
		m_glRenderer.last_y = point.y;

		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}

BOOL CGLKView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0)
	{
		m_glRenderer.zoom -= zDelta / 1000.0;
	}
	else
	{
		m_glRenderer.zoom -= zDelta / 1000.0;
	}
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);

	return TRUE;
}
