#include "logic.h"
#include "common.h"
#include <cassert>
#include <ctime>

static CELL** _S_g_Map=NULL;

CELL** GetCellMap()
{
    return _S_g_Map;
}

VOID ResetSgMap(VOID)
{
    for(INT I(0);I<_g_Runtime.nRowCount;I++)
    {
        CheckDeleteArray<CELL>(_S_g_Map[I]);
    }
    CheckDeleteArray<CELL*>(_S_g_Map);
}

// ÉèÖÃÀ×Çø
BOOL SetMap(INT Width,INT Height,INT Count)
{
    if(Count<=0) Count=1;
    if(NULL!=_S_g_Map)
    {
        for(INT I(0);I<_g_Runtime.nRowCount;I++)
        {
            CheckDeleteArray<CELL>(_S_g_Map[I]);
        }
        CheckDeleteArray<CELL*>(_S_g_Map);
    }
    _g_Runtime.nColCount=Width;
    _g_Runtime.nRowCount=Height;

    _S_g_Map=new CELL*[_g_Runtime.nRowCount];
    for(INT I(0);I<_g_Runtime.nRowCount;I++)
    {
        _S_g_Map[I]=new CELL[_g_Runtime.nColCount];
    }
    for(INT I(0);I<_g_Runtime.nColCount;I++)
    {
        for(INT J(0);J<_g_Runtime.nColCount;J++)
        {
            _S_g_Map[I][J].bShow=FALSE;
            _S_g_Map[I][J].bFlag=FALSE;
            _S_g_Map[I][J].bWhyFlag=FALSE;
            _S_g_Map[I][J].nCount=0;
        }
    }

    srand((unsigned int)(time(NULL)));
    INT nMineCount(0);
    while(nMineCount<Count)
    {
        INT nX=rand()%_g_Runtime.nColCount;
        INT nY=rand()%_g_Runtime.nRowCount;

        if(-1!=_S_g_Map[nX][nY].nCount)
        {
            _S_g_Map[nX][nY].nCount=-1;
            nMineCount++;
        }
    }
    for(INT I(0);I<_g_Runtime.nRowCount;I++)
    {
        for(INT J(0);J<_g_Runtime.nColCount;J++)
        {
            if(0==_S_g_Map[I][J].nCount)
            {
                for(INT X(I-1);X<I+2;X++)           //LEFT UP -> RIGHT DOWN
                {
                    for(INT Y(J-1);Y<J+2;Y++)
                    {
                        if(X>=0 && X<_g_Runtime.nRowCount && Y>=0 && Y<_g_Runtime.nColCount)
                        {
                            if(-1==_S_g_Map[X][Y].nCount)
                            {
                                _S_g_Map[I][J].nCount++;
                            }
                        }
                    }
                }
            }
        }
    }
    _g_Runtime.nCurColCount=_g_Runtime.nColCount;
    _g_Runtime.nCurRowCount=_g_Runtime.nCurRowCount;
    _g_Runtime.nMineCount=nMineCount;
    return TRUE;
}

VOID ShowEmpty(INT nPosRow,INT nPosCol)
{
    if(nPosRow<0 || nPosRow>_g_Runtime.nRowCount || nPosCol<0 || nPosCol>_g_Runtime.nColCount) return ;
    for(INT RowIndex(nPosRow-1);RowIndex<nPosRow+2;RowIndex++)
    {
        for(INT ColIndex(nPosCol-1);ColIndex<nPosCol+2;ColIndex++)
        {
            if(ColIndex>=0 && RowIndex>=0 && ColIndex<_g_Runtime.nColCount && RowIndex<_g_Runtime.nRowCount)
            {
                if(0==_S_g_Map[RowIndex][ColIndex].nCount && (!_S_g_Map[RowIndex][ColIndex].bShow))
                { 
                    _S_g_Map[RowIndex][ColIndex].bShow=TRUE;
                    ShowEmpty(RowIndex,ColIndex);
                }
                _S_g_Map[RowIndex][ColIndex].bShow=TRUE;
            }
        }
    }
}

VOID ShowAll()
{
    for(INT I(0);I<_g_Runtime.nColCount;I++)
    {
        for(INT J(0);J<_g_Runtime.nRowCount;J++)
        {
            _S_g_Map[J][I].bShow=TRUE;
        }
    }
}

BOOL IsEnd()
{
    for(INT I(0);I<_g_Runtime.nColCount;I++)
    {
        for(INT J(0);J<_g_Runtime.nRowCount;J++)
        {
            if(-1==_S_g_Map[I][J].nCount)
            {
                if(!_S_g_Map[I][J].bFlag) return FALSE;
            }
        }
    }
    return TRUE;
}
