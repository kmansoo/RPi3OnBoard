
// IoTDeviceManagerEmulatorDlg.cpp : ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CIoTDeviceManagerEmulatorDlg ��ȭ ����



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


// CIoTDeviceManagerEmulatorDlg �޽��� ó����

BOOL CIoTDeviceManagerEmulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

    //  for Network
    Luna::ccNetworkManager::instance().init();

    Luna::ccWebServerManager::instance().attach_factory(std::make_shared<Luna::ccMongooseWebServerObjectFactory>());
    Luna::ccWebServerManager::instance().create_web_server("MS Azure IoT PoC API Server", "8000", ".");

    Luna::ccWebServerManager::instance().add_restful_api(std::make_shared<IoTDeviceOpenAPIManager>(*this));
    Luna::ccWebServerManager::instance().start();
    

    for (int count = 0; count < IoTDeviceDeviceManager::CV_MAX_BULB_COUNT; count++)
        ctlBulb_[count].SetBkColor(getBulbColor(oDeviceManager_.BulbStatus_[count]));

    ctlDoorLock1_.SetBkColor(getDoorLockColor(oDeviceManager_.DoorLockStatus1_));

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CIoTDeviceManagerEmulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
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


// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CIoTDeviceManagerEmulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIoTDeviceManagerEmulatorDlg::OnBnClickedSetName()
{
    UpdateData();
}
