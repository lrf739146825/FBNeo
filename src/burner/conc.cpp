#include "burner.h"

#if defined(BUILD_WIN32)
#include <windows.h>
#else
#include <iconv.h>
#include <cerrno>
#endif

#if defined(BUILD_WIN32)
/**
 * @brief Convert a TCHAR string to an ANSI string (Windows implementation).
 * @param pszInString Input TCHAR string to convert.
 * @param pszOutString Output buffer for the ANSI string. If NULL, a static buffer is used.
 * @param nOutSize Size of the output buffer in bytes.
 * @return Pointer to the converted ANSI string (pszOutString or static buffer), or NULL on failure.
 * @note On Windows, uses WideCharToMultiByte for conversion from UTF-16 to ANSI.
 * @warning The static buffer is not thread-safe. Avoid using NULL for pszOutString in multi-threaded contexts.
 */
char* TCHAR2ANSI(const TCHAR* pszInString, char* pszOutString, int nOutSize) {
    static char szStringBuffer[1024];
    if (!pszOutString) {
        pszOutString = szStringBuffer;
        nOutSize = sizeof(szStringBuffer);
    }

    // Ensure the output buffer is large enough to hold the converted string
    if (nOutSize <= 0) {
        return NULL; // Invalid buffer size
    }

    // Clear the static buffer if used
    if (pszOutString == szStringBuffer) {
        memset(szStringBuffer, 0, sizeof(szStringBuffer));
    }

    int nBufferSize = nOutSize;

    if (WideCharToMultiByte(CP_ACP, 0, pszInString, -1, pszOutString, nBufferSize, NULL, NULL)) {
        return pszOutString;
    }

    return NULL;
}

/**
 * @brief Convert an ANSI string to a TCHAR string (Windows implementation).
 * @param pszInString Input ANSI string to convert.
 * @param pszOutString Output buffer for the TCHAR string. If NULL, a static buffer is used.
 * @param nOutSize Size of the output buffer in TCHARs.
 * @return Pointer to the converted TCHAR string (pszOutString or static buffer), or NULL on failure.
 * @note On Windows, uses MultiByteToWideChar for conversion from ANSI to UTF-16.
 * @warning The static buffer is not thread-safe. Avoid using NULL for pszOutString in multi-threaded contexts.
 */
TCHAR* ANSI2TCHAR(const char* pszInString, TCHAR* pszOutString, int nOutSize) {
    static TCHAR szStringBuffer[1024];
    if (!pszOutString) {
        pszOutString = szStringBuffer;
        nOutSize = sizeof(szStringBuffer) / sizeof(TCHAR);
    }

    // Ensure the output buffer is large enough to hold the converted string
    if (nOutSize <= 0) {
        return NULL; // Invalid buffer size
    }

    int nBufferSize = nOutSize;

    if (MultiByteToWideChar(CP_ACP, 0, pszInString, -1, pszOutString, nBufferSize)) {
        return pszOutString;
    }

    return NULL;
}
#else // Non-Windows implementation
#ifdef _UNICODE
/**
 * @brief Convert a TCHAR string to an ANSI string (Unicode/iconv implementation).
 * @param pszInString Input TCHAR string to convert.
 * @param pszOutString Output buffer for the ANSI string. If NULL, a static buffer is used.
 * @param nOutSize Size of the output buffer in bytes.
 * @return Pointer to the converted ANSI string (pszOutString or static buffer), or NULL on failure.
 * @note Uses iconv for conversion from WCHAR_T to UTF-8.
 * @warning The static buffer is not thread-safe. Avoid using NULL for pszOutString in multi-threaded contexts.
 */
char* TCHAR2ANSI(const TCHAR* pszInString, char* pszOutString, int nOutSize) {
    static char szStringBuffer[1024];
    if (!pszOutString) {
        pszOutString = szStringBuffer;
        nOutSize = sizeof(szStringBuffer);
    }

    iconv_t cd = iconv_open("UTF-8", "WCHAR_T");
    if (cd == (iconv_t)-1) {
        return NULL;
    }

    size_t in_len = wcslen(pszInString) * sizeof(TCHAR);
    size_t out_len = nOutSize;

    if (iconv(cd, (char**)&pszInString, &in_len, &pszOutString, &out_len) == (size_t)-1) {
        iconv_close(cd);
        return NULL;
    }

    iconv_close(cd);
    return pszOutString;
}

/**
 * @brief Convert an ANSI string to a TCHAR string (Unicode/iconv implementation).
 * @param pszInString Input ANSI string to convert.
 * @param pszOutString Output buffer for the TCHAR string. If NULL, a static buffer is used.
 * @param nOutSize Size of the output buffer in TCHARs.
 * @return Pointer to the converted TCHAR string (pszOutString or static buffer), or NULL on failure.
 * @note Uses iconv for conversion from UTF-8 to WCHAR_T.
 * @warning The static buffer is not thread-safe. Avoid using NULL for pszOutString in multi-threaded contexts.
 */
TCHAR* ANSI2TCHAR(const char* pszInString, TCHAR* pszOutString, int nOutSize) {
    static TCHAR szStringBuffer[1024];
    if (!pszOutString) {
        pszOutString = szStringBuffer;
        nOutSize = sizeof(szStringBuffer) / sizeof(TCHAR);
    }

    iconv_t cd = iconv_open("WCHAR_T", "UTF-8");
    if (cd == (iconv_t)-1) {
        return NULL;
    }

    size_t in_len = strlen(pszInString);
    size_t out_len = nOutSize;

    if (iconv(cd, (char**)&pszInString, &in_len, (char**)&pszOutString, &out_len) == (size_t)-1) {
        iconv_close(cd);
        return NULL;
    }

    iconv_close(cd);
    return pszOutString;
}
#else
/**
 * @brief Convert a TCHAR string to an ANSI string (non-Unicode implementation).
 * @param pszInString Input TCHAR string to convert.
 * @param pszOutString Output buffer for the ANSI string. If NULL, a static buffer is used.
 * @param nOutSize Size of the output buffer in bytes.
 * @return Pointer to the converted ANSI string (pszOutString or static buffer), or NULL on failure.
 * @note Assumes TCHAR is the same as char (no conversion needed).
 * @warning The static buffer is not thread-safe. Avoid using NULL for pszOutString in multi-threaded contexts.
 */
char* TCHAR2ANSI(const TCHAR* pszInString, char* pszOutString, int nOutSize) {
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
TCHAR* ANSI2TCHAR(const char* pszInString, TCHAR* pszOutString, int nOutSize) {
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
#endif // _UNICODE
#endif // BUILD_WIN32

// GameGenie stuff is handled a little differently..
#define HW_NES ( ((BurnDrvGetHardwareCode() & HARDWARE_PUBLIC_MASK) == HARDWARE_NES) || ((BurnDrvGetHardwareCode() & HARDWARE_PUBLIC_MASK) == HARDWARE_FDS) )
#define HW_SNES ( ((BurnDrvGetHardwareCode() & HARDWARE_PUBLIC_MASK) == HARDWARE_SNES) )
#define HW_GGENIE ( HW_NES || HW_SNES )
std::vector<TCHAR> CurrentMameCheatContent; // Global
std::vector<TCHAR> CurrentIniCheatContent; // Global
int usedCheatType = 0; //Global so we'll know if cheatload is already done or which cheat type it uses?

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

	CheatInfo* pCurrentCheat = NULL;

	FILE* h = NULL;
	const TCHAR* iniPtr = NULL;

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

			// Link new node into the list
			CheatInfo* pPreviousCheat = pCurrentCheat;
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
			pCurrentCheat->nType = 0;								// Default to cheat type 0 (apply each frame)
			pCurrentCheat->nStatus = -1;							// Disable cheat

			memcpy(pCurrentCheat->szCheatName, szQuote, QUOTE_MAX);

			continue;
		}

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
	if (fp == NULL) {
		if ((BurnDrvGetFlags() & BDF_CLONE) && BurnDrvGetText(DRV_PARENT)) {
			TCHAR szAlternative[MAX_PATH] = { 0 };
			_stprintf(szAlternative, _T("%s%s.dat"), szAppCheatsPath, BurnDrvGetText(DRV_PARENT));

			pszReadMode = AdaptiveEncodingReads(szAlternative);
			if (NULL == pszReadMode) pszReadMode = _T("rt");

			if (NULL == (fp = _tfopen(szAlternative, pszReadMode)))
				return 1;
		} else {
			return 1;	// Parent driver
		}
	}

	INT32 nLen;
	INT32 i, j, n = 0;
	TCHAR tmp[32];
	TCHAR szLine[1024];

	CheatInfo* pCurrentCheat = NULL;

	while (1)
	{
		if (_fgetts(szLine, 1024, fp) == NULL)
			break;

		nLen = _tcslen(szLine);

		if (nLen < 3 || szLine[0] == '[') continue;

		if (!_tcsncmp (_T("Name="), szLine, 5))
		{
			n = 0;

			// Link new node into the list
			CheatInfo* pPreviousCheat = pCurrentCheat;
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
			pCurrentCheat->nType = 0;							// Default to cheat type 0 (apply each frame)
			pCurrentCheat->nStatus = -1;							// Disable cheat
			pCurrentCheat->nDefault = 0;							// Set default option

			_tcsncpy (pCurrentCheat->szCheatName, szLine + 5, QUOTE_MAX);
			pCurrentCheat->szCheatName[nLen-6] = _T('\0');

			continue;
		}

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

static INT32 ConfigParseMAMEFile_internal(const TCHAR *name)
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

	CheatInfo* pCurrentCheat = NULL;
	_stprintf(gName, _T(":%s:"), name);

	const TCHAR* iniPtr = CurrentMameCheatContent.data();
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

			// Link new node into the list
			CheatInfo* pPreviousCheat = pCurrentCheat;
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
			pCurrentCheat->nType = 0;								// Default to cheat type 0 (apply each frame)
			pCurrentCheat->nStatus = -1;							// Disable cheat
			pCurrentCheat->nDefault = 0;							// Set default option
			pCurrentCheat->bOneShot = 0;							// Set default option (off)
			pCurrentCheat->bWatchMode = 0;							// Set default option (off)

			_tcsncpy (pCurrentCheat->szCheatName, tmp, QUOTE_MAX);

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

static INT32 ExtractMameCheatFromDat(FILE* MameDatCheat, const TCHAR* matchDrvName) {

	CurrentMameCheatContent.clear();
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
					CurrentMameCheatContent.push_back(*p);
				}
			}
		}
	}

	return foundData ? 0 : 1;
}

static INT32 ConfigParseMAMEFile()
{
	TCHAR szFileName[MAX_PATH] = _T("");
	if (HW_NES) {
		_stprintf(szFileName, _T("%scheatnes.dat"), szAppCheatsPath);
	} else if (HW_SNES) {
		_stprintf(szFileName, _T("%scheatsnes.dat"), szAppCheatsPath);
	} else {
		_stprintf(szFileName, _T("%scheat.dat"), szAppCheatsPath);
	}

	TCHAR* pszReadMode = AdaptiveEncodingReads(szFileName);
	if (NULL == pszReadMode) pszReadMode = _T("rt");

	FILE *fz = _tfopen(szFileName, pszReadMode);

	INT32 ret = 1;

	const TCHAR* DrvName = BurnDrvGetText(DRV_NAME);

	if (fz) {
		ret = ExtractMameCheatFromDat(fz, DrvName);
		if (ret == 0) {
			ret = ConfigParseMAMEFile_internal(DrvName);
			usedCheatType = (ret == 0) ? 1 : usedCheatType;	// see usedCheatType define
		}
		// let's try using parent entry as a fallback if no cheat was found for this romset
		if (ret > 0 && (BurnDrvGetFlags() & BDF_CLONE) && BurnDrvGetText(DRV_PARENT)) {
			fseek(fz, 0, SEEK_SET);
			DrvName = BurnDrvGetText(DRV_PARENT);
			ret = ExtractMameCheatFromDat(fz, DrvName);
			if (ret == 0) {
				ret = ConfigParseMAMEFile_internal(DrvName);
				usedCheatType = (ret == 0) ? 2 : usedCheatType; // see usedCheatType define
			}
		}

		fclose(fz);
	}

	if (ret) {
		CurrentMameCheatContent.clear();
	}

	return ret;
}

static INT32 LoadIniContentFromZip(const TCHAR* DrvName, const TCHAR* zipFileName, std::vector<TCHAR>& iniContent) {
	TCHAR iniFileName[MAX_PATH] = {0};
	_stprintf(iniFileName,_T("%s.ini"), DrvName);

	TCHAR zipCheatPath[MAX_PATH] = {0};
	_stprintf(zipCheatPath, _T("%s%s"), szAppCheatsPath, zipFileName);

	if (ZipOpen(TCHAR2ANSI(zipCheatPath,NULL,0)) != 0) {
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
		if (_tcsicmp(ANSI2TCHAR(pList[i].szName,NULL,0), iniFileName) == 0) {
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

	if (LoadIniContentFromZip(DrvName, zipFileName, CurrentIniCheatContent) != 0) {
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

INT32 ConfigCheatLoad() {
	TCHAR szFilename[MAX_PATH] = _T("");
	INT32 ret = 1;

	// During running game,while ConfigCheatLoad is called the second time or more,
	// Try to load cheat directly,skip unnecessary steps.
	// usedCheatType define:
	// 0:first ConfigCheatLoad() while launching game
	// 1:first ConfigCheatLoad() used MameDatCheat,we directly reload existing cache(DRV_NAME) from cheat.dat
	// 2:first ConfigCheatLoad() used MameDatCheat,we directly reload existing cache(DRV_PARENT) from cheat.dat
	// 3:first ConfigCheatLoad() used ini cheat in Zip/7Z,we directly reload existing cache from cheat.zip/7z
	// 4:first ConfigCheatLoad() used ini cheat in folder,we directly reload from <drvname>.ini
	// 5:first ConfigCheatLoad() used NebulaDatCheat in folder,we directly reload from <drvname>.dat
	// 6:first ConfigCheatLoad() no cheats found,we do nothing,never check again.
	switch (usedCheatType) {
		case 0:
			if (ConfigParseMAMEFile()) {
				ret = ExtractIniFromZip(BurnDrvGetText(DRV_NAME), _T("cheat"), CurrentIniCheatContent);
				if (ret == 0) {
					// pszFilename only uses for cheaterror as string,not a file
					_stprintf(szFilename, _T("%s%s.ini(cheat.zip/7z)"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
					ret = ConfigParseFile(szFilename, &CurrentIniCheatContent);
				}
				if (ret > 0) {
					_stprintf(szFilename, _T("%s%s.ini"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
					ret = ConfigParseFile(szFilename,NULL);
					if (ret != 0) {
						_stprintf(szFilename, _T("%s%s.dat"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
						ret = ConfigParseNebulaFile(szFilename);
						if (ret != 0) {
							usedCheatType = 6;
						}
					}
				}
			}
			break;
		case 1:
			ret = ConfigParseMAMEFile_internal(BurnDrvGetText(DRV_NAME));
			break;
		case 2:
			ret = ConfigParseMAMEFile_internal(BurnDrvGetText(DRV_PARENT));
			break;
		case 3:
			// pszFilename only uses for cheaterror as string, not a file in this step
			_stprintf(szFilename, _T("%s%s.ini(cheat.zip/7z)"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
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
		default: //case 6 aswell
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