
// IoTDeviceManagerEmulator.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CIoTDeviceManagerEmulatorApp:
// �� Ŭ������ ������ ���ؼ��� IoTDeviceManagerEmulator.cpp�� �����Ͻʽÿ�.
//

class CIoTDeviceManagerEmulatorApp : public CWinApp
{
public:
	CIoTDeviceManagerEmulatorApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CIoTDeviceManagerEmulatorApp theApp;
