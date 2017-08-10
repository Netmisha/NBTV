
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
#include "SettingsDlg.h"
#define TID_ONLY_ONCE WM_USER + 202


void CChatUIDlg::ProcessMessage(const  UnpackedMessage &um, AppManager& am)
{
    switch(um.type_)
    {
    case CHAT_MESSAGE:
    {
        if(!is_private)
            Chat.InsertString(Chat.GetCount(), '\n' + CString((*(UserMsg*)um.msg_).name_.c_str()) + " : " + CString((*(UserMsg*)um.msg_).msg_.c_str()));
        else
            if((*(UserMsg*)um.msg_).name_ == ModeName && (*(UserMsg*)um.msg_).type_ == PRIVATE)          
                Chat.InsertString(Chat.GetCount(), '\n' + CString((*(UserMsg*)um.msg_).name_.c_str()) + " : " + CString((*(UserMsg*)um.msg_).msg_.c_str()));

        break;
    }
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

void CChatUIDlg::SetPrivateMode(CString str)
{
    const WCHAR* wc = str;
    _bstr_t b(wc);
    const char* name = b;
    if (!strncmp(name, "Global", 6))
    {
        ModeName.clear();
        is_private = false;
        SetFileList();
        SetChat();
        OnBnClickedSwitchC();
    }
    else
    {
        is_private = true;
        ModeName = name;
        SetFileListP();
        SetChatP();
        OnBnClickedSwitchC();
    }


}

void CChatUIDlg::OnTimer(UINT_PTR nIDEvent)
{
    //set user icon
    CStatic*
        icon = (CStatic*)GetDlgItem(IDC_USERICON);
    CImage image_;
    std::string name = app_man.GetName();
    std::string path("Resourses\\");
    path += ((char)toupper(name[0]));
    path += ".png";
    image_.Load(CString(path.c_str()));

    CBitmap bitmap_;
    bitmap_.Attach(image_.Detach());
    icon->SetBitmap(bitmap_);

    //set user list
    std::vector<UserInfo>* list = (std::vector<UserInfo>*)app_man.ActivateCommand(std::string("/userlist"));
    CListBox *usr_list = (CListBox *)GetDlgItem(IDC_LIST1);
    if (is_working_)
        usr_list->ResetContent();

    usr_list->InsertString(0, CString("Global"));

    if (list->size() != 0)
        for (auto i : *list)
        {

            usr_list->InsertString(usr_list->GetCount(), CString(i.user_name_.c_str()));
        }

    //set file grid
    std::vector<File> *list2 = (std::vector<File>*)app_man.ActivateCommand(std::string("/fl "));
    FileGrid.ClearCells(CCellRange(FileGrid.GetCellRange()));
    for (int i = 0; i < list2->size(); i++)
    {
        FileGrid.SetItemText(i, 0, CString(std::to_string(i + 1).c_str()));
        FileGrid.SetItemText(i, 1, CString((*list2)[i].GetName().c_str()));

        FileGrid.SetItemText(i, 2, CString(std::to_string((*list2)[i].GetSizeMB()).c_str()) + CString(" MB"));
    }
    FileGrid.Refresh();
  
       KillTimer(TID_ONLY_ONCE);


}

inline void CChatUIDlg::SetUserIcon()
{
    static CStatic*
        icon = (CStatic*)GetDlgItem(IDC_USERICON);
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

inline void CChatUIDlg::SetUserList()
{
    std::vector<UserInfo>* list = (std::vector<UserInfo>*)app_man.ActivateCommand(std::string("/userlist"));
    CListBox *usr_list = (CListBox *)GetDlgItem(IDC_LIST1);
    if(is_working_)
        usr_list->ResetContent();

    usr_list->InsertString(0, CString("Global"));

    if(list->size() != 0)
    for (auto i : *list)
    {
        usr_list->InsertString(usr_list->GetCount(), CString(i.user_name_.c_str()));
    }
}

void CChatUIDlg::SetFileListP()
{
    std::vector<RecvFileInfo> *list = (std::vector<RecvFileInfo>*)app_man.ActivateCommand(std::string("/fl " + ModeName));
    FileGrid.ClearCells(CCellRange(FileGrid.GetCellRange()));
    for (int i = 0; i < list->size(); i++)
    {
        FileGrid.SetItemText(i, 0, CString(std::to_string(i + 1).c_str()));
        FileGrid.SetItemText(i, 1, CString((*list)[i].name_.c_str()));

        FileGrid.SetItemText(i, 2, CString(std::to_string((*list)[i].size_KB_/1024).c_str()) + CString(" MB"));
    }
    FileGrid.Refresh();
}

void CChatUIDlg::SetChatP()
{
    std::vector<UserMsg>* list = (std::vector<UserMsg>*)app_man.GetPrivateChatMsgs(ModeName);
    
    CListBox *usr_list = (CListBox *)GetDlgItem(IDC_CHAT);
    if (is_working_)
        usr_list->ResetContent();

    if (list != NULL && list->size() != 0)
        for (auto i : *list)
        {
            Chat.InsertString(Chat.GetCount(), '\n' + CString(i.name_.c_str()) + " : " + CString(i.msg_.c_str()));
        }
}

void  CChatUIDlg::RecvLoop(AppManager& am)
{
    is_working_ = true;
    while (is_working_)
    {
        UnpackedMessage packet = am.RecieveMessage();
        if(is_working_)
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
    is_working_ = false;
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
    DDX_Control(pDX, IDC_SWITCH_C, CheckChat);
    DDX_Control(pDX, IDC_SWITCH_FL, CheckFileList);
    DDX_Control(pDX, IDC_BUTTON1, AddFileButt);
    DDX_Control(pDX, IDC_SETTING_BUT, OpsButt);
}

BEGIN_MESSAGE_MAP(CChatUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_MAINBUTTON, &CChatUIDlg::OnBnClickedMainbutton)
    ON_STN_CLICKED(IDC_USER_NAME, &CChatUIDlg::OnStnClickedUserName)
    ON_STN_CLICKED(IDC_USERICON, &CChatUIDlg::OnStnClickedUsericon)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_SWITCH_C, &CChatUIDlg::OnBnClickedSwitchC)
    ON_BN_CLICKED(IDC_SWITCH_FL, &CChatUIDlg::OnBnClickedSwitchFl)
    ON_BN_CLICKED(IDC_BUTTON1, &CChatUIDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_SETTING_BUT, &CChatUIDlg::OnBnClickedSettingBut)
    ON_LBN_DBLCLK(IDC_LIST1, &CChatUIDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()



BOOL CChatUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    app_man.LoadUserInfo();
  
    UserNameLabel.SetWindowTextW(CString(app_man.GetName().c_str()));
    UserListLabel.SetWindowTextW(CString("Active Users: "));
    CheckFileList.SetWindowTextW(CString("FL"));
    CheckChat.SetWindowTextW(CString("C"));
    AddFileButt.SetWindowTextW(CString("ADD FILE"));
    OpsButt.SetWindowTextW(CString("Ops"));

   
    recv_thread_.BeginThread(StartRecvLoop, this);
    



    //set file table
    RECT rect = { LONG(25), LONG(65), LONG(735), LONG(440) };
    FileGrid.Create(rect, this, 25);
    FileGrid.SetColumnCount(3); 
    FileGrid.SetRowCount(10);

    FileGrid.SetColumnWidth(0, 50);
    FileGrid.SetColumnWidth(1, 550);
    FileGrid.SetColumnWidth(2, 105);

    OnBnClickedSwitchC();//set chat mode

    SetTimer(TID_ONLY_ONCE, 200, NULL);


  
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
            if (app_man.ActivateCommand(std::string(c)) == NULL)   
                AfxMessageBox(CString("Name is used !!!"));
            else
            {
                UserNameLabel.SetWindowTextW(CString(app_man.GetName().c_str()));
                SetUserIcon();
            }
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
                
            }
            else
            {
                std::vector<RecvFileInfo> *list = (std::vector<RecvFileInfo>*) app_man.ActivateCommand(std::string(c));
                for (size_t i = 0; i < list->size(); i++)
                {
                    std::string str = std::to_string(i + 1) + ' ' + (*list)[i].name_ + ' ' + std::to_string((*list)[i].size_KB_ / 1024);
                    Chat.InsertString(Chat.GetCount(), CString(str.c_str()));
                }
                
            }
        }
        else if (!strncmp(c, "/getf", 5))
            app_man.ActivateCommand(std::string(c));
        else if (!strncmp(c, "/addf", 5))
        {
            app_man.ActivateCommand(std::string(c));
            SetFileList();
        }
        else if (!strncmp(c, "/removef", 8))
        {
            app_man.ActivateCommand(std::string(c));
            SetFileList();
        }
        else if (!strncmp(c, "/setcolor", 9))
        {
            app_man.ActivateCommand(std::string(c));
            UserNameLabel.GetDC()->SetTextColor(GetColorByIndex(app_man.GetColor()));
            UserNameLabel.SetWindowTextW(CString(app_man.GetName().c_str()));
        }
    }
    else
    {
        if (is_private)
        app_man.SendMsgTo(c, ModeName);
        else
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

    }
    else if (pWnd->GetDlgCtrlID() == IDC_LIST1)
    {
        pDC->SetTextColor(LIGHTGREEN);
        pDC->SetBkMode(TRANSPARENT);
    }
    return hbr;
}


void CChatUIDlg::OnBnClickedSwitchC()
{
    MainButton.ShowWindow(SW_SHOW);
    ChatEdit.ShowWindow(SW_SHOW);
    Chat.ShowWindow(SW_SHOW);
    FileGrid.ShowWindow(SW_HIDE);
    AddFileButt.ShowWindow(SW_HIDE);
}


void CChatUIDlg::OnBnClickedSwitchFl()
{
    MainButton.ShowWindow(SW_HIDE);
    ChatEdit.ShowWindow(SW_HIDE);
    Chat.ShowWindow(SW_HIDE);
    FileGrid.ShowWindow(SW_SHOW);
    AddFileButt.ShowWindow(SW_SHOW);
}

inline void CChatUIDlg::SetFileList()
{
    std::vector<File> *list = (std::vector<File>*)app_man.ActivateCommand(std::string("/fl "));
   FileGrid.ClearCells(CCellRange(FileGrid.GetCellRange()));
    for (int i = 0; i < list->size(); i++)
    {
        FileGrid.SetItemText(i, 0, CString(std::to_string(i+1).c_str()));
        FileGrid.SetItemText(i, 1, CString(  (*list)[i].GetName().c_str()  ));
        
        FileGrid.SetItemText(i, 2, CString( std::to_string((*list)[i].GetSizeMB()).c_str()) + CString(" MB"));
    }
    FileGrid.Refresh();
}

void CChatUIDlg::SetChat()
{
    std::vector<UserMsg>* list = (std::vector<UserMsg>*)app_man.GetPrivateChatMsgs(PUBLIC_MSGS);

    CListBox *usr_list = (CListBox *)GetDlgItem(IDC_CHAT);
    if (is_working_)
        usr_list->ResetContent();

    if (list != NULL && list->size() != 0)
        for (auto i : *list)
        {
            Chat.InsertString(Chat.GetCount(), '\n' + CString(i.name_.c_str()) + " : " + CString(i.msg_.c_str()));
        }
}


void CChatUIDlg::OnBnClickedButton1()
{
    CFileDialog fileDialog(TRUE, NULL);	//create file d_log
    int result = fileDialog.DoModal();	//set up
    if (result == IDOK)	//if is ok
    {
        CString file_path("/addf "); 
        file_path += fileDialog.GetPathName(); //set up string
        CStringA charstr(file_path);
        app_man.ActivateCommand(std::string((charstr)));
        SetFileList();
    }
}


void CChatUIDlg::OnBnClickedSettingBut()
{
   
    SettingsDlg dlg; //setting dlg
    dlg.SetAM(&app_man);

    if (dlg.DoModal() == IDOK) // DoModal() - open dlg
    {
        UserNameLabel.SetWindowTextW(CString(app_man.GetName().c_str()));
        SetUserIcon();
    }

}


void CChatUIDlg::OnLbnDblclkList1()
{
    
    CListBox* ul = (CListBox*)GetDlgItem(IDC_LIST1);
        int j, i = 0;
        for (; i < ul->GetCount(); i++)
        {
            j = ul->GetSel(i);
            if (j > 0)
                break;
        }
        
        CString ItemSelected;
        if (j>0 && i!= ul->GetCount())
        {
            ul->GetText(i, ItemSelected);
            AfxMessageBox(ItemSelected);
            SetPrivateMode(ItemSelected);
        }



   
}
