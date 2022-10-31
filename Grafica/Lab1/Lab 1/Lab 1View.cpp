
// Lab 1View.cpp : implementation of the CLab1View class
//
#define _USE_MATH_DEFINES
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab 1.h"
#endif


#include <cmath>

#include "Lab 1Doc.h"
#include "Lab 1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>
#include<string>
#include <fstream>

using namespace std;
// CLab1View

IMPLEMENT_DYNCREATE(CLab1View, CView)

BEGIN_MESSAGE_MAP(CLab1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLab1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CLab1View construction/destruction

CLab1View::CLab1View() noexcept
{
	// TODO: add construction code here

}

CLab1View::~CLab1View()
{
}

BOOL CLab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

const int SCALEX = 50;
const int SCALEY = 50;
const int DISTANCE = 50; 

//Functions
double sigmaFnc(double x, double eps) {
	
	double sum = x;
	double t = x;
	for (int k = 1; fabs(t) > eps;k++) {
		t = t * (pow(-1, k) / ((2 * k + 1) * pow(x, 2 * k + 1)));
		sum += t;
	}
	return sum + 3.14;
}

double sigmaFncAlt(double x, double eps) {
	double sum = 0;
	double lastSum = 0;
	double k = 0;
	double value = 0;
	double step = 1;
	do
	{
		value = (-1) * ((2 * k - 1) / (2 * k - 1)) - pow(x, 2);
		k+=step;
		lastSum = sum;
		sum += value;
	} while (abs(sum- lastSum) >= eps);
	return sum+(3.14/2);
}

double roundUp(double number) {
	return floor(number * 10000.0) / 10000.0;
}

void drawFunction(CDC* pDC, CPoint centru, double precision) {

	int a = -1, b = 1;
	double step = 0.02;

	ofstream InfoFile;
	ofstream CSVFile;
	InfoFile.open("info.txt");
	CSVFile.open("data.csv");

	pDC->MoveTo(centru.x+(int)(- 1 * SCALEX), centru.y - (int)(sigmaFncAlt(-1, precision) * SCALEY));

	for (double x = a; x > b; x -= step) {
		//double y = sigmaFnc(x, precision);
		double y = sigmaFncAlt(x, precision);
		InfoFile << x << "\t" << sigmaFncAlt(x, precision) << "\t" << (3.14 / 2) - atan(x) << "\t" << fabs(sigmaFncAlt(x, precision) - ((3.14/2) - atan(x))) << "\t" << precision << endl;
		CSVFile << x << "," << (3.14/2) - atan(x) << endl;
		
		pDC->LineTo(centru.x + (int)(x * SCALEX),
			centru.y - (int)(y * SCALEY));

	}
	//pDC->MoveTo(centru.x, centru.y);
	
	InfoFile.close();
	CSVFile.close();
	/*CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pen = pDC->SelectObject(&redPen);*/

	pDC->MoveTo(centru.x + (int)(-1 * SCALEX), centru.y - (int)(sigmaFncAlt(-1, precision) * SCALEY));
	for (double x = a; x > b; x -= step) {
		double y = (3.14 / 2) - atan(x);
		pDC->SetPixel(centru.x + (int)(x * SCALEX),
			centru.y - (int)(y * SCALEY), RGB(255, 0, 0));

		pDC->SetPixel(centru.x + (int)(x * SCALEX) - 1,
			centru.y - (int)(y * SCALEY), RGB(255, 0, 0));
		pDC->SetPixel(centru.x + (int)(x * SCALEX),
			centru.y - (int)(y * SCALEY) + 1, RGB(255, 0, 0));
		pDC->SetPixel(centru.x + (int)(x * SCALEX),
			centru.y - (int)(y * SCALEY) - 1, RGB(255, 0, 0));
		pDC->SetPixel(centru.x + (int)(x * SCALEX) + 1,
			centru.y - (int)(y * SCALEY), RGB(255, 0, 0));

	}
}


// CLab1View drawing
void CLab1View::OnDraw(CDC* pDC)
{
	CLab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPen penAxa(PS_SOLID, 2, RGB(0, 0, 0));

	CRect rect;
	GetClientRect(&rect);
	CPen* pen = pDC->SelectObject(&penAxa);
	CPoint centru = rect.CenterPoint();

	//Sistemul de coordonate
	pDC->MoveTo(0, centru.y);
	pDC->LineTo(rect.Width() - 1, centru.y);
	pDC->LineTo(rect.Width() - 21, centru.y - 20);
	pDC->MoveTo(rect.Width() - 1, centru.y);
	pDC->LineTo(rect.Width() - 21, centru.y + 20);

	pDC->MoveTo(centru.x, 0);
	pDC->LineTo(centru.x, rect.Height() - 1);
	pDC->MoveTo(centru.x, 0);
	pDC->LineTo(centru.x - 21, 0 + 20);
	pDC->MoveTo(centru.x, 0);
	pDC->LineTo(centru.x + 21, 0 + 20);

	//Delimitarile
	int width = 10;
	float times = 0;
	for (size_t i = centru.x; i < rect.Width()-20; i+= DISTANCE,times+=1)
	{
		pDC->MoveTo(i, centru.y + width);
		pDC->LineTo(i, centru.y - width);

		if (times!=0) {
			CString s;
			s.Format(_T("%.1f"), times);
			pDC->TextOut(i-10, centru.y + width, s);
		}
		

		pDC->MoveTo(centru.x-(i-centru.x), centru.y + width);
		pDC->LineTo(centru.x - (i - centru.x), centru.y - width);

		if (times != 0) {
			CString s;
			s.Format(_T("-%.1f"), times);
			pDC->TextOut(centru.x - (i - centru.x)-15, centru.y + width, s);
		}
	}
	times = 0;

	for (size_t i = centru.y; i < rect.Height()-20; i += DISTANCE,times+=1)
	{
		pDC->MoveTo(centru.x- width, i);
		pDC->LineTo(centru.x+ width, i);


		if (times != 0) {
			CString s;
			s.Format(_T("-%.1f"), times);
			pDC->TextOut(centru.x + width +5, i-10, s);
		}

		pDC->MoveTo(centru.x- width, centru.y -(i-centru.y));
		pDC->LineTo(centru.x+ width, centru.y- (i - centru.y));

		if (times != 0) {
			CString s;
			s.Format(_T("%.1f"), times);
			pDC->TextOut(centru.x + width +5, centru.y - (i - centru.y) -10, s);
		}
	}

	drawFunction(pDC, centru,0.005);
}

// CLab1View printing


void CLab1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLab1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLab1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLab1View diagnostics

#ifdef _DEBUG
void CLab1View::AssertValid() const
{
	CView::AssertValid();
}

void CLab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab1Doc* CLab1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab1Doc)));
	return (CLab1Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab1View message handlers
