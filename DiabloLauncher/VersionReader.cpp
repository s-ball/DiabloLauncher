#include "pch.h"
#include "VersionReader.h"
#include "DiabloLauncher.h"

void VersionReader::init(HWND hwnd)
{
	static LPCTSTR unknown = _T("unknown");
	static LPCTSTR default = _T("Copyright (C) 2024");
	DWORD handle;
	unsigned int size = 256;
	LPTSTR name = nullptr;
	for (;;) {
		name = new TCHAR[size];
		DWORD cr = GetModuleFileName(NULL, name, size);
		if (cr == size) {
			size *= 2;
			delete[] name;
		}
		else break;
	}
	size = GetFileVersionInfoSize(name, &handle);
	if (size == 0) {
		data.version = unknown;
		data.copyright = default;
		LPCTSTR msg;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
			0, (LPTSTR)&msg, 0, NULL);
		MessageBox(hwnd, msg, RsrcString(IDS_ERRORVERSIONINFO), MB_ICONERROR);
	}
	else {
		info = (void*) new char[size];
		GetFileVersionInfo(name, 0, size, info);
		BOOL cr = VerQueryValue(info, _T("\\StringFileInfo\\040904b0\\FileVersion"), (LPVOID*)&data.version, &size);
		if (!cr) {
			data.version = unknown;
			LPCTSTR msg;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
				0, (LPTSTR)&msg, 0, NULL);
			MessageBox(hwnd, msg, RsrcString(IDS_ERRORVERSION), MB_ICONERROR);
		}
		cr = VerQueryValue(info, _T("\\StringFileInfo\\040904b0\\LegalCopyright"), (LPVOID*)&data.copyright, &size);
		if (!cr) {
			data.copyright = default;
			LPCTSTR msg;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
				0, (LPTSTR)&msg, 0, NULL);
			MessageBox(hwnd, msg, RsrcString(IDS_ERRORCOPYRIGHT), MB_ICONERROR);
		}
	}
	delete[] name;
}
