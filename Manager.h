// Manager.h: interface for the CManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MANAGER_H__3EB5880A_2A3A_11D5_947A_00C0262C68EE__INCLUDED_)
#define AFX_MANAGER_H__3EB5880A_2A3A_11D5_947A_00C0262C68EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"

class CManager  
{
public:
	int admin;
	void ShowTree(long n,int num);
	fstream *lnk,*msg;
	fstream *lnk1,*msg1;	//дл€ gc
	void gc();				//сборщик мусора
	void ShowNew();			//показывает новые собщени€
	void ShowAll();			//показывает все собщени€
	void ShowNode(int n);	//показывает один узел
	CManager();
	virtual ~CManager();

};

#endif // !defined(AFX_MANAGER_H__3EB5880A_2A3A_11D5_947A_00C0262C68EE__INCLUDED_)
