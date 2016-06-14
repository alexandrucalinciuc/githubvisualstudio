#define _CRT_SECURE_NO_WARNINGS
#define COD    1000
#define RESET  1001
#define INST   1002
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>

typedef struct nod {
	char *cheie;
	int ind;
	nod *urm;
};

nod *prim, *ultim;
int ok = 1;
int index = 0;

void genereaza(char *key)
{
	nod *p;
	if (prim == NULL)
	{
		p = (nod*)malloc(sizeof(nod));
		p->cheie = (char*)malloc(strlen(key)*sizeof(char));
		strcpy(p->cheie, key);
		p->urm = NULL;
		prim = p;
		ultim = p;
		index++;
		p->ind = index;
	}
	nod *q = prim;
	while (q != NULL)
	{
		if (strcmp(q->cheie, key) == 0) {
			ok = 0;
		}
		q = q->urm;
	}
	if (ok == 1)
	{
		p = (nod*)malloc(sizeof(nod));
		p->cheie = (char*)malloc(strlen(key)*sizeof(char));
		strcpy(p->cheie, key);
		p->urm = NULL;
		ultim->urm = p;
		ultim = p;
		index++;
		p->ind = index;
	}
	ok = 1;
}

void destroy()
{
	nod *p;
	while (prim != NULL)
	{
		p = prim;
		prim = prim->urm;
		free(p);
	}
}

HINSTANCE _hInstance;
int _nCmdShow;
HWND _hwnd; 
MSG msg; 
HWND _caseta1, _caseta2,_caseta3;

HDC hDC;
PAINTSTRUCT Ps;
HFONT font;
LOGFONT LogFont;

void colorare(){
	hDC = BeginPaint(_hwnd, &Ps);
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF qLineColor;

	LogFont.lfStrikeOut = 0;
	LogFont.lfUnderline = 1;
	LogFont.lfWeight = FW_BOLD;					
	LogFont.lfHeight = 40;
	LogFont.lfEscapement = 0;
	LogFont.lfItalic = FALSE;

	qLineColor = RGB(0, 255, 0); 
	font = CreateFontIndirect(&LogFont);
	SelectObject(hDC, font);
	SetTextColor(hDC, qLineColor);    
	TextOut(hDC, 120, 80, "Introdu text", 12); 
	TextOut(hDC, 120, 200, "Dictionar", 9);
	TextOut(hDC, 120, 320, "Textul codat", 12); 
	
	DeleteObject(font);
}

void butoane(HWND hwnd)
{
	_caseta1 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 350, 80, 500, 70, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
	CreateWindow("BUTTON", "Codeaza", WS_BORDER | WS_CHILD | WS_VISIBLE, 5, 80, 100, 40, hwnd, (HMENU)COD, _hInstance, NULL);
	CreateWindow("BUTTON", "Reseteaza", WS_BORDER | WS_CHILD | WS_VISIBLE, 5, 200, 100, 40, hwnd, (HMENU)RESET, _hInstance, NULL);
	CreateWindow("BUTTON", "Instructiuni", WS_BORDER | WS_CHILD | WS_VISIBLE, 5, 320, 100, 40, hwnd, (HMENU)INST, _hInstance, NULL);
}

LRESULT CALLBACK WndProc_Instruction(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE:
	{
		FILE *stream= fopen("in.txt", "r");
		char linie[1000];
		int nl = 0;
		while (fgets(linie, 1000, stream))
		{
			CreateWindow("static", linie, WS_CHILD | WS_VISIBLE, 20, nl*30, 700, 30, hwnd, (HMENU)1, NULL, NULL);
			nl++;
		}
		fclose(stream);
	}
	break;

	case WM_DESTROY:
		DestroyWindow(hwnd);
		break;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_PAINT:
		colorare();
		break;

	case WM_CREATE:
		butoane(hwnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case COD:
			 {		
			char vect1[4000];
			SendMessage(_caseta1, WM_GETTEXT, sizeof(vect1) / sizeof(char), reinterpret_cast<LPARAM>(vect1));	
			char vect[4000], vect2[4000];
			strcpy(vect, vect1);
			strcpy(vect2, vect);
			char *key=NULL;
			key = strtok(vect, " \n");
			while (key)
			{
				genereaza(key);
				key = strtok(NULL, " \n");
			}
			int k = 0;
			char *v = (char*)malloc(300 * sizeof(char));
			nod *p = prim;
			while (p != NULL)
			{
				if (k == 0) {
					strcpy(v, p->cheie); k++;
				}
				else {
					strcat(v, " ");
					strcat(v, p->cheie);
				}
				p = p->urm;
			}
			int k1 = 0;
			char *v1 = (char*)malloc(300 * sizeof(char));
			char *v2 = (char*)malloc(3 * sizeof(char));
			char *key1 = NULL;
			key1 = strtok(vect2, " \n");
			while(key1)
			{
				nod *u = prim;
				while (u != NULL)
				{
					if (strcmp(u->cheie, key1) == 0)
					{if (k1 == 0) {
							itoa(u->ind, v2, 10);
							strcpy(v1, v2); k1++;
						}
						else {
							itoa(u->ind, v2, 10);
							strcat(v1, " ");
							strcat(v1, v2);
						}
				    }
					u = u->urm;
				}
				key1 = strtok(NULL, " \n");
			}
			_caseta2= CreateWindow("static", v, WS_CHILD | WS_VISIBLE, 350, 200, 500, 70, hwnd, (HMENU)1, NULL, NULL);
			_caseta3 = CreateWindow("static", v1, WS_CHILD | WS_VISIBLE, 350, 320, 500, 70, hwnd, (HMENU)1, NULL, NULL);
			destroy();
			free(v);
			free(v1);
			free(v2);
			k = 0; k1 = 0;
			index = 0;
			}
		break;
		case RESET: 
			DestroyWindow(_caseta2);
			DestroyWindow(_caseta3);
			DestroyWindow(_caseta1);		
			_caseta1 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 350, 80, 500, 70, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
			break;	

		case INST:
		{
			WNDCLASSW wc;

			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.lpszClassName = L"Instructiuni";
			wc.hInstance = _hInstance;
			wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
			wc.lpszMenuName = NULL;
			wc.lpfnWndProc = WndProc_Instruction;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

			RegisterClassW(&wc);

			HWND _inst = CreateWindowW(L"Instructiuni", L"Instructiuni",
				WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
				0, 0, 600, 350, hwnd, NULL, _hInstance, NULL);

			ShowWindow(_inst, _nCmdShow);
			UpdateWindow(_inst);
		}
		break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_hInstance = hInstance;
	_nCmdShow = nCmdShow;
	
	WNDCLASSW wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"MainWindow";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&wc); 

	_hwnd = CreateWindowW(L"MainWindow", L"Dictionary Coding",  
		WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE, 
		0, 0, 900, 500, NULL, NULL, hInstance, NULL);

	ShowWindow(_hwnd, nCmdShow); 
	UpdateWindow(_hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {  
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}


