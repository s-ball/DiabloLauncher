// Copyright 2024 s-ball - MIT License

#pragma once
struct Info {
	LPCTSTR version;
	LPCTSTR copyright;
	LANGID lid;
	unsigned int nblangs;
	WORD* langs;
};

class VersionLoader {
public:
	void vrInit(class VersionReader&, HWND hwnd, LANGID = 0) ;
};

class VersionReader
{
	void* info = NULL;
	Info data = { NULL };
	LPTSTR name = NULL;

	void init(HWND hwnd, LANGID = 0);

public:
	VersionReader() {}

	VersionReader(const VersionReader&) = delete;
	VersionReader(VersionReader&&) = delete;
	VersionReader& operator = (const VersionReader&) = delete;
	VersionReader& operator = (VersionReader&&) = delete;

	VersionReader(HWND hwnd, LANGID lid);
	~VersionReader() {
		delete[] info;
		delete[] name;
	}
	const Info& getInfo(HWND hwnd = NULL) {
		if (info == nullptr) init(hwnd);
		return data;
	}
	LANGID getLangId() {
		return data.lid;
	}
	int SetLang(unsigned int ilang);

	LPCTSTR getExeName() {
		return name;
	}

	friend void VersionLoader::vrInit(class VersionReader&, HWND hwnd, LANGID);
};

