
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#include "pe.h"
#include <Psapi.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, EditBrowse);
	DDX_Control(pDX, IDC_LIST1, ProcListBox);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
bool GetProcessPath(HANDLE hProcess, wchar_t *szFilePath)
{
	szFilePath[0] = '1';
	wchar_t buff[MAX_PATH];
	wchar_t DosFilePath[MAX_PATH + 10];
	ZeroMemory(DosFilePath, sizeof(wchar_t) * (MAX_PATH + 10));
	if (0 == GetProcessImageFileName(hProcess, DosFilePath, MAX_PATH + 10))
	{
		return false;
	}
	// 获取Logic Drive String长度  
	int LDLen = GetLogicalDriveStrings(0, NULL);
	if (0 == LDLen)
	{
		return false;
	}
	wchar_t* pLogicDriveString = new wchar_t[MAX_PATH + 10];
	ZeroMemory(pLogicDriveString, (MAX_PATH + 10) * sizeof(wchar_t));
	LDLen = GetLogicalDriveStrings(LDLen, pLogicDriveString);
	if (0 == LDLen)
	{
		delete[]pLogicDriveString;
		return false;
	}

	wchar_t szDrive[3] = TEXT(" :");
	wchar_t* pDosDriveName = new wchar_t[MAX_PATH];
	wchar_t* pLogicIndex = pLogicDriveString;

	do
	{
		szDrive[0] = *pLogicIndex;
		LDLen = QueryDosDevice(szDrive, pDosDriveName, MAX_PATH);
		if (0 == LDLen)
		{
			if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
			{
				break;
			}

			delete[]pDosDriveName;
			pDosDriveName = new wchar_t[LDLen + 1];
			LDLen = QueryDosDevice(szDrive, pDosDriveName, LDLen + 1);
			if (0 == LDLen)
			{
				break;
			}
		}

		LDLen = _tcslen(pDosDriveName);
		if (0 == _tcsnicmp(DosFilePath, pDosDriveName, LDLen))
		{
			swprintf_s(buff, MAX_PATH, L"%s%s", szDrive, DosFilePath + LDLen);
			
			break;
		}

		while (*pLogicIndex++);
	} while (*pLogicIndex);
	int i = 0;
	delete[]pLogicDriveString;
	delete[]pDosDriveName;
	for (i = 0; buff[i]; i++)
	{
		szFilePath[i] = buff[i];
	}
	szFilePath[i] = '\0';
	return true;
}

void CMFCApplication1Dlg::OnBnClickedOk()
{
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
	if (INVALID_HANDLE_VALUE==hProcess)
	{
		MessageBox(L"创建进程快照失败！"); return;
	}
	PROCESSENTRY32 pe;
	SYSTEM_INFO si;
	wchar_t buff[MAX_PATH + 50];

	pe.dwSize = sizeof PROCESSENTRY32;
	GetNativeSystemInfo(&si);//获取当前cpu的信息.
	while (Process32Next(hProcess,&pe))
	{
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		{
			BOOL Is64;
			HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, false, pe.th32ProcessID);
						
			if (hp == INVALID_HANDLE_VALUE)
			{
				MessageBox(L"error"); return;
			}
			IsWow64Process(hp,&Is64);
			if (Is64)
			{
				wsprintf(buff, L"%s", L"（32位）");
			}
			else
			{
				wsprintf(buff, L"%s", L"（64位）");
			}
		}
		else
		{
			wsprintf(buff, L"%s", L"（32位）");
		}
		wsprintf(buff, L"%s%s", buff, pe.szExeFile);
		ProcListBox.SetItemData(ProcListBox.AddString(buff),pe.th32ProcessID);
	}
}



void CMFCApplication1Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void RemoteLoadLibrary(HANDLE hProcess, LPCTSTR lpLibFileName)
{
	DWORD ThreadID;
	//在目标进程中分配一部分内存空间用来存放dll路径
	wchar_t path[MAX_PATH + 10];
	if (!GetProcessPath(hProcess, path))return;

	int PageSize;
	//根据进程获取的文件路径创建一个pe对象然后获取其SectionAlignment
	BasePe ObjPe{path};
	ObjPe.SetValue();
	//获取文件的内存中页对齐大小

	PageSize = ObjPe.GetOptionHeaders()->SectionAlignment;
	LPVOID PMemory = VirtualAllocEx(hProcess, NULL, PageSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(hProcess, PMemory, lpLibFileName, lstrlenW(lpLibFileName)*2 + 2, NULL);
	//dll文件在高地址位，不随程序改变而改变地址，只要在本进程中获取到入口地址
	//在目标进程中也可以使用
	HMODULE hModule = LoadLibrary(L"kernel32.dll");
	LPTHREAD_START_ROUTINE PFThread = (LPTHREAD_START_ROUTINE)(GetProcAddress(hModule, "LoadLibraryW"));
	//创建线程
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, PFThread, PMemory, 0,&ThreadID);
	//等待线程运行完毕
	WaitForSingleObject(hThread, INFINITE);
	//释放申请的内存空间
	VirtualFreeEx(hThread, PMemory, PageSize, MEM_RELEASE);
	return;
}

void CMFCApplication1Dlg::OnBnClickedButton1()
{
	CString Path;
	EditBrowse.GetWindowTextW(Path);
	if (Path == L"")
	{
		MessageBox(L"路径不能为空！");
		return;
	}
	int index = ProcListBox.GetCurSel();
	if (index == -1)
	{
		MessageBox(L"未选择进程！");
		return;
	}
	DWORD dwPID = (DWORD)ProcListBox.GetItemData(index);

	RemoteLoadLibrary(OpenProcess(PROCESS_ALL_ACCESS, false, dwPID), Path);
}
