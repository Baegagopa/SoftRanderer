
#include "stdafx.h"
#include "gdihelper.h"

bool g_bIsActive;
int g_nClientWidth = 640;
int g_nClientHeight = 480;
HDC	g_hScreenDC, g_hMemoryDC;
HBITMAP g_hDefaultBitmap, g_hDIBitmap;
BYTE *g_pBits;
ULONG g_CurrentColor;
float g_Angle = 0.0f;

UINT g_nframeCounter;
LARGE_INTEGER Frequency;
LARGE_INTEGER StarterCounter, EndCounter;


Vector3 g_Cameara2D;
Matrix3 g_ViewMatrix2D;

Vector3 g_Cameara3D;
Matrix4 g_VPMatrix3D;

void InitFrame3D();
Matrix4 GetViewMatrix();
Matrix4 GetPorjectionMatrix(float fovDegree, float aspect, float nearz, float farz);

void InitGDI(HWND hWnd)
{
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StarterCounter);

	g_hScreenDC = GetDC(hWnd);

	g_hMemoryDC = CreateCompatibleDC(g_hScreenDC);
	//BytesPerScanline = ((g_nClientWidth * 2) + 3) & ~3;

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = g_nClientWidth;
	bmi.bmiHeader.biHeight = -g_nClientHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	g_hDIBitmap = CreateDIBSection(g_hMemoryDC, &bmi, DIB_RGB_COLORS, (void**)&g_pBits, NULL, 0);
	g_hDefaultBitmap = (HBITMAP)SelectObject(g_hMemoryDC, g_hDIBitmap);

	g_bIsActive = TRUE;
	InitFrame3D();
}

void ReleaseGDI(HWND hWnd)
{
	g_bIsActive = FALSE;
	SelectObject(g_hMemoryDC, g_hDefaultBitmap);
	DeleteObject(g_hDIBitmap);
	ReleaseDC(hWnd, g_hScreenDC);
}

void Clear(BYTE r, BYTE g, BYTE b)
{
	ULONG* dest = (ULONG*)g_pBits;
	DWORD bytecount = g_nClientWidth * g_nClientHeight * sizeof(ULONG);
	ULONG value = RGB(b, g, r);
	bytecount /= 4;
	while (bytecount--)
	{
		*dest++ = value;
	}
	return;
}

void SetColor(BYTE r, BYTE g, BYTE b)
{
	g_CurrentColor = RGB(b, g, r);
}

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void SetPixelRaster(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	//DWORD offset = pos.y * g_nClientWidth + pos.x;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}

void SetPixelPoint(Vector3 pos)
{
	if (!pos.IsPoint()) return;

	SetPixelRaster((int)pos.x, (int)pos.y);
}

//2D
void SetLine(Vector3 startpos, Vector3 destpos, float inc = 0.01f)
{
	if (!startpos.IsPoint()) return;
	if (!destpos.IsPoint()) return;

	// Add Code Here
	float xs = (destpos.x - startpos.x);
	float ys = (destpos.y - startpos.y);
	float length = sqrtf(xs * xs + ys * ys);
	if (length == 0.0f) return;
	inc = 1.0f / length;

	float t = 0.0f;
	while (t <= 1.0f)
	{
		Vector3 ResultPos;
		ResultPos.SetPoint(
			(1.0f - t) * startpos.x + t * destpos.x,
			(1.0f - t) * startpos.y + t * destpos.y
		);
		Vector3 ViewPos = ResultPos * g_ViewMatrix2D;
		SetPixelPoint(ViewPos);
		t += inc;
	}
}

void UpdateFrame2D(void)
{
	// Buffer Clear
	Clear(0, 0, 255);

	if (GetAsyncKeyState(VK_LEFT)) g_Cameara2D.x -= 1;
	if (GetAsyncKeyState(VK_RIGHT)) g_Cameara2D.x += 1;
	if (GetAsyncKeyState(VK_UP)) g_Cameara2D.y += 1;
	if (GetAsyncKeyState(VK_DOWN)) g_Cameara2D.y -= 1;

	g_ViewMatrix2D.SetTranslation(-g_Cameara2D.x, -g_Cameara2D.y);

	// Cartesian Coordinate
	SetColor(0, 0, 0);
	Vector3 XAxis1, XAxis2, YAxis1, YAxis2;
	XAxis1.SetPoint(g_Cameara2D.x - 500.f, 0.0f);
	XAxis2.SetPoint(g_Cameara2D.x + 500.f, 0.0f);
	YAxis1.SetPoint(0.0f, g_Cameara2D.x - 500.f);
	YAxis2.SetPoint(0.0f, g_Cameara2D.x + 500.f);

	SetColor(255, 0, 0);
	SetLine(XAxis1, XAxis2);
	SetColor(0, 255, 0);
	SetLine(YAxis1, YAxis2);


	// Add Rendering Code Here
	SetColor(255, 255, 0);
	Vector3 Pt1, Pt2, Pt3;
	Pt1.SetPoint(10, 10);
	Pt2.SetPoint(500, 500);
	Pt3.SetPoint(80, 20);
	SetLine(Pt1, Pt2);
	SetLine(Pt2, Pt3);
	SetLine(Pt3, Pt1);

	Matrix3 tMat;
	tMat.SetTranslation(-10, 10);

	Matrix3 rMat;
//	g_Angle += frameTime * 180.0f;
	rMat.SetRotation(g_Angle);

	Matrix3 sMat;
	sMat.SetScale(2.0f, 0.5f);

	SetColor(0, 255, 0);
	Vector3 nPt1 = Pt1 * sMat * rMat * tMat;
	Vector3 nPt2 = Pt2 * sMat * rMat * tMat;
	Vector3 nPt3 = Pt3 * sMat * rMat * tMat;
	SetLine(nPt1, nPt2);
	SetLine(nPt2, nPt3);
	SetLine(nPt3, nPt1);

	SetColor(255, 0, 0);
	Matrix3 trsMat = tMat * rMat * sMat;
	Vector3 mPt1 = Pt1 * trsMat;
	Vector3 mPt2 = Pt2 * trsMat;
	Vector3 mPt3 = Pt3 * trsMat;
	SetLine(mPt1, mPt2);
	SetLine(mPt2, mPt3);
	SetLine(mPt3, mPt1);
}

void SetLine3D(Vector3 startpos, Vector3 destpos, float inc = 0.01f)
{
	Vector3 VPStartPos = startpos * g_VPMatrix3D;
	Vector3 VPDestPos = destpos * g_VPMatrix3D;

	float xs = (VPDestPos.x - VPStartPos.x);
	float ys = (VPDestPos.y - VPStartPos.y);
	float length = sqrtf(xs * xs + ys * ys);
	if (length == 0.0f) return;
	inc = 1.0f / length;

	float t = 0.0f;
	while (t <= 1.0f)
	{
		Vector3 ResultPos;
		ResultPos.SetPoint(
			(1.0f - t) * VPStartPos.x + t * VPDestPos.x,
			(1.0f - t) * VPStartPos.y + t * VPDestPos.y
		);
		SetPixelPoint(ResultPos);
		t += inc;
	}
}

Matrix4 GetViewMatrix()
{
	Matrix4 tMat;
	tMat.SetTranslation(-g_Cameara3D.x, -g_Cameara3D.y, -g_Cameara3D.z);

	Matrix4 rMat;
	Vector3 Target;
	Vector3 Up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 zVector = (Target - g_Cameara3D).Normalize();
	Vector3 xVector = (Up ^ zVector).Normalize();
	Vector3 yVector = (zVector ^ xVector).Normalize();

	rMat.Identity();
	rMat._11 = xVector.x;
	rMat._12 = xVector.y;
	rMat._13 = xVector.z;

	rMat._21 = yVector.x;
	rMat._22 = yVector.y;
	rMat._23 = yVector.z;

	rMat._31 = zVector.x;
	rMat._32 = zVector.y;
	rMat._33 = zVector.z;
	return rMat * tMat;
}

Matrix4 GetPorjectionMatrix(float fovDegree, float aspect, float nearz, float farz)
{
	Matrix4 pMat;
	pMat.Identity();
	pMat._44 = 0.0f;
	float fovRadian = fovDegree * 3.141592 / 180.0f;
	float width = 2 * (nearz * tanf(fovRadian * 0.5f)); 
	float height = aspect * width;

	pMat._11 = 2 * nearz / width;
	pMat._22 = 2 * nearz / height;
	pMat._33 = 2 * farz / (farz - nearz);
	pMat._34 = -(nearz * farz) / (farz - nearz);
	pMat._43 = -1.0f;

	return pMat;
}

void UpdateFrame3D(void)
{
	Clear(128, 128, 128);

	static float fovDegree = 45.0f;

	if (GetAsyncKeyState(VK_PRIOR))	fovDegree += 1.0f;
	if (GetAsyncKeyState(VK_NEXT))	fovDegree -= 1.0f;
	
	Matrix4 VMat = GetViewMatrix();
	Matrix4 PMat = GetPorjectionMatrix(fovDegree, 0.75f, 128.0f, 10000.0f);
	g_VPMatrix3D = PMat * VMat;

	Vector3 Origin, XAxis, YAxis, ZAxis;
	Origin.Set(0.0f, 0.0f, 0.0f);
	XAxis.Set(g_Cameara3D.x + 5000.0f, 0.0f, 0.0f);
	YAxis.Set(0.0f, g_Cameara3D.y + 5000.0f,0.0f);
	ZAxis.Set(0.0f, 0.0f, g_Cameara3D.z + 5000.0f);


	// 좌표계
	SetColor(255, 0, 0);
	SetLine3D(Origin, XAxis);
	SetColor(0, 255, 0);
	SetLine3D(Origin, YAxis);
	SetColor(0, 0, 255);
	SetLine3D(Origin, ZAxis);


	// 삼각형
	SetColor(255,255, 0);
	
	Vector3 wPt1, wPt2, wPt3;
	wPt1.Set(30, 0, 0);
	wPt2.Set(0, 30, 0);
	wPt3.Set(0, 0, 30);
	SetLine3D(wPt1, wPt2);
	SetLine3D(wPt1, wPt3);
	SetLine3D(wPt3, wPt2);


}

void UpdateFrame(void)
{
	g_nframeCounter++;
	QueryPerformanceCounter(&EndCounter);
	float frameTime =
		(float)(EndCounter.QuadPart - StarterCounter.QuadPart) / (float)Frequency.QuadPart;
	StarterCounter = EndCounter;
	float fps = 1.0f / frameTime;


	UpdateFrame3D();

	wchar_t fpsText[100];
	wchar_t frameText[100];

	swprintf(fpsText, 100, L"FPS : %f", fps);
	swprintf(frameText, 100, L"COUNT : %d", g_nframeCounter);
	TextOut(g_hMemoryDC, 10, 10, frameText, lstrlen(frameText));
	TextOut(g_hMemoryDC, 10, 30, fpsText, lstrlen(fpsText));

	// Buffer Swap 
	BitBlt(g_hScreenDC, 0, 0, g_nClientWidth, g_nClientHeight, g_hMemoryDC, 0, 0, SRCCOPY);

}

void InitFrame3D()
{
	g_Cameara3D = Vector3(50.0f, 50.0f, 50.0f);
}

int MsgLoop(HACCEL hAccelTable)
{
	MSG msg;

	// 기본 메시지 루프입니다.
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, nullptr, 0, 0))
			{
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

			}
			else
			{
				return (int)msg.wParam;
			}
		}
		else if (g_bIsActive)
		{
			UpdateFrame();
		}
		else
		{
			WaitMessage();
		}

	}

	return (int)msg.wParam;
}

BOOL WindowInit(HINSTANCE hInstance, int nCmdShow)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = g_nClientWidth - 1;
	rect.bottom = g_nClientHeight - 1;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
	int WindowWidth = rect.right - rect.left + 1;
	int WindowHeight = rect.bottom - rect.top + 1;

	HWND hWnd = CreateWindowW(L"TM", L"Marika", WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXFULLSCREEN) - WindowWidth) / 2, (GetSystemMetrics(SM_CYFULLSCREEN) - WindowHeight) / 2, WindowWidth, WindowHeight,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


/*
#include "stdafx.h"
#include "gdihelper.h"

bool g_bIsActive;
int g_nClientWidth = 640;
int g_nClientHeight = 480;
HDC	g_hScreenDC, g_hMemoryDC;
HBITMAP g_hDefaultBitmap, g_hDIBitmap;
BYTE *g_pBits;
ULONG g_CurrentColor;

float g_frameCounter;
UINT g_nframeCounter;
LARGE_INTEGER Frequency;
LARGE_INTEGER StartCounter, EndCounter;

Vector3 g_Cameara2D;
Matrix3 g_ViewMatrix;

void InitGDI(HWND hWnd)
{
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartCounter);
	g_hScreenDC = GetDC(hWnd);

	g_hMemoryDC = CreateCompatibleDC(g_hScreenDC);
	//BytesPerScanline = ((g_nClientWidth * 2) + 3) & ~3;

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = g_nClientWidth;
	bmi.bmiHeader.biHeight = -g_nClientHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	g_hDIBitmap = CreateDIBSection(g_hMemoryDC, &bmi, DIB_RGB_COLORS, (void**)&g_pBits, NULL, 0);
	g_hDefaultBitmap = (HBITMAP)SelectObject(g_hMemoryDC, g_hDIBitmap);

	g_bIsActive = TRUE;
}

void ReleaseGDI(HWND hWnd)
{
	g_bIsActive = FALSE;
	SelectObject(g_hMemoryDC, g_hDefaultBitmap);
	DeleteObject(g_hDIBitmap);
	ReleaseDC(hWnd, g_hScreenDC);
}

void Clear(BYTE r, BYTE g, BYTE b)
{
	ULONG* dest = (ULONG*)g_pBits;
	DWORD bytecount = g_nClientWidth * g_nClientHeight * sizeof(ULONG);
	ULONG value = RGB(b, g, r);
	bytecount /= 4;
	while (bytecount--)
	{
		*dest++ = value;
	}
	return;
}

void SetColor(BYTE r, BYTE g, BYTE b)
{
	g_CurrentColor = RGB(b, g, r);
}

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void SetPixelRaster(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	//DWORD offset = pos.y * g_nClientWidth + pos.x;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}

void SetPixelPoint(Vector3 pos)
{
	if (!pos.IsPoint()) return;

	SetPixelRaster((int)pos.x, (int)pos.y);
}

void SetLine(Vector3 startpos, Vector3 destpos, float inc = 0.01f)
{
	if (!startpos.IsPoint()) return;
	if (!destpos.IsPoint()) return;

	float xs = (destpos.x - startpos.x);
	float ys = (destpos.y - startpos.y);
	float length = sqrtf(xs *xs + ys*ys);
	if (length == 0.0f) return;
	inc = 1.0f / length;

	// Add Code Here
	for(float t = inc; t < 1.0f; t=t+inc)
	{
		SetPixelRaster( (1.0f-t)*startpos.x + t*destpos.x, (1.0f - t)*startpos.y + t*destpos.y);
	}
}

void SetTriangle(Vector3 point1, Vector3 point2, Vector3 point3, float inc = 0.01f)
{
	SetLine(point1, point2, inc);
	SetLine(point2, point3, inc);
	SetLine(point3, point1, inc);
}

void UpdateFrame(void)
{
	g_nframeCounter++;
	QueryPerformanceCounter(&EndCounter);
	float frameTime =
		(float)(EndCounter.QuadPart - StartCounter.QuadPart) / (float)Frequency.QuadPart;
	StartCounter = EndCounter;
	float fps = 1.0f / frameTime;

	// Buffer Clear
	Clear(127, 127, 127);

	// Cartesian Coordinate
	MoveToEx(g_hMemoryDC, 0, g_nClientHeight / 2, NULL);
	LineTo(g_hMemoryDC, g_nClientWidth, g_nClientHeight / 2);
	MoveToEx(g_hMemoryDC, g_nClientWidth / 2, 0, NULL);
	LineTo(g_hMemoryDC, g_nClientWidth / 2, g_nClientHeight);

	// Add Rendering Code Here
	SetColor(255, 255, 0);
	Vector3 point1;
	Matrix3 mat1;
	point1.SetPoint(10.0f, 10.0f);
	SetPixelPoint(point1);


	mat1.SetTranslation(15.0f, 20.0f);
	SetColor(255, 0, 0);
	SetPixelPoint(point1 * mat1);

	mat1.SetRotation(90.0f);
	SetColor(0, 255, 0);
	SetPixelPoint(point1 * mat1);

	mat1.SetScale(5.0f, 5.0f);
	SetColor(0, 255, 255);
	SetPixelPoint(point1 * mat1);

	Vector3 p2, p3;
	p2.SetPoint(10.0f, 10.0f);
	p3.SetPoint(50.0f, 50.0f);
	SetColor(255, 255, 255);
	SetLine(p2, p3);


	//====================
	Vector3 p4, p5, p6;
	p4.SetPoint(10.0f, 10.0f);
	p5.SetPoint(50.0f, 50.0f);
	p6.SetPoint(30.0f, 70.0f);

	Matrix3 mT, mR, mS;
	mS.SetScale(2.0f, 0.5f);
	mR.SetRotation(90.0f);
	mT.SetTranslation(-10.f, -150.0f);

	SetColor(255, 0, 255);
	SetTriangle(p4, p5, p6);

	Vector3 nP4 = p4 * mS *mR * mT;
	Vector3 nP5 = p5 * mS *mR * mT;
	Vector3 nP6 = p6 * mS *mR * mT;
	SetColor(255, 255, 255);
	SetTriangle(nP4, nP5, nP6);

	mS.SetScale(8.0f, 2.0f);
	Matrix3 matA = mT * mR * mS;
	Vector3 mP4 = p4*matA;
	Vector3 mP5 = p5*matA;
	Vector3 mP6 = p6*matA;
	SetColor(0, 0, 0);
	SetTriangle(mP4, mP5, mP6);


	//for (int i = 0; i < 200; i++)
	//	SetPixelRaster(cos(i)*i, sin(i)*i);



	wchar_t fpsText[100];
	wchar_t frameText[100];

	swprintf(fpsText, 100, L"FPS : %f", fps);
	swprintf(frameText, 100, L"COUNT : %f", g_frameCounter);

	TextOut(g_hMemoryDC, 10, 15, fpsText, lstrlen(fpsText));
	TextOut(g_hMemoryDC, 10, 0, frameText, lstrlen(frameText));

	// Buffer Swap 
	BitBlt(g_hScreenDC, 0, 0, g_nClientWidth, g_nClientHeight, g_hMemoryDC, 0, 0, SRCCOPY);

}

int MsgLoop(HACCEL hAccelTable)
{
	MSG msg;

	// 기본 메시지 루프입니다.
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, nullptr, 0, 0))
			{
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

			}
			else
			{
				return (int)msg.wParam;
			}
		}
		else if (g_bIsActive)
		{
			UpdateFrame();
		}
		else
		{
			WaitMessage();
		}

	}

	return (int)msg.wParam;
}

BOOL WindowInit(HINSTANCE hInstance, int nCmdShow)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = g_nClientWidth - 1;
	rect.bottom = g_nClientHeight - 1;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
	int WindowWidth = rect.right - rect.left + 1;
	int WindowHeight = rect.bottom - rect.top + 1;

	HWND hWnd = CreateWindowW(L"CK", L"Marika", WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXFULLSCREEN) - WindowWidth) / 2, (GetSystemMetrics(SM_CYFULLSCREEN) - WindowHeight) / 2, WindowWidth, WindowHeight,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
*/