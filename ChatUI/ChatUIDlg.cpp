
// ChatUIDlg.cpp : implementation file
//
#pragma comment(lib, "NBTV.lib")

#include "stdafx.h"
#include "ChatUI.h"
#include "ChatUIDlg.h"
#include "afxdialogex.h"
#include <comdef.h>  // you will need this

#include "NBTV.h"


void CChatUIDlg::ProcessMessage(const  UnpackedMessage &um, AppManager& am)
{
    switch(um.type_)
    {
    case CHAT_MESSAGE:
        am.AddMsg(*(UserMsg*)um.msg_);
        Chat.InsertString(Chat.GetCount(), '\n'+ CString((*(UserMsg*)um.msg_).name_.c_str())  + " : " + CString((*(UserMsg*)um.msg_).msg_.c_str()));
        break;

    case LOG_MESSAGE:
    
        switch (((LogMessage*)um.msg_)->type_)
        {
        case LOG_OFFLINE:
            Chat.InsertString(Chat.GetCount(), CString((*(LogMessage*)um.msg_).name_.c_str()) + " left the chat ");
            break;

        case LOG_ONLINE:
            Chat.InsertString(Chat.GetCount(), CString((*(LogMessage*)um.msg_).name_.c_str()) + " is online! ");
            break;

        case LOG_UPDATE:
            Chat.InsertString(Chat.GetCount(), CString((*(LogMessage*)um.msg_).name_.c_str()) + " is a new name of " + (*(LogMessage*)um.msg_).prev_name_.c_str());
            break;
        }
        break;
    }
}

void  CChatUIDlg::RecvLoop(AppManager& am)
{
    static bool is_working_ = true;
    while (is_working_)
    {
        UnpackedMessage packet = am.RecieveMessage();
        //TODO
        ProcessMessage(packet, am);

        packet.Clear();
        //Send new info to AppManager
        // And AFTER THAT refresh UI (will use GetCurrentChat() func)
    }
}


void StartRecvLoop(void* vptr)
{
    ((CChatUIDlg*)vptr)->RecvLoop(app_man);
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
    _beginthread(StartRecvLoop, 0, this);
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

void CChatUIDlg::OnBnClickedMainbutton()
{

    CString str;
    ChatEdit.GetWindowTextW(str);

    const WCHAR* wc = str;
    _bstr_t b(wc);
    const char* c = b;

   

    ChatEdit.SetWindowTextW(TEXT(""));

    std::string name = app_man.GetName();
    if (*c == '/')
    {
        if (!strncmp(c, "/userlist", 9))
        {
            std::vector<std::string> *users = (std::vector<std::string>*) app_man.ActivateCommand(std::string(c));
            for(auto i : *users)
            {
                std::string str = '\n' + i;
                Chat.InsertString(Chat.GetCount(), CString(str.c_str()));
            }
        }
        else if (!strncmp(c, "/setname", 8))
            app_man.ActivateCommand(std::string(c));
        else if (!strncmp(c, "/fl ", 4))
        {
            if (c[4] == '\0')
            {
                std::vector<File>* list = (std::vector<File>*) app_man.ActivateCommand(std::string(c));
                for (size_t i = 0; i < list->size(); i++)
                {
                    std::string str = std::to_string(i + 1) + ' ' + (*list)[i].GetName() + ' ' + std::to_string((*list)[i].GetSizeMB());
                    Chat.InsertString( Chat.GetCount(), CString( str.c_str() )  );
                }
                delete list;
            }
            else
            {
                std::vector<RecvFileInfo> *list = (std::vector<RecvFileInfo>*) app_man.ActivateCommand(std::string(c));
                for (size_t i = 0; i < list->size(); i++)
                {
                    std::string str = std::to_string(i + 1) + ' ' + (*list)[i].name_ + ' ' + std::to_string((*list)[i].size_KB_ / 1024);
                    Chat.InsertString(Chat.GetCount(), CString(str.c_str()));
                }
                delete list;
            }
        }
        else if (!strncmp(c, "/getf", 5))
            app_man.ActivateCommand(std::string(c));
        else if (!strncmp(c, "/addf", 5))
            app_man.ActivateCommand(std::string(c));
        else if (!strncmp(c, "/removef", 8))
            app_man.ActivateCommand(std::string(c));
    }
    else
    {
        app_man.SendMsg(c);
        Chat.InsertString(Chat.GetCount(), CString(name.c_str()) + CString(" : ") + str);
    }
}
