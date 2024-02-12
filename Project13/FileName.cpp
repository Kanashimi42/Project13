#include <windows.h>
#include <vector>
#include <tchar.h>
#include "windowsx.h"
#include "resource.h"
using namespace std;
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HWND hStatic1, hStatic2;
HINSTANCE hInst;
TCHAR szCoordinates[20];


UINT_PTR timerId;
POINT startPoint;
POINT endPoint;

bool f = true;
int staticId = 0;

vector<vector<int>> staticArray = {};

void UpdateCoordinates(HWND);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow) {
    hInst = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CLOSE: {
        EndDialog(hWnd, 0);

        return TRUE;
    }
    case WM_INITDIALOG: {
        hStatic1 = GetDlgItem(hWnd, DLGC_STATIC);
        SetWindowText(hWnd, _TEXT("Dialogue"));

        return TRUE;
    }
    case WM_LBUTTONDOWN: {
        timerId = SetTimer(hWnd, 1, 10, NULL);
        startPoint.x = GET_X_LPARAM(lParam);
        startPoint.y = GET_Y_LPARAM(lParam);

        return TRUE;
    }


    case WM_RBUTTONDOWN: {
        POINTS points = MAKEPOINTS(lParam);
        POINT point;

        _TCHAR buff[256];

        point.x = points.x;
        point.y = points.y;

        HWND hClickedStatic = ChildWindowFromPoint(hWnd, point);

        if (hClickedStatic != NULL && hClickedStatic != hWnd) {
            HWND hChild = ChildWindowFromPointEx(hWnd, point, CWP_ALL);
            int uniqueNumber = GetWindowLongPtr(hChild, GWLP_USERDATA);

            wsprintf(buff, _TEXT("id: %d. start x: %d, start y: %d. w: %d, h: %d"), uniqueNumber,
                staticArray[uniqueNumber][0], staticArray[uniqueNumber][1],
                staticArray[uniqueNumber][2], staticArray[uniqueNumber][3]);
            SetWindowText(hWnd, buff);
        }


        return TRUE;
    }

    case WM_LBUTTONUP: {
        KillTimer(hWnd, timerId);
        endPoint.x = GET_X_LPARAM(lParam);
        endPoint.y = GET_Y_LPARAM(lParam);

        if (endPoint.x - startPoint.x < 10 || endPoint.y - startPoint.y < 10) {
            SetWindowText(hWnd, _TEXT("Minimum size 10x10"));

        }
        else {
            hStatic2 = CreateWindowEx(0, TEXT("STATIC"), 0,
                WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE,
                (endPoint.x - startPoint.x < 0 ? endPoint.x : startPoint.x),
                (endPoint.y - startPoint.y < 0 ? endPoint.y : startPoint.y),
                (endPoint.x - startPoint.x < 0 ? startPoint.x - endPoint.x : endPoint.x - startPoint.x),
                (endPoint.y - startPoint.y < 0 ? startPoint.y - endPoint.y : endPoint.y - startPoint.y),
                hWnd, 0, hInst, 0);

            SetWindowLongPtr(hStatic2, GWLP_USERDATA, staticId++);
            SetWindowText(hStatic2, 0);

            staticArray.push_back({ endPoint.x - startPoint.x < 0 ? endPoint.x : startPoint.x, endPoint.y - startPoint.y < 0 ? endPoint.y : startPoint.y,
                endPoint.x - startPoint.x < 0 ? startPoint.x - endPoint.x : endPoint.x - startPoint.x,
                endPoint.y - startPoint.y < 0 ? startPoint.y - endPoint.y : endPoint.y - startPoint.y });
        }

        return TRUE;
    }
    case WM_LBUTTONDBLCLK: {
        POINTS points = MAKEPOINTS(lParam);
        POINT point;

        point.x = points.x;
        point.y = points.y;

        HWND hClickedStatic = ChildWindowFromPoint(hWnd, point);

        DestroyWindow(hClickedStatic);

        return TRUE;
    }
    case WM_TIMER: {
        UpdateCoordinates(hWnd);
        return TRUE;
    }
    }

    return FALSE;
}

void UpdateCoordinates(HWND hWnd) {
    POINT pt;
    WCHAR str[20];

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    wsprintf(str, L"x: %d, y: %d", pt.x, pt.y);
    SetWindowText(hStatic1, str);
}
