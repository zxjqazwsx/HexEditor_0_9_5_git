#pragma once
#include "StaticDialog.h"
#include "Hex.h"
#include "ext4.h"



// implement init Function!  

// Dlg inherit StaticDialog ,Must implement doDialog() func and run_dlgProc() Func
// and other need add for some feature!s


class SuperDlg :public StaticDialog
{

public:
	SuperDlg() :StaticDialog() {};

	void init(HINSTANCE hInst, NppData nppData)
	{
		_nppData = nppData;
		Window::init(hInst, nppData._nppHandle);
	};

	void doDialog();
	void doDialog(UINT pos);
	virtual void destroy() {
		if(super_block)
			super_block=NULL;
	};

protected :
	virtual BOOL CALLBACK run_dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void getSuperBlock(UINT star);
	void displaySuperBlock();
	int searchSuperBlock();
	int asSuperBlock(UINT pos);

private:
	/* Handles */
	NppData			_nppData;
	//HWND			_HSource;

	CHAR SuperBlock[4096];   //Save SuperBlock/DataBitBlock/InodeBitBlock

	ext4_super_block * super_block;

};

