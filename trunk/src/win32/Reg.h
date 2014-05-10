#ifndef VBA_REG_H
#define VBA_REG_H

extern bool regEnabled;

TCHAR *regQueryStringValue(const TCHAR *key, TCHAR *def);
DWORD regQueryDwordValue(const TCHAR *key, DWORD def, bool force=false);
BOOL regQueryBinaryValue(const TCHAR *key, void *value, int count);
void regSetStringValue(const TCHAR *key,const TCHAR *value);
void regSetDwordValue(const TCHAR *key,DWORD value,bool force=false);
void regSetBinaryValue(const TCHAR *key, void *value, int count);
void regDeleteValue(TCHAR *key);
void regInit(const TCHAR *, bool force = false);
void regShutdown();
bool regCreateFileType( const TCHAR *ext, const TCHAR *type );
bool regAssociateType( const TCHAR *type, const TCHAR *desc, const TCHAR *application, const TCHAR *icon = NULL );
void regExportSettingsToINI();
#endif // VBA_REG_H
