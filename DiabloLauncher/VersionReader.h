// Copyright 2024 s-ball - MIT License

#pragma once
struct Info {
	LPCTSTR version;
	LPCTSTR copyright;
	LANGID lid;
	unsigned int nblangs;
	WORD* langs;
};

class VersionReader
{
	void* info = NULL;
	Info data = { NULL };

	void init(HWND hwnd, LANGID = 0);

public:
	VersionReader() {}
	VersionReader(const VersionReader&) = delete;
	VersionReader(VersionReader&&) = delete;
	VersionReader(HWND hwnd, LANGID lid);
	~VersionReader() {
		delete[] info;
	}
	const Info& getInfo(HWND hwnd = NULL) {
		if (info == nullptr) init(hwnd);
		return data;
	}
	LANGID getLangId() {
		return data.lid;
	}
	int SetLang(unsigned int ilang);

	friend class CDiabloLauncherApp;
};

