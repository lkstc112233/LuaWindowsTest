// LuaWindowsTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "LuaWindowsTest.h"

#include "Windowsx.h"

#include <string>
using std::string;

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#define MAX_LOADSTRING 100

lua_State *L;

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LUAWINDOWSTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LUAWINDOWSTEST));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LUAWINDOWSTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LUAWINDOWSTEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

HWND mainWindow = NULL;

string str;
string instring;
const int colors[] = { RGB(50, 50, 50), RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0) };

void callFunc(int index)
{
	//char buffer[255];
	//sprintf_s(buffer, "The function is %d", index);
	//MessageBoxA(NULL, buffer, "Caption", 0);

	switch (index)
	{
	case 0:
		lua_getglobal(L, "aFun");
		lua_pushinteger(L, 1);
		lua_pcall(L, 1, 0, 0);
		break;
	case 1:
		lua_getglobal(L, "aFun");
		lua_pushinteger(L, 2);
		lua_pcall(L, 1, 0, 0);
		break;
	case 2:
		lua_pushliteral(L, "My function works!");
		lua_setglobal(L, "str");
		break;
	case 3:
		lua_getglobal(L, "changeStr");
		lua_pushliteral(L, "Call function works!");
		lua_pcall(L, 1, 0, 0);
		break;
	case 4:
		lua_getglobal(L, "getRandomStr");
		lua_pcall(L, 0, 1, 0);
		lua_setglobal(L, "str");
		break;
	default:
		break;
	}
}

//int changeStrFunction(lua_State *L)
//{
//
//}

int postMessageFunction(lua_State *L)
{
	int n = lua_gettop(L);    /* 参数的个数 */
	if (n < 1)
	{
		lua_pushliteral(L, "No Enough Information : Missing Message Type");
		lua_error(L);
	}
	lua_Integer messageType = 0;
	lua_Integer wParam = 0;
	lua_Integer lParam = 0;
	messageType = lua_tointeger(L, 1);
	if (messageType > UINT_MAX)
	{
		lua_pushliteral(L, "Information Invalid : Invalid Message Type");
		lua_error(L);
	}
	messageType &= UINT_MAX;
	if (n > 1)
		wParam = lua_tointeger(L, 2)&UINT_MAX;
	if (n > 2)
		lParam = lua_tointeger(L, 3)&UINT_MAX;
	SendMessage(mainWindow, static_cast<UINT>(messageType),static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
	return 0;                   /* 返回值的个数 */
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	int xPos, yPos;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		mainWindow = hWnd;
		L = luaL_newstate();
		luaL_openlibs(L);
		lua_pushinteger(L, WM_MYMESSAGE);
		lua_setglobal(L, "messageId");
		lua_register(L, "sendFunction", postMessageFunction);
		if (luaL_dofile(L, "script.lua"))
			MessageBoxA(NULL, lua_tostring(L, -1), "Caption", 0);
		lua_pushstring(L, "Hello world");
		lua_setglobal(L, "str");
		break;
	//case WM_CHAR:
	//	switch (wParam)
	//	{
	//	case 0x08:
	//		// Process a backspace. 
	//		instring.pop_back();
	//		break;
	//	case 0x0A:
	//		// Process a linefeed. 
	//		break;
	//	case 0x1B:
	//		// Process an escape.
	//		break;
	//	case 0x09:
	//		instring += '\t';
	//		// Process a tab.
	//		break;
	//	case 0x0D:
	//		// Process a carriage return. 
	//		lua_pushstring(L, instring.c_str());
	//		instring = "";
	//		break;
	//	default:
	//		instring += (char)wParam;
	//		break;
	//	}
	//	InvalidateRect(hWnd, NULL, true);
	//	break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		RECT rect;
		HGDIOBJ brushes[5];
		for (int i = 0; i < 5; ++i)
		{
			rect.top = 10;
			rect.left = 10 + 210 * i;
			rect.right = 210 + 210 * i;
			rect.bottom = 210;
			brushes[i] = SelectObject(hdc, CreateSolidBrush(colors[i]));
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		}
		brushes[0] = SelectObject(hdc, brushes[0]);
		for (HGDIOBJ h : brushes)
			DeleteObject(h);
		rect.left = 10;
		rect.right = 1000;
		rect.bottom = 400;
		rect.top = 300;
		lua_settop(L, 0);
		lua_getglobal(L, "str");
		if (lua_isnil(L, 1))
			str = "";
		else
			str = lua_tostring(L, 1);
		DrawTextA(hdc, ("str = " + str).c_str(), -1, &rect, DT_LEFT | DT_TOP);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONUP:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		if ((xPos - 10) / 210 < 5 && xPos > 10 && (xPos - 210 < 0 || (xPos - 10) / 210 != (xPos - 210) / 210))
			if (yPos > 10 && yPos < 210)
			{
				callFunc((xPos - 10) / 210);
				InvalidateRect(hWnd, NULL, true);
			}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MYMESSAGE:
		switch (wParam)
		{
		case 0:
			break;
		case 1:
			lua_pushliteral(L, "Message type works!");
			lua_setglobal(L, "str");
			break;
		case 2:
			lua_getglobal(L, "changeStr");
			lua_pushliteral(L, "Callback function works!");
			lua_pcall(L, 1, 0, 0);
			break;
		default:
			MessageBoxA(NULL, "Received message type Unknown", "Caption", 0);
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
