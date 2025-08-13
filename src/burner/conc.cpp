#include "burner.h"

#if defined(BUILD_WIN32)
#include <windows.h>
#endif

// Helper function to check if a given byte sequence is a valid UTF-8 sequence
bool IsValidUtf8Sequence(const unsigned char* data, size_t length) {
	for (size_t i = 0; i < length;) {
		unsigned char c = data[i];
		if (c <= 0x7F) { // 1-byte (ASCII)
			i++;
		} else if ((c & 0xE0) == 0xC0) { // 2-byte
			if (i + 1 >= length || (data[i + 1] & 0xC0) != 0x80) return false;
			unsigned int codePoint = ((c & 0x1F) << 6) | (data[i + 1] & 0x3F);
			if (codePoint < 0x80) return false; // Overlong encoding
			i += 2;
		} else if ((c & 0xF0) == 0xE0) { // 3-byte
			if (i + 2 >= length ||
				(data[i + 1] & 0xC0) != 0x80 ||
				(data[i + 2] & 0xC0) != 0x80) return false;
			unsigned int codePoint = ((c & 0x0F) << 12) | ((data[i + 1] & 0x3F) << 6) | (data[i + 2] & 0x3F);
			if (codePoint < 0x800 || (codePoint >= 0xD800 && codePoint <= 0xDFFF)) return false; // Surrogates
			i += 3;
		} else if ((c & 0xF8) == 0xF0) { // 4-byte
			if (i + 3 >= length ||
				(data[i + 1] & 0xC0) != 0x80 ||
				(data[i + 2] & 0xC0) != 0x80 ||
				(data[i + 3] & 0xC0) != 0x80) return false;
			unsigned int codePoint = ((c & 0x07) << 18) | ((data[i + 1] & 0x3F) << 12) |
									((data[i + 2] & 0x3F) << 6) | (data[i + 3] & 0x3F);
			if (codePoint < 0x10000 || codePoint > 0x10FFFF) return false; // Beyond Unicode range
			i += 4;
		} else {
			return false;
		}
	}
	return true;
}

// Main detection function for char vector
const char* DetectFileEncoding(const std::vector<char>& data) {
	if (IsValidUtf8Sequence(reinterpret_cast<const unsigned char*>(data.data()), data.size())) {
		return "UTF-8";
	}
	return "ANSI";
}

// Overload for wchar_t vector (handles both Windows and non-Windows)
const char* DetectFileEncoding(const std::vector<wchar_t>& wideData) {
#if defined(BUILD_WIN32)
	// Convert wide characters to UTF-8
	int nLen = WideCharToMultiByte(CP_UTF8, 0, wideData.data(), -1, NULL, 0, NULL, NULL);
	if (nLen > 0) {
		std::vector<char> utf8Data(nLen);
		WideCharToMultiByte(CP_UTF8, 0, wideData.data(), -1, utf8Data.data(), nLen, NULL, NULL);
		return DetectFileEncoding(utf8Data);
	} else {
		return "ANSI"; // Fallback if conversion fails
	}
#else
	// On non-Windows, assume wchar_t is UTF-32 or similar and convert to UTF-8
	std::vector<char> utf8Data;
	for (std::vector<wchar_t>::const_iterator it = wideData.begin(); it != wideData.end(); ++it) {
		const wchar_t wc = *it;
		// For simplicity, assume each wchar_t is a single UTF-8 character (not always correct)
		utf8Data.push_back(static_cast<char>(wc));
	}
	return DetectFileEncoding(utf8Data);
#endif

}

// Overload for char array (C-style string)
const char* DetectFileEncoding(const char* data, size_t length) {
	std::vector<char> vec(data, data + length);
	return DetectFileEncoding(vec);
}

// Overload for wchar_t array (C-style wide string)
const char* DetectFileEncoding(const wchar_t* data, size_t length) {
	std::vector<wchar_t> vec(data, data + length);
	return DetectFileEncoding(vec);
}


#if defined(BUILD_WIN32)

/**
 * @brief Convert a TCHAR string to a UTF-8 string (Windows implementation).
 * @param pszInString Input TCHAR string to convert.
 * @param pszOutString Output buffer for the UTF-8 string. If NULL, a static buffer is used.
 * @param nOutSize Size of the output buffer in bytes.
 * @return Pointer to the converted UTF-8 string (pszOutString or static buffer), or NULL on failure.
 * @note On Windows, uses WideCharToMultiByte for conversion from UTF-16 to UTF-8.
 * @warning The static buffer is not thread-safe. Avoid using NULL for pszOutString in multi-threaded contexts.
 */
char* TCHAR2CHAR(const TCHAR* pszInString, char* pszOutString, int nOutSize) {
	static char szStringBuffer[1024];
	if (!pszOutString) {
		pszOutString = szStringBuffer;
		nOutSize = sizeof(szStringBuffer);
	}

	if (nOutSize <= 0) return NULL;

	if (pszOutString == szStringBuffer) {
		memset(szStringBuffer, 0, sizeof(szStringBuffer));
	}
	// Detect the encoding of the input string
	const char* encoding = DetectFileEncoding(pszInString, _tcslen(pszInString) * sizeof(TCHAR));

	UINT codePage = CP_UTF8; // Default to UTF-8
	if (strcmp(encoding, "ANSI") == 0) {
		codePage = CP_ACP; // Use ANSI code page
	}

	// Perform the actual conversion
	int nBufferSize = WideCharToMultiByte(codePage, 0, pszInString, -1, pszOutString, nOutSize, NULL, NULL);
	if (nBufferSize > 0) {
		return pszOutString;
	}

	return NULL;
}

/**
 * @brief Convert a UTF-8 string to a TCHAR string (Windows implementation).
 * @param pszInString Input UTF-8 string to convert.
 * @param pszOutString Output buffer for the TCHAR string. If NULL, a static buffer is used.
 * @param nOutSize Size of the output buffer in TCHARs.
 * @return Pointer to the converted TCHAR string (pszOutString or static buffer), or NULL on failure.
 * @note On Windows, uses MultiByteToWideChar for conversion from UTF-8 to UTF-16.
 * @warning The static buffer is not thread-safe. Avoid using NULL for pszOutString in multi-threaded contexts.
 */
TCHAR* CHAR2TCHAR(const char* pszInString, TCHAR* pszOutString, int nOutSize) {
	static TCHAR szStringBuffer[1024];
	if (!pszOutString) {
		pszOutString = szStringBuffer;
		nOutSize = sizeof(szStringBuffer) / sizeof(TCHAR);
	}

	if (nOutSize <= 0) return NULL;

	// Detect the encoding of the input string
	const char* encoding = DetectFileEncoding(pszInString, strlen(pszInString));

	UINT codePage = CP_UTF8; // Default to UTF-8
	if (strcmp(encoding, "ANSI") == 0) {
		codePage = CP_ACP; // Use ANSI code page
	}

	// Perform the actual conversion
	int nBufferSize = MultiByteToWideChar(codePage, 0, pszInString, -1, pszOutString, nOutSize);
	if (nBufferSize > 0) {
		return pszOutString;
	}
	return NULL;
}

#else // Non-Windows implementation

/**
 * @brief Convert a TCHAR string to an ANSI string (non-Unicode implementation).
 * @param pszInString Input TCHAR string to convert.
 * @param pszOutString Output buffer for the ANSI string. If NULL, a static buffer is used.
 * @param nOutSize Size of the output buffer in bytes.
 * @return Pointer to the converted ANSI string (pszOutString or static buffer), or NULL on failure.
 * @note Assumes TCHAR is the same as char (no conversion needed).
 * @warning The static buffer is not thread-safe. Avoid using NULL for pszOutString in multi-threaded contexts.
 */
char* TCHAR2CHAR(const TCHAR* pszInString, char* pszOutString, int nOutSize) {
	static char szStringBuffer[1024];
	if (!pszOutString) {
		pszOutString = szStringBuffer;
		nOutSize = sizeof(szStringBuffer);
	}

	if (nOutSize > 0) {
		snprintf(pszOutString, nOutSize, "%s", pszInString);
		return pszOutString;
	}
	return NULL;
}

/**
 * @brief Convert an ANSI string to a TCHAR string (non-Unicode implementation).
 * @param pszInString Input ANSI string to convert.
 * @param pszOutString Output buffer for the TCHAR string. If NULL, a static buffer is used.
 * @param nOutSize Size of the output buffer in TCHARs.
 * @return Pointer to the converted TCHAR string (pszOutString or static buffer), or NULL on failure.
 * @note Assumes TCHAR is the same as char (no conversion needed).
 * @warning The static buffer is not thread-safe. Avoid using NULL for pszOutString in multi-threaded contexts.
 */
TCHAR* CHAR2TCHAR(const char* pszInString, TCHAR* pszOutString, int nOutSize) {
	static TCHAR szStringBuffer[1024];
	if (!pszOutString) {
		pszOutString = szStringBuffer;
		nOutSize = sizeof(szStringBuffer) / sizeof(TCHAR);
	}

	if (nOutSize > 0) {
		snprintf((char*)pszOutString, nOutSize * sizeof(TCHAR), "%s", pszInString);
		return pszOutString;
	}
	return NULL;
}

#endif

/**
 * @brief Convert a TCHAR string to a char string.
 * @param dest Input TCHAR string to convert.
 * @return A vector of chars containing the converted string.
 * @note On Windows, uses WideCharToMultiByte for conversion from UTF-16 to UTF-8 or ANSI.
 * @note On non-Windows platforms, uses iconv for conversion.
 */
std::vector<char> TCHAR2CHAR(const std::vector<TCHAR>& dest) {
	std::vector<char> charContent;

#if defined(BUILD_WIN32)
	int nLen = WideCharToMultiByte(CP_UTF8, 0, dest.data(), -1, NULL, 0, NULL, NULL);
	if (nLen > 0) {
		charContent.resize(nLen);
		WideCharToMultiByte(CP_UTF8, 0, dest.data(), -1, charContent.data(), nLen, NULL, NULL);
	}
#else
	charContent.resize(dest.size());
	for (size_t i = 0; i < dest.size(); ++i) {
		charContent[i] = static_cast<char>(dest[i]);
	}
#endif

	return charContent;
}

/**
 * @brief Convert a vector of chars to a vector of TCHARs using the existing CHAR2TCHAR function.
 * @param dest Input vector of chars to convert.
 * @return A vector of TCHARs containing the converted string.
 * @note The function uses the existing CHAR2TCHAR function to perform the actual character conversion.
 * @warning The function assumes that CHAR2TCHAR handles buffer management correctly.
 */
std::vector<TCHAR> CHAR2TCHAR(const std::vector<char>& dest) {
	std::vector<TCHAR> tcharContent;
	tcharContent.resize(dest.size() + 1);

	TCHAR* pszOutString = tcharContent.data();
	if (CHAR2TCHAR(dest.data(), pszOutString, tcharContent.size())) {
		return tcharContent;
	} else {
		tcharContent.clear();
		return tcharContent;
	}
}

/**
 * @brief Converts an ANSI-encoded string to a TCHAR-encoded string.
 * @param pszInString Input ANSI string to convert.
 * @param pszOutString Output buffer for the TCHAR string. If NULL, a new buffer is allocated.
 * @param nOutSize Size of the output buffer in TCHARs.
 * @return Pointer to the converted TCHAR string, or NULL on failure.
 * @note On Windows, uses MultiByteToWideChar to convert ANSI to UTF-16 (TCHAR is UTF-16 on Windows).
 * @note On non-Windows platforms, uses iconv to convert ANSI to WCHAR_T.
 * @warning The caller is responsible for freeing the memory allocated for the output buffer if pszOutString is NULL.
 */
TCHAR* CHAR2TCHAR_ANSI(const char* pszInString, TCHAR* pszOutString, int nOutSize) {
#if defined(BUILD_WIN32)
	// Convert ANSI to UTF-16 (TCHAR on Windows)
	if (!pszOutString) {
		// Calculate buffer size
		int nLen = MultiByteToWideChar(CP_ACP, 0, pszInString, -1, NULL, 0);
		if (nLen <= 0) return NULL;

		// Allocate buffer
		pszOutString = new TCHAR[nLen];
		nOutSize = nLen;
	}

	// Perform conversion
	if (MultiByteToWideChar(CP_ACP, 0, pszInString, -1, pszOutString, nOutSize) > 0) {
		return pszOutString;
	}

	// Cleanup on failure
	delete[] pszOutString;
	return NULL;
#else
	// Perform ASCII conversion
	if (!pszOutString) {
		// Allocate buffer
		size_t in_len = strlen(pszInString);
		pszOutString = new TCHAR[in_len + 1];
		nOutSize = in_len + 1;
	}

	// Convert each character
	for (int i = 0; i < nOutSize - 1 && pszInString[i] != '\0'; ++i) {
		pszOutString[i] = static_cast<TCHAR>(pszInString[i]);
	}
	pszOutString[nOutSize - 1] = _T('\0'); // Null-terminate

	return pszOutString;
#endif
}

/*
1.Extract the corresponding line from cheat.dat to a global variable, and pass the variable to ConfigParseMAMEFile_internal.
2.Extract the corresponding <drvname.ini> from cheat.zip or cheat.7z, process the 'include' line to extract another corresponding ini, 
  store to a global variable, and pass the variable to ConfigParseFile.

Overall, reseting and applying cheats will directly jump to the step(use mamedat or zip/7z or ini or nebula dat) that was determined during 
the loading of the game, without further processing of dat and zip/7z files.This will prevent a brief freezing when returning to the game.
*/
std::vector<TCHAR> CurrentMameCheatContent; // Global
std::vector<TCHAR> CurrentWayderMameCheatContent; // Global
std::vector<TCHAR> CurrentIniCheatContent; // Global
int usedCheatType = 0; //Global so we'll know if cheatload is already done or which cheat type it uses?

// GameGenie stuff is handled a little differently..
#define HW_NES ( ((BurnDrvGetHardwareCode() & HARDWARE_PUBLIC_MASK) == HARDWARE_NES) || ((BurnDrvGetHardwareCode() & HARDWARE_PUBLIC_MASK) == HARDWARE_FDS) )
#define HW_SNES ( ((BurnDrvGetHardwareCode() & HARDWARE_PUBLIC_MASK) == HARDWARE_SNES) )
#define HW_GGENIE ( HW_NES || HW_SNES )

static CheatInfo* pCurrentCheat = NULL;
static CheatInfo* pPreviousCheat = NULL;

static bool SkipComma(TCHAR** s)
{
	while (**s && **s != _T(',')) {
		(*s)++;
	}

	if (**s == _T(',')) {
		(*s)++;
	}

	if (**s) {
		return true;
	}

	return false;
}

static void CheatError(TCHAR* pszFilename, INT32 nLineNumber, CheatInfo* pCheat, TCHAR* pszInfo, TCHAR* pszLine)
{
#if defined (BUILD_WIN32)
	FBAPopupAddText(PUF_TEXT_NO_TRANSLATE, _T("Cheat file %s is malformed.\nPlease remove or repair the file.\n\n"), pszFilename);
	if (pCheat) {
		FBAPopupAddText(PUF_TEXT_NO_TRANSLATE, _T("Parse error at line %i, in cheat \"%s\".\n"), nLineNumber, pCheat->szCheatName);
	} else {
		FBAPopupAddText(PUF_TEXT_NO_TRANSLATE, _T("Parse error at line %i.\n"), nLineNumber);
	}

	if (pszInfo) {
		FBAPopupAddText(PUF_TEXT_NO_TRANSLATE, _T("Problem:\t%s.\n"), pszInfo);
	}
	if (pszLine) {
		FBAPopupAddText(PUF_TEXT_NO_TRANSLATE, _T("Text:\t%s\n"), pszLine);
	}

	FBAPopupDisplay(PUF_TYPE_ERROR);
#endif

#if defined(BUILD_SDL2)
	printf("Cheat file %s is malformed.\nPlease remove or repair the file.\n\n", pszFilename);
	if (pCheat) {
		printf("Parse error at line %i, in cheat \"%s\".\n", nLineNumber, pCheat->szCheatName);
	} else {
		printf("Parse error at line %i.\n", nLineNumber);
	}

	if (pszInfo) {
		printf("Problem:\t%s.\n", pszInfo);
	}
	if (pszLine) {
		printf("Text:\t%s\n", pszLine);
	}
#endif
}

static TCHAR* getFilenameFromPath(TCHAR* path) {
	if (!path) {
		return NULL;
	}

    TCHAR* filename = path;

    for (TCHAR* p = path; *p != '\0'; p++) {
        if (*p == '/' || *p == '\\') {
            filename = p + 1;
        }
    }
    return filename;
}

static void CheatLinkNewNode(TCHAR *szDerp)
{
	// Link new node into the list
	pPreviousCheat = pCurrentCheat;
	pCurrentCheat = (CheatInfo*)malloc(sizeof(CheatInfo));
	if (pCheatInfo == NULL) {
		pCheatInfo = pCurrentCheat;
	}

	memset(pCurrentCheat, 0, sizeof(CheatInfo));
	pCurrentCheat->pPrevious = pPreviousCheat;
	if (pPreviousCheat) {
		pPreviousCheat->pNext = pCurrentCheat;
	}

	// Fill in defaults
	pCurrentCheat->nType = 0;							    // Default to cheat type 0 (apply each frame)
	pCurrentCheat->nStatus = -1;							// Disable cheat
	pCurrentCheat->nDefault = 0;							// Set default option
	pCurrentCheat->bOneShot = 0;							// Set default option (off)
	pCurrentCheat->bWatchMode = 0;							// Set default option (off)

	_tcsncpy (pCurrentCheat->szCheatName, szDerp, QUOTE_MAX);
}

// pszFilename only uses for cheaterror as string while iniContent,not as file
// while no iniContent,process ini File
static INT32 ConfigParseFile(TCHAR* pszFilename, const std::vector<TCHAR>* iniContent = NULL)
{
#define INSIDE_NOTHING (0xFFFF & (1 << ((sizeof(TCHAR) * 8) - 1)))

	TCHAR szLine[8192];
	TCHAR* s;
	TCHAR* t;
	INT32 nLen;

	INT32 nLine = 0;
	TCHAR nInside = INSIDE_NOTHING;
	bool bFirst = true;

	FILE* h = NULL;
	const TCHAR* iniPtr = NULL;

	TCHAR* pszFileHeading = getFilenameFromPath(pszFilename);
	
	if (iniContent) {
		iniPtr = iniContent->data();
	} else {
		TCHAR* pszReadMode = AdaptiveEncodingReads(pszFilename);
		if (NULL == pszReadMode) pszReadMode = _T("rt");

		h = _tfopen(pszFilename, pszReadMode);
		if (h == NULL) {
			if ((BurnDrvGetFlags() & BDF_CLONE) && BurnDrvGetText(DRV_PARENT)) {
				TCHAR szAlternative[MAX_PATH] = { 0 };
				_stprintf(szAlternative, _T("%s%s.ini"), szAppCheatsPath, BurnDrvGetText(DRV_PARENT));

				pszReadMode = AdaptiveEncodingReads(szAlternative);
				if (NULL == pszReadMode) pszReadMode = _T("rt");

				if (NULL == (h = _tfopen(szAlternative, pszReadMode)))
					return 1;
				pszFileHeading = getFilenameFromPath(szAlternative);

			} else {
				return 1;	// Parent driver
			}
		}
	}

	while (1) {
		if (iniContent) {
			if (*iniPtr == _T('\0')) {
				break;
			}
			TCHAR* p = szLine;
			while (*iniPtr && *iniPtr != _T('\n') && (p - szLine) < 8190) {
				*p++ = *iniPtr++;
			}
			if (*iniPtr == _T('\n') && (p - szLine) < 8190) {
				*p++ = *iniPtr++;
				*p = _T('\0');
			} else if ((p - szLine) == 8190) {
				*p++ = _T('\n');
				*p = _T('\0');
				while (*iniPtr && *iniPtr != _T('\n')) {
					iniPtr++;
				}
				if (*iniPtr == _T('\n')) {
					iniPtr++;
				}
			} else {
				*p = _T('\0');
			}
		} else {
			if (_fgetts(szLine, 8192, h) == NULL) {
				break;
			}
		}

		nLine++;

		nLen = _tcslen(szLine);

		// Get rid of the linefeed at the end
		while ((nLen > 0) && (szLine[nLen - 1] == 0x0A || szLine[nLen - 1] == 0x0D)) {
			szLine[nLen - 1] = 0;
			nLen--;
		}

		s = szLine;													// Start parsing

		if (s[0] == _T('/') && s[1] == _T('/')) {					// Comment
			continue;
		}

		if (!iniContent) {
			if ((t = LabelCheck(s, _T("include"))) != 0) {				// Include a file
				s = t;

				TCHAR szFilename[MAX_PATH] = _T("");

				// Read name of the cheat file
				TCHAR* szQuote = NULL;
				QuoteRead(&szQuote, NULL, s);

				_stprintf(szFilename, _T("%s%s.dat"), szAppCheatsPath, szQuote);	// Is it a fault?Why do we read a NebulaDatCheat here?
																					// Never mind,we already checked included ini before read to inicontent.
				if (ConfigParseFile(szFilename)) {
					_stprintf(szFilename, _T("%s%s.ini"), szAppCheatsPath, szQuote);
					if (ConfigParseFile(szFilename)) {
						CheatError(pszFilename, nLine, NULL, _T("included file doesn't exist"), szLine);
					}
				}

				continue;
			}
		}

		if ((t = LabelCheck(s, _T("cheat"))) != 0) {				// Add new cheat
			s = t;

			// Read cheat name
			TCHAR* szQuote = NULL;
			TCHAR* szEnd = NULL;

			QuoteRead(&szQuote, &szEnd, s);

			s = szEnd;

			if ((t = LabelCheck(s, _T("advanced"))) != 0) {			// Advanced cheat
				s = t;
			}

			SKIP_WS(s);

			if (nInside == _T('{')) {
				CheatError(pszFilename, nLine, pCurrentCheat, _T("missing closing bracket"), NULL);
				break;
			}
#if 0
			if (*s != _T('\0') && *s != _T('{')) {
				CheatError(pszFilename, nLine, NULL, _T("malformed cheat declaration"), szLine);
				break;
			}
#endif
			nInside = *s;

#ifndef __LIBRETRO__
			if (bFirst) {
				TCHAR szHeading[256];
				_stprintf(szHeading, _T("[ Cheats \"%s\" ]"), pszFileHeading);
				CheatLinkNewNode(szHeading);
				bFirst = false;
			}
#endif

			CheatLinkNewNode(szQuote);
			continue;
		}

#ifdef __LIBRETRO__
		_tcsncpy (pCurrentCheat->szCheatFilename, pszFileHeading, QUOTE_MAX);
#endif

		if ((t = LabelCheck(s, _T("type"))) != 0) {					// Cheat type
			if (nInside == INSIDE_NOTHING || pCurrentCheat == NULL) {
				CheatError(pszFilename, nLine, pCurrentCheat, _T("rogue cheat type"), szLine);
				break;
			}
			s = t;

			// Set type
			pCurrentCheat->nType = _tcstol(s, NULL, 0);

			continue;
		}

		if ((t = LabelCheck(s, _T("default"))) != 0) {				// Default option
			if (nInside == INSIDE_NOTHING || pCurrentCheat == NULL) {
				CheatError(pszFilename, nLine, pCurrentCheat, _T("rogue default"), szLine);
				break;
			}
			s = t;

			// Set default option
			pCurrentCheat->nDefault = _tcstol(s, NULL, 0);

			continue;
		}

		INT32 n = _tcstol(s, &t, 0);
		if (t != s) {				   								// New option

			if (nInside == INSIDE_NOTHING || pCurrentCheat == NULL) {
				CheatError(pszFilename, nLine, pCurrentCheat, _T("rogue option"), szLine);
				break;
			}

			// Link a new Option structure to the cheat
			if (n < CHEAT_MAX_OPTIONS) {
				s = t;

				// Read option name
				TCHAR* szQuote = NULL;
				TCHAR* szEnd = NULL;
				if (QuoteRead(&szQuote, &szEnd, s)) {
					CheatError(pszFilename, nLine, pCurrentCheat, _T("option name omitted"), szLine);
					break;
				}
				s = szEnd;

				if (pCurrentCheat->pOption[n] == NULL) {
					pCurrentCheat->pOption[n] = (CheatOption*)malloc(sizeof(CheatOption));
				}
				memset(pCurrentCheat->pOption[n], 0, sizeof(CheatOption));

				memcpy(pCurrentCheat->pOption[n]->szOptionName, szQuote, QUOTE_MAX * sizeof(TCHAR));

				INT32 nCurrentAddress = 0;
				bool bOK = true;
				while (nCurrentAddress < CHEAT_MAX_ADDRESS) {
					INT32 nCPU = 0, nAddress = 0, nValue = 0;

					if (SkipComma(&s)) {
						if (HW_GGENIE) {
							t = s;
							INT32 newlen = 0;
#if defined(BUILD_WIN32)
							for (INT32 z = 0; z < lstrlen(t); z++) {
#else
							for (INT32 z = 0; z < strlen(t); z++) {
#endif
								char c = toupper((char)*s);
								if ( ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '-' || c == ':')) && newlen < 10)
									pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].szGenieCode[newlen++] = c;
								s++;
								if (*s == _T(',')) break;
							}
							nAddress = 0xffff; // nAddress not used, but needs to be nonzero (NES/Game Genie)
						} else {
							nCPU = _tcstol(s, &t, 0);		// CPU number
							if (t == s) {
								CheatError(pszFilename, nLine, pCurrentCheat, _T("CPU number omitted"), szLine);
								bOK = false;
								break;
							}
							s = t;

							SkipComma(&s);
							nAddress = _tcstol(s, &t, 0);	// Address
							if (t == s) {
								bOK = false;
								CheatError(pszFilename, nLine, pCurrentCheat, _T("address omitted"), szLine);
								break;
							}
							s = t;

							SkipComma(&s);
							nValue = _tcstol(s, &t, 0);		// Value
							if (t == s) {
								bOK = false;
								CheatError(pszFilename, nLine, pCurrentCheat, _T("value omitted"), szLine);
								break;
							}
						}
					} else {
						if (nCurrentAddress) {			// Only the first option is allowed no address
							break;
						}
						if (n) {
							bOK = false;
							CheatError(pszFilename, nLine, pCurrentCheat, _T("CPU / address / value omitted"), szLine);
							break;
						}
					}

					pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nCPU = nCPU;
					pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nAddress = nAddress;
					pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nValue = nValue;
					nCurrentAddress++;
				}

				if (!bOK) {
					break;
				}

			}

			continue;
		}

		SKIP_WS(s);
		if (*s == _T('}')) {
			if (nInside != _T('{')) {
				CheatError(pszFilename, nLine, pCurrentCheat, _T("missing opening bracket"), NULL);
				break;
			}

			nInside = INSIDE_NOTHING;
		}

		// Line isn't (part of) a valid cheat
#if 0
		if (*s) {
			CheatError(pszFilename, nLine, NULL, _T("rogue line"), szLine);
			break;
		}
#endif

	}

	if (h) {
		fclose(h);
		usedCheatType = 4; // see usedCheatType define
	} else {
		usedCheatType = 3; // see usedCheatType define
	}

	return 0;
}

//TODO: make cross platform
static INT32 ConfigParseNebulaFile(TCHAR* pszFilename)
{
	TCHAR* pszReadMode = AdaptiveEncodingReads(pszFilename);
	if (NULL == pszReadMode) pszReadMode = _T("rt");

	FILE *fp = _tfopen(pszFilename, pszReadMode);
	TCHAR* pszFileHeading = getFilenameFromPath(pszFilename);
	if (fp == NULL) {
		if ((BurnDrvGetFlags() & BDF_CLONE) && BurnDrvGetText(DRV_PARENT)) {
			TCHAR szAlternative[MAX_PATH] = { 0 };
			_stprintf(szAlternative, _T("%s%s.dat"), szAppCheatsPath, BurnDrvGetText(DRV_PARENT));

			pszReadMode = AdaptiveEncodingReads(szAlternative);
			if (NULL == pszReadMode) pszReadMode = _T("rt");

			if (NULL == (fp = _tfopen(szAlternative, pszReadMode)))
				return 1;
			pszFileHeading = getFilenameFromPath(szAlternative);
		} else {
			return 1;	// Parent driver
		}
	}

	INT32 nLen;
	INT32 i, j, n = 0;
	TCHAR tmp[32];
	TCHAR szLine[1024];
	bool bFirst = true;

	while (1)
	{
		if (_fgetts(szLine, 1024, fp) == NULL)
			break;

		nLen = _tcslen(szLine);

		if (nLen < 3 || szLine[0] == '[') continue;

		if (!_tcsncmp (_T("Name="), szLine, 5))
		{
			n = 0;

#ifndef __LIBRETRO__
			if (bFirst) {
				TCHAR szHeading[256];
				_stprintf(szHeading, _T("[ Cheats \"%s\" (Nebula) ]"), pszFileHeading);
				CheatLinkNewNode(szHeading);
				bFirst = false;
			}
#endif

			CheatLinkNewNode(szLine + 5);

			pCurrentCheat->szCheatName[nLen-6] = _T('\0');

			continue;
		}

#ifdef __LIBRETRO__
		_tcsncpy (pCurrentCheat->szCheatFilename, pszFileHeading, QUOTE_MAX);
#endif

		if (!_tcsncmp (_T("Default="), szLine, 8) && n >= 0)
		{
			_tcsncpy (tmp, szLine + 8, nLen-9);
			tmp[nLen-9] = _T('\0');
#if defined(BUILD_WIN32)
			_stscanf (tmp, _T("%d"), &(pCurrentCheat->nDefault));
#else
			sscanf (tmp, _T("%d"), &(pCurrentCheat->nDefault));
#endif
			continue;
		}


		i = 0, j = 0;
		while (i < nLen)
		{
			if (szLine[i] == _T('=') && i < 4) j = i+1;
			if (szLine[i] == _T(',') || szLine[i] == _T('\r') || szLine[i] == _T('\n'))
			{
				if (pCurrentCheat->pOption[n] == NULL) {
					pCurrentCheat->pOption[n] = (CheatOption*)malloc(sizeof(CheatOption));
				}
				memset(pCurrentCheat->pOption[n], 0, sizeof(CheatOption));

				_tcsncpy (pCurrentCheat->pOption[n]->szOptionName, szLine + j, QUOTE_MAX * sizeof(TCHAR));
				pCurrentCheat->pOption[n]->szOptionName[i-j] = _T('\0');

				i++; j = i;
				break;
			}
			i++;
		}

		INT32 nAddress = -1, nValue = 0, nCurrentAddress = 0;
		while (nCurrentAddress < CHEAT_MAX_ADDRESS)
		{
			if (i == nLen) break;

			if (szLine[i] == _T(',') || szLine[i] == _T('\r') || szLine[i] == _T('\n'))
			{
				_tcsncpy (tmp, szLine + j, i-j);
				tmp[i-j] = _T('\0');

				if (nAddress == -1) {
#if defined(BUILD_WIN32)
					_stscanf (tmp, _T("%x"), &nAddress);
#else
					sscanf (tmp, _T("%x"), &nAddress);
#endif
				} else {
#if defined(BUILD_WIN32)
					_stscanf (tmp, _T("%x"), &nValue);
#else
					sscanf (tmp, _T("%x"), &nValue);
#endif

					pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nCPU = 0; 	// Always
					pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nAddress = nAddress ^ 1;
					pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nValue = nValue;
					nCurrentAddress++;

					nAddress = -1;
					nValue = 0;
				}
				j = i+1;
			}
			i++;
		}
		n++;
	}

	fclose (fp);
	usedCheatType = 5;// see usedCheatType define
	return 0;
}

#define IS_MIDWAY ((BurnDrvGetHardwareCode() & HARDWARE_PREFIX_MIDWAY) == HARDWARE_PREFIX_MIDWAY)

static INT32 ConfigParseMAMEFile_internal(const TCHAR *name, const TCHAR *pszFileHeading, int is_wayder)
{
#define AddressInfo()	\
	INT32 k = (flags >> 20) & 3;	\
	INT32 cpu = (flags >> 24) & 0x1f; \
	if (cpu > 3) cpu = 0; \
	for (INT32 i = 0; i < k+1; i++) {	\
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nCPU = cpu;	\
		if ((flags & 0xf0000000) == 0x80000000) { \
			pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].bRelAddress = 1; \
			pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nRelAddressOffset = nAttrib; \
			pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nRelAddressBits = (flags & 0x3000000) >> 24; \
		} \
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nAddress = (pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].bRelAddress) ? nAddress : nAddress + i;	\
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nExtended = nAttrib; \
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nValue = (nValue >> ((k*8)-(i*8))) & 0xff;	\
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nMask = (nAttrib >> ((k*8)-(i*8))) & 0xff;	\
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nMultiByte = i;	\
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nTotalByte = k+1;	\
		nCurrentAddress++;	\
	}	\

#define AddressInfoGameGenie() { \
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nTotalByte = 1;	\
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nAddress = 0xffff; \
		strcpy(pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].szGenieCode, szGGenie); \
		nCurrentAddress++;	\
	}

#define OptionName(a)	\
	if (pCurrentCheat->pOption[n] == NULL) {						\
		pCurrentCheat->pOption[n] = (CheatOption*)malloc(sizeof(CheatOption));		\
	}											\
	memset(pCurrentCheat->pOption[n], 0, sizeof(CheatOption));				\
	_tcsncpy (pCurrentCheat->pOption[n]->szOptionName, a, QUOTE_MAX * sizeof(TCHAR));	\

#define tmpcpy(a)	\
	_tcsncpy (tmp, szLine + c0[a] + 1, c0[a+1] - (c0[a]+1));	\
	tmp[c0[a+1] - (c0[a]+1)] = '\0';				\

	TCHAR tmp[256];
	TCHAR tmp2[256];
	TCHAR gName[64];
	TCHAR szLine[1024];
	char szGGenie[128] = { 0, };

	INT32 nLen;
	INT32 n = 0;
	INT32 menu = 0;
	INT32 nFound = 0;
	INT32 nCurrentAddress = 0;
	UINT32 flags = 0;
	UINT32 nAddress = 0;
	UINT32 nValue = 0;
	UINT32 nAttrib = 0;
	bool bFirst = true;

	_stprintf(gName, _T(":%s:"), name);

	const TCHAR* iniPtr = is_wayder? CurrentWayderMameCheatContent.data(): CurrentMameCheatContent.data();

	while (*iniPtr)
	{
		TCHAR* s = szLine;
		while (*iniPtr && *iniPtr != _T('\n')) {
			*s++ = *iniPtr++;
		}
		// szLine should include '\n'
		if (*iniPtr == _T('\n')) {
			*s++ = *iniPtr++;
		}
		*s = _T('\0');

		nLen = _tcslen (szLine);

		if (szLine[0] == _T(';')) continue;

		/*
		 // find the cheat flags & 0x80000000 cheats (for debugging) -dink
		 int derpy = 0;
		 for (INT32 i = 0; i < nLen; i++) {
		 	if (szLine[i] == ':') {
		 		derpy++;
		 		if (derpy == 2 && szLine[i+1] == '8') {
					bprintf(0, _T("%s\n"), szLine);
				}
			}
		}
		*/

#if defined(BUILD_WIN32)
		if (_tcsncmp (szLine, gName, lstrlen(gName))) {
#else
		if (_tcsncmp (szLine, gName, strlen(gName))) {
#endif
			if (nFound) break;
			else continue;
		}

		if (_tcsstr(szLine, _T("----:REASON"))) {
			// reason to leave!
			break;
		}

		nFound = 1;

		INT32 c0[16], c1 = 0;					// find colons / break
		for (INT32 i = 0; i < nLen; i++)
			if (szLine[i] == _T(':') || szLine[i] == _T('\r') || szLine[i] == _T('\n'))
				c0[c1++] = i;

		tmpcpy(1);						// control flags
#if defined(BUILD_WIN32)
		_stscanf (tmp, _T("%x"), &flags);
#else
		sscanf (tmp, _T("%x"), &flags);
#endif

		tmpcpy(2);						// cheat address
#if defined(BUILD_WIN32)
		_stscanf (tmp, _T("%x"), &nAddress);
		strcpy(szGGenie, TCHARToANSI(tmp, NULL, 0));
#else
		sscanf (tmp, _T("%x"), &nAddress);
		strcpy(szGGenie, tmp);
#endif

		tmpcpy(3);						// cheat value
#if defined(BUILD_WIN32)
		_stscanf (tmp, _T("%x"), &nValue);
#else
		sscanf (tmp, _T("%x"), &nValue);
#endif

		tmpcpy(4);						// cheat attribute
#if defined(BUILD_WIN32)
		_stscanf (tmp, _T("%x"), &nAttrib);
#else
		sscanf (tmp, _T("%x"), &nAttrib);
#endif

		tmpcpy(5);						// cheat name

		// & 0x4000 = don't add to list
		// & 0x0800 = BCD
		if (flags & 0x00004800) continue;			// skip various cheats (unhandled methods at this time)

		if ((flags & 0xff000000) == 0x39000000 && IS_MIDWAY) {
			nAddress |= 0xff800000 >> 3; // 0x39 = address is relative to system's ROM block, only midway uses this kinda cheats
		}

		if ( flags & 0x00008000 || (flags & 0x00010000 && !menu)) { // Linked cheat "(2/2) etc.."
			if (nCurrentAddress < CHEAT_MAX_ADDRESS) {
				if (HW_GGENIE) {
					AddressInfoGameGenie();
				} else {
					AddressInfo();
				}
			}

			continue;
		}

		if (~flags & 0x00010000) {
			n = 0;
			menu = 0;
			nCurrentAddress = 0;

#ifndef __LIBRETRO__
			if (bFirst) {
				TCHAR szHeading[256];
				_stprintf(szHeading, _T("[ Cheats \"%s\" ]"), pszFileHeading);
				CheatLinkNewNode(szHeading);
				bFirst = false;
			}
#endif

			CheatLinkNewNode(tmp);

			_tcsncpy (pCurrentCheat->szCheatName, tmp, QUOTE_MAX);

#ifdef __LIBRETRO__
			_tcsncpy (pCurrentCheat->szCheatFilename, pszFileHeading, QUOTE_MAX);
#endif

#if defined(BUILD_WIN32)
			if (lstrlen(tmp) <= 0 || flags == 0x60000000) {
#else
			if (strlen(tmp) <= 0 || flags == 0x60000000) {
#endif
				n++;
				continue;
			}

			OptionName(_T("Disabled"));

			if (nAddress || HW_GGENIE) {
				if ((flags & 0x80018) == 0 && nAttrib != 0xffffffff) {
					pCurrentCheat->bWriteWithMask = 1; // nAttrib field is the mask
				}
				if (flags & 0x1) {
					pCurrentCheat->bOneShot = 1; // apply once and stop
				}
				if (flags & 0x2) {
					pCurrentCheat->bWaitForModification = 1; // wait for modification before changing
				}
				if (flags & 0x80000) {
					pCurrentCheat->bWaitForModification = 2; // check address against extended field before changing
				}
				if (flags & 0x800000) {
					pCurrentCheat->bRestoreOnDisable = 1; // restore previous value on disable
				}
				if (flags & 0x3000) {
					pCurrentCheat->nPrefillMode = (flags & 0x3000) >> 12;
				}
				if ((flags & 0x6) == 0x6) {
					pCurrentCheat->bWatchMode = 1; // display value @ address
				}
				if (flags & 0x100) { // add options
					INT32 nTotal = nValue + 1;
					INT32 nPlus1 = (flags & 0x200) ? 1 : 0; // displayed value +1?
					INT32 nStartValue = (flags & 0x400) ? 1 : 0; // starting value

					//bprintf(0, _T("adding .. %X. options\n"), nTotal);
					if (nTotal > 0xff) continue; // bad entry (roughrac has this)
					for (nValue = nStartValue; nValue < nTotal; nValue++) {
#if defined(UNICODE)
						swprintf(tmp2, L"# %d.", nValue + nPlus1);
#else
						sprintf(tmp2, _T("# %d."), nValue + nPlus1);
#endif
						n++;
						nCurrentAddress = 0;
						OptionName(tmp2);
						if (HW_GGENIE) {
							AddressInfoGameGenie();
						} else {
							AddressInfo();
						}
					}
				} else {
					n++;
					OptionName(tmp);
					if (HW_GGENIE) {
						AddressInfoGameGenie();
					} else {
						AddressInfo();
					}
				}
			} else {
				menu = 1;
			}

			continue;
		}

		if ( flags & 0x00010000 && menu) {
			n++;
			nCurrentAddress = 0;

			if ((flags & 0x80018) == 0 && nAttrib != 0xffffffff) {
				pCurrentCheat->bWriteWithMask = 1; // nAttrib field is the mask
			}
			if (flags & 0x1) {
				pCurrentCheat->bOneShot = 1; // apply once and stop
			}
			if (flags & 0x2) {
				pCurrentCheat->bWaitForModification = 1; // wait for modification before changing
			}
			if (flags & 0x80000) {
				pCurrentCheat->bWaitForModification = 2; // check address against extended field before changing
			}
			if (flags & 0x800000) {
				pCurrentCheat->bRestoreOnDisable = 1; // restore previous value on disable
			}
			if (flags & 0x3000) {
				pCurrentCheat->nPrefillMode = (flags & 0x3000) >> 12;
			}
			if ((flags & 0x6) == 0x6) {
				pCurrentCheat->bWatchMode = 1; // display value @ address
			}

			OptionName(tmp);
			if (HW_GGENIE) {
				AddressInfoGameGenie();
			} else {
				AddressInfo();
			}

			continue;
		}
	}

	// if no cheat was found, don't return success code
	if (pCurrentCheat == NULL) return 1;
	return 0;
}

static INT32 ExtractMameCheatFromDat(FILE* MameDatCheat, const TCHAR* matchDrvName, int is_wayder) {

	if(is_wayder){
		CurrentWayderMameCheatContent.clear();
	}else{
		CurrentMameCheatContent.clear();
	}
	TCHAR szLine[1024];
	TCHAR gName[64];
	_stprintf(gName, _T(":%s:"), matchDrvName);

	bool foundData = false;

	while (_fgetts(szLine, 1024, MameDatCheat) != NULL) {
		// Check if the current line contains matchDrvName
#if defined(BUILD_WIN32)
		if (_tcsncmp(szLine, gName, lstrlen(gName)) == 0) {
#else
		if (_tcsncmp(szLine, gName, strlen(gName)) == 0) {
#endif
			foundData = true;

			// Add the current line to CurrentMameCheatContent
			for (TCHAR* p = szLine; *p; ++p) {
				if (*p != _T('\0')) {
					if(is_wayder){
						CurrentWayderMameCheatContent.push_back(*p);
					}else{
						CurrentMameCheatContent.push_back(*p);
					}
				}
			}
		}
	}

	return foundData ? 0 : 1;
}

int mame_cheat_use_itself = 0;
int wayder_cheat_use_itself = 0;
int mame_cheat_use_parent = 0;
int wayder_cheat_use_parent = 0;

static INT32 ConfigParseMAMEFile(int is_wayder)
{
	TCHAR szFileName[MAX_PATH] = _T("");

	if (is_wayder) {
		if (HW_NES || HW_SNES) return 1;
		_stprintf(szFileName, _T("%swayder_cheat.dat"), szAppCheatsPath);
	} else {
		if (HW_NES) {
			_stprintf(szFileName, _T("%scheatnes.dat"), szAppCheatsPath);
		} else if (HW_SNES) {
			_stprintf(szFileName, _T("%scheatsnes.dat"), szAppCheatsPath);
		} else {
			_stprintf(szFileName, _T("%scheat.dat"), szAppCheatsPath);
		}
	}

	TCHAR* pszReadMode = AdaptiveEncodingReads(szFileName);
	if (NULL == pszReadMode) pszReadMode = _T("rt");

	FILE *fz = _tfopen(szFileName, pszReadMode);
	TCHAR* pszFileHeading = getFilenameFromPath(szFileName);

	INT32 ret = 1;

	const TCHAR* DrvName = BurnDrvGetText(DRV_NAME);

	if (fz) {
		ret = ExtractMameCheatFromDat(fz, DrvName, is_wayder);
		if (ret == 0) {
			ret = ConfigParseMAMEFile_internal(DrvName, pszFileHeading, is_wayder );
			usedCheatType = (ret == 0) ? 1 : usedCheatType;	// see usedCheatType define
			if(ret == 0 ){
				if(is_wayder){
					wayder_cheat_use_itself = 1;
				}else{
					mame_cheat_use_itself = 1;
				}
			}
		}
		// let's try using parent entry as a fallback if no cheat was found for this romset
		if (ret > 0 && (BurnDrvGetFlags() & BDF_CLONE) && BurnDrvGetText(DRV_PARENT)) {
			fseek(fz, 0, SEEK_SET);
			DrvName = BurnDrvGetText(DRV_PARENT);
			ret = ExtractMameCheatFromDat(fz, DrvName, is_wayder);
			if (ret == 0) {
				ret = ConfigParseMAMEFile_internal(DrvName, pszFileHeading, is_wayder );
				usedCheatType = (ret == 0) ? 2 : usedCheatType; // see usedCheatType define
				if(ret == 0 ){
					if(is_wayder){
						wayder_cheat_use_parent = 1;
					}else{
						mame_cheat_use_parent = 1;
					}
				}
			}
		}

		fclose(fz);
	}

	if (ret) {
		if(is_wayder){
			CurrentWayderMameCheatContent.clear();
		}else{
			CurrentMameCheatContent.clear();
		}
	}

	return ret;
}

static INT32 LoadIniContentFromZip(const TCHAR* DrvName, const TCHAR* zipFileName, std::vector<TCHAR>& iniContent) {
	TCHAR iniFileName[MAX_PATH] = {0};
	_stprintf(iniFileName,_T("%s.ini"), DrvName);

	TCHAR zipCheatPath[MAX_PATH] = {0};
	_stprintf(zipCheatPath, _T("%s%s"), szAppCheatsPath, zipFileName);

	if (ZipOpen(TCHAR2CHAR(zipCheatPath,NULL,0)) != 0) {
		ZipClose();
		return 1;
	}

	struct ZipEntry* pList = NULL;
	INT32 pnListCount = 0;

	if (ZipGetList(&pList, &pnListCount) != 0) {
		ZipClose();
		return 1;
	}

	INT32 ret = 1;

	for (int i = 0; i < pnListCount; i++) {
		if (_tcsicmp(CHAR2TCHAR(pList[i].szName,NULL,0), iniFileName) == 0) {
			std::vector<char> dest(pList[i].nLen + 1);

			INT32 pnWrote = 0;

			if (ZipLoadFile(reinterpret_cast<UINT8*>(dest.data()), pList[i].nLen, &pnWrote, i) != 0){
				break;
			}

			if (pnWrote <= static_cast<INT32>(dest.size())) {
				dest[pnWrote] = '\0';
			} else {
				break;
			}

			const char* encoding = DetectFileEncoding(dest);

			if (strcmp(encoding, "UTF-8") == 0) {
				// Decode UTF-8 to platform-specific encoding
#if defined(BUILD_WIN32)
				int wide_len = MultiByteToWideChar(CP_UTF8, 0, dest.data(), -1, NULL, 0);
				TCHAR* wideContent = new TCHAR[wide_len];
				MultiByteToWideChar(CP_UTF8, 0, dest.data(), -1, wideContent, wide_len);
				size_t wideLen = _tcslen(wideContent);
				iniContent.insert(iniContent.end(), wideContent, wideContent + wideLen);
				delete[] wideContent;
#else
				// Linux/macOS: UTF-8 is usually the default, so no conversion is needed
				size_t len = dest.size();
				iniContent.insert(iniContent.end(), reinterpret_cast<TCHAR*>(dest.data()), reinterpret_cast<TCHAR*>(dest.data()) + len);
#endif
			}else{
				// Decode ANSI
				std::vector<TCHAR> tcharContent;
				TCHAR* pszOutString = NULL;
				pszOutString = CHAR2TCHAR_ANSI(dest.data(), NULL, 0); // First call to get required size
				if (pszOutString) {
					tcharContent.assign(pszOutString, pszOutString + _tcslen(pszOutString));
					delete[] pszOutString; // Clean up allocated memory
				}
				iniContent.insert(iniContent.end(), tcharContent.begin(), tcharContent.end());
			}
			ret = 0;
			break;
		}
	}

	for (int i = 0; i < pnListCount; i++) {
		free(pList[i].szName);
	}

	free(pList);

	ZipClose();

	return ret;
}

 //Extract matched INI in cheat.zip or 7z
static INT32 ExtractIniFromZip(const TCHAR* DrvName, const TCHAR* zipFileName, std::vector<TCHAR>& CurrentIniCheat) {

	if (LoadIniContentFromZip(DrvName, zipFileName, CurrentIniCheatContent) != 0
		// try load parent cheat
		&& LoadIniContentFromZip(BurnDrvGetText(DRV_PARENT), zipFileName, CurrentIniCheatContent) != 0){
		return 1;
	}

	int depth = 0;
	bool processInclude = true;
	//max searching included files 5 depth
	while (processInclude && depth < 5) {
		processInclude = false;
		std::vector<TCHAR> newContent;
		const TCHAR* iniPtr = CurrentIniCheatContent.data();
		std::vector<TCHAR> szLine;

		// Let's check each line of CurrentIniCheatContent
		// Looking for include file and hooking them to CurrentIniCheatContent
		while (*iniPtr) {
			szLine.clear();
			while (*iniPtr && *iniPtr != _T('\n')) {
				szLine.push_back(*iniPtr++);
			}
			if (*iniPtr == _T('\n')) {
				szLine.push_back(*iniPtr++);
			}
			szLine.push_back(_T('\0'));

			TCHAR* t;
			if ((t = LabelCheck(szLine.data(), _T("include"))) != 0) {
				processInclude = true;
				TCHAR* szQuote = NULL;
				QuoteRead(&szQuote, NULL, t);

				if (szQuote) {
					std::vector<TCHAR> includedContent;

					if (LoadIniContentFromZip(szQuote, zipFileName, includedContent) == 0) {
						newContent.insert(newContent.end(), includedContent.begin(), includedContent.end());
						newContent.push_back(_T('\n'));
					}
				}
			} else {
				newContent.insert(newContent.end(), szLine.begin(), szLine.end() - 1);
			}
		}

		CurrentIniCheatContent = newContent;
		depth++;
	}

	return 0;
}

static int encodeNES(int address, int value, int compare, char *result) {
	const char ALPHABET_NES[2][16+1] = { { "APZLGITYEOXUKSVN" }, { "apzlgityeoxuksvn" } };

	bool address_lower = !(address & 0x8000);

	unsigned int genie = ((value & 0x80) >> 4) | (value & 0x7);
    unsigned int temp = ((address & 0x80) >> 4) | ((value & 0x70) >> 4);
	genie <<= 4;
	genie |= temp;

	temp = (address & 0x70) >> 4;

	if (compare != -1) {
		temp |= 0x8;
	}

	genie <<= 4;
	genie |= temp;

	temp = (address & 0x8) | ((address & 0x7000) >> 12);
	genie <<= 4;
	genie |= temp;

	temp = ((address & 0x800) >> 8) | (address & 0x7);
	genie <<= 4;
	genie |= temp;

	if (compare != -1) {

		temp = (compare & 0x8) | ((address & 0x700) >> 8);
		genie <<= 4;
		genie |= temp;

		temp = ((compare & 0x80) >> 4) | (compare & 0x7);
		genie <<= 4;
		genie |= temp;

		temp = (value & 0x8) | ((compare & 0x70) >> 4);
		genie <<= 4;
		genie |= temp;
	} else {
		temp = (value & 0x8) | ((address & 0x700) >> 8);
		genie <<= 4;
		genie |= temp;
	}

	result[6] = result[7] = result[8] = 0;

	for (int i = 0; i < ((compare != -1) ? 8 : 6); i++) {
		result[((compare != -1) ? 8 : 6) - 1 - i] = ALPHABET_NES[address_lower][(genie >> (i * 4)) & 0xF];
	}

	return 0;
}

#ifdef _MSC_VER
#define strtok_r strtok_s
#endif

void normalize_spaces(TCHAR* str) {
    TCHAR* dest = str;
    TCHAR* src = str;
    bool prev_blank = false;

    while (*src == ' ' || *src == '\t') src++;

    while (*src) {
        if (*src == ' ' || *src == '\t') {
            if (!prev_blank) {
                *dest++ = ' ';
                prev_blank = true;
            }
            src++;
        } else {
            *dest++ = *src++;
            prev_blank = false;
        }
    }

    if (dest > str && (dest[-1] == ' ' || dest[-1] == '\t')) {
        dest--;
    }
    *dest = '\0';
}

// VirtuaNES .vct format
static INT32 ConfigParseVCT(TCHAR* pszFilename)
{
#define AddressInfoGameGenie() { \
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nTotalByte = 1;	\
		pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].nAddress = 0xffff; \
		strcpy(pCurrentCheat->pOption[n]->AddressInfo[nCurrentAddress].szGenieCode, szGGenie); \
		nCurrentAddress++;	\
	}

#define OptionName(a)	\
	if (pCurrentCheat->pOption[n] == NULL) {						\
		pCurrentCheat->pOption[n] = (CheatOption*)malloc(sizeof(CheatOption));		\
	}											\
	memset(pCurrentCheat->pOption[n], 0, sizeof(CheatOption));				\
	_tcsncpy (pCurrentCheat->pOption[n]->szOptionName, a, QUOTE_MAX * sizeof(TCHAR));	\

#define tmpcpy(a)	\
	_tcsncpy (tmp, szLine + c0[a] + 1, c0[a+1] - (c0[a]+1));	\
	tmp[c0[a+1] - (c0[a]+1)] = '\0';				\
	normalize_spaces(tmp); \

	TCHAR tmp[256];
	TCHAR szLine[1024];
	char szGGenie[256] = { 0, };

	INT32 nLen;
	INT32 n = 0;
	INT32 nCurrentAddress = 0;

	bool bFirst = true;

	TCHAR* pszReadMode = AdaptiveEncodingReads(pszFilename);
	if (NULL == pszReadMode) pszReadMode = _T("rt");

	FILE* h = _tfopen(pszFilename, pszReadMode);
	TCHAR* pszFileHeading = getFilenameFromPath(pszFilename);
	if (h == NULL) {
		if ((BurnDrvGetFlags() & BDF_CLONE) && BurnDrvGetText(DRV_PARENT)) {
			TCHAR szAlternative[MAX_PATH] = { 0 };
			_stprintf(szAlternative, _T("%s%s.vct"), szAppCheatsPath, BurnDrvGetText(DRV_PARENT));

			pszReadMode = AdaptiveEncodingReads(szAlternative);
			if (NULL == pszReadMode) pszReadMode = _T("rt");

			if (NULL == (h = _tfopen(szAlternative, pszReadMode)))
				return 1;
			pszFileHeading = getFilenameFromPath(szAlternative);
		} else {
			return 1;	// Parent driver
		}
	}


	while (1)
	{
		if (_fgetts(szLine, 1024, h) == NULL)
			break;

		nLen = _tcslen (szLine);

		if (szLine[0] == ';') continue;

		INT32 c0[16], c1 = 0, cprev = 0; // find columns / break
		bool in_blank = false;
		for (INT32 i = 0; i < nLen; i++) {
			if (szLine[i] == ' ' || szLine[i] == '\t' || szLine[i] == '\r' || szLine[i] == '\n') {
				if (!in_blank) {
					if (cprev + 1 != i) {
						c0[c1++] = i;
					}
					cprev = i;
					in_blank = true;
				}
			} else {
				in_blank = false;
			}
		}
		tmpcpy(0);
#if defined(BUILD_WIN32)
		strcpy(szGGenie, TCHARToANSI(tmp, NULL, 0));
#else
		strcpy(szGGenie, tmp);
#endif
		szGGenie[255] = '\0';
		tmpcpy(1);

		if (HW_GGENIE) {
			//szGGenie "0077-01-FF" or "0077-04-80808080"
			char *tok_main = NULL;
			char temp2[256] = { 0, };
			UINT32 fAddr = 0;
			UINT32 fCount = 0;
			UINT32 fAttr = 0; // always = 0, oneshot = 1, greater = 2 (mem[addr] > bytes), less = 3 (mem[addr] < bytes)
			UINT32 fBytes = 0;

			strcpy(temp2, szGGenie);

			// split up "0077-01-FF" format: "address-[attribute][bytecount]-bytes_to_program"
			char *tok = strtok_r(temp2, "-", &tok_main);
			if (!tok) continue;
			sscanf(tok, "%x", &fAddr);

			tok = strtok_r(NULL, "-", &tok_main);
			if (!tok) continue;
			sscanf(tok, "%x", &fCount);
			fAttr = (fCount & 0x30) >> 4;
			fCount &= 0x07;
			if (fCount < 1 || fCount > 4) fCount = 1;

			tok = strtok_r(NULL, "-", &tok_main);
			if (!tok) continue;
			sscanf(tok, "%x", &fBytes);

			//bprintf(0, _T(".vct: addr[%x] count[%x] bytes[%x]\n"), fAddr, fCount, fBytes);

#ifndef __LIBRETRO__
			if (bFirst) {
				TCHAR szHeading[256];
				_stprintf(szHeading, _T("[ Cheats \"%s\" ]"), pszFileHeading);
				CheatLinkNewNode(szHeading);
				bFirst = false;
			}
#endif

			// -- add to cheat engine --
			n = 0;
			nCurrentAddress = 0;

			CheatLinkNewNode(tmp);

#ifdef __LIBRETRO__
			_tcsncpy (pCurrentCheat->szCheatFilename, pszFileHeading, QUOTE_MAX);
#endif

			OptionName(_T("Disabled"));
			n++;
			OptionName(_T("Enabled"));

			for (int i = 0; i < fCount; i++) {
				memset(szGGenie, 0, sizeof(szGGenie));
				encodeNES(fAddr + i, fBytes >> (i*8), -1, szGGenie);
				INT32 cLen = strlen(szGGenie);
				szGGenie[cLen] = '0' + fAttr; // append the attribute to the end of the GameGenie code (handled in drv/nes/nes.cpp)
				AddressInfoGameGenie();
			}
		}

		continue;
	}

	// if no cheat was found, don't return success code
	if (pCurrentCheat == NULL) return 1;

	return 0;
}

#ifdef _MSC_VER
#undef strtok_r
#endif


INT32 ConfigCheatLoad() {
	TCHAR szFilename[MAX_PATH] = _T("");

	pCurrentCheat = NULL;
	pPreviousCheat = NULL;

	int is_wayder = 1;
	int cheat_dat_ret = 1;
	int wayder_cheat_dat_ret = 1;

	INT32 ret = 1;

	// Load single cheat types { nes vct > cheat.dat,cheatnes.dat,cheatsnes.dat,wayder_cheat.dat > ini > 7z/zip ini > nebula dat }
	// During running game,while ConfigCheatLoad is called the second time or more,
	// Try to load cheat directly,skip unnecessary steps.
	// usedCheatType define:
	// 0:first ConfigCheatLoad() while launching game
	// 1:first ConfigCheatLoad() used MameDatCheat,we directly reload existing cache(DRV_NAME) from cheat.dat
	// 2:first ConfigCheatLoad() used MameDatCheat,we directly reload existing cache(DRV_PARENT) from cheat.dat
	// 3:first ConfigCheatLoad() used ini cheat in Zip/7Z,we directly reload existing cache from cheat.zip/7z
	// 4:first ConfigCheatLoad() used ini cheat in folder,we directly reload from <drvname>.ini
	// 5:first ConfigCheatLoad() used NebulaDatCheat in folder,we directly reload from <drvname>.dat
	// 6:first ConfigCheatLoad() used NES <drvname>.vct
	//-1:first ConfigCheatLoad() no cheats found,we do nothing,never check again.
	switch (usedCheatType) {
		case 0:
			if (HW_NES) { // only for NES/FC!
				_stprintf(szFilename, _T("%s%s.vct"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
				ConfigParseVCT(szFilename);
				usedCheatType = 6;
				break;
			} // keep loading & adding stuff even if .vct file loads.

			cheat_dat_ret = ConfigParseMAMEFile(!is_wayder);
			wayder_cheat_dat_ret = ConfigParseMAMEFile(is_wayder);
			
			// when cheat.dat and wayder_cheat.dat both not exist, try load other type cheat
			if (cheat_dat_ret && wayder_cheat_dat_ret) {
				//use single ini first
				_stprintf(szFilename, _T("%s%s.ini"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
				ret = ConfigParseFile(szFilename,NULL);
				//try load ini from zip/7z
				if (ret > 0) {
					ret = ExtractIniFromZip(BurnDrvGetText(DRV_NAME), _T("cheat"), CurrentIniCheatContent);
					if (ret == 0) {
						// (cheat.zip/7z) pszFilename only uses for cheaterror and pszFileHeading as string, not a file in this step
						_stprintf(szFilename, _T("%sx_%s.ini"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
						ret = ConfigParseFile(szFilename, &CurrentIniCheatContent);
					}
					// try use Nebula cheat
					if (ret != 0) {
						_stprintf(szFilename, _T("%s%s.dat"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
						ret = ConfigParseNebulaFile(szFilename);
						if (ret != 0) {
							usedCheatType = -1;
						}
					}
				}
			}
			break;
		case 1:
		case 2:
			if(mame_cheat_use_itself){
				ret = ConfigParseMAMEFile_internal(BurnDrvGetText(DRV_NAME),_T("cheat.dat"),!is_wayder);
			}
			if(mame_cheat_use_parent){
				ret = ConfigParseMAMEFile_internal(BurnDrvGetText(DRV_PARENT),_T("cheat.dat"),!is_wayder);
			}
			if(wayder_cheat_use_itself){
				ret = ConfigParseMAMEFile_internal(BurnDrvGetText(DRV_NAME),_T("wayder_cheat.dat"),is_wayder);
			}
			if(wayder_cheat_use_parent){
				ret = ConfigParseMAMEFile_internal(BurnDrvGetText(DRV_PARENT),_T("wayder_cheat.dat"),is_wayder);
			}
			break;
		case 3:
			// (cheat.zip/7z) pszFilename only uses for cheaterror and pszFileHeading as string, not a file in this step
			_stprintf(szFilename, _T("%sx_%s.ini"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
			ret = ConfigParseFile(szFilename, &CurrentIniCheatContent);
			break;
		case 4:
			_stprintf(szFilename, _T("%s%s.ini"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
			ret = ConfigParseFile(szFilename, NULL);
			break;
		case 5:
			_stprintf(szFilename, _T("%s%s.dat"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
			ret = ConfigParseNebulaFile(szFilename);
			break;
		case 6:
			_stprintf(szFilename, _T("%s%s.vct"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
			ret = ConfigParseVCT(szFilename);
			break;
		default: //case -1 aswell
			ret = 1;
			break;
	}

	if (pCheatInfo) {
		INT32 nCurrentCheat = 0;
		while (CheatEnable(nCurrentCheat, -1) == 0) {
			nCurrentCheat++;
		}

		CheatUpdate();
	}

	return ret;
}