// Copyright 2024 s-ball - MIT License

#pragma once

#include <vector>
#include <map>
#include <stdexcept>

class NameException: public std::invalid_argument {
	CString name;
public:
	NameException(const CString& name) : name(name), std::invalid_argument("Invalid save file") {}
	const CString& getName() const { return name; }
};

class State {
	CString name;
	int lev;
//	bool special = false;
	FILETIME time;
	static const LPCTSTR exts[6];

public:
	State() : lev(-1), time{ 0,0 } {};
	State(LPCTSTR filename);

	CString prefix(int lev = -1) const;

//	bool setLev(int level, bool special = false);
	const State copyTo(int lev) const;
	void del();
	int getLev() const { return lev; }
//	bool isSpecial() const { return special; }
	const CString& getName() const { return name; }
	const FILETIME& getTime() const { return time; }
	bool operator < (const State& other) const {
		return 0 < CompareFileTime(&this->time, &other.time);
	}
};
class Character {
public:
	State cur;
	std::vector<State> saves;

	bool operator < (const Character& other) const {
		return cur < other.cur;
	}

	int getNextLevel() const;
	const State save(bool recycle = false);
	bool restore(State state);
};
class Cd2sManager
{
public:
	std::map<CString, Character> characters;
	bool load(void (*err)(LPCTSTR) = nullptr);
	std::vector<const Character*> getItems() const;
};

