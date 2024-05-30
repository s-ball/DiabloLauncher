// Copyright 2024 s-ball - MIT License

#pragma once
struct Info {
	LPCTSTR version;
	LPCTSTR copyright;
};

class VersionReader
{
	void* info = NULL;
	Info data;

	void init(HWND hwnd);

public:
	~VersionReader() {
		delete[] info;
	}
	const Info& getInfo(HWND hwnd = NULL) {
		if (info == nullptr) init(hwnd);
		return data;
	}
};

