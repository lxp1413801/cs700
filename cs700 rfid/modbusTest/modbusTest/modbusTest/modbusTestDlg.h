
// modbusTestDlg.h : 头文件
//
#include <stdint.h>

#pragma once

#define WM_COM_RECEIVED	WM_USER + 1122
#define WM_COM_SENDED	WM_USER + 1123
#define WM_COM_WRITE_CLIB_OVER	WM_USER + 1124
#define RECEVIED_BUFFER_LEN 1024

extern DWORD WINAPI ThreadProcReceived(LPVOID lpParameter );
// CmodbusTestDlg 对话框
class CmodbusTestDlg : public CDialogEx
{
// 构造
public:
	CmodbusTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MODBUSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_comb_init(void);
	int m_button_init(void);
	int m_dlg_init_call_back(void);
	afx_msg void OnBnClickedButtonOpenCom();
	HANDLE hCom;
	//HANDLE hCommThread;
	bool m_ComPortOpened;
	bool m_com_open(void);
	void m_com_close(void);
	afx_msg void OnBnClickedButtonCloseCom();
	afx_msg void OnBnClickedButtonComSendTest();

	uint16_t m_com_send_byte(uint8_t b);
	uint16_t m_com_send_len( uint8_t* buf,uint16_t len );
	void m_com_send_str( uint8_t* str );
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//
	uint8_t receivedBuf[RECEVIED_BUFFER_LEN];
	uint16_t receivedBufCount;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_calibration_point;
	CListCtrl m_list;
	//int m_com_received_echo(uint8_t* rbuf, uint16_t len);
	CEdit m_dmsg;
	afx_msg void OnBnClickedButtonpreviouspoint();
	afx_msg void OnBnClickedButtonlastpoint();
	CEdit m_referenceDeviceId;
	afx_msg void OnEnChangeEditReferenceDeviceId();
	//afx_msg void OnBnClickedButtonSetZero2();
	int modbus_pool_slave(void);
	uint8_t modBusAddrArr[256];
	uint16_t modBusAddrCount;
	//bool uartRreceivedFlg;
	void set_uartRreceivedFlg(bool b){uartRreceivedFlg=b;}
	bool uartRreceivedFlg;
	afx_msg void OnBnClickedButtonPoolSalave();
	afx_msg void OnBnClickedButtonSetZero();
	int m_modebus_received_process(uint8_t* buf, uint16_t len);
	int enum_com_port(void);
	afx_msg void OnCbnSelchangeComboComPort();
	afx_msg void OnDropdownComboComPort();
	CListBox m_listImdMsg;
	int m_com_imd_msg(CString header , uint8_t* buf, uint16_t len);
	afx_msg void OnBnClickedButtonStartPoll();
	//int m_modbusAddrCount;
	//uint8_t m_modbusAddrBuf[32];
	afx_msg void OnBnClickedButtonStopPoll();
	afx_msg void OnBnClickedButtonSetPoint();
	afx_msg void OnEnChangeEditcalibrationpoint();
	void m_get_clib_point(void);
	void m_get_clib_standard_flow(void);
	int m_get_manual_input_modbus_addr(void);
	CEdit m_manulInputAddr;
	CButton m_check_cover;
	CEdit m_proSetedPoint;
	afx_msg void OnBnClickedButtonStartRead();
	//
	void m_clear_list_comm_status(void);
	bool m_get_list_clib_status(void);

};
