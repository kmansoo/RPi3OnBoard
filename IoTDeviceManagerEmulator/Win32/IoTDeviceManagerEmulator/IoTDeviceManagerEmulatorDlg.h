
// IoTDeviceManagerEmulatorDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"

#include "UIControl/UIColorStaticST.h"

#include "../../Impl/IoTDeviceDeviceManager.h"
#include "../../Impl/IoTDeviceOpenAPIManager.h"


// CIoTDeviceManagerEmulatorDlg 대화 상자
class CIoTDeviceManagerEmulatorDlg :
    public CDialogEx,
    public IoTDeviceManagerIF
{
// 생성입니다.
public:
	CIoTDeviceManagerEmulatorDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IoTDeviceManagerEmulator_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedSetName();
	DECLARE_MESSAGE_MAP()

protected:
    virtual bool    setBulbStatus(std::string name, bool on);
    virtual bool    getBulbStatus(std::string name, std::string& status_json);
    virtual bool    getAllBulbsStatus(std::string& status_json);

    virtual bool    setDoorLockStatus(std::string name, std::string status);
    virtual bool    getDoorLockStatus(std::string name, std::string& status_json);
    virtual bool    getAllDoorLockStatus(std::string& status_json);

protected:
    COLORREF getBulbColor(bool status) {
        return (!status) ? RGB(0, 0, 0) : RGB(0, 192, 0);
    }

    COLORREF getDoorLockColor(bool status) {
        return (!status) ? RGB(192, 0, 0) : RGB(0, 0, 192);
    }

public:
    IoTDeviceDeviceManager  oDeviceManager_;
    IoTDeviceOpenAPIManager oOpenAPIManager_;

    CString BulbName_[IoTDeviceDeviceManager::CV_MAX_BULB_COUNT];
    CUIColorStaticST ctlBulb_[IoTDeviceDeviceManager::CV_MAX_BULB_COUNT];

    CString DoorLockName1_;
    CUIColorStaticST ctlDoorLock1_;
};
