#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)



CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
	auto	iter = m_mapPath.begin();
	auto	iterEnd = m_mapPath.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CPathManager::Init()
{
	TCHAR	Root[MAX_PATH] = {};

	// 실행파일이있는 폴더까지의 전체경로/실행파일이름.exe 로 문자열이
	// 나오게 된다.
	// Bin/aa.exe
	GetModuleFileName(0, Root, MAX_PATH);

	int	Length = lstrlen(Root);

	for (int i = Length - 1; i >= 0; --i)
	{
		if (Root[i] == '/' || Root[i] == '\\')
		{
			memset(&Root[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, Root);

	// #ifdef : 뒤에 있는 내용이 #define으로 정의되어 있는지를
	// 판단하는 if문이다. 
	// 컴파일 단계에서 뒤에 있는 내용이 #define으로 정의되어 있는지 판단.
#ifdef UNICODE

	// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를
	// 얻어온다.
	int	PathLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		0, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		Info->PathMultibyte, PathLength, 0, 0);

#else

	strcpy_s(Info->PathMultibyte, Info->Path);

#endif // UNICODE


	m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

	AddPath(TEXTURE_PATH, TEXT("Texture/"));
	AddPath(SHADER_PATH, TEXT("Shader/"));
	AddPath(SOUND_PATH, TEXT("Sound/"));
	AddPath(FONT_PATH, TEXT("Font/"));
	AddPath(ANIMATION2D_PATH, TEXT("Animation2D/"));
	AddPath(SCENE_PATH, TEXT("Scene/"));
	AddPath(MAP_PATH, TEXT("Maps/"));
	AddPath(CDO_PATH, TEXT("CDO/"));

	return true;
}

bool CPathManager::AddPath(const std::string& Name,
	const TCHAR* Path, const std::string& BasePathName)
{
	if (FindPath(Name))
		return false;

	const PathInfo* BasePath = FindPath(BasePathName);

	PathInfo* Info = new PathInfo;

	if (BasePath)
		lstrcpy(Info->Path, BasePath->Path);

	lstrcat(Info->Path, Path);

#ifdef UNICODE

	// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를
	// 얻어온다.
	int	PathLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		0, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		Info->PathMultibyte, PathLength, 0, 0);

#else

	strcpy_s(Info->PathMultibyte, Info->Path);

#endif // UNICODE

	m_mapPath.insert(std::make_pair(Name, Info));

	return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
	auto	iter = m_mapPath.find(Name);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second;
}

DWORD CPathManager::ConvertMultibyteToUnicode(std::wstring& unicode, const char* ansi, const size_t ansi_size)
{

	DWORD error = 0;

	do {

		if ((nullptr == ansi) || (0 == ansi_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		unicode.clear();

		//
		// getting required cch.
		//

		int required_cch = ::MultiByteToWideChar(
			CP_ACP,
			0,
			ansi, static_cast<int>(ansi_size),
			nullptr, 0
		);

		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		unicode.resize(required_cch);

		//
		// convert.
		//

		if (0 == ::MultiByteToWideChar(
			CP_ACP,
			0,
			ansi, static_cast<int>(ansi_size),
			const_cast<wchar_t*>(unicode.c_str()), static_cast<int>(unicode.size())
		)) {
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}

DWORD CPathManager::ConvertUnicodeToMultibyte(std::string& ansi, const wchar_t* unicode, const size_t unicode_size)
{

	DWORD error = 0;

	do {

		if ((nullptr == unicode) || (0 == unicode_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		ansi.clear();

		//
		// getting required cch.
		//

		int required_cch = ::WideCharToMultiByte(
			CP_ACP,
			0,
			unicode, static_cast<int>(unicode_size),
			nullptr, 0,
			nullptr, nullptr
		);

		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		//
		// allocate.
		//

		ansi.resize(required_cch);

		//
		// convert.
		//

		if (0 == ::WideCharToMultiByte(
			CP_ACP,
			0,
			unicode, static_cast<int>(unicode_size),
			const_cast<char*>(ansi.c_str()), static_cast<int>(ansi.size()),
			nullptr, nullptr
		)) {
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}

DWORD CPathManager::ConvertUnicodeToUTF8(std::string& utf8, const wchar_t* unicode, const size_t unicode_size)
{

	DWORD error = 0;

	do {

		if ((nullptr == unicode) || (0 == unicode_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		utf8.clear();

		//
		// getting required cch.
		//

		int required_cch = ::WideCharToMultiByte(
			CP_UTF8,
			WC_ERR_INVALID_CHARS,
			unicode, static_cast<int>(unicode_size),
			nullptr, 0,
			nullptr, nullptr
		);

		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		//
		// allocate.
		//

		utf8.resize(required_cch);

		//
		// convert.
		//

		if (0 == ::WideCharToMultiByte(
			CP_UTF8,
			WC_ERR_INVALID_CHARS,
			unicode, static_cast<int>(unicode_size),
			const_cast<char*>(utf8.c_str()), static_cast<int>(utf8.size()),
			nullptr, nullptr
		)) {
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}

DWORD CPathManager::ConvertUTF8ToUnicode(std::wstring& unicode, const char* utf8, const size_t utf8_size)
{

	DWORD error = 0;

	do {

		if ((nullptr == utf8) || (0 == utf8_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		unicode.clear();

		//
		// getting required cch.
		//

		int required_cch = ::MultiByteToWideChar(
			CP_UTF8,
			MB_ERR_INVALID_CHARS,
			utf8, static_cast<int>(utf8_size),
			nullptr, 0
		);
		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		//
		// allocate.
		//

		unicode.resize(required_cch);

		//
		// convert.
		//

		if (0 == ::MultiByteToWideChar(
			CP_UTF8,
			MB_ERR_INVALID_CHARS,
			utf8, static_cast<int>(utf8_size),
			const_cast<wchar_t*>(unicode.c_str()), static_cast<int>(unicode.size())
		)) {
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}

