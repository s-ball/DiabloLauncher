#include "pch.h"
#include "CDiablo.h"

namespace {
	LSTATUS loadvalue(LPCTSTR name, LPTSTR* str) {
		DWORD sz = 0;
		LSTATUS cr = RegGetValue(
			HKEY_CURRENT_USER,
			CDiablo::getReg(),
			name,
			RRF_RT_REG_SZ,
			NULL,
			str,
			&sz
		);
		if (cr == ERROR_MORE_DATA) {
			*str = new TCHAR[sz];
			cr = RegGetValue(
				HKEY_CURRENT_USER,
				CDiablo::getReg(),
				name,
				RRF_RT_REG_SZ,
				NULL,
				*str,
				&sz
			);
		}
		return cr;
	}
}
int CDiablo::init()
{
	LSTATUS cr = loadvalue(_T("GamePath"), &gamePath);
	if (cr == ERROR_SUCCESS) {
		cr = loadvalue(_T("NewSavePath"), &savePath);
	}
	return cr;
}

LPCTSTR CDiablo::diabloReg = _T("Software\\Blizzard Entertainment\\Diablo II");
