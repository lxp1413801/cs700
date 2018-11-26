
// modbusTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "modbusTest.h"
#include "modbusTestDlg.h"
#include "afxdialogex.h"
#include "stdint.h"

#include "m_string.h"
#include "modbus.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE __hCom;
HANDLE __hCommThread;
HWND __hwndMain = NULL;

uint32_t FlowClibValue=0;
int FlowClibPoint=0;

int modbusAddrCount;
uint8_t modbusAddrBuf[32];
bool Cover=false;
//
uint8_t readStardMeterModeBusCmd[16];
bool StantardMeterOnline=false;
//
static uint8_t bShowData[RECEVIED_BUFFER_LEN];//= new byte[dwRead];

HANDLE hThreadModbusRead=NULL;
HANDLE hThreadModebusPoll=NULL;
HANDLE hThreadModebusWriteClib=NULL;
HANDLE hCommReceivedThread=NULL;
bool hThreadModbusReadExitIrq=false;
bool modbusPollingFlg=false;
const uint32_t BaudRateTable[14]=
{
	CBR_110, 
	CBR_300, 
	CBR_600, 
	CBR_1200, 
	CBR_2400, 
	CBR_4800, 
	CBR_9600, 
	CBR_14400, 
	CBR_19200, 
	CBR_38400, 
	CBR_57600, 
	CBR_115200, 
	CBR_128000, 
	CBR_256000, 
};
#define BAUD_RATE_DEFAULT_IND 6
typedef struct __DCB_BITES_INF
{
	TCHAR  Name[10];
	BYTE bitsNum;
}DCB_BITES_INF,*pDCB_BITES_INF;
const DCB_BITES_INF ParityTable[5]=
{
	{_T("EVEN"),EVENPARITY},
	{_T("MARK"), MARKPARITY},
	{_T("NONE"), NOPARITY},
	{_T("ODDP"),ODDPARITY}, 
	{_T("SPACE"),SPACEPARITY},
};
#define PARITY_DEFAULT_IND 2
const DCB_BITES_INF  StopBitsTable[3]=
{
	{_T("1"),ONESTOPBIT },
	{_T("1.5") ,ONE5STOPBITS},
	{_T("2") ,TWOSTOPBITS},
};
#define STOP_DEFAULT_IND 0
const BYTE  ByteSizeTable[4]=
{
	5,6,7,8,	
};
#define BYTE_WIDTH_DEFAULT_IND 3
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmodbusTestDlg 对话框




CmodbusTestDlg::CmodbusTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmodbusTestDlg::IDD, pParent)
	, m_ComPortOpened(false)
	, uartRreceivedFlg(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmodbusTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_calibration_point, m_calibration_point);
	DDX_Control(pDX, IDC_LIST2, m_list);
	//DDX_Control(pDX, IDC_EDIT_dmsg, m_dmsg);
	DDX_Control(pDX, IDC_EDIT_REFERENCE_DEVICE_ID, m_referenceDeviceId);
	DDX_Control(pDX, IDC_LIST_IMD_MSG, m_listImdMsg);
	DDX_Control(pDX, IDC_EDIT_MODBUS_ADDR, m_manulInputAddr);
	DDX_Control(pDX, IDC_CHECK_MAX_ENABLE, m_check_cover);
	DDX_Control(pDX, IDC_EDIT_PRIO_SETED, m_proSetedPoint);
}

BEGIN_MESSAGE_MAP(CmodbusTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CmodbusTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_COM, &CmodbusTestDlg::OnBnClickedButtonOpenCom)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_COM, &CmodbusTestDlg::OnBnClickedButtonCloseCom)
	ON_BN_CLICKED(IDC_BUTTON_COM_SEND_TEST, &CmodbusTestDlg::OnBnClickedButtonComSendTest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_previous_point, &CmodbusTestDlg::OnBnClickedButtonpreviouspoint)
	ON_BN_CLICKED(IDC_BUTTON_last_point, &CmodbusTestDlg::OnBnClickedButtonlastpoint)
	ON_EN_CHANGE(IDC_EDIT_REFERENCE_DEVICE_ID, &CmodbusTestDlg::OnEnChangeEditReferenceDeviceId)
	//ON_BN_CLICKED(IDC_BUTTON_SET_ZERO2, &CmodbusTestDlg::OnBnClickedButtonSetZero2)
	ON_BN_CLICKED(IDC_BUTTON_POOL_SALAVE, &CmodbusTestDlg::OnBnClickedButtonPoolSalave)
	ON_BN_CLICKED(IDC_BUTTON_SET_ZERO, &CmodbusTestDlg::OnBnClickedButtonSetZero)
	ON_CBN_SELCHANGE(IDC_COMBO_COM_PORT, &CmodbusTestDlg::OnCbnSelchangeComboComPort)
	ON_CBN_DROPDOWN(IDC_COMBO_COM_PORT, &CmodbusTestDlg::OnDropdownComboComPort)
	//ON_BN_CLICKED(IDC_BUTTON_START_POLL, &CmodbusTestDlg::OnBnClickedButtonStartPoll)
	ON_BN_CLICKED(IDC_BUTTON_STOP_POLL, &CmodbusTestDlg::OnBnClickedButtonStopPoll)
	ON_BN_CLICKED(IDC_BUTTON_SET_POINT, &CmodbusTestDlg::OnBnClickedButtonSetPoint)
	ON_EN_CHANGE(IDC_EDIT_calibration_point, &CmodbusTestDlg::OnEnChangeEditcalibrationpoint)
	ON_BN_CLICKED(IDC_BUTTON_START_READ, &CmodbusTestDlg::OnBnClickedButtonStartRead)
END_MESSAGE_MAP()


// CmodbusTestDlg 消息处理程序

BOOL CmodbusTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_dlg_init_call_back();
	__hwndMain=this->m_hWnd ;
	receivedBufCount=0;
	modBusAddrCount=0;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CmodbusTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmodbusTestDlg::OnPaint()
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
HCURSOR CmodbusTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmodbusTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}
int CmodbusTestDlg::enum_com_port(void)
{
	int i=0;
	CString str;
	CComboBox* pComboBox;
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_COM_PORT));
	pComboBox->ResetContent();
	HKEY hKey;    
	if(ERROR_SUCCESS ==::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"),NULL, KEY_READ, &hKey ))//打开串口注册表对应的键值   
	{
		i=0;
		TCHAR  RegKeyName[128],SerialPortName[128];
		DWORD  dwLong,dwSize; 
		while(1){  
			dwLong = dwSize=sizeof(RegKeyName); 
			long ret;
			ret=::RegEnumValue(hKey,i,RegKeyName,&dwLong,NULL,NULL,(PUCHAR)SerialPortName, &dwSize);
			if(ret==ERROR_NO_MORE_ITEMS)break;
			memcmp(RegKeyName,"\\Device\\",8);
			pComboBox->AddString(SerialPortName);
			i++;
		}
		RegCloseKey(hKey); 
	}
	else{
		RegCloseKey(hKey);
	}
	if((pComboBox->GetCount())>0){
		pComboBox->SetCurSel(0);
	}
	return 0;
}


int CmodbusTestDlg::m_comb_init(void)
{
	int i=0;
	CString str;
	CComboBox* pComboBox;
	/*
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_COM_PORT));
	pComboBox->ResetContent();
	HKEY hKey;    
	if(ERROR_SUCCESS ==::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"),NULL, KEY_READ, &hKey ))//打开串口注册表对应的键值   
	{
		i=0;
		TCHAR  RegKeyName[128],SerialPortName[128];
		DWORD  dwLong,dwSize; 
		while(1){  
			dwLong = dwSize=sizeof(RegKeyName); 
			long ret;
			ret=::RegEnumValue(hKey,i,RegKeyName,&dwLong,NULL,NULL,(PUCHAR)SerialPortName, &dwSize);
			if(ret==ERROR_NO_MORE_ITEMS)break;
			memcmp(RegKeyName,"\\Device\\",8);
			pComboBox->AddString(SerialPortName);
			i++;
		}
		RegCloseKey(hKey); 
	}
	else{
		RegCloseKey(hKey);
	}
	if((pComboBox->GetCount())>0){
		pComboBox->SetCurSel(0);
	}
	*/
	enum_com_port();
	//
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_UART_DATA_WIDTH));
	pComboBox->ResetContent();
	for(i=0;i<sizeof(ByteSizeTable)/sizeof(BYTE);i++){
		str.Format(_T("%d"),ByteSizeTable[i]); 
		pComboBox->AddString(str) ;
	}
	if((pComboBox->GetCount())>0){
		pComboBox->SetCurSel(BYTE_WIDTH_DEFAULT_IND);
	}
	//
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_UART_BAUD_RATE));
	pComboBox->ResetContent();
	for(i=0;i<sizeof(BaudRateTable)/sizeof(BaudRateTable[0]);i++){
		str.Format(_T("%d"),BaudRateTable[i]); 
		pComboBox->AddString(str) ;
	}
	if((pComboBox->GetCount() )>0){
		pComboBox->SetCurSel(BAUD_RATE_DEFAULT_IND);
	}
	//
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_UART_STOP_BITS));
	pComboBox->ResetContent();
	for(i=0;i<sizeof(StopBitsTable)/sizeof(StopBitsTable[0]);i++){
		pComboBox->AddString(StopBitsTable[i].Name) ;
	}
	if((pComboBox->GetCount())>0){
		pComboBox->SetCurSel(STOP_DEFAULT_IND);
	}
	//
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_UART_PARITY_BIT));
	pComboBox->ResetContent();
	for(i=0;i<sizeof(ParityTable)/sizeof(ParityTable[0]);i++){
		pComboBox->AddString(ParityTable[i].Name) ;
	}
	if((pComboBox->GetCount() )>0){
		pComboBox->SetCurSel(PARITY_DEFAULT_IND);
	}
	//IDC_COMBO_UART_FLOW_CTRL
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_UART_FLOW_CTRL));
	pComboBox->AddString(__T("NONE"));
	pComboBox->SetCurSel(0);
	return 0;
}


int CmodbusTestDlg::m_button_init(void)
{
	/*
CFont * f; 
     f = new CFont; 
     f->CreateFont(48, // nHeight 
     0, // nWidth 
     0, // nEscapement 
     0, // nOrientation 
     FW_BOLD, // nWeight 
     false, // bItalic 
     false, // bUnderline 
     0, // cStrikeOut 
     ANSI_CHARSET, // nCharSet 
     OUT_DEFAULT_PRECIS, // nOutPrecision 
     CLIP_DEFAULT_PRECIS, // nClipPrecision 
     DEFAULT_QUALITY, // nQuality 
     DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
     _T("宋体")); // lpszFac 
   
     GetDlgItem(IDC_BUTTON_SET_ZERO)->SetFont(f);
	 GetDlgItem(IDC_BUTTON_SET_POINT)->SetFont(f);
	 //
	 GetDlgItem(IDC_BUTTON_previous_point)->SetFont(f);
	 GetDlgItem(IDC_BUTTON_last_point)->SetFont(f);
	 GetDlgItem(IDC_EDIT_calibration_point)->SetFont(f);
	 //GetDlgItem(IDC_EDIT_calibration_point)->set
	CFont * ff; 
		 ff = new CFont; 
		 ff->CreateFont(96, // nHeight 
		 0, // nWidth 
		 0, // nEscapement 
		 0, // nOrientation 
		 FW_BOLD, // nWeight 
		 false, // bItalic 
		 false, // bUnderline 
		 0, // cStrikeOut 
		 ANSI_CHARSET, // nCharSet 
		 OUT_DEFAULT_PRECIS, // nOutPrecision 
		 CLIP_DEFAULT_PRECIS, // nClipPrecision 
		 DEFAULT_QUALITY, // nQuality 
		 DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		 _T("黑体")); // lpszFac 
		 m_calibration_point.SetFont(ff);
		 */
		m_calibration_point.SetWindowTextW(__T("-1"));
		//list//http://blog.csdn.net/tanghaili/article/details/8162269
		//http://www.jizhuomi.com/software/197.html
		m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   
		CRect rect;   
  
		// 获取编程语言列表视图控件的位置和大小   
		m_list.GetClientRect(&rect);   

		// 为列表视图控件添加三列   
		m_list.InsertColumn(0, _T("说明"), LVCFMT_CENTER,			rect.Width()*1/8,	0);
		m_list.InsertColumn(1, _T("modBUS地址"), LVCFMT_CENTER,		rect.Width()*1/8,	1);  
		m_list.InsertColumn(2, _T("通讯状态"), LVCFMT_CENTER,		rect.Width()*1/8,	2);
		m_list.InsertColumn(3, _T("流量值"), LVCFMT_CENTER,			rect.Width()*3/8,	3);   
		m_list.InsertColumn(4, _T("单位"), LVCFMT_CENTER,			rect.Width()*2/8,	4);   

	//
   // CRect rect;   
  
    // 获取编程语言列表视图控件的位置和大小   
    //m_programLangList.GetClientRect(&rect);   
		m_proSetedPoint.SetWindowTextW(__T("-1"));
		m_referenceDeviceId.SetWindowTextW(__T("253"));
	return 0;
}


int CmodbusTestDlg::m_dlg_init_call_back(void)
{
	hCom=NULL;
	//hCommThread=NULL;
	m_ComPortOpened=false;
	m_button_init();
	m_comb_init();
	return 0;
}


void CmodbusTestDlg::OnBnClickedButtonOpenCom()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString str;
	CComboBox* pComboBox;
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_COM_PORT));
	
	if(!m_com_open()){
		if(m_ComPortOpened){
			m_com_close();
		}
	}
	if(m_ComPortOpened){
		//pComboBox->setw
		pComboBox->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_OPEN_COM)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_CLOSE_COM)->EnableWindow(true);
	}else{
		pComboBox->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_OPEN_COM)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CLOSE_COM)->EnableWindow(false);
	}
	__hCom=hCom;
	DWORD threadID;
 	hCommReceivedThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0,(LPTHREAD_START_ROUTINE)ThreadProcReceived, __hwndMain, 0, &threadID);
 	if ( hCommReceivedThread == NULL )
 	{
 		//return FALSE;
		::AfxMessageBox(__T("打开串口失败!!!"));
 	}
	//
	if(m_ComPortOpened){
		//KillTimer(1);
		//SetTimer(2,500,NULL);
	}
}


bool CmodbusTestDlg::m_com_open(void)
{
	//http://blog.csdn.net/l_andy/article/details/51131232
	int sel=0;
	int q31;
	//int tmp;
	CComboBox* pComboBox;
	CString str;
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_COM_PORT));
	sel=pComboBox->GetCurSel();
	//pComboBox->
	pComboBox->GetLBText(sel,str); 
	CString ss=__T("\\\\.\\");
	str=ss+str;
	hCom = CreateFile( str,
					GENERIC_READ|GENERIC_WRITE, 
					0,
					NULL ,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
					//0,
					NULL);

	if ( hCom== INVALID_HANDLE_VALUE )
	{
		::AfxMessageBox(__T("打开串口失败!!!"));
		return false;
		
	}
	m_ComPortOpened=true;
	SetupComm(hCom, 8192, 8192);
	DCB wdcb;
	GetCommState( hCom, &wdcb ) ;

	TRACE(_T("DCBlength :%d\n"), wdcb.DCBlength );
	TRACE(_T("BaudRate :%d\n"), wdcb.BaudRate );
	TRACE(_T("fBinary :%d\n"), wdcb.fBinary );
	TRACE(_T("fParity :%d\n"), wdcb.fParity );
	TRACE(_T("fOutxCtsFlow :%d\n"), wdcb.fOutxCtsFlow );
	TRACE(_T("fOutxDsrFlow :%d\n"), wdcb.fOutxDsrFlow );
	TRACE(_T("fDtrControl :%d\n"), wdcb.fDtrControl );
	TRACE(_T("fDsrSensitivity :%d\n"), wdcb.fDsrSensitivity );	
	TRACE(_T("fTXContinueOnXoff :%d\n"), wdcb.fTXContinueOnXoff );
	TRACE(_T("fOutX :%d\n"), wdcb.fOutX );
	TRACE(_T("fInX :%d\n"), wdcb.fInX );
	TRACE(_T("fErrorChar :%d\n"), wdcb.fErrorChar );
	TRACE(_T("fNull :%d\n"), wdcb.fNull );
	TRACE(_T("fRtsControl :%d\n"), wdcb.fRtsControl );
	TRACE(_T("fAbortOnError :%d\n"), wdcb.fAbortOnError );
	TRACE(_T("fDummy2 :%d\n"), wdcb.fDummy2 );
//	TRACE(_T("wReserved :%d\n"), wdcb.wReserved );
	TRACE(_T("XonLim :%d\n"), wdcb.XonLim );
	TRACE(_T("XoffLim :%d\n"), wdcb.XoffLim );;        
	TRACE(_T("ByteSize :%d\n"), wdcb.ByteSize );
	TRACE(_T("Parity :%d\n"), wdcb.Parity );
	TRACE(_T("StopBits :%d\n"), wdcb.StopBits );
	TRACE(_T("XonChar :%d\n"), wdcb.XonChar );
	TRACE(_T("XoffChar :%d\n"), wdcb.XoffChar );
	TRACE(_T("ErrorChar :%d\n"), wdcb.ErrorChar );
	TRACE(_T("EofChar :%d\n"), wdcb.EofChar );
	TRACE(_T("EvtChar :%d\n"), wdcb.EvtChar );
	TRACE(_T("wReserved1 :%d\n"), wdcb.wReserved1 );


	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_UART_BAUD_RATE));
	sel=pComboBox->GetCurSel();
	q31=BaudRateTable[sel];
	wdcb.BaudRate = q31 ;
	//
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_UART_DATA_WIDTH));
	sel=pComboBox->GetCurSel();
	q31= ByteSizeTable[sel] ;
 	wdcb.ByteSize = q31;
	//
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_UART_PARITY_BIT));
	sel=pComboBox->GetCurSel();
	q31=ParityTable[sel].bitsNum ;
	wdcb.Parity= NOPARITY;
	//wdcb.Parity=q31;
	//
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_UART_STOP_BITS));
	sel=pComboBox->GetCurSel();
	q31=StopBitsTable[sel].bitsNum ;
 	wdcb.StopBits = ONESTOPBIT;
	//wdcb.StopBits =q31;

	SetCommState( hCom, &wdcb ) ;
	
	COMMTIMEOUTS _CO;
	if(!::GetCommTimeouts(hCom, &_CO))
	return false;
	_CO.ReadIntervalTimeout = 10;
	_CO.ReadTotalTimeoutMultiplier =256;
	_CO.ReadTotalTimeoutConstant = 5000;
	_CO.WriteTotalTimeoutMultiplier = 100;
	_CO.WriteTotalTimeoutConstant = 5000;
	if(!::SetCommTimeouts(hCom, &_CO))
	return false; 
	PurgeComm( hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
	return true;
}


void CmodbusTestDlg::m_com_close(void)
{
	if(hCom){
		CloseHandle(hCom);
		hCom=NULL;
	}
	m_ComPortOpened=false;
}


void CmodbusTestDlg::OnBnClickedButtonCloseCom()
{
	// TODO: 在此添加控件通知处理程序代码

	//
	/*
	HANDLE hThreadModbusRead=NULL;
	HANDLE hThreadModebusPoll=NULL;
	HANDLE hThreadModebusWriteClib=NULL;
	HANDLE hCommReceivedThread=NULL;

	*/
	DWORD dw=0;
	if((void*)hThreadModbusRead){
		TerminateThread(hThreadModbusRead,dw);
		CloseHandle(hThreadModbusRead);		
	}
	if((void*)hThreadModebusPoll){
		TerminateThread(hThreadModebusPoll,dw);
		CloseHandle(hThreadModebusPoll);	
	}
	if((void*)hThreadModebusWriteClib){
		TerminateThread(hThreadModebusWriteClib,dw);
		CloseHandle(hThreadModebusWriteClib);
	}
	if((void*)hCommReceivedThread){
		TerminateThread(hCommReceivedThread,dw);
		CloseHandle(hCommReceivedThread);
	}

	CComboBox* pComboBox;
	pComboBox=((CComboBox*)GetDlgItem(IDC_COMBO_COM_PORT));
	m_com_close();
	if(m_ComPortOpened){
		pComboBox->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_OPEN_COM)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_CLOSE_COM)->EnableWindow(true);
	}else{
		pComboBox->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_OPEN_COM)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_CLOSE_COM)->EnableWindow(false);
	}

}


void CmodbusTestDlg::OnBnClickedButtonComSendTest()
{
	// TODO: 在此添加控件通知处理程序代码
	uint8_t buf[]={0x01, 0x03, 0x40 ,0x01 ,0x00 ,0x01 ,0xC0 ,0x0A };
	if(!m_ComPortOpened){
		MessageBox(__T("串口未打开！！"));
		return;
	}
	m_com_send_len((uint8_t*)buf,8);
	//m_com_send_str((uint8_t*)"UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUr");
}

uint16_t CmodbusTestDlg::m_com_send_byte(uint8_t b)
{
	//DWORD wdPacket = 64;
	DWORD dwdWriten = 0;

	COMSTAT ComStat;
	DWORD dwErrorFlags;
	ClearCommError(hCom, &dwErrorFlags, &ComStat);

	OVERLAPPED m_osWrite;
	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	m_osWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	BOOL bResult = WriteFile( hCom, &b, 1, &dwdWriten, &m_osWrite );

	if(!bResult)
	{
		if(GetLastError()==ERROR_IO_PENDING)//the com is writing!
		{
		
			DWORD dwdResult = WaitForSingleObject(m_osWrite.hEvent,1000);
			if ( dwdResult == WAIT_TIMEOUT )
			{
				TRACE(_T("Write Failed!"));
			}
			else
			{
				TRACE(_T("Write Success!\n"));
			}
		}
	}	
	return 0;
}
uint16_t uart_send_len( uint8_t* buf,uint16_t len )
{
	DWORD wdPacket = (DWORD)len;
	DWORD dwdWriten = 0;

	COMSTAT ComStat;
	DWORD dwErrorFlags;
	ClearCommError(__hCom, &dwErrorFlags, &ComStat);

	OVERLAPPED m_osWrite;
	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	m_osWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	BOOL bResult = WriteFile(__hCom, buf, wdPacket, &dwdWriten, &m_osWrite );

	if(!bResult)
	{
		if(GetLastError()==ERROR_IO_PENDING)//the com is writing!
		{
		
			DWORD dwdResult = WaitForSingleObject(m_osWrite.hEvent,1000);
			//PurgeComm(__hCom, PURGE_TXABORT| PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			PurgeComm(__hCom, PURGE_TXABORT| PURGE_TXCLEAR);
			if ( dwdResult == WAIT_TIMEOUT )
			{
				TRACE(_T("Write Failed!"));
			}
			else
			{
				TRACE(_T("\r\nWrite Success!\r\n"));
			}
		}
	}

	return (uint16_t)dwdWriten;
}
uint16_t CmodbusTestDlg::m_com_send_len( uint8_t* buf,uint16_t len )
{
	DWORD wdPacket = (DWORD)len;
	DWORD dwdWriten = 0;

	COMSTAT ComStat;
	DWORD dwErrorFlags;
	ClearCommError(hCom, &dwErrorFlags, &ComStat);

	OVERLAPPED m_osWrite;
	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	m_osWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	BOOL bResult = WriteFile( hCom, buf, wdPacket, &dwdWriten, &m_osWrite );

	if(!bResult)
	{
		if(GetLastError()==ERROR_IO_PENDING)//the com is writing!
		{
		
			DWORD dwdResult = WaitForSingleObject(m_osWrite.hEvent,1000);
			//PurgeComm(__hCom, PURGE_TXABORT| PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			PurgeComm(__hCom, PURGE_TXABORT| PURGE_TXCLEAR);
			if ( dwdResult == WAIT_TIMEOUT )
			{
				TRACE(_T("Write Failed!"));
			}
			else
			{
				TRACE(_T("Write Success!\n"));
			}
		}
	}
	return (uint16_t)dwdWriten;
}
void CmodbusTestDlg::m_com_send_str( uint8_t* str )
{
	while(*str!='\0'){
		m_com_send_byte(*str);
		str++;
	}
}



/*

//可以正常运行的程序
DWORD WINAPI ThreadProcReceived(LPVOID lpParameter )
{	
	OVERLAPPED m_osRead;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	
	DWORD dwBytesRead = 0 ;
	DWORD dwRead = 0;
	bool bReadStat;
	byte bGet[RECEVIED_BUFFER_LEN] = {0};
	unsigned short i=0;
	//bool received=false;
	//bool to=0;
	while ( 1 )
	{
		Sleep(50);
		memset(bGet, 0, RECEVIED_BUFFER_LEN );
		dwRead =0;
		dwBytesRead = 0;
		memset( &m_osRead, 0, sizeof(OVERLAPPED) );
		m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
		ClearCommError(__hCom, &dwErrorFlags, &ComStat);
		
		if(!(ComStat.cbInQue)){
			Sleep(50);
			continue;
		}
		
		bReadStat=ReadFile(__hCom, bGet,ComStat.cbInQue, &dwBytesRead, &m_osRead);
		if( !bReadStat )
		{
			if(GetLastError()==ERROR_IO_PENDING)
			{
				
				DWORD dwdResult = WaitForSingleObject(m_osRead.hEvent,1000);
				if ( dwdResult == WAIT_TIMEOUT )
				{
					PurgeComm(__hCom,  PURGE_RXABORT|PURGE_RXCLEAR);
					TRACE(_T("Read Failed!\n"));
				}
				else
				{	
					GetOverlappedResult(__hCom,&m_osRead,	&dwRead,TRUE ) ;	
				}
				PurgeComm(__hCom, PURGE_TXABORT| PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR); 
				
			}
			//if ( dwBytesRead != 0 ){
			if(dwRead>=0){
				//byte* bShowData= new byte[dwRead];
				//memcpy( bShowData, bGet, dwBytesRead );
				memcpy( bShowData, bGet, dwRead );
				::SendMessage(__hwndMain, WM_COM_RECEIVED, dwRead, (LPARAM)bShowData );
				TRACE(_T("Get data Count : %d\n"),dwRead );
				for ( DWORD i=0; i< dwRead; i++ )
				{
					TRACE(_T("%02x "), bGet[i]);
				}	
			}
		}else{
		
			if ( dwBytesRead != 0 ){

				memcpy( bShowData, bGet, dwBytesRead );
				::SendMessage(__hwndMain, WM_COM_RECEIVED, dwBytesRead, (LPARAM)bShowData );
				TRACE(_T("Get data Count : %d\n"),dwBytesRead );
				for ( DWORD i=0; i< dwBytesRead; i++ )
				{
					TRACE(_T("%02x "), bGet[i]);
				}	
			}		
		}


		
	}
	return 1;
}
*/

//======================================================================
//稳定规范的代码
//======================================================================
DWORD WINAPI ThreadProcReceived(LPVOID lpParameter )
{	
	OVERLAPPED m_osRead;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	
	DWORD dwBytesRead = 0 ;
	bool bReadStat;
	byte bGet[RECEVIED_BUFFER_LEN] = {0};
	unsigned short i=0;
	while ( 1 )
	{
		Sleep(50);
		memset(bGet, 0, RECEVIED_BUFFER_LEN );
		dwBytesRead = 0;
		memset( &m_osRead, 0, sizeof(OVERLAPPED) );
		m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
		ClearCommError(__hCom, &dwErrorFlags, &ComStat);

		bReadStat=ReadFile(__hCom, bGet,ComStat.cbInQue, &dwBytesRead, &m_osRead);
		if( !bReadStat ){
			if(GetLastError()==ERROR_IO_PENDING){
				dwBytesRead=0;
				DWORD dwdResult = WaitForSingleObject(m_osRead.hEvent,2000);
				if ( dwdResult == WAIT_TIMEOUT ){
					PurgeComm(__hCom,  PURGE_RXABORT|PURGE_RXCLEAR);
					TRACE(_T("Read Failed!\n"));
				}else{	
					GetOverlappedResult(__hCom,&m_osRead,	&dwBytesRead,TRUE ) ;	
				}
				PurgeComm(__hCom, PURGE_TXABORT| PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR); 
			}
		}
		if ( dwBytesRead != 0 ){
			memcpy( bShowData, bGet, dwBytesRead );
			::SendMessage(__hwndMain, WM_COM_RECEIVED, dwBytesRead, (LPARAM)bShowData );
			TRACE(_T("Get data Count : %d\n"),dwBytesRead );
			for ( DWORD i=0; i< dwBytesRead; i++ ){
				TRACE(_T("%02x "), bGet[i]);
			}	
		}	
	}	
	return 1;
}
LRESULT CmodbusTestDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString str;
	uint8_t buf[RECEVIED_BUFFER_LEN];
	int nCurSel;
	uint16_t count;
	uint16_t i;
	switch(message){
	case WM_COM_RECEIVED:
		if(receivedBufCount<256){
			if((void*)lParam!=NULL && wParam<256){
				memcpy(receivedBuf+receivedBufCount,(uint8_t*)lParam,(uint16_t)wParam);
				receivedBufCount+=wParam;
			}
		}
		KillTimer(1);
		SetTimer(1,300,NULL);
		break;
	case WM_COM_SENDED:
		m_com_imd_msg(__T(" Sended:"),(uint8_t*)lParam,(uint16_t)wParam);
		break;
	default:
		break;
	
	}
	/*
	if ( message == WM_COM_MESSENGE )
	{
		if(receivedBufCount<256){
			memcpy(receivedBuf+receivedBufCount,(uint8_t*)lParam,(uint16_t)wParam);
			receivedBufCount+=wParam;
		}
		KillTimer(1);
		SetTimer(1,300,NULL);
		//recLen=(uint16_t)wParam;
	}else if(WM_MODBUS_POLL_ADDR)
	{
	}
	*/
	return CDialogEx::WindowProc(message, wParam, lParam);
}
int CmodbusTestDlg::m_modebus_received_process(uint8_t* buf, uint16_t len)
{
	int count,i;
	CString str;
	uint16_t t16;
	if(len>256)len=256;
	t16=crc_verify(buf,len);
	if(!t16)return 0;
	switch(buf[1]){
	case FUNC_READ_HOLDING_REGISTERS:
		if(modbusPollingFlg)
		{
			//搜查表id
			count=m_list.GetItemCount();		
			for(i=0;i<count;i++){
				str=m_list.GetItemText(i,1);
				if(buf[0]==	_tstoi(str.GetBuffer(str.GetLength())))break;
			}
			if(i>=count){
				m_list.InsertItem(count, __T("被标定表"));
				str.Format(__T("%03d"),buf[0]);
				m_list.SetItemText(count,1,str);
				//滚动到最后一行
				//http://bbs.csdn.net/topics/60486645
				m_list.SendMessage(WM_VSCROLL, SB_BOTTOM, NULL);
			}
		}
		else{
	#if 0
			if(buf[3]==0x03 && buf[5]==0x02)
			{
				buf[2]=4;
				buf[3]=0;
				buf[4]=0;
				buf[5]=3;
				buf[6]=0xe8;
				crc_append(buf,7);
				m_modebus_received_process(buf,9);
				break;
			}
	#endif
			count=m_list.GetItemCount();
			for(i=0;i<count;i++){
				str=m_list.GetItemText(i,1);
				if(buf[0]==	_tstoi(str.GetBuffer(str.GetLength()))){
					if(i==0){
						StantardMeterOnline=true;
					}
					break;
				}
			}
			if(i<count){
				uint32_t flow=0;
				flow=buf[3]<<24;
				flow|=buf[4]<<16;
				flow|=buf[5]<<8;
				flow|=buf[6]<<0;
				//flow +=buf[0];
				float f=(float)flow;
				f=f*1000/60;
				str.Format(__T("%.02f"),f/100);
				m_list.SetItemText(i,3,str);
				str=__T("L/min");
				m_list.SetItemText(i,4,str);
				//::update
			}
		}
		break;
	default:break;
	}
	return 0;
}
void CmodbusTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString str;
	uint8_t recBuf[RECEVIED_BUFFER_LEN];
	uint16_t recLen;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent){
	case 1:
		KillTimer(1);
		memcpy(recBuf,receivedBuf,receivedBufCount);
		recLen=receivedBufCount;
		receivedBufCount=0;
		recBuf[recLen]='\0';
		m_com_imd_msg(__T("received:"),recBuf,recLen);
		m_modebus_received_process(recBuf,recLen);
		//m_com_received_echo(recBuf,recLen);
		uartRreceivedFlg=true;
		break;
	case 2:
		KillTimer(2);
		m_referenceDeviceId.GetWindowTextW(str);
		/*
		if(str==__T("")){
			//MessageBox(__T("请输入"));
		}
		*/
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
void CmodbusTestDlg::OnEnChangeEditcalibrationpoint()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	
	// TODO:  在此添加控件通知处理程序代码

	
}
void CmodbusTestDlg::OnBnClickedButtonpreviouspoint()
{
	// TODO: 在此添加控件通知处理程序代码
	int point;
	CString str;
	m_calibration_point.GetWindowTextW(str);
	point=_tstoi(str.GetBuffer(str.GetLength())); 
	point--;
	if(point<0)point=49;
	str.Format(__T("%d"),point);
	m_calibration_point.SetWindowTextW(str);
	FlowClibPoint=point;

}


void CmodbusTestDlg::OnBnClickedButtonlastpoint()
{
	// TODO: 在此添加控件通知处理程序代码
	int point;
	CString str;
	m_calibration_point.GetWindowTextW(str);
	point=_tstoi(str.GetBuffer(str.GetLength())); 
	point++;
	if(point>49)point=0;
	str.Format(__T("%d"),point);
	m_calibration_point.SetWindowTextW(str);
	//FlowClibPoint=point;
}


void CmodbusTestDlg::OnEnChangeEditReferenceDeviceId()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	m_referenceDeviceId.GetWindowTextW(str);
	int t=	_tstoi(str.GetBuffer(str.GetLength())); 
	if(t<100)t=100;
	if(t>255)t=255;
	str.Format(__T("%03d"),t);
	if(m_list.GetItemCount()==0){
		m_list.InsertItem(0, __T("标准表"));
	}
	m_list.SetItemText(0,1,str);
	//
	st_modbusReadClib* stp;
	stp=(st_modbusReadClib*)readStardMeterModeBusCmd;
	stp->addr=t;
	stp->addr_hi=0x40;
	stp->addr_lo=0x03;
	stp->len_hi=0;
	stp->len_lo=2;
	stp->func=FUNC_READ_HOLDING_REGISTERS;
	crc_append(readStardMeterModeBusCmd,sizeof(st_modbusReadClib)-2);
}

//BOOL StopFlag=TRUE;
DWORD WINAPI modbus_poll_slave_thread(LPVOID pvVoid)
{
	CString str;
	uint8_t sendBuf[64];
	uint16_t receivedCount=0;
	uint16_t i;
	st_modbusReadClib *stp=(st_modbusReadClib*)sendBuf;
	modbusPollingFlg=true;
	stp->addr_hi=0x40;
	stp->addr_lo=0x01;
	stp->len_hi=0;
	stp->len_lo=1;
	stp->func=FUNC_READ_HOLDING_REGISTERS;
	int ll=sizeof(st_modbusReadClib);
	for(i=1;i<16;i++){
		stp->addr=(uint8_t)i;
		crc_append(sendBuf,sizeof(st_modbusReadClib)-2);
		uart_send_len(sendBuf,sizeof(st_modbusReadClib));
		sendBuf[ll]='\0';
		//str.Format(__T("send:%s"),sendBuf);
		Sleep(500);
		::SendMessage(__hwndMain, WM_COM_SENDED,ll, (LPARAM)sendBuf );
	}
	modbusPollingFlg=false;
	return 1;

}	 

int CmodbusTestDlg::modbus_pool_slave(void)
{

	DWORD dw=0,dwThread;
	//HANDLE hThread;
	GetDlgItem(IDC_BUTTON_POOL_SALAVE)->EnableWindow(false);
	//DWORD dw;
	if((void*)hThreadModbusRead){SuspendThread(hThreadModbusRead);}
	if((void*)hThreadModebusWriteClib){SuspendThread(hThreadModebusWriteClib);}
	if((void*)hThreadModebusPoll){
		TerminateThread(hThreadModebusPoll,dw);
		CloseHandle(hThreadModebusPoll);	
		//ResumeThread(hThreadModbusRead);
	}
	hThreadModebusPoll = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modbus_poll_slave_thread,    __hwndMain, 0, &dwThread);
	//CloseHandle(hThreadModebusPoll);
	GetDlgItem(IDC_BUTTON_POOL_SALAVE)->EnableWindow(true);
	return 0;
}

int CmodbusTestDlg::m_get_manual_input_modbus_addr(void)
{
	//(uint8_t)(_tstoi(str.GetBuffer(str.GetLength()))); 
	CString str,stradr;
	int loc=0;
	m_manulInputAddr.GetWindowTextW(str);
	int inaddr;
	int count,i;
	while(str.GetLength()>0){
		
		loc=str.Find(__T(' '));
		if(loc==0){
			if(str.GetLength()>0)str=str.Right(str.GetLength()-1);
			continue;
		}
		if(loc>3)loc=3;
		//if(loc>=str.GetLength())break;
		stradr=str.Left(loc);
		inaddr=	_tstoi(stradr.GetBuffer(stradr.GetLength()));
		if(inaddr>253)inaddr=253;
		if(inaddr<1)inaddr=1;
		str=str.Right(str.GetLength()-loc);
		do{
			//搜查表id
			count=m_list.GetItemCount();		
			for(i=0;i<count;i++){
				stradr=m_list.GetItemText(i,1);
				if(inaddr==	_tstoi(stradr.GetBuffer(stradr.GetLength())))break;
			}
			if(i>=count){
				m_list.InsertItem(count, __T("被标定表"));
				stradr.Format(__T("%03d"),inaddr);
				m_list.SetItemText(count,1,stradr);
				//滚动到最后一行
				//http://bbs.csdn.net/topics/60486645
				m_list.SendMessage(WM_VSCROLL, SB_BOTTOM, NULL);
			}
		}while(0);
	}
	return 0;
}
void CmodbusTestDlg::OnBnClickedButtonPoolSalave()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if(!m_ComPortOpened){
		MessageBox(__T("串口未打开！！"));
		return;
	}
	m_list.DeleteAllItems();
	OnEnChangeEditReferenceDeviceId();
	m_get_manual_input_modbus_addr();
	modbus_pool_slave();
}


void CmodbusTestDlg::OnBnClickedButtonSetZero()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CmodbusTestDlg::OnCbnSelchangeComboComPort()
{
	// TODO: 在此添加控件通知处理程序代码
	//enum_com_port();
}


void CmodbusTestDlg::OnDropdownComboComPort()
{
	// TODO: 在此添加控件通知处理程序代码
	enum_com_port();
}


int CmodbusTestDlg::m_com_imd_msg(CString header , uint8_t* buf, uint16_t len)
{
	CString str;
	if(len>100)len=100;
	buf[len]='\0';
	uint8_t hex[256];
	int tt=m_str_b2h(hex,buf,len);
	hex[tt]='\0';
	str=hex;
	//str.Format(__T("%02x"),buf);
	str=header+str;
	//m_listImdMsg.AddString(str);
	if(m_listImdMsg.GetCount()>256)m_listImdMsg.ResetContent();

	m_listImdMsg.InsertString(m_listImdMsg.GetCount(),str);
	
	int count;
	count=m_listImdMsg.GetCount();
	if(count>1)	m_listImdMsg.SetCurSel(count-1);
	
	return 0;
}
DWORD WINAPI modbus_thread_read_flow(LPVOID pvVoid)
{
	OVERLAPPED m_osRead;
	memset( &m_osRead, 0, sizeof(OVERLAPPED) );
	CString str;
	uint8_t sendBuf[64];
	uint16_t receivedCount=0;
	uint16_t i;


	st_modbusReadClib *stp=(st_modbusReadClib*)sendBuf;
	stp->addr_hi=0x40;
	stp->addr_lo=0x03;
	stp->len_hi=0;
	stp->len_lo=2;
	stp->func=FUNC_READ_HOLDING_REGISTERS;
	int ll=sizeof(st_modbusReadClib);
	i=0;
	while(1){
		stp->addr=modbusAddrBuf[i];
		i++;
		if(i>=modbusAddrCount)i=0;

		crc_append(sendBuf,sizeof(st_modbusReadClib)-2);
		uart_send_len(sendBuf,sizeof(st_modbusReadClib));
		sendBuf[ll]='\0';
		//str.Format(__T("send:%s"),sendBuf);
		Sleep(500);
		::SendMessage(__hwndMain, WM_COM_SENDED,ll, (LPARAM)sendBuf );
		if(hThreadModbusReadExitIrq){
			hThreadModbusReadExitIrq=false;
			return 0;
		}
			
	}
	return 1;
}
/*
void CmodbusTestDlg::OnBnClickedButtonStartPoll()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取list Ctrol中的设备地址；
	//http://jingyan.baidu.com/article/1709ad80a079f94634c4f0a2.html
	uint8_t addrArray[32];
	int addrCount=0;
	int i;
	TCHAR szBuffer[32]=TEXT("");
	CString str;
	int rowCount=m_list.GetItemCount();
	LVITEM lvi;
    for (i=0;i<rowCount; i++)
    {
		lvi.iItem =i;
		lvi.mask = LVIF_TEXT;
		lvi.pszText=(LPWSTR)szBuffer;
		lvi.cchTextMax = 32;
		lvi.iSubItem=1;
		m_list.GetItem(&lvi);
		str.Format(__T("%s"),szBuffer);
		addrArray[i]=(uint8_t)(_tstoi(str.GetBuffer(str.GetLength()))); 
	}
	addrCount=i;
	memcpy((uint8_t*)modbusAddrBuf,addrArray,addrCount);
	modbusAddrCount=addrCount;
	DWORD dwThread;
	hThreadModbusReadExitIrq=false;
	if((void*)hThreadModbusRead==NULL){
		hThreadModbusRead = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modbus_thread_read_flow,    __hwndMain, 0, &dwThread);
	
	}else
	{
		ResumeThread(hThreadModbusRead);
	}
}
*/
void CmodbusTestDlg::OnBnClickedButtonStopPoll()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dw=0;
	if((void*)hThreadModbusRead){

		//TerminateThread(hThreadModbusRead,dw);
		SuspendThread(hThreadModbusRead);
	}
}
DWORD WINAPI modbus_thread_set_flow(LPVOID pvVoid)
{
/*
	typedef struct{
		uint8_t 	addr;
		uint8_t		func;
		uint8_t		addr_hi;
		uint8_t 	addr_lo;
		uint8_t		data_hi;
		uint8_t		data_lo;
		uint16_t	crc16;
	}st_modbusWriteSingleClib;	
*/
	
	CString str;
	uint8_t sendBuf[64];
	uint16_t receivedCount=0;
	uint16_t i;
#if 0
	//uint8_t readStardMeterModeBusCmd[16];
	//bool StantardMeterOnline=false;
	crc_append(readStardMeterModeBusCmd,sizeof(st_modbusReadClib)-2);
	uart_send_len(readStardMeterModeBusCmd,sizeof(st_modbusReadClib));
	StantardMeterOnline=false;
	Sleep(2000);
	if(!StantardMeterOnline){
		::AfxMessageBox(__T(" 标准表离线，禁止标定!!!"));
	}
	StantardMeterOnline=false;
	return 0;
#endif

	st_modbusWriteSingleClib *stp=(st_modbusWriteSingleClib*)sendBuf;

	stp->addr_hi=0;
	stp->addr_lo=(uint8_t)(FlowClibPoint & 0xff);
	stp->data_hi=(uint8_t)((FlowClibValue>>8) & 0xff);
	stp->data_lo=(uint8_t)((FlowClibValue>>0) & 0xff);;
	if(Cover)
	{
		stp->func=FUNC_WRITE_FLASH_CLIBDATA_COVER;
	}else{
		stp->func=FUNC_WRITE_FLASH_CLIBDATA;
		
	}
	Cover=0;
	int ll=sizeof(st_modbusWriteSingleClib);
	i=0;

	if((void*)hThreadModbusRead){SuspendThread(hThreadModbusRead);	}
	Sleep(500);
	//读取标准表


	for(i=1;i<modbusAddrCount;i++){
		stp->addr=modbusAddrBuf[i];
		if(modbusAddrBuf[i]>100)continue;
		crc_append(sendBuf,sizeof(st_modbusWriteSingleClib)-2);
		uart_send_len(sendBuf,sizeof(st_modbusWriteSingleClib));
		sendBuf[ll]='\0';
		//str.Format(__T("send:%s"),sendBuf);
		Sleep(500);
		::SendMessage(__hwndMain, WM_COM_SENDED,ll, (LPARAM)sendBuf );		
	}
	Sleep(500);
	if(hThreadModbusRead!=NULL){
		ResumeThread(hThreadModbusRead);
	}
	return 1;
}
void CmodbusTestDlg::m_get_clib_point(void){
	int point;
	CString str;
	m_calibration_point.GetWindowTextW(str);
	point=_tstoi(str.GetBuffer(str.GetLength())); 

	//point--;
	if(point<0)point=0;
	if(point>49)point=49;
	str.Format(__T("%d"),point);
	m_calibration_point.SetWindowTextW(str);
	FlowClibPoint=point;
	//CString str;
	str.Format(__T("%d"),FlowClibPoint);
	m_proSetedPoint.SetWindowTextW(str);
	OnBnClickedButtonlastpoint();
}
void CmodbusTestDlg::m_get_clib_standard_flow(void){
	LVITEM lvi;
	TCHAR szBuffer[32]=TEXT("");
	CString str;
	lvi.iItem =0;
	lvi.mask = LVIF_TEXT;
	lvi.pszText=(LPWSTR)szBuffer;
	lvi.cchTextMax = 32;
	lvi.iSubItem=3;
	m_list.GetItem(&lvi);
	str.Format(__T("%s"),szBuffer);
	float  fl=(_tstof(str.GetBuffer(str.GetLength()))); //atof
	//float  fl=(uint8_t)(_ttof(str.GetBuffer(str.GetLength())));
	fl*=10;
	//fl=fl*1000/60;
	FlowClibValue=(uint32_t)fl;
}
void CmodbusTestDlg::OnBnClickedButtonSetPoint()
{
	
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwThread;
	//HANDLE hThread=NULL;
	uint8_t addrArray[32];
	int addrCount=0;
	int i;
	TCHAR szBuffer[32]=TEXT("");
	CString str=__T("");
	int rowCount=m_list.GetItemCount();
	LVITEM lvi;
    for (i=0;i<rowCount; i++)
    {
		lvi.iItem =i;
		lvi.mask = LVIF_TEXT;
		lvi.pszText=(LPWSTR)szBuffer;
		lvi.cchTextMax = 32;
		lvi.iSubItem=1;
		m_list.GetItem(&lvi);
		str.Format(__T("%s"),szBuffer);

		addrArray[i]=(uint8_t)(_tstoi(str.GetBuffer(str.GetLength()))); 
	}

	addrCount=i;
	memcpy((uint8_t*)modbusAddrBuf,addrArray,addrCount);
	modbusAddrCount=addrCount;
#if 1
		lvi.iItem =0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText=(LPWSTR)szBuffer;
		lvi.cchTextMax = 32;
		lvi.iSubItem=3;
		m_list.GetItem(&lvi);
		str.Format(__T("%s"),szBuffer);
		if(str.GetLength()==0){
			::AfxMessageBox(__T("标准表未读取流量，不能标定！！！"));
			return;
		}

	int point;
	//CString str;
	m_calibration_point.GetWindowTextW(str);
	point=_tstoi(str.GetBuffer(str.GetLength())); 
	if(point<0 || point>49){
		::AfxMessageBox(__T("标定点！！！"));
		return;		
	}

#endif

	//获取标定点和标准表流量
	m_get_clib_point();
	m_get_clib_standard_flow();
	//获取覆盖
	Cover=m_check_cover.GetCheck();
	m_check_cover.SetCheck(0);
	//创建标定线程
	/*
	DWORD dw;
	if((void*)hThreadModebusPoll){SuspendThread(hThreadModebusPoll);}
	if((void*)hThreadModebusWriteClib){SuspendThread(hThreadModebusPoll);}
	if((void*)hThreadModbusRead){
		//TerminateThread(hThreadModebusWriteClib,dw);
		//CloseHandle(hThreadModebusWriteClib);	
		ResumeThread(hThreadModbusRead);
	}else{
		hThreadModbusRead = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modbus_thread_read_flow,    __hwndMain, 0, &dwThread);	
	}
	*/
	DWORD dw=0;
	if((void*)hThreadModebusPoll){SuspendThread(hThreadModebusPoll);}
	if((void*)hThreadModbusRead){SuspendThread(hThreadModbusRead);}
	if((void*)hThreadModebusWriteClib){
		TerminateThread(hThreadModebusWriteClib,dw);
		CloseHandle(hThreadModebusWriteClib);		
	}

	//if((void*)hThreadModebusWriteClib==NULL){
		hThreadModebusWriteClib = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modbus_thread_set_flow,    __hwndMain, 0, &dwThread);
	//}
	//CloseHandle(hThreadModebusWriteClib);
	
}

void CmodbusTestDlg::OnBnClickedButtonStartRead()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	//获取list Ctrol中的设备地址；
	//http://jingyan.baidu.com/article/1709ad80a079f94634c4f0a2.html
	uint8_t addrArray[32];
	int addrCount=0;
	int i;
	TCHAR szBuffer[32]=TEXT("");
	CString str;
	int rowCount=m_list.GetItemCount();
	LVITEM lvi;
    for (i=0;i<rowCount; i++)
    {
		lvi.iItem =i;
		lvi.mask = LVIF_TEXT;
		lvi.pszText=(LPWSTR)szBuffer;
		lvi.cchTextMax = 32;
		lvi.iSubItem=1;
		m_list.GetItem(&lvi);
		str.Format(__T("%s"),szBuffer);
		addrArray[i]=(uint8_t)(_tstoi(str.GetBuffer(str.GetLength()))); 
	}
	addrCount=i;
	memcpy((uint8_t*)modbusAddrBuf,addrArray,addrCount);
	modbusAddrCount=addrCount;
	DWORD dwThread;
	hThreadModbusReadExitIrq=false;
	/*
	if((void*)hThreadModbusRead){
		//TerminateThread(hThreadModbusRead,dw);
		SuspendThread(hThreadModbusRead);
	}
	if((void*)hThreadModebusPoll){
		TerminateThread(hThreadModebusPoll,dw);
		CloseHandle(hThreadModebusPoll);
	}	
	HANDLE hThreadModbusRead=NULL;
	HANDLE hThreadModebusPoll=NULL;
	HANDLE hThreadModebusWriteClib=NULL;
	*/
	DWORD dw=0;
	if((void*)hThreadModebusPoll){SuspendThread(hThreadModebusPoll);}
	if((void*)hThreadModebusWriteClib){SuspendThread(hThreadModebusWriteClib);}
	if((void*)hThreadModbusRead){
		TerminateThread(hThreadModbusRead,dw);
		CloseHandle(hThreadModbusRead);	
		//ResumeThread(hThreadModbusRead);
	}
	//else{
		hThreadModbusRead = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modbus_thread_read_flow,    __hwndMain, 0, &dwThread);	
	//}
}
