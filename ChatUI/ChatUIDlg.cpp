
// ChatUIDlg.cpp : implementation file
//
#pragma comment(lib, "NBTV.lib")

#include "stdafx.h"
#include "ChatUI.h"
#include "ChatUIDlg.h"
#include "afxdialogex.h"
#include <comdef.h>  // you will need this

#include "NBTV.h"
#include "Color.h"


void CChatUIDlg::ProcessMessage(const  UnpackedMessage &um, AppManager& am)
{
    switch(um.type_)
    {
    case CHAT_MESSAGE:
        Chat.InsertString(Chat.GetCount(), '\n'+ CString((*(UserMsg*)um.msg_).name_.c_str())  + " : " + CString((*(UserMsg*)um.msg_).msg_.c_str()));
        break;

    case LOG_MESSAGE:
    
        switch (((LogMessage*)um.msg_)->type_)
        {
        case LOG_OFFLINE:
            SetUserList();
            Chat.InsertString(Chat.GetCount(), CString((*(LogMessage*)um.msg_).name_.c_str()) + " left the chat ");
            break;

        case LOG_ONLINE:
            SetUserList();
            Chat.InsertString(Chat.GetCount(), CString((*(LogMessage*)um.msg_).name_.c_str()) + " is online! ");
            break;

        case LOG_UPDATE:
            SetUserList();
            if(!(*(LogMessage*)um.msg_).prev_name_.empty())
                Chat.InsertString(Chat.GetCount(), CString((*(LogMessage*)um.msg_).name_.c_str()) + " is a new name of " + (*(LogMessage*)um.msg_).prev_name_.c_str());
            break;
        }
        break;
    }
}

COLORREF CChatUIDlg::GetColorByIndex(char index)
{
    switch (index)
    {
    case 1: return RED;         break;
    case 2: return GREEN;       break;
    case 3: return BLUE;        break;
    case 4: return LIGHTRED;    break;
    case 5: return LIGHTGREEN;  break;
    case 6: return LIGHTBLUE;   break;
    case 7: return BLACK;       break;
    case 8: return GRAY;        break;
    default:return BLACK;       break;
    }
}

void CChatUIDlg::SetUserIcon()
{
    CImage image_;
    std::string name = app_man.GetName();
    std::string path("Resourses\\");
    path += ((char)toupper(name[0]));
    path += ".png";
    image_.Load(CString(path.c_str()));

    CBitmap bitmap_;
    bitmap_.Attach(image_.Detach());
    icon->SetBitmap(bitmap_);

}

void CChatUIDlg::SetUserList()
{

    std::vector<UserInfo>* list = (std::vector<UserInfo>*)app_man.ActivateCommand(std::string("/userlist"));
    CListBox *usr_list = (CListBox *)GetDlgItem(IDC_LIST1);
    usr_list->ResetContent();
    if(list->size() != 0)
    for (auto i : *list)
    {

        usr_list->InsertString(usr_list->GetCount(), CString(i.user_name_.c_str()));
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


unsigned StartRecvLoop(void* vptr)
{
    ((CChatUIDlg*)vptr)->RecvLoop(app_man);
    return 0;
}
// CChatUIDlg dialog

CChatUIDlg::CChatUIDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CChatUIDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CChatUIDlg::~CChatUIDlg()
{
    app_man.StopNetwork();
    recv_thread_.Join();
}

void CChatUIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MAINBUTTON, MainButton);
    DDX_Control(pDX, IDC_CHAT_EDIT, ChatEdit);
    DDX_Control(pDX, IDC_CHAT, Chat);
    DDX_Control(pDX, IDC_USER_NAME, UserNameLabel);
    DDX_Control(pDX, IDC_USERLIST_LABEL, UserListLabel);
}

BEGIN_MESSAGE_MAP(CChatUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_MAINBUTTON, &CChatUIDlg::OnBnClickedMainbutton)
    ON_STN_CLICKED(IDC_USER_NAME, &CChatUIDlg::OnStnClickedUserName)
    ON_STN_CLICKED(IDC_USERICON, &CChatUIDlg::OnStnClickedUsericon)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CChatUIDlg message handlers
CStatic* icon;

BOOL CChatUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    app_man.LoadUserInfo();
  
    UserNameLabel.SetWindowTextW(CString(app_man.GetName().c_str()));
    UserListLabel.SetWindowTextW(CString("Active Users: "));

    icon = (CStatic*)GetDlgItem(IDC_USERICON);
   
    recv_thread_.BeginThread(StartRecvLoop, this);
    


    SetUserIcon();
    SetUserList();
    


  
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    CFont font;
    font.CreatePointFont(65, TEXT("Arial"));
    GetDlgItem(IDC_USER_NAME)->SetFont(&font, TRUE);

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


void CChatUIDlg::OnBnClickedMainbutton() //msg sending
{
   /* CListBox *usr_list = (CListBox *)GetDlgItem(IDC_LIST1);
    usr_list->InsertString(usr_list->GetCount(), CString("sssss"));*/

    CString str;
    ChatEdit.GetWindowTextW(str);

    const WCHAR* wc = str;
    _bstr_t b(wc);
    const char* c = b;

  
    ChatEdit.SetWindowTextW(TEXT(""));

    std::string name = app_man.GetName();
   
    if (*c == '/')
    {
        if (!strncmp(c, "/w ", 3))
        {
            void* is_ok = app_man.ActivateCommand(std::string(c));
            if (is_ok != NULL)
            {
                std::string str = "\n NO USER WITH THIS NAME ";
                Chat.InsertString(Chat.GetCount(), CString(str.c_str()));
                delete is_ok;
            }
            else
            Chat.InsertString(Chat.GetCount(), CString(name.c_str()) + CString(" : ") + *(&str+4));
        }
        else if (!strncmp(c, "/userlist", 9))
        {
            std::vector<std::string> *users = (std::vector<std::string>*) app_man.ActivateCommand(std::string(c));
            for(auto i : *users)
            {
                std::string str = '\n' + i;
                Chat.InsertString(Chat.GetCount(), CString(str.c_str()));
            }
        }
        else if (!strncmp(c, "/setname", 8))
        {
            app_man.ActivateCommand(std::string(c));
            UserNameLabel.SetWindowTextW(CString(app_man.GetName().c_str()));
            SetUserIcon();
        }
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


void CChatUIDlg::OnStnClickedUserName()
{
    // TODO: Add your control notification handler code here
}


void CChatUIDlg::OnStnClickedUsericon()
{
    // TODO: Add your control notification handler code here
}

HBRUSH CChatUIDlg::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
    // Call the base class implementation first! Otherwise, it may
    // undo what we're trying to accomplish here.
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // Are we painting the IDC_MYSTATIC control? We can use
    // CWnd::GetDlgCtrlID() to perform the most efficient test.
    if (pWnd->GetDlgCtrlID() == IDC_USER_NAME)
    {
        // Set the text color to red
        pDC->SetTextColor(GetColorByIndex(app_man.GetColor()));

        // Set the background mode for text to transparent
        // so background will show thru.
        pDC->SetBkMode(TRANSPARENT);

        // Return handle to our CBrush object
       // hbr = m_brush;
    }
    else if (pWnd->GetDlgCtrlID() == IDC_LIST1)
    {
        pDC->SetTextColor(LIGHTGREEN);
        pDC->SetBkMode(TRANSPARENT);
    }
    return hbr;
}
