
// Klk2024AView.cpp : implementation of the CKlk2024AView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2024A.h"
#endif

#include "Klk2024ADoc.h"
#include "Klk2024AView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592
#define DEG_TO_RAD(a) (((a) * PI) / 180.0)


// CKlk2024AView

IMPLEMENT_DYNCREATE(CKlk2024AView, CView)

BEGIN_MESSAGE_MAP(CKlk2024AView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2024AView construction/destruction

CKlk2024AView::CKlk2024AView() noexcept
{
	background = new DImage();
	basket = new DImage();
	arm = new DImage();

	background->Load(CString("Background.jpg"));
	basket->Load(CString("Basket.png"));;
	arm->Load(CString("Arm.png"));;
}

CKlk2024AView::~CKlk2024AView()
{
	delete background;
	delete basket;
	delete arm;
}

BOOL CKlk2024AView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2024AView drawing

void CKlk2024AView::DrawBackground(CDC* pdc)
{
	CRect client;
	GetClientRect(client);
	
	int xoffset = client.Width() / 2 - background->Width() / 2;
	int yoffset = client.Height() - background->Height();

	background->Draw(pdc, CRect(0, 0, background->Width(), background->Height()), CRect(xoffset, yoffset, xoffset + background->Width(), yoffset + background->Height()));
}

void CKlk2024AView::DrawImageTransparent(CDC* pdc, DImage* pImage)
{
	unsigned char* bits = pImage->GetDIBBits();
	pImage->DrawTransparent(pdc, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()), RGB(bits[2], bits[1], bits[0]));
}

void CKlk2024AView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false)
{
	XFORM mat = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void  CKlk2024AView::Rotate(CDC* pDC, float angle, bool rightMultiply = false)
{
	angle = DEG_TO_RAD(angle);
	XFORM mat = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void  CKlk2024AView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false)
{
	XFORM mat = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

//Написати функцију void DrawArm(CDC* pDC) којом се црта држач корпи рингишпила
//за шта се користи слика Arm.png.Слику треба исцртати тако да се њена тачка(10, 10)
//нађе у координатином почетку када се не би позивале друге трансформације пре ове
//функције

void  CKlk2024AView::DrawArm(CDC* pdc)
{
	Translate(pdc, -10, -10);
	DrawImageTransparent(pdc, arm);
}

//Написати функцију void DrawBasket(CDC* pDC, int r) којом се црта корпа рингишпила.
//За цртање корпе користити слику Basket.png тако буде 67.5 % оригиналне величине и
//да се њен центар нађе у координатином почетку када се не би позивале друге
//трансформације пре ове функције.На корпи написати текст „R_G“ тако да буде
//центриран у центру корпе и нагнут под углом - 30°.Текст исписати тамно плавом бојом
//коришћењем фонта Verdana висине 0.9∙r који је подебљан(bold) тако да има
//транспарентну позадину.

void CKlk2024AView::DrawBasket(CDC* pdc, int r)
{
	XFORM mat;
	pdc->GetWorldTransform(&mat);

	Scale(pdc, 0.675, 0.675);
	Translate(pdc, -basket->Width() / 2, -basket->Height() / 2);
	DrawImageTransparent(pdc, basket);
	Translate(pdc, basket->Width() / 2, basket->Height() / 2); // sad sam u centru korpe
	Scale(pdc, 1 / 0.675, 1 / 0.675);
	Rotate(pdc, -30);
	pdc->SetBkMode(TRANSPARENT);
	COLORREF clr = pdc->SetTextColor(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(0.9 * r, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Verdana");
	pdc->SelectObject(&font);
	CSize textsize = pdc->GetTextExtent(CString("R_G"));
	Translate(pdc, -textsize.cx / 2, -textsize.cy / 2);
	pdc->TextOutW(0, 0, CString("R_G"));
	Rotate(pdc, 30);
	pdc->SetBkMode(OPAQUE);
	pdc->SetTextColor(clr);

	pdc->SetWorldTransform(&mat);
}

//Написати функцију void DrawBasketCouple(CDC* pDC, int l, int r, float angle) којом се
//цртају две корпе повезане држачем.Центар прве корпе био би у координатном почетку
//када се не би позивале друге трансформације пре ове функције.Она треба да буде
//заротирана за угао angle око своје осе.У тачки помереној водоравно за 0.8∙r од центра
//прве корпе је причвршћен држач његовом већом осовином(центром ротације).Другом
//мањом осовином држач је причвршћен на исту тачку друге корпе(померена водоравно
//	за 0.8∙r од центра).Растојање између осовина држача је l.Држач обезбеђује да се друга
//	корпа ротира пратећи прву корпу

void CKlk2024AView::DrawBasketCouple(CDC* pdc, int l, int r, float angle)
{
	XFORM old;
	pdc->GetWorldTransform(&old);

	Rotate(pdc, angle);
	DrawBasket(pdc, r);
	Translate(pdc, 0.8 * r, 0);
	Rotate(pdc, -90);
	XFORM ruka;
	pdc->GetWorldTransform(&ruka);
	Translate(pdc, 0, l);
	pdc->SetPixel(0, 0, RGB(255, 255, 255));
	Translate(pdc, 0, -0.8 * r);
	Rotate(pdc, 90);
	DrawBasket(pdc, r);
	pdc->SetWorldTransform(&ruka);
	DrawArm(pdc);

	pdc->SetWorldTransform(&old);
}

//Написати функцију void DrawPlatform(CDC* pDC, int l, int r, double angle) којом се црта
//платформа која се састоји од централне корпе око које је правилно постављено 6 парова
//корпи из претходне ставке.Централну корпу заротирати за угао angle, а све остале
//парове корпи за супротан угао(-angle) позивом функције из претходне ставке.Угао се
//може мењати одговарајућим тастерима.
void CKlk2024AView::DrawPlatform(CDC* pdc, int l, int r, float angle)
{
	XFORM old;
	pdc->GetWorldTransform(&old);
	Rotate(pdc, angle);
	DrawBasket(pdc, r);
	Rotate(pdc, -angle);
	int step = 60;
	int rot = 0;
	for (int i = 0; i < 6; i++)
	{
		Rotate(pdc, rot);
		Translate(pdc, 2 * r, 0);
		DrawBasketCouple(pdc, l, r, -angle);
		Translate(pdc, -2 * r, 0);
		Rotate(pdc, -rot);
		rot += step;
	}

	pdc->SetWorldTransform(&old);
}

//  Написати функцију void DrawCarousel(CDC* pDC, int h, int r, double offset, double alpha,
//	double beta, double angle) којом се црта рингишпил.Он имa почетну корпу која може да
//	се креће водоравно и пут који пређе од почетне тачке је offset.При преласку тог пута
//	корпа се ротира за тачно онај угао који одговара пређеном путу.Из ове корпе полази
//	држач центриран у својој осовини који је заротиран за угао alpha у односу на у осу.На
//	овај држач се наставља други држач при чену се центар мање осовине првог поклапа са
//	центром веће осовине другог, и који је заротиран за угао beta.За мању осовину другог
//	држача је повезана платформа која је заротирана за угао angle.Иницијално оба држача
//	стоје усправно.Сви углови се могу мењати одговарајућим тастерима, као и позиција
//	почетне корпе.

void CKlk2024AView::DrawCarousel(CDC* pdc, int l, int r, double offset, double alpha, double beta, double angle)
{
	// o = 2 r pi, l = r alfa, alfa = l / r
	double rot = (double)offset / (double)r;
	Translate(pdc, offset, 0);
	Rotate(pdc, rot * 180.0 / PI);
	DrawBasket(pdc, r);
	Rotate(pdc, -rot * 180.0 / PI);
	Rotate(pdc, alpha);
	//Translate(pdc, -10, -10); // za centriranje osovine u centar kruga
	DrawArm(pdc);
	Translate(pdc, 10, 192); // cenatr druge osovine
	Rotate(pdc, beta);
	//Translate(pdc, -10, -10);
	DrawArm(pdc);
	Translate(pdc, 10, 192);
	DrawPlatform(pdc, l, r, angle);
}

void CKlk2024AView::OnDraw(CDC* pdc)
{
	CKlk2024ADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(client);
	CDC* memdc = new CDC();
	CBitmap bmp;
	XFORM old;
	bmp.CreateCompatibleBitmap(pdc, client.Width(), client.Height());
	memdc->CreateCompatibleDC(pdc); // ovo !!!
	memdc->GetWorldTransform(&old);
	memdc->SetGraphicsMode(GM_ADVANCED);
	memdc->SelectObject(&bmp);
	memdc->FillSolidRect(client, RGB(255, 255, 255));
	// crtanje u memdc!
	DrawBackground(memdc);
	//DrawArm(memdc);
	Translate(memdc, client.Width() / 2, client.Height() - 50);
	//DrawBasket(memdc, 50);
	//DrawBasketCouple(memdc, 182, 50, 0);
	//DrawPlatform(memdc, 182, 50, 0);
	DrawCarousel(memdc, 182, 50, offset, alpha, beta, angle);
	memdc->SetWorldTransform(&old);
	pdc->BitBlt(0, 0, client.Width(), client.Height(), memdc, 0, 0, SRCCOPY);

}

// CKlk2024AView printing

BOOL CKlk2024AView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2024AView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2024AView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2024AView diagnostics

#ifdef _DEBUG
void CKlk2024AView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2024AView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2024ADoc* CKlk2024AView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2024ADoc)));
	return (CKlk2024ADoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2024AView message handlers

void CKlk2024AView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) 
	{
	case 'Q':
	{
		offset -= 5;
		Invalidate(FALSE);
		break;
	}
	case 'W':
	{
		offset += 5;
		Invalidate(FALSE);
		break;
	}
	case 'A':
	{
		alpha += 5;
		Invalidate(FALSE);
		break;
	}
	case 'S':
	{
		alpha -= 5;
		Invalidate(FALSE);
		break;
	}
	case 'D':
	{
		beta += 5;
		Invalidate(FALSE);
		break;
	}
	case 'F':
	{
		beta -= 5;
		Invalidate(FALSE);
		break;
	}
	case 'E':
	{
		angle += 5;
		Invalidate(FALSE);
		break;
	}
	case 'R':
	{
		angle -= 5;
		Invalidate(FALSE);
		break;
	}
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
