#include "pch.h"
#include "CDiablo.h"
#include "tchar.h"
#include "DiabloLauncher.h"
#include "Cd2sManager.h"

class StateTest : public testing::Test {
protected:
	static LPTSTR curDir;
	static LPTSTR origDir;
	static const LPCTSTR data;

	static void SetUpTestCase();
	void SetUp() override;

	static void TearDownTestCase();
};