#include "pch.h"
#include "Cd2sManager.h"
#include "DiabloLauncher.h"
#include <ktmw32.h>
#include <algorithm>

extern CDiabloLauncherApp theApp;

CString State::prefix(int lev) const
{
	CString p(name);

	if (lev == -1) {
		lev = this->lev;
	}
	if (lev > 0) {
		p.AppendFormat(_T("%d"), lev);
	}
	return p;
}

State::State(LPCTSTR filename) {
	CString path;
	path.Format(_T("%s\\%s.d2s"), theApp.getDiablo().getSave(), filename);
	LPCTSTR s = path.GetBuffer();
	CFile file;
	if (!file.Open(s, CFile::modeRead | CFile::typeBinary)) {
		throw NameException(filename);
	}
	GetFileTime(file.m_hFile, NULL, NULL,
		&(this->time));
	path.ReleaseBuffer();
	file.Seek(20, CFile::begin);
	char name[16];
	file.Read(name, 16);
	CString charName;
#ifdef UNICODE
	wchar_t buf[16] = { 0 };
	::MultiByteToWideChar(CP_ACP, 0,
		name, -1, buf, 16);
	charName = CString(buf);
#else
	charName = CString(name);
#endif // 
	if (0 == charName.CompareNoCase(filename)) {
		this->name = charName;
		lev = 0;
	}
	else if (charName.GetLength() < lstrlen(filename)
		&& 0 == charName.Left(lstrlen(filename)).CompareNoCase(charName)) {
		LPTSTR end;
		int pos = charName.GetLength();
		lev = _tcstol(filename+pos, &end, 10);
		if (errno != 0 || *end != 0) {
			throw NameException(filename);
		}
	}
	else throw NameException(filename);
	this->name = charName;
}


/*
bool State::setLev(int lev, bool special) {
	int cr;
	if (lev == this->lev && special == this->special) return true;
	HANDLE htransact = CreateTransaction(NULL, 0,
		0, 0, 0, 0, NULL);
	for (LPCTSTR ext : exts) {
		CString orig(theApp.getDiablo().getSave());
		orig.AppendChar('\\');
		orig.Append(prefix());
		orig.Append(ext);
		CString path(theApp.getDiablo().getSave());
		path.AppendChar('\\');
		path.Append(prefix(lev, special));
		path.Append(ext);
		cr = (this->lev == 0 || lev == 0) ? CopyFileTransacted(orig, path, NULL, NULL, NULL, 0, htransact)
			: MoveFileTransacted(orig, path, NULL, NULL, 0, htransact);
		if (cr == 0) {
			RollbackTransaction(htransact);
			CloseHandle(htransact);
			return false;
		}
	}
	cr = CommitTransaction(htransact);
	CloseHandle(htransact);
	if (cr == 0) return false;
	this->lev = lev;
	return true;
}
*/

const State State::copyTo(int lev) const {
	State state(*this);
	bool cr;
	if (lev != this->lev) {
		HANDLE htransact = CreateTransaction(NULL, 0,
			0, 0, 0, 0, NULL);
		for (LPCTSTR ext : exts) {
			CString orig(theApp.getDiablo().getSave());
			orig.AppendChar('\\');
			orig.Append(prefix());
			orig.Append(ext);
			CString path(theApp.getDiablo().getSave());
			path.AppendChar('\\');
			path.Append(prefix(lev));
			path.Append(ext);
			cr = CopyFileTransacted(orig, path, NULL, NULL, NULL, 0, htransact);
			if (cr == 0) {
				RollbackTransaction(htransact);
				CloseHandle(htransact);
				return State();
			}
		}
		cr = CommitTransaction(htransact);
		CloseHandle(htransact);
		if (cr == 0) return State();
		state.lev = lev;
	}
	return state;
}
void State::del() {
	for (LPCTSTR ext : exts) {
		CString path(theApp.getDiablo().getSave());
		path.AppendChar('\\');
		path.Append(prefix());
		path.Append(ext);
		DeleteFile(path);
	}
}

const LPCTSTR State::exts[4] = { _T(".d2s"), _T(".key"), _T(".ma0"), _T(".map") };

bool Cd2sManager::load(void (*err)(LPCTSTR))
{
	characters.clear();
	CString pattern(theApp.getDiablo().getSave());
	pattern.Append(_T("\\*.d2s"));
	WIN32_FIND_DATA data;
	HANDLE hfind = FindFirstFile(pattern, &data);
	if (hfind == INVALID_HANDLE_VALUE) {
		return (ERROR_FILE_NOT_FOUND == GetLastError());
	}
	for (;;) {
		try {
			data.cFileName[lstrlen(data.cFileName)-4] = 0;
			State state(PathFindFileName(data.cFileName));
			if (state.getLev() == 0) {
				characters[state.getName()].cur = state;
			}
			else {
				characters[state.getName()].saves.push_back(state);
			}
		}
		catch (NameException& exc) {
			if (err) {
				err(exc.getName());
			}
		}
		if (0 == FindNextFile(hfind, &data)) break;
	}
	FindClose(hfind);
	for (auto& x : characters) {
		std::sort(x.second.saves.begin(), x.second.saves.end());
	}
	
	return true;
}

std::vector<const Character*> Cd2sManager::getItems() const {
	std::vector<const Character*> items(characters.size(), nullptr);
	int i = 0;
	for (const auto& x : characters) {
		items[i++] = &(x.second);
	}
	std::sort(items.begin(), items.end(),
		[](const Character*& a, const Character*& b) {
			return *a < *b;
		});
	return items;
}

int Character::getNextLevel() const {
	if (saves.size() == 0) return 1;
	std::map<int, bool> taken;
	int max = 0;
	for (const State& st : saves) {
		int lev = st.getLev();
		if (lev > max) max = lev;
		taken[lev] = true;
	}
	for (int i = 1; i < max; i++) {
		if (taken.find(i) == taken.end()) return i;
	}
	return max + 1;
}

const State Character::save(bool recycle) {
	int lev;
	if (recycle) {
		State& state = saves.back();
		lev = state.getLev();
		state.del();
		saves.pop_back();
	}
	else lev = getNextLevel();
	State state = cur.copyTo(lev);
	if (state.getLev() == lev) {
		saves.insert(saves.cbegin(), state);
		if (saves.size() > 1 && saves[1] < saves[0]) {
			std::sort(saves.begin(), saves.end());
		}
	}
	return state;
}

 bool Character::restore(State state) {
	 if (state.getLev() == 0) return false;
	State nState = state.copyTo(0);
	bool cr = nState.getLev() == 0;
	if (cr) {
		cur = nState;
	}
	return cr;
}