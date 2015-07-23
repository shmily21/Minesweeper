#include "start.h"
#include "common.h"
#include "logic.h"
#include <cmath>
#include <atlstr.h>
#include <Windows.h>
#include <WinBase.h>
#include <CommCtrl.h>
#include <WindowsX.h>

#define FRAME_CLASS_NAME    TEXT("UIMineSweeper")
#define CELLWIDTH 35
#define CELLHEIGHT 35
#define ROW_SPX 40
#define ROW_SPY 80

static HWND _S_g_MainHwnd(NULL);

MINESWEEPER_RUNTIME _g_Runtime;

BOOL OnUICreate(HWND hWnd,LPCREATESTRUCT param)
{
    _g_Runtime.bIsOver=FALSE;
    _g_Runtime.nCurMineCount=0;
    _g_Runtime.nTimer=0;

//    InitRuntime(10,10,15);

    return TRUE;
}

VOID OnUIDestroy(HWND hWnd)
{
    ::PostQuitMessage(EXIT_SUCCESS);
}

VOID OnUIPaint(HWND hWnd)
{
    ::PAINTSTRUCT ps;
    HDC hDC=::BeginPaint(hWnd,&ps);
    if(hDC)
    {
        ::RECT rcClient,rcClientSRC;
        ::SetRectEmpty(&rcClient);
        ::GetClientRect(hWnd,&rcClient);
        ::CopyRect(&rcClientSRC,&rcClient);
        ::SetBkColor(hDC,RGB(50,50,60));
        ::SetRect(&rcClient,0,0,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top);
        ::ExtTextOut(hDC,rcClient.left,rcClient.top,ETO_OPAQUE,&rcClient,NULL,0,NULL);
        
        _g_Runtime.hImageClock.Draw(hDC,rcClient.right/2-140,10);
        _g_Runtime.hImageFlag.Draw(hDC,rcClient.right/2+70,10);

        _g_Runtime.hImageNum[1].Draw(hDC,rcClient.right/2+70,50);

        INT nPosX(0),nPosY(0);
        for(INT I(0);I<_g_Runtime.nRowCount;I++)
        {
            for(INT J(0);J<_g_Runtime.nColCount;J++)
            {
                nPosX=ROW_SPX+I*CELLWIDTH;
                nPosY=ROW_SPY+J*CELLHEIGHT;

                _g_Runtime.hImageBack.Draw(hDC,nPosX,nPosY);
                if(NULL!=GetCellMap())
                {
                    INT nCount=GetCellMap()[I][J].nCount;
                    
                    if(FALSE!=GetCellMap()[I][J].bShow)
                    {
                        
                        if(-1==nCount)
                        {
                            _g_Runtime.hImageNum[9].Draw(hDC,nPosX,nPosY);
                        }
                        else
                        {
                            _g_Runtime.hImageNum[nCount].Draw(hDC,nPosX,nPosY);
                        }
                    }
                    else
                    {
                        
                        if(GetCellMap()[I][J].bFlag)
                        {
                            _g_Runtime.hImageNum[10].Draw(hDC,nPosX,nPosY);
                        }

                        if(GetCellMap()[I][J].bWhyFlag)
                        {
                            _g_Runtime.hImageNum[11].Draw(hDC,nPosX,nPosY);
                        }

                    }
                }
                else
                {
                    //assert(false);
                }

            }
        }
        ::EndPaint(hWnd,&ps);
    }
}

VOID OnLButtonDown(HWND hWnd,BOOL fDoubleClick,INT X,INT Y,UINT keyFlags)
{
    if(_g_Runtime.bIsOver) return;
    INT nPosX(0),nPosY(0);
    for(INT I(0);I<_g_Runtime.nRowCount;I++)
    {
        for(INT J(0);J<_g_Runtime.nColCount;J++)
        {
            nPosX=ROW_SPX+I*CELLWIDTH;
            nPosY=ROW_SPY+J*CELLHEIGHT;
            RECT rcToken;
            ::SetRect(&rcToken,nPosX,nPosY,nPosX+CELLWIDTH,nPosY+CELLHEIGHT);
            POINT ptClick={X,Y};
            if(::PtInRect(&rcToken,ptClick))
            {
                if(GetCellMap()[I][J].bShow)
                {
                    return ;
                }
                else
                {
                    GetCellMap()[I][J].bShow=TRUE;
                    ::InvalidateRect(hWnd,&rcToken,TRUE);
                    if(-1==GetCellMap()[I][J].nCount)
                    {
                        ShowAll();
                        ::InvalidateRect(hWnd,NULL,FALSE);
                    }
                    else
                    {
                        if(0==GetCellMap()[I][J].nCount)
                        {
                            ShowEmpty(I,J);
                            ::InvalidateRect(hWnd,NULL,FALSE);
                        }
                    }
                }
            }
            else
            {
                //::OutputDebugString(TEXT("Not In Rect\n"));
            }
        }
    }
    
}

VOID OnRButtonDown(HWND hWnd,BOOL fDoubleClieck,INT X,INT Y,UINT keyFlags)
{
    if(_g_Runtime.bIsOver) return;
    INT nPosX(0),nPosY(0);
    for(INT I(0);I<_g_Runtime.nRowCount;I++)
    {
        for(INT J(0);J<_g_Runtime.nColCount;J++)
        {
            nPosX=ROW_SPX+I*CELLWIDTH;
            nPosY=ROW_SPY+J*CELLHEIGHT;
            RECT rcToken;
            ::SetRect(&rcToken,nPosX,nPosY,nPosX+CELLWIDTH,nPosY+CELLHEIGHT);
            POINT ptClick={X,Y};
            if(::PtInRect(&rcToken,ptClick))
            {
                //GetCellMap()[I][J].bShow=TRUE;
                if(GetCellMap()[I][J].bShow)
                {
                    return ;
                }
                else
                {
                    GetCellMap()[I][J].bFlag=GetCellMap()[I][J].bFlag?FALSE:TRUE;
                    ::InvalidateRect(hWnd,&rcToken,FALSE);
                    if(GetCellMap()[I][J].bFlag)
                    {
                        _g_Runtime.nMineCount++;
                    }
                    else
                    {
                        _g_Runtime.nMineCount--;
                    }
                }
            }
            else
            {
                //::OutputDebugString(TEXT("Not In Rect\n"));
            }
        }
    }
}

VOID OnTimer(HWND hWnd,UINT id)
{

}

CImageEx::CImageEx()
{

}

CImageEx::~CImageEx() throw()
{

}


BOOL CImageEx::LoadImageFromResource(
    HINSTANCE   hInstance,
    LPCTSTR     pszResourceName,
    LPCTSTR     pszResourceType
    )
{
    HRSRC hRes=::FindResource(hInstance,pszResourceName,pszResourceType);
    if(!hRes) return FALSE;
    DWORD dwImageSize=::SizeofResource(hInstance,hRes);
    LPVOID pImageBuffer=::LoadResource(hInstance,hRes);

    IStream* pStream(NULL);
    if(S_OK!=::CreateStreamOnHGlobal(NULL,TRUE,&pStream)) return FALSE;
    pStream->Write(pImageBuffer,dwImageSize,NULL);
    HRESULT hr=CImage::Load(pStream);
    pStream->Release();
    pStream=NULL;
    if(S_OK!=hr)
    {
        return FALSE;
    }
    else
    {
        if(CImage::IsNull()) return FALSE;
        if(32==CImage::GetBPP())
        {
            BITMAP bmp;
            HBITMAP hBmp=(HBITMAP)*this;
            ::GetObject(hBmp,sizeof(BITMAP),&bmp);
            if(32!=bmp.bmBitsPixel || NULL==bmp.bmBits) return FALSE;
            BYTE* pByte=(BYTE*)bmp.bmBits;

            for(INT nPosHeight(0);nPosHeight<std::abs(bmp.bmHeight);nPosHeight++)
            {
                for(INT nPosWidth=bmp.bmWidth;nPosWidth>0;nPosWidth--)
                {
                    BYTE alpha=pByte[3];
                    pByte[0]=(BYTE)((pByte[0]*alpha)/255);
                    pByte[1]=(BYTE)((pByte[1]*alpha)/255);
                    pByte[2]=(BYTE)((pByte[2]*alpha)/255);
                    pByte+=4;
                }
            }
            return TRUE;
        }
        else
            return TRUE;
    }
}


HWND GetMainWindow()
{
    return _S_g_MainHwnd;
}

BOOL InitRuntime(INT nRowCount,INT nColCount,INT nMineCount)
{
    if(nRowCount*nColCount<=nMineCount && nMineCount<=0) return FALSE;
    INT nWidth=nRowCount*CELLWIDTH+ROW_SPX*2;
    INT nHeight=nColCount*CELLHEIGHT+ROW_SPY*2;
    _g_Runtime.bIsOver=FALSE;
    ::RECT rcWin;
    ::SetRectEmpty(&rcWin);
    ::GetWindowRect(GetMainWindow(),&rcWin);
    INT nDisX=nWidth-(rcWin.right-rcWin.left);
    INT nDisY=nHeight-(rcWin.bottom-rcWin.top);
    ::SetWindowPos(GetMainWindow(),NULL,rcWin.left-nDisX/2,rcWin.top-nDisY/2,nWidth,nHeight,0);
    _g_Runtime.nTimer=0;
    ::KillTimer(GetMainWindow(),0);
    ::SetTimer(GetMainWindow(),0,1000,NULL);
    SetMap(nColCount,nRowCount,nMineCount);
    return TRUE;
}

BOOL InitResource(HINSTANCE hInstance)
{
    _g_Runtime.hImageBack.LoadImageFromResource(hInstance,TEXT("CELL_BACK"),TEXT("PNG"));
    _g_Runtime.hImageClock.LoadImageFromResource(hInstance,TEXT("CLOCK"),TEXT("PNG"));
    _g_Runtime.hImageClockItem.LoadImageFromResource(hInstance,TEXT("CLOCK_ITEM"),TEXT("PNG"));
    _g_Runtime.hImageFlag.LoadImageFromResource(hInstance,TEXT("MINEFLAG"),TEXT("PNG"));

    CString str;
    for(INT _I(0);_I<12;_I++)
    {
        str.Format(TEXT("CELL_%d"),_I);
        _g_Runtime.hImageNum[_I].LoadImageFromResource(hInstance,str,TEXT("PNG"));
        assert(!_g_Runtime.hImageNum[_I].IsNull());
    }
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
    switch(Msg)
    {
        HANDLE_MSG(hWnd,WM_CREATE,OnUICreate);
        HANDLE_MSG(hWnd,WM_DESTROY,OnUIDestroy);
        HANDLE_MSG(hWnd,WM_PAINT,OnUIPaint);
        HANDLE_MSG(hWnd,WM_LBUTTONDOWN,OnLButtonDown);
        HANDLE_MSG(hWnd,WM_RBUTTONDOWN,OnRButtonDown);
        HANDLE_MSG(hWnd,WM_TIMER,OnTimer);
    default:break;
    }
    return ::DefWindowProc(hWnd,Msg,wParam,lParam);
}

static ATOM RegisterFrameClass(HINSTANCE hInstance)
{
    ::WNDCLASSEX wcex={ sizeof(WNDCLASSEX),0,WndProc,0,0,(HINSTANCE)::GetModuleHandle(NULL)
        ,NULL,::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)),(::HBRUSH)(COLOR_APPWORKSPACE)
        ,NULL,FRAME_CLASS_NAME,NULL};
    return ::RegisterClassEx(&wcex);
}

BOOL InitWindow(HINSTANCE hInstance)
{
    INT _start_pos_width(120),_start_pos_height(120);
    INT _start_pos_x((::GetSystemMetrics(SM_CXSCREEN)-_start_pos_width)/2);
    INT _start_pos_y((::GetSystemMetrics(SM_CYSCREEN)-_start_pos_height)/2);
    _S_g_MainHwnd=::CreateWindowEx(0,FRAME_CLASS_NAME,TEXT("Minesweeper"),WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
        _start_pos_x,_start_pos_y,_start_pos_width,_start_pos_height,::GetDesktopWindow(),NULL,hInstance,NULL);
    
    if(!_S_g_MainHwnd) return FALSE;

    InitRuntime(12,12,18);
    InitResource(hInstance);
    ::ShowWindow(_S_g_MainHwnd,SW_SHOW);
    ::UpdateWindow(_S_g_MainHwnd);
    return TRUE;
}

INT APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPreInstance,LPSTR lpCmdLine,INT nCmdShow)
{
    UNREFERENCED_PARAMETER(hPreInstance);
    //::INITCOMMONCONTROLSEX _COM_CTRL={sizeof(ICC_STANDARD_CLASSES),ICC_STANDARD_CLASSES};
    //::InitCommonControlsEx(&_COM_CTRL);
    if(!RegisterFrameClass(hInstance) || !InitWindow(hInstance) ) return EXIT_FAILURE;
    ::MSG Msg;
    ::ZeroMemory(&Msg,sizeof(Msg));
    while(WM_QUIT!=Msg.message)
    {
        if(::PeekMessage(&Msg,NULL,0U,0U,PM_REMOVE))
        {
            ::TranslateMessage(&Msg);
            ::DispatchMessage(&Msg);
        }
        else
            Sleep(10);
    }
    ::UnregisterClass(FRAME_CLASS_NAME,hInstance);
    return EXIT_SUCCESS;
}