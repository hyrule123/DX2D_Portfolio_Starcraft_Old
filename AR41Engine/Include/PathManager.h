#pragma once

#include "EngineInfo.h"

struct PathInfo
{
	TCHAR	Path[MAX_PATH];
	char	PathMultibyte[MAX_PATH];

	PathInfo() :
		Path{},
		PathMultibyte{}
	{
	}
};

class CPathManager
{
private:
	std::unordered_map<std::string, PathInfo*>	m_mapPath;

public:
	bool Init();
	bool AddPath(const std::string& Name, const TCHAR* Path,
		const std::string& BasePathName = ROOT_PATH);
	const PathInfo* FindPath(const std::string& Name);


    static DWORD ConvertMultibyteToUnicode(
        __out std::wstring& unicode,
        __in const char* ansi,
        __in const size_t ansi_size
    );

	static DWORD ConvertUnicodeToMultibyte(
		__out std::string& ansi,
		__in const wchar_t* unicode,
		__in const size_t unicode_size
	);

	static DWORD ConvertUnicodeToUTF8(
		__out std::string& utf8,
		__in const wchar_t* unicode,
		__in const size_t unicode_size
	);

	static DWORD ConvertUTF8ToUnicode(
		__out std::wstring& unicode,
		__in const char* utf8,
		__in const size_t utf8_size
	);


	DECLARE_SINGLE(CPathManager)
};

