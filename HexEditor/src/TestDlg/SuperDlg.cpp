#include "SuperDlg.h"
#include "PluginInterface.h"
#include "resource.h"
#include "atlstr.h"
#include "Hex.h"

// implement  Dlg doDialog Function and run_dlgProc Function!

void SuperDlg::doDialog()
{
	if (!isCreated())
		create(IDD_DLG_SUPER);

	if(searchSuperBlock()!= -1)
		displaySuperBlock();

	goToCenter();
}

void SuperDlg::doDialog(UINT pos)
{
	if(asSuperBlock(pos)!=-1){

		if (!isCreated())
			create(IDD_DLG_SUPER);
		displaySuperBlock();
		goToCenter();
		display(true);
	}
}

//static CHAR  Magic[2] = "\0";

BOOL CALLBACK SuperDlg::run_dlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message) 
	{
	case WM_INITDIALOG :
		{
#if 0
			if(searchSuperBlock()!= -1);
			//getSuperBlock(1024);
				displaySuperBlock();

#endif
			return TRUE;
		}
	case WM_COMMAND : 
		{
			switch (wParam)
			{
			case IDOK :
			case IDCANCEL :
				display(FALSE);
				return TRUE;

			default :
				break;
			}
			break;
		}
	}
	return FALSE;
}

void SuperDlg::getSuperBlock(UINT start)
{
	ScintillaGetText(_nppData._scintillaMainHandle,SuperBlock, start, start+ 4096);
}
void SuperDlg::displaySuperBlock()
{
	super_block = (ext4_super_block*)SuperBlock;
	
	CString str=_T("");

	//0x00 32bit Total Inode Count
	str.Format(_T("0x00 Inodes Count:%d"),super_block->s_inodes_count);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_INODE_COUNT),str);

	//0x04 32bit Total Block Count Lo
	str.Format(_T("0x04 Blocks Count Lo:%d"),super_block->s_blocks_count_lo);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_BLOCK_COUNT_LO),str);

	//0x08 32bit Reserved Block Count Lo
	str.Format(_T("0x08 Reserved Block Lo:%d"),super_block->s_r_blocks_count_lo);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_RES_BLOCK_LO),str);

	//0x0C 32bit Free Blocks Lo
	str.Format(_T("0x0C Free Blocks Lo:%d"),super_block->s_free_blocks_count_lo);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_FREE_BLOCK_LO),str);

	//0x10 32bit Free Inodes
	str.Format(_T("0x10 Free Inodes:%d"),super_block->s_free_inodes_count);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_FREE_INODE),str);

	//0x14 32bit First Date Block
	str.Format(_T("0x14 First Date Block:%d"),super_block->s_first_data_block);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_FIRST_DATA),str);

	//0x18 32bit Block Size
	str.Format(_T("0x18 Block Size:%d"),super_block->s_log_block_size<<10);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_BLOCK_SIZE),str);

	//0x20 32bit Blocks Pre Group
	str.Format(_T("0x20 Blocks Pre Group:%d"),super_block->s_blocks_per_group);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_BLOCK_PRE_G),str);

	//0x28 32bit Inodes Pre Group
	str.Format(_T("0x28 Inodes Pre Group:%d"),super_block->s_inodes_per_group);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_INODE_PRE_G),str);

	//0x38 16bit Magic number, always EF53
	str.Format(_T("0x38 Magic[0xEF53]:%X"),super_block->s_magic);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_MAGIC),str);

	//0x3A 16bit State [1 Clean 2 Errors 3 Orephans Begin recovery]
	str.Format(_T("0x3A State:%d"),super_block->s_state);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_STATE),str);

	//0x54 32bit First Inode
	str.Format(_T("0x54 First Inode:%d"),super_block->s_first_ino);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_FIRST_INO),str);

	//0x58 16bit Inode Size
	str.Format(_T("0x58 Inode Size:%d"),super_block->s_inode_size);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_INODE_SIZE),str);

	//0x5A 16bit  Block Group #
	str.Format(_T("0x5A Block Group #:%d"),super_block->s_block_group_nr);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_BLOCK_G_N),str);

	//0x5C 32bit Feature Compat
	str.Format(_T("0x5C Feature Compat:%X"),super_block->s_feature_compat);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_FEATURE_COMPAT),str);

	//0x60 32bit Feature inCompao
	str.Format(_T("0x60 Feature inCompat:%X"),super_block->s_feature_incompat);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_FEATURE_INCOMPAT),str);

	//0x64 32bit Feature RO Compat
	str.Format(_T("0x64 Feature RO Compat:0x%X"),super_block->s_feature_ro_compat);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_FEATURE_RO_COMPAT),str);

	//0x04 16byte UUID
	str.Format(_T("0x68 UUID:%X"),super_block->s_uuid);
	::SetWindowText(::GetDlgItem(_hSelf, IDC_STATIC_UUID),str);
	
}

int SuperDlg::searchSuperBlock()
{
	UINT tempStart;
	UINT i;
	for(i=0; i<=10 ; i++)
	{
		tempStart=i * 1024;
		getSuperBlock(tempStart);

		super_block = (ext4_super_block*)SuperBlock;
		if( super_block->s_magic != 0xef53)
			continue;
		else
			break;
	}

	if (i <=10 )
	{
		::MessageBox(
			NULL,
			(LPCWSTR)L"Super Block Found! !",
			(LPCWSTR)L"OK",
			MB_OKCANCEL
			);
		return i;
	}else
	{
		::MessageBox(
			NULL,
			(LPCWSTR)L"Super Block Not Found!",
			(LPCWSTR)L"Fail",
			MB_ICONERROR | MB_OKCANCEL
			);
		return -1;
	}
}

int SuperDlg::asSuperBlock(UINT pos)
{	
	getSuperBlock(pos);

	super_block = (ext4_super_block*)SuperBlock;
	if( super_block->s_magic != 0xef53){
		::MessageBox(
			NULL,
			(LPCWSTR)L"Super Block Magic Number Invalid!",
			(LPCWSTR)L"ERROR",
			MB_ICONERROR | MB_OKCANCEL
			);

		return -1;

	}
	else{
		return 0;
	}
}