/*
***************************************************************************************************
* 名称        ：logic.h
*
* 作者        ：王斌
*
* 描述        ：
*
* 版权        ：perturbed@sina.com
*
***************************************************************************************************
*/

#ifndef __WANGBIN_MINESWEEPER_LOGIC_H__
#define __WANGBIN_MINESWEEPER_LOGIC_H__

/*
* For Microsoft Compiler
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif /* defined(_MSC_VER) && _MSC_VER > 1000 */

#include <Windows.h>
#include "common.h"

extern MINESWEEPER_RUNTIME _g_Runtime;

BOOL SetMap(INT Width,INT Height,INT Count);

VOID ShowEmpty(INT X,INT Y);

VOID ShowAll();

BOOL IsEnd();

#endif /* __WANGBIN_MINESWEEPER_LOGIC_H__ */