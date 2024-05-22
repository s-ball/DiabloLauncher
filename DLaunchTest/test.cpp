#include "pch.h"
#include "test.h"

extern CDiabloLauncherApp theApp;

class TestRelay {
public:
	static void setDiabloReg(LPCTSTR name) {
		CDiablo::diabloReg = name;
	}
	static void setSavePath(LPTSTR path) {
		theApp.diablo.savePath = path;
	}
};
TEST(CDiabloTest, RegOk) {
	CDiablo diablo;
	EXPECT_EQ(ERROR_SUCCESS, diablo.init());
}

TEST(CDiabloTest, NotInstalled) {
	CDiablo diablo;
	TestRelay::setDiabloReg(_T("MISSING_KEY"));
	EXPECT_NE(ERROR_SUCCESS, diablo.init());
}

void StateTest::SetUpTestCase() {
	static const LPCTSTR cur_ext = _T("_cur");
	DWORD sz = ::GetCurrentDirectory(NULL, 0);
	curDir = new TCHAR[sz+lstrlen(data) + lstrlen(cur_ext)+1];
	::GetCurrentDirectory(sz, curDir);
	lstrcat(curDir, data);
	origDir = new TCHAR[sz + lstrlen(data) + 1];
	lstrcpy(origDir, curDir);
	lstrcat(curDir, cur_ext);
	origDir[lstrlen(origDir) + 1] = 0;
	curDir[lstrlen(curDir) + 1] = 0;
	TestRelay::setSavePath(curDir);
}
void StateTest::SetUp() {
	SHFILEOPSTRUCT fileOp = { NULL, FO_DELETE, curDir, curDir, FOF_NO_UI };
	::SHFileOperation(&fileOp);
	fileOp.pFrom = origDir;
	fileOp.pTo = curDir;
	fileOp.wFunc = FO_COPY;
	ASSERT_EQ(0, SHFileOperation(&fileOp));
}

void StateTest::TearDownTestCase() {
 	TestRelay::setSavePath(NULL);
	SHFILEOPSTRUCT fileOp = { NULL, FO_DELETE, curDir, curDir, FOF_NO_UI };
	::SHFileOperation(&fileOp);
	delete[] curDir;
	delete[] origDir;
}

LPTSTR StateTest::curDir = NULL;
LPTSTR StateTest::origDir = NULL;
const LPCTSTR StateTest::data = _T("\\data");

TEST_F(StateTest, DirOK) {
	ASSERT_NE(theApp.getDiablo().getSave(), nullptr);
}

TEST_F(StateTest, OrigCharacter) {
	static const LPCTSTR name = _T("abcdefghijklmno");
	State state(name);
	EXPECT_TRUE(state.prefix() == name);
}

TEST_F(StateTest, WithLev) {
	State state(_T("abcdefghijklmno"));
	EXPECT_TRUE(state.prefix(2) == _T("abcdefghijklmno2"));
}


TEST_F(StateTest, WrongName) {
	static const LPCTSTR origname = _T("abcdefghijklmno.d2s");
	static const LPCTSTR name = _T("xyz");
	CString origPath(theApp.getDiablo().getSave());
	origPath.AppendChar('\\');
	origPath.Append(origname);
	CString path(theApp.getDiablo().getSave());
	path.AppendChar('\\');
	path.Append(name);
	path.Append(_T(".d2s"));
	EXPECT_NE(0, MoveFile(origPath, path));
	EXPECT_THROW(State state(name), NameException);
}

TEST_F(StateTest, Copy) {
	State state(_T("abcdefghijklmno"));
	state = state.copyTo(1);
	EXPECT_EQ(1, state.getLev());
	EXPECT_STREQ(_T("abcdefghijklmno"), state.getName());
}

TEST_F(StateTest, Copy1) {
	State state(_T("abcdefghijklmno"));
	state = state.copyTo(1);
	state = state.copyTo(2);
	EXPECT_EQ(1, State(_T("abcdefghijklmno1")).getLev());
	EXPECT_EQ(2, state.getLev());
	EXPECT_STREQ(_T("abcdefghijklmno"), state.getName());
}

TEST_F(StateTest, Time) {
	State state(_T("abcdefghijklmno"));
	state = state.copyTo(1);
	ASSERT_EQ(1, state.getLev());
	CString path(theApp.getDiablo().getSave());
	path.AppendChar('\\');
	path.Append(_T("abcdefghijklmno.d2s"));
	HANDLE h = CreateFile(path, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
	ASSERT_NE(INVALID_HANDLE_VALUE, h);
	SYSTEMTIME stime;
	GetSystemTime(&stime);
	stime.wHour = stime.wMinute = stime.wSecond = stime.wMilliseconds = 0;
	FILETIME ftime;
	ASSERT_NE(0, SystemTimeToFileTime(&stime, &ftime));
	ASSERT_NE(0, SetFileTime(h, NULL, NULL, &ftime));
	CloseHandle(h);
	state = State(_T("abcdefghijklmno"));
	FILETIME ftime2 = state.getTime();
	SYSTEMTIME stime2;
	FileTimeToSystemTime(&ftime2, &stime2);
	ASSERT_EQ(0, CompareFileTime(&ftime, &ftime2));
}
