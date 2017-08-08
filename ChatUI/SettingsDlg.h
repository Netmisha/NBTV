#pragma once
#include "NBTV.h"
#include "afxwin.h"

// SettingsDlg dialog

class SettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(SettingsDlg)

public:
	SettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SettingsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGSDLG };
#endif

    void SetAM(AppManager* am);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
    AppManager* am_;
public:
    afx_msg void OnBnClickedOk();
    CStatic SetNameLable;
    CStatic SetColorLabel;
    CEdit NameEdit;
    CEdit ColorEdit;
};
