// Copyright 2024 s-ball - MIT License

#include "pch.h"
#include "VersionReader.h"
#include "DiabloLauncher.h"

void VersionReader::init(HWND hwnd, LANGID lid)
{
	if (lid == 0) lid = GetThreadUILanguage();

	static LPCTSTR unknown = _T("unknown");
	static LPCTSTR default = _T("Copyright (C) 2024");
	DWORD handle;
	unsigned int size = 256;
	delete[] name;
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
		BOOL cr = VerQueryValue(info, _T("\\VarFileInfo\\Translation"), (LPVOID*)&data.langs, &data.nblangs);
		if (!cr) {
			data.version = unknown;
			LPCTSTR msg;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
				0, (LPTSTR)&msg, 0, NULL);
			MessageBox(hwnd, msg, RsrcString(IDS_ERRORVAR), MB_ICONERROR);
		}
		else {
			data.nblangs /= sizeof(DWORD);
			WORD cod = 0;
			for (unsigned int i = 0; i < data.nblangs; i++) {
				if (data.langs[2 * i] == lid) {
					cod = data.langs[2 * i + 1];
					data.lid = lid;
					break;
				}
			}
			if (cod == 0) {
				data.lid = data.langs[0];
				cod = data.langs[1];
			}
			CString key;
			key.Format((_T("\\StringFileInfo\\%04x%04x\\")), data.lid, cod);
			BOOL cr = VerQueryValue(info, key + _T("FileVersion"), (LPVOID*)&data.version, &size);
			if (!cr) {
				data.version = unknown;
				LPCTSTR msg;
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
					0, (LPTSTR)&msg, 0, NULL);
				MessageBox(hwnd, msg, RsrcString(IDS_ERRORVERSION), MB_ICONERROR);
			}
			cr = VerQueryValue(info, key + _T("LegalCopyright"), (LPVOID*)&data.copyright, &size);
			if (!cr) {
				data.copyright = default;
				LPCTSTR msg;
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
					0, (LPTSTR)&msg, 0, NULL);
				MessageBox(hwnd, msg, RsrcString(IDS_ERRORCOPYRIGHT), MB_ICONERROR);
			}
		}
	}
}

VersionReader::VersionReader(HWND hwnd, LANGID lid)
{
	init(hwnd, lid);
}


int VersionReader::SetLang(unsigned int ilang)
{
	// TODO: Ajoutez ici votre code d'implémentation..
	if (ilang >= data.nblangs) return -1;
	data.lid = data.langs[2 * ilang];
	return 0;
}

void VersionLoader::vrInit(VersionReader& vr, HWND hwnd, LANGID lid)
{
	vr.init(hwnd, lid);
}
