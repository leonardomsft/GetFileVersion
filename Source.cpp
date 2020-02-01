#include <Windows.h>
#include <winver.h>
#include <stdio.h>

int wmain(int argc, wchar_t* argv[])
{
	DWORD dwReserved, dwsize = NULL;
	UINT intResult, intArgLen, intFFILen, intTranslationLen, intPVLen = 0;
	BYTE * pVersionInfo = NULL;
	VS_FIXEDFILEINFO * FixedFileInfo = NULL;
	WCHAR szPath[MAX_PATH], szLangProductVersion[MAX_PATH] = {};
	WCHAR * lpVersion = NULL;

	if (argc != 2)
	{
		wprintf(L"\GetFileVersion Utility\n");
		wprintf(L"Created by Leonardo Fagundes. No rights reserved.\n\n");
		wprintf(L"Usage: GetFileVersion.exe <file path>\n");
		wprintf(L"Example: GetFileVersion.exe C:\\Windows\\System32\\ntdll.dll\n");
		wprintf(L"\n\n");

		return -1;
	}
	
	intArgLen = wcslen(argv[1]);
	
	wcscpy_s(szPath, intArgLen+1, argv[1]);

	//
	//Get the size of VS_VERSION_INFO
	//
	dwsize = GetFileVersionInfoSize(szPath, &dwReserved);

	if (dwsize == NULL) {

		wprintf(L"Error %d calling GetFileVersionInfoSize.\n", GetLastError());

		return -1;
	}

	//
	//Allocate a buffer
	//
	pVersionInfo = new BYTE[dwsize];


	//
	//Get VS_VERSION_INFO
	//
	if (!GetFileVersionInfo(szPath, dwReserved, dwsize, pVersionInfo))
	{
		wprintf(L"Error %d calling GetFileVersionInfo\n", GetLastError());

		return -1;
	}

	
	//
	//Get fixed part VS_FIXEDFILEINFO
	//
	if (!VerQueryValue(pVersionInfo, L"\\", (LPVOID*)&FixedFileInfo, &intFFILen))
	{
		wprintf(L"Error %d calling VerQueryValue\n", GetLastError());

		return -1;
	}

	wprintf(L"\nPrinting versioning information for file: %s:\n\n", szPath);
	wprintf(L"VS_FIXEDFILEINFO:\n  FileVersion: %d.%d.%d.%d\n", HIWORD(FixedFileInfo->dwFileVersionMS), LOWORD(FixedFileInfo->dwFileVersionMS), HIWORD(FixedFileInfo->dwFileVersionLS), LOWORD(FixedFileInfo->dwFileVersionLS));
	wprintf(L"  ProductVersion %d.%d.%d.%d\n", HIWORD(FixedFileInfo->dwProductVersionMS), LOWORD(FixedFileInfo->dwProductVersionMS), HIWORD(FixedFileInfo->dwProductVersionLS), LOWORD(FixedFileInfo->dwProductVersionLS));




	//
	// Read the list of languages and code pages.
	//
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslation;

	

	VerQueryValue(pVersionInfo, L"\\VarFileInfo\\Translation", (LPVOID*)& lpTranslation, &intTranslationLen);

	//
	// Loop through all languages printing FileVersion and ProductVersion
	//
	for (int i = 0; i < (intTranslationLen / sizeof(struct LANGANDCODEPAGE)); i++)
	{

		//
		// Get non-fixed FileVersion string
		//
		swprintf_s(szLangProductVersion, MAX_PATH, L"\\StringFileInfo\\%04x%04x\\FileVersion", lpTranslation[i].wLanguage, lpTranslation[i].wCodePage);

		VerQueryValue(pVersionInfo, (LPTSTR)szLangProductVersion, (LPVOID*)& lpVersion, &intPVLen);

		wprintf(L"StringFileInfo:\n  FileVersion: %s\n", lpVersion);


		//
		// Get non-fixed ProductVersion string
		//
		swprintf_s(szLangProductVersion, MAX_PATH, L"\\StringFileInfo\\%04x%04x\\ProductVersion", lpTranslation[i].wLanguage, lpTranslation[i].wCodePage);

		VerQueryValue(pVersionInfo, (LPTSTR)szLangProductVersion, (LPVOID*)& lpVersion, &intPVLen);

		wprintf(L"  ProductVersion: %s\n", lpVersion);

	}

	delete[] pVersionInfo;
}