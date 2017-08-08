// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatUI.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"



// SettingsDlg dialog

IMPLEMENT_DYNAMIC(SettingsDlg, CDialog)

SettingsDlg::SettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SETTINGSDLG, pParent)
{

}

SettingsDlg::~SettingsDlg()
{
}

void SettingsDlg::SetAM(AppManager * am)
{
    am_ = am;
}

void SettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_NAMEL, SetNameLable);
    DDX_Control(pDX, IDC_COLORL, SetColorLabel);
    DDX_Control(pDX, IDC_EDIT_NAME, NameEdit);
    DDX_Control(pDX, IDC_EDIT_COLOR, ColorEdit);
}


BEGIN_MESSAGE_MAP(SettingsDlg, CDialog)
    ON_BN_CLICKED(IDOK, &SettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SettingsDlg message handlers


void SettingsDlg::OnBnClickedOk()
{
    CString name;
    NameEdit.GetWindowTextW(name);
    
    CString color;
    ColorEdit.GetWindowTextW(color);

    const WCHAR* wc = name; //unicod to ANSI magic
    _bstr_t b1(wc);
    const char* c1 = b1;

    const WCHAR* wc2 = color;
    _bstr_t b2(wc2);
    const char* c2 = b2;

    am_->SetUserInfo( std::string(b1) ,  (char)std::atoi(b2) );
    CDialog::OnOK();
}
