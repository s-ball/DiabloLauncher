#include "pch.h"
#include "test.h"

extern CDiabloLauncherApp theApp;

namespace {
	bool getTime(LPCTSTR name, CString& path, LPFILETIME time) {
		path.Empty();
		path.SetString(theApp.getDiablo().getSave());
		path.AppendChar('\\');
		path.Append(name);
		path.Append(_T(".d2s"));
		HANDLE h_file = CreateFile(path, FILE_READ_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (h_file == INVALID_HANDLE_VALUE) {
			LPTSTR msg = NULL;
			//DWORD err = GetLastError();
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, 0, 0, (LPTSTR) & msg, 0, NULL);
			wprintf(_T("ERROR in CreateFile for >%s<: %s\n"), (LPCTSTR) path, msg);
			return false;
		}
		bool cr = GetFileTime(h_file, NULL, NULL, time);
		CloseHandle(h_file);
		return cr;
	}

	bool setTime(const CString& path, const LPFILETIME time) {
		HANDLE h_file = CreateFile(path, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (h_file == INVALID_HANDLE_VALUE) return false;
		bool cr = SetFileTime(h_file, NULL, NULL, time);
		CloseHandle(h_file);
		return cr;
	}

	void addMinutes(LPFILETIME time, int delta) {
		ULARGE_INTEGER ltime;
		ltime.LowPart = time->dwLowDateTime;
		ltime.HighPart = time->dwHighDateTime;
		ltime.QuadPart += (long long) delta * (600000000LL);
		time->dwLowDateTime = ltime.LowPart;
		time->dwHighDateTime = ltime.HighPart;
	}
}
class ManagerTest: public StateTest {
protected:
	void SetUp() override {
		StateTest::SetUp();
		State state(_T("foo"));
		CString path;
		FILETIME time;
		state.copyTo(1);
		if (getTime(_T("foo1"), path, &time)) {
			addMinutes(&time, -15);
			setTime(path, &time);
		}
		state.copyTo(2);
		if (getTime(_T("foo2"), path, &time)) {
			addMinutes(&time, -30);
			setTime(path, &time);
		}
	}

};

TEST_F(ManagerTest, Sorted) {
	Cd2sManager mgr;
	ASSERT_TRUE(mgr.load());
	const std::vector<State>& v = mgr.characters[_T("Foo")].saves;
	ASSERT_EQ(2, v.size());
	for (int i = 1; i < 3; i++) {
		EXPECT_EQ(i, v[i-1].getLev());
	}
}

TEST_F(ManagerTest, ChangeSort) {
	Cd2sManager mgr;
	FILETIME time;
	CString path;
	ASSERT_TRUE(getTime(_T("Foo1"), path, &time));
	addMinutes(&time, -45);
	ASSERT_TRUE(setTime(path, &time));
	ASSERT_TRUE(mgr.load());
	const std::vector<State>& v = mgr.characters[_T("Foo")].saves;
	EXPECT_EQ(2, v.size());
	EXPECT_EQ(2, v[0].getLev());
	EXPECT_EQ(1, v[1].getLev());
}

TEST_F(ManagerTest, NoCur) {
	Cd2sManager mgr;
	CString path(theApp.getDiablo().getSave());
	path.AppendChar('\\');
	path.Append(_T("Foo.d2s"));
	::DeleteFile(path);
	ASSERT_TRUE(mgr.load());
	Character& c = mgr.characters[_T("Foo")];
	EXPECT_EQ(2, c.saves.size());
	EXPECT_EQ(-1, c.cur.getLev());
	EXPECT_TRUE(c.saves[0] < c.cur);
}

TEST_F(ManagerTest, SortItems) {
	Cd2sManager mgr;
	mgr.load();
	std::vector<const Character*> items = mgr.getItems();
	ASSERT_EQ(2, items.size());
	EXPECT_STREQ(_T("Foo"), (LPCTSTR)items[0]->cur.getName());
	EXPECT_STREQ(_T("abcdefghijklmno"), (LPCTSTR)items[1]->cur.getName());
}

TEST_F(ManagerTest, NextLevForEmpty) {
	Cd2sManager mgr;
	mgr.load();
	Character& c = mgr.characters[_T("abcdefghijklmno")];
	ASSERT_EQ(1, c.getNextLevel());
}

TEST_F(ManagerTest, NextNoGap) {
	Cd2sManager mgr;
	mgr.load();
	Character& c = mgr.characters[_T("Foo")];
	ASSERT_EQ(3, c.getNextLevel());
}

TEST_F(ManagerTest, NextGap1) {
	State st1(_T("Foo1"));
	st1.copyTo(4);
	st1.del();
	Cd2sManager mgr;
	mgr.load();
	Character& c = mgr.characters[_T("Foo")];
	ASSERT_EQ(1, c.getNextLevel());
}

TEST_F(ManagerTest, NextGap2) {
	State st1(_T("Foo2"));
	st1.copyTo(4);
	st1.del();
	Cd2sManager mgr;
	mgr.load();
	Character& c = mgr.characters[_T("Foo")];
	ASSERT_EQ(2, c.getNextLevel());
}

TEST_F(ManagerTest, Save) {
	Cd2sManager mgr;
	mgr.load();
	Character& c = mgr.characters[_T("Foo")];
	const State st = c.save();
	EXPECT_EQ(3, st.getLev());
	EXPECT_EQ(3, c.saves[0].getLev());
	EXPECT_EQ(3, c.saves.size());
}

TEST_F(ManagerTest, SaveFirst) {
	Cd2sManager mgr;
	mgr.load();
	Character& c = mgr.characters[_T("abcdefghijklmno")];
	const State st = c.save();
	EXPECT_EQ(1, st.getLev());
	EXPECT_EQ(1, c.saves[0].getLev());
	EXPECT_EQ(1, c.saves.size());
}

TEST_F(ManagerTest, SaveSort) {
	Cd2sManager mgr;
	FILETIME time;
	CString path;
	ASSERT_TRUE(getTime(_T("Foo"), path, &time));
	addMinutes(&time, -20);
	ASSERT_TRUE(setTime(path, &time));
	mgr.load();
	Character& c = mgr.characters[_T("Foo")];
	const State st = c.save();
	EXPECT_EQ(3, st.getLev());
	EXPECT_EQ(3, c.saves[1].getLev());
	EXPECT_EQ(3, c.saves.size());
}

TEST_F(ManagerTest, SaveSortRecycle) {
	Cd2sManager mgr;
	FILETIME time;
	CString path;
	ASSERT_TRUE(getTime(_T("Foo"), path, &time));
	addMinutes(&time, -20);
	ASSERT_TRUE(setTime(path, &time));
	mgr.load();
	Character& c = mgr.characters[_T("Foo")];
	const State st = c.save(true);
	EXPECT_EQ(2, st.getLev());
	EXPECT_EQ(2, c.saves[1].getLev());
	EXPECT_EQ(2, c.saves.size());
}

TEST_F(ManagerTest, Restore) {
	Cd2sManager mgr;
	FILETIME time, time1;
	CString path;
	ASSERT_TRUE(getTime(_T("Foo"), path, &time));
	ASSERT_TRUE(getTime(_T("Foo1"), path, &time1));
	ASSERT_NE(time.dwLowDateTime, time1.dwLowDateTime);
	ASSERT_NE(time.dwHighDateTime, time1.dwHighDateTime);
	State state(_T("Foo1"));
	state.copyTo(0);
	ASSERT_TRUE(getTime(_T("Foo"), path, &time));
	ASSERT_EQ(time.dwLowDateTime, time1.dwLowDateTime);
	ASSERT_EQ(time.dwHighDateTime, time1.dwHighDateTime);
	mgr.load();
	Character& c = mgr.characters[_T("Foo")];
	ASSERT_EQ(time1.dwLowDateTime, c.cur.getTime().dwLowDateTime);
	ASSERT_EQ(time1.dwHighDateTime, c.cur.getTime().dwHighDateTime);
}

TEST_F(ManagerTest, Restore2) {
	Cd2sManager mgr;
	FILETIME time, time1;
	CString path;
	ASSERT_TRUE(getTime(_T("Foo"), path, &time));
	ASSERT_TRUE(getTime(_T("Foo2"), path, &time1));
	ASSERT_NE(time.dwLowDateTime, time1.dwLowDateTime);
	ASSERT_NE(time.dwHighDateTime, time1.dwHighDateTime);
	State state(_T("Foo2"));
	state.copyTo(0);
	ASSERT_TRUE(getTime(_T("Foo"), path, &time));
	ASSERT_EQ(time.dwLowDateTime, time1.dwLowDateTime);
	ASSERT_EQ(time.dwHighDateTime, time1.dwHighDateTime);
	mgr.load();
	Character& c = mgr.characters[_T("Foo")];
	ASSERT_EQ(time1.dwLowDateTime, c.cur.getTime().dwLowDateTime);
	ASSERT_EQ(time1.dwHighDateTime, c.cur.getTime().dwHighDateTime);
}