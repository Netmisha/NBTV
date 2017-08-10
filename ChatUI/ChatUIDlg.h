
// ChatUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "NBTV.h"
#include "afxbutton.h"
#include "afxcmn.h"

#include "GridCtrl_src\GridCtrl.h"
// CChatUIDlg dialog
class CChatUIDlg : public CDialogEx
{
// Construction
public:
	CChatUIDlg(CWnd* pParent = NULL);	// standard constructor
    ~CChatUIDlg();

// Dialog Data
	enum { IDD = IDD_CHATUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    
public:
    void  RecvLoop(AppManager& am);

    CButton MainButton;
    afx_msg void OnBnClickedMainbutton();
    CEdit ChatEdit;
    CListBox Chat;

    std::string ModeName;
    bool is_private;
    void SetPrivateMode(CString str);
    

    void OnTimer(UINT_PTR nIDEvent);
    inline void SetUserIcon();
    inline void SetUserList();
    void SetFileListP();
    void SetChatP();
    inline void SetFileList();
    void SetChat();
private:
    void ProcessMessage(const UnpackedMessage &um, AppManager& am);
    COLORREF  GetColorByIndex(char index);

   
public:
    volatile bool is_working_ = false;
    // user name next to icon
    CStatic UserNameLabel;
    afx_msg void OnStnClickedUserName();
  
    // user icon with first letter of his name
    CStatic* icon;
    // label for "Active Users"
    CStatic UserListLabel;

    afx_msg void OnStnClickedUsericon();


    // online users
    CListBox UserList;

    Thread recv_thread_;

    HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
   

    CGridCtrl FileGrid;
    // switch to chat    // switch to chat
    CButton CheckChat;
    // switch to file list
    CButton CheckFileList;

    afx_msg void OnBnClickedSwitchC();
    afx_msg void OnBnClickedSwitchFl();

    CButton AddFileButt;
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedSettingBut();

    CButton OpsButt;
    afx_msg void OnLbnDblclkList1();
};