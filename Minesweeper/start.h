/*
***************************************************************************************************
* 名称        ：start.h
*
* 作者        ：王斌
*
* 描述        ：
*
* 版权        ：perturbed@sina.com
*
***************************************************************************************************
*/

#ifndef __WANGBIN_MINESWEEPER_START_H__
#define __WANGBIN_MINESWEEPER_START_H__

/*
* For Microsoft Compiler
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif /* defined(_MSC_VER) && _MSC_VER > 1000 */

#include <Windows.h>
#include "common.h"
#include <atlimage.h>
extern MINESWEEPER_RUNTIME _g_Runtime;

BOOL InitRuntime(INT nRowCount,INT nColCount,INT mMineCount=15);

INT APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPreInstance,LPSTR lpCmdLine,INT nCmdShow);

#endif /* __WANGBIN_MINESWEEPER_START_H__ */