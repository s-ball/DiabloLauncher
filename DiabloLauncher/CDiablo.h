#pragma once

class CDiablo
{
	LPTSTR gamePath = NULL;
	LPTSTR savePath = NULL;

public:
	~CDiablo() {
		delete[] gamePath;
		delete[] savePath;
	}
	int init();
	static inline LPCTSTR getReg() {
		return diabloReg;
	}
	LPCTSTR getGame() const {
		return gamePath;
	}
	LPCTSTR getSave() const {
		return savePath;
	}
private:
	static LPCTSTR diabloReg;
#ifdef IN_TEST
	friend class TestRelay;
#endif // IN_TEST
};

