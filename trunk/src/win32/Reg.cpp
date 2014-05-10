#include "stdafx.h"
#include "VBA.h"
#include "..\gba\GBALink.h"

static TCHAR buffer[2048];
static HKEY vbKey = NULL;
static CString *regVbaPath = NULL;

#define VBA_PREF _T("preferences")

bool regEnabled = true;

void regInit(const TCHAR *path, bool force)
{
	if( regEnabled ) {
  DWORD disp = 0;
  LONG res = RegCreateKeyEx(HKEY_CURRENT_USER,
                            _T("Software\\Emulators\\VisualBoyAdvance"),
                            0,
                            _T(""),
                            REG_OPTION_NON_VOLATILE,
                            KEY_ALL_ACCESS,
                            NULL,
                            &vbKey,
                            &disp);
	}
  if( regVbaPath != NULL ) {
	  delete regVbaPath;
	  regVbaPath = NULL;
  }
  regVbaPath = new CString();
  regVbaPath->Format(_T("%s\\vbam.ini"), path);
}

void regShutdown()
{
  LONG res = RegCloseKey(vbKey);
}

const TCHAR *regGetINIPath()
{
  return *regVbaPath;
}

TCHAR *regQueryStringValue(const TCHAR *key, TCHAR *def)
{
  if(regEnabled) {
    DWORD type = 0;
    DWORD size = 2048;

    LONG res = RegQueryValueEx(vbKey,
                               key,
                               NULL,
                               &type,
                               (UCHAR *)buffer,
                               &size);

    if(res == ERROR_SUCCESS && type == REG_SZ)
      return buffer;

    return def;
  }

  DWORD res = GetPrivateProfileString(VBA_PREF,
                                      key,
                                      def,
                                      (LPTSTR)buffer,
                                      2048,
                                      *regVbaPath);

  if(res)
    return buffer;

  return def;
}

DWORD regQueryDwordValue(const TCHAR * key, DWORD def, bool force)
{
  if(regEnabled || force) {
    DWORD type = 0;
    DWORD size = sizeof(DWORD);
    DWORD result = 0;

    LONG res = RegQueryValueEx(vbKey,
                               key,
                               NULL,
                               &type,
                               (UCHAR *)&result,
                               &size);

    if(res == ERROR_SUCCESS && type == REG_DWORD)
      return result;

    return def;
  }

  return GetPrivateProfileInt(VBA_PREF,
                              key,
                              def,
                              *regVbaPath);
}

BOOL regQueryBinaryValue(const TCHAR * key, void *value, int count)
{
  if(regEnabled) {
    DWORD type = 0;
    DWORD size = count;
    DWORD result = 0;


    LONG res = RegQueryValueEx(vbKey,
                               key,
                               NULL,
                               &type,
                               (UCHAR *)value,
                               &size);

    if(res == ERROR_SUCCESS && type == REG_BINARY)
      return TRUE;

    return FALSE;
  }
  CString k = key;
  k += _T("Count");
  int size = GetPrivateProfileInt(VBA_PREF,
                                  k,
                                  -1,
                                  *regVbaPath);
  if(size >= 0 && size < count)
    count = size;
  return GetPrivateProfileStruct(VBA_PREF,
                                 key,
                                 value,
                                 count,
                                 *regVbaPath);
}

void regSetStringValue(const TCHAR * key, const TCHAR * value)
{
  if(regEnabled) {
    LONG res = RegSetValueEx(vbKey,
                             key,
                             NULL,
                             REG_SZ,
                             (const UCHAR *)value,
                             lstrlen(value)+1);
  } else {
    WritePrivateProfileString(VBA_PREF,
                              key,
                              value,
                              *regVbaPath);
  }
}

void regSetDwordValue(const TCHAR * key, DWORD value, bool force)
{
  if(regEnabled || force) {
    LONG res = RegSetValueEx(vbKey,
                             key,
                             NULL,
                             REG_DWORD,
                             (const UCHAR *)&value,
                             sizeof(DWORD));
  } else {
    wsprintf(buffer, _T("%u"), value);
    WritePrivateProfileString(VBA_PREF,
                              key,
                              buffer,
                              *regVbaPath);
  }
}

void regSetBinaryValue(const TCHAR *key, void *value, int count)
{
  if(regEnabled) {
    LONG res = RegSetValueEx(vbKey,
                             key,
                             NULL,
                             REG_BINARY,
                             (const UCHAR *)value,
                             count);
  } else {
    CString k = key;
    k += "Count";
    wsprintf(buffer, _T("%u"), count);

    WritePrivateProfileString(VBA_PREF,
                              k,
                              buffer,
                              *regVbaPath);

    WritePrivateProfileStruct(VBA_PREF,
                              key,
                              value,
                              count,
                              *regVbaPath);
  }
}

void regDeleteValue(TCHAR *key)
{
  if(regEnabled) {
    LONG res = RegDeleteValue(vbKey,
                              key);
  } else {
    WritePrivateProfileString(VBA_PREF,
                              key,
                              NULL,
                              *regVbaPath);
  }
}

bool regCreateFileType(const TCHAR *ext, const TCHAR *type)
{
	HKEY key;
	CString temp;
	temp.Format(_T("Software\\Classes\\%s"), ext );
	LONG res = RegCreateKeyEx(
		HKEY_CURRENT_USER,
		temp,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&key,
		NULL );
	if( res == ERROR_SUCCESS ) {
		RegSetValueEx(
			key,
			_T(""),
			0,
			REG_SZ,
			(const BYTE *)type,
			lstrlen(type) + 1 );
		RegCloseKey( key );
		// Notify Windows that extensions have changed
		SHChangeNotify( SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL );
		return true;
	}
	return false;
}

bool regAssociateType(const TCHAR *type, const TCHAR *desc, const TCHAR *application, const TCHAR *icon)
{
	HKEY key;
	CString temp;
	temp.Format(_T("Software\\Classes\\%s"), type );
	LONG res = RegCreateKeyEx(
		HKEY_CURRENT_USER,
		temp,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&key,
		NULL );
	if( res == ERROR_SUCCESS ) {
		res = RegSetValueEx(
			key,
			_T(""),//"FriendlyTypeName",
			0,
			REG_SZ,
			(const BYTE *)desc,
			lstrlen(desc) + 1 );
		HKEY key2;
		res = RegCreateKeyEx(
			key,
			_T("Shell\\Open\\Command"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&key2,
			NULL );
		if( res == ERROR_SUCCESS ) {
			RegSetValueEx(
				key2,
				_T(""),
				0,
				REG_SZ,
				(const BYTE *)application,
				lstrlen(application) + 1 );
			if( icon != NULL ) {
				HKEY key3;
				res = RegCreateKeyEx(
					key,
					_T("DefaultIcon"),
					0,
					NULL,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL,
					&key3,
					NULL );
				if( res == ERROR_SUCCESS ) {
					RegSetValueEx(
						key3,
						_T(""),
						0,
						REG_SZ,
						(const BYTE *)icon,
						lstrlen(icon) + 1 );
				}
				RegCloseKey(key3);
			}
			RegCloseKey(key2);
			RegCloseKey(key);
			// Notify Windows that extensions have changed
			SHChangeNotify( SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL );
			return true;
		}
		RegCloseKey(key);
	}
	return false;
}

static void regExportSettingsToINI(HKEY key, const TCHAR *section)
{
    TCHAR valueName[256];
  int index = 0;
  while(1) {
    DWORD nameSize = 256;
    DWORD size = 2048;
    DWORD type;
    LONG res = RegEnumValue(key,
                            index,
                            valueName,
                            &nameSize,
                            NULL,
                            &type,
                            (LPBYTE)buffer,
                            &size);

    if(res == ERROR_SUCCESS) {
      switch(type) {
      case REG_DWORD:
        {
          TCHAR temp[256];
          wsprintf(temp, _T("%u"), *((DWORD *)buffer));
          WritePrivateProfileString(section,
                                    valueName,
                                    temp,
                                    *regVbaPath);
        }
        break;
      case REG_SZ:
        WritePrivateProfileString(section,
                                  valueName,
                                  buffer,
                                  *regVbaPath);
        break;
      case REG_BINARY:
        {
          TCHAR temp[256];

          wsprintf(temp, _T("%u"), size);
          CString k = valueName;
          k += "Count";
          WritePrivateProfileString(section,
                                    k,
                                    temp,
                                    *regVbaPath);
          WritePrivateProfileStruct(section,
                                    valueName,
                                    buffer,
                                    size,
                                    *regVbaPath);
        }
        break;
      }
      index++;
    } else
      break;
  }
}

void regExportSettingsToINI()
{
  if(vbKey != NULL) {
    regExportSettingsToINI(vbKey, VBA_PREF);
  }

  HKEY key;

  if(RegOpenKey(HKEY_CURRENT_USER,
                _T("Software\\Emulators\\VisualBoyAdvance\\Viewer"), &key) ==
     ERROR_SUCCESS) {
    regExportSettingsToINI(key, _T("Viewer"));
    RegCloseKey(key);
  }
}
