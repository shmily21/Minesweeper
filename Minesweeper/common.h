/*
***************************************************************************************************
* 名称        ：common.h
*
* 作者        ：王斌
*
* 描述        ：
*
* 版权        ：perturbed@sina.com
*
***************************************************************************************************
*/

#ifndef __WANGBIN_MINESWEEPER_COMMON_H__
#define __WANGBIN_MINESWEEPER_COMMON_H__

/*
* For Microsoft Compiler
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif /* defined(_MSC_VER) && _MSC_VER > 1000 */

#include <Windows.h>
#include <atlimage.h>

#include <cassert>

template<class T> 
__forceinline void CheckDelete(T* tp)
{
    typedef char complete_type[sizeof(T)?1:-1];
    (void)sizeof(complete_type);
    try
    {
        delete tp;
        tp=0;
    }
    catch(...)
    {
        assert(false);
    }
}

template<class T>
__forceinline void CheckDeleteArray(T* tp)
{
    typedef char complete_type[sizeof(T)?1:-1];
    (void)sizeof(complete_type);
    try
    {
        delete [] tp;
        tp=0;
    }
    catch(...)
    {
        assert(false);
    }
}

class CImageEx:public CImage
{
public:
    CImageEx(VOID);
    virtual ~CImageEx(VOID) throw();
public:
    BOOL LoadImageFromResource(
        __in HINSTANCE hInstance,
        __in LPCTSTR pszResoureName,
        __in LPCTSTR pszResourceType);
};

struct MINESWEEPER_RUNTIME
{
    CImageEx            hImageBack;
    CImageEx            hImageNum[12];
    CImageEx            hImageClock;
    CImageEx            hImageClockItem;
    CImageEx            hImageFlag;
    INT                 nRowCount;
    INT                 nColCount;
    INT                 nMineCount;
    INT                 nCurMineCount;
    INT                 nCurRowCount;
    INT                 nCurColCount;
    BOOL                bIsOver;
    INT                 nTimer;
};

struct CELL
{
    INT             nCount;
    BOOL            bShow;
    BOOL            bFlag;
    BOOL            bWhyFlag;
};

HWND GetMainWindow();

CELL** GetCellMap();

#endif /* __WANGBIN_MINESWEEPER_COMMON_H__ */