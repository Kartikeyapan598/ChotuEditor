#pragma once

#include <wrl.h>
#include <d2d1.h>
#include <iostream>
#include <crtdbg.h>
#include <Windows.h>
#include <dwrite_3.h>
#include <DirectXColors.h>

#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"d2d1.lib")

#define RELEASE(x) { if(x) {x->Release(); x = 0; } }

#define IDM_FILE_NEW					1
#define IDM_FILE_OPEN					2
#define IDM_FILE_SAVE					3
#define IDM_FILE_SAVE_AS				4
#define IDM_FILE_QUIT					5
#define IDM_FILE_ABOUT					6
#define IDM_FILE_USAGE					7
#define IDM_APP_INFO					8
#define IDM_FILE_VIEW					9
#define IDM_FILE_FONT					10
#define IDM_FILE_TAB					11

#define ID_FIRSTCHILD					12

#define ID_TABCTRL						111