#pragma once

#include <math.h>
#include "vector.h"

extern bool g_bIsActive;
extern int g_nClientWidth;
extern int g_nClientHeight;
extern HDC	g_hScreenDC, g_hMemoryDC;
extern HBITMAP g_hDefaultBitmap, g_hDIBitmap;
extern BYTE *g_pBits;
extern ULONG g_CurrentColor;

void InitGDI(HWND hWnd);
void ReleaseGDI(HWND hWnd);
void UpdateFrame(void);
void Clear(BYTE r, BYTE g, BYTE b);
void SetColor(BYTE r, BYTE g, BYTE b);
bool IsInRange(int x, int y);
void SetPixelRaster(int x, int y);
void SetPixelPoint(Vector3 pos);
void SetLine(Vector3 startpos, Vector3 destpos, float inc);
int MsgLoop(HACCEL hAccelTable);
BOOL WindowInit(HINSTANCE hInstance, int nCmdShow);
