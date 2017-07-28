
// ChatUIDlg.cpp : implementation file
//
#pragma comment(lib, "NBTV.lib")

#include "stdafx.h"
#include "ChatUI.h"
#include "ChatUIDlg.h"
#include "afxdialogex.h"
#include <comdef.h>  // you will need this

#include "NBTV.h"


void CChatUIDlg::ProcessMessage(const RecvStruct &recv_str, AppManager& am)
{
    if ((recv_str.ip_ != am.GetIP()))
    {
        //allocation in heap
        UnpackedMessage unp_msg = Parcer::UnpackMessage(recv_str.packet_);

        switch (unp_msg.type_)
        {
        case CHAT_MESSAGE:
            am.AddMsg(*(UserMsg*)unp_msg.msg_);
            Chat.InsertString(Chat.GetCount(), '\n' + CString((*(UserMsg*)unp_msg.msg_).msg_.c_str()));
            break;

        case LOG_MESSAGE:
            am.ProcessLogMessage(*(LogMessage*)unp_msg.msg_, recv_str.ip_);
            break;

        case GET_FILE_MESSAGE:
            am.SendFile(am.GetFilePath(*((int*)unp_msg.msg_)),
                        recv_str.ip_,
                        am.GetFileName(*((int*)unp_msg.msg_)));
            break;

        case FILE_LIST_REQUEST:
            am.SendList(recv_str.ip_);
            break;

        }

        delete unp_msg.msg_;
    }

}
void  CChatUIDlg::RecvLoop(AppManager& am)
{
    static bool is_working_ = true;
    while (is_working_)
    {
        UnpackedMessage packet = am.RecieveMessage();
        //TODO
        //ProcessMessage(packet, am);

        packet.Clear();
        //Send new info to AppManager
        // And AFTER THAT refresh UI (will use GetCurrentChat() func)
    }
}

// CChatUIDlg dialog



CChatUIDlg::CChatUIDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CChatUIDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatUIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MAINBUTTON, MainButton);
    DDX_Control(pDX, IDC_CHAT_EDIT, ChatEdit);
    DDX_Control(pDX, IDC_CHAT, Chat);
}

BEGIN_MESSAGE_MAP(CChatUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_MAINBUTTON, &CChatUIDlg::OnBnClickedMainbutton)
END_MESSAGE_MAP()


// CChatUIDlg message handlers

BOOL CChatUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

AppManager* GetAM()
{
    static AppManager am ;
    return &am;
}

void CChatUIDlg::OnBnClickedMainbutton()
{
    CString str;
    ChatEdit.GetWindowTextW(str); 
    const WCHAR* wc = str;
    _bstr_t b(wc);
    const char* c = b;
    GetAM()->SendMsg(c);

    str+= '\n';

    ChatEdit.SetWindowTextW(TEXT(""));

    Chat.InsertString(Chat.GetCount(), str);
}
