
// IoTDeviceManagerEmulatorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "IoTDeviceManagerEmulator.h"
#include "IoTDeviceManagerEmulatorDlg.h"
#include "afxdialogex.h"

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccString.h"
#include "ccNetwork/ccNetworkManager.h"
#include "ccWebServer/ccWebServerManager.h"
#include "ccMongooseServer/ccMongooseWebServerObjectFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CIoTDeviceManagerEmulatorDlg 대화 상자



CIoTDeviceManagerEmulatorDlg::CIoTDeviceManagerEmulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IoTDeviceManagerEmulator_DIALOG, pParent)
    , doorLockName1_(_T(""))
    , oOpenAPIManager_(*this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    for (int index = 0; index < IoTDeviceDeviceManager::CV_MAX_BULB_COUNT; index++) {
        bulbName_[index] = oDeviceManager_.bulbName_[index].c_str();
    }

    doorLockName1_ = oDeviceManager_.doorLockName1_.c_str();
}

void CIoTDeviceManagerEmulatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_BULB_NAME_1, bulbName_[0]);
    DDX_Text(pDX, IDC_BULB_NAME_2, bulbName_[1]);
    DDX_Text(pDX, IDC_BULB_NAME_3, bulbName_[2]);
    DDX_Control(pDX, IDC_BULB_1, ctlBulb_[0]);
    DDX_Control(pDX, IDC_BULB_2, ctlBulb_[1]);
    DDX_Control(pDX, IDC_BULB_3, ctlBulb_[2]);
    DDX_Text(pDX, IDC_DOOR_LOCK_NAME_1, doorLockName1_);
    DDX_Control(pDX, IDC_DOOR_LOCK_1, ctlDoorLock1_);
}

BEGIN_MESSAGE_MAP(CIoTDeviceManagerEmulatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_SET_NAME, &CIoTDeviceManagerEmulatorDlg::OnBnClickedSetName)
END_MESSAGE_MAP()


// CIoTDeviceManagerEmulatorDlg 메시지 처리기

BOOL CIoTDeviceManagerEmulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

    //  for Network
    Luna::ccNetworkManager::instance().init();

    Luna::ccWebServerManager::instance().attach_factory(std::make_shared<Luna::ccMongooseWebServerObjectFactory>());
    Luna::ccWebServerManager::instance().create_web_server("MS Azure IoT PoC API Server", "8000", ".");

    Luna::ccWebServerManager::instance().add_restful_api(std::make_shared<IoTDeviceOpenAPIManager>(*this));
    Luna::ccWebServerManager::instance().start();
    

    for (int count = 0; count < IoTDeviceDeviceManager::CV_MAX_BULB_COUNT; count++)
        ctlBulb_[count].SetBkColor(getBulbColor(oDeviceManager_.BulbStatus_[count]));

    ctlDoorLock1_.SetBkColor(getDoorLockColor(oDeviceManager_.DoorLockStatus1_));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CIoTDeviceManagerEmulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CIoTDeviceManagerEmulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//
bool    CIoTDeviceManagerEmulatorDlg::setBulbStatus(std::string name, bool on) {

    if (name == "all_lightbulbs") {
        for (int count = 0; count < 3; count++) {
            oDeviceManager_.BulbStatus_[count] = on;
            ctlBulb_[count].SetBkColor(getBulbColor(oDeviceManager_.BulbStatus_[count]));
        }

        return true;
    }
    else {
        for (int count = 0; count < oDeviceManager_.CV_MAX_BULB_COUNT; count++) {
            if (bulbName_[count] == name.c_str()) {
                oDeviceManager_.BulbStatus_[count] = on;
                ctlBulb_[count].SetBkColor(getBulbColor(oDeviceManager_.BulbStatus_[count]));
                return true;
            }
        }
    }

    return false;
}

bool    CIoTDeviceManagerEmulatorDlg::getBulbStatus(std::string name, std::string& status_json) {
    if (name == "all_lightbulbs") {
        return getAllBulbsStatus(status_json);
    }
    else {
        for (int count = 0; count < oDeviceManager_.CV_MAX_BULB_COUNT; count++) {
            if (bulbName_[count] == name.c_str()) {
                Luna::ccString::format(
                    status_json,
                    "{\n" \
                    "  \"power_status\": %d,\n" \
                    "  \"bright_level\": 100\n" \
                    "}",
                    oDeviceManager_.BulbStatus_[count]);

                return true;
            }
        }
    }

    return false;
}

bool    CIoTDeviceManagerEmulatorDlg::getAllBulbsStatus(std::string& status_json) {
    std::string accessory_status;

    status_json =
        "{\n" \
        "  \"accessories\": [\n";

    for (int count = 0; count < oDeviceManager_.CV_MAX_BULB_COUNT; count++) {
        CT2A name(bulbName_[count]);

        Luna::ccString::format(
            accessory_status,
            "    {\n" \
            "      \"name\": \"%s\",\n" \
            "      \"power_status\": %d,\n" \
            "      \"bright_level\": 100\n" \
            "    }",
            name.m_psz,
            oDeviceManager_.BulbStatus_[count]);

        status_json += accessory_status;

        if (count == oDeviceManager_.CV_MAX_BULB_COUNT - 1)
            status_json += "\n";
        else
            status_json += ",\n";
    }

    status_json +=
        "  ]\n" \
        "}";

    return true;
}

bool    CIoTDeviceManagerEmulatorDlg::setDoorLockStatus(std::string name, std::string status) {

    if (doorLockName1_ == name.c_str()) {

        if (status == "closed")
            oDeviceManager_.DoorLockStatus1_ = true;
        else
            oDeviceManager_.DoorLockStatus1_ = false;

        ctlDoorLock1_.SetBkColor(getDoorLockColor(oDeviceManager_.DoorLockStatus1_));

        return true;
    }

    return false;
}

bool    CIoTDeviceManagerEmulatorDlg::getDoorLockStatus(std::string name, std::string& status_json) {

    if (doorLockName1_ == name.c_str()) {
        CT2A name(doorLockName1_);

        Luna::ccString::format(
            status_json,
            "    {\n" \
            "      \"name\": \"%s\",\n" \
            "      \"lock_status\": \"%s\"\n" \
            "    }",
            name.m_psz,
            (oDeviceManager_.DoorLockStatus1_) ? "closed" : "open");

        return true;
    }

    return false;
}

bool    CIoTDeviceManagerEmulatorDlg::getAllDoorLockStatus(std::string& status_json) {

    std::string accessory_status;

    status_json =
        "{\n" \
        "  \"accessories\": [\n";

    CT2A name(doorLockName1_);

    Luna::ccString::format(
        accessory_status,
        "    {\n" \
        "      \"name\": \"%s\",\n" \
        "      \"lock_status\": %d\n" \
        "    }\n",
        name.m_psz,
        oDeviceManager_.DoorLockStatus1_);

    status_json += accessory_status;

    status_json +=
        "  ]\n" \
        "}";

    return true;
}


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CIoTDeviceManagerEmulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIoTDeviceManagerEmulatorDlg::OnBnClickedSetName()
{
    UpdateData();
}
