#include "nfiles.h"
#include <ShlObj.h>
#include <Windows.h>

static void _NGetProgramName(WCHAR *outName)
{
    //wcscpy(outName, L"custom_program_name");
    //return;

    WCHAR path[MAX_PATH] = L"\0";
    GetModuleFileNameW(NULL, path, MAX_PATH);

    WCHAR *name = wcsrchr(path, L'\\') + 1;
    WCHAR *end  = wcsrchr(path, L'.');

    size_t len = end - name;
    if (len > 0)
    {
        wcsncpy(outName, name, len);
        outName[len] = L'\0';
    }
    else
    {
        outName[0] = L'\0';
    }
}

static void _NConcatenatePath(WCHAR *buffer, const WCHAR *item)
{
    if (!item || *item == L'\0')
    {
        return;
    }

    if (*buffer)
    {
        WCHAR back = buffer[wcslen(buffer) - 1];
        if ( back != L'\\' &&  back != L'/'
         && *item != L'\\' && *item != L'/')
        {
            wcscat(buffer, L"\\");
        }
        wcscat(buffer, item);
    }
    else
    {
        wcscpy(buffer, item);
    }
}

static NString *_NCreateUserDirectory(const WCHAR *parent, const WCHAR *sub)
{
    WCHAR name[MAX_PATH] = L"\0";
    _NGetProgramName(name);

    WCHAR buffer[MAX_PATH] = L"\0";
    _NConcatenatePath(buffer, parent);
    _NConcatenatePath(buffer, name);
    _NConcatenatePath(buffer, sub);

    NString *path = NStringCreateWithUTFChars(NUTF16, buffer);
    if (!NPathExists(path, NULL))
    {
        NMakeDirectory(path, true);
    }
    return path;
}

NString *NCopyDocumentPath(void)
{
    static NString *path = NULL;
    nsynonce()
    {
        WCHAR directory[MAX_PATH] = L"\0";
        SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, 0, directory);

        path = _NCreateUserDirectory(directory, NULL);
    }
    return NStringCopy(path);
}

NString *NCopyCachesPath(void)
{
    static NString *path = NULL;
    nsynonce()
    {
        WCHAR directory[MAX_PATH] = L"\0";
        GetTempPathW(MAX_PATH, directory);

        path = _NCreateUserDirectory(directory, L"caches");
    }
    return NStringCopy(path);
}

NString *NCopyTemporaryPath(void)
{
    static NString *path = NULL;
    nsynonce()
    {
        WCHAR directory[MAX_PATH] = L"\0";
        GetTempPathW(MAX_PATH, directory);

        path = _NCreateUserDirectory(directory, L"temporary");
    }
    return NStringCopy(path);
}

bool NMakeDirectory(NString *path, bool intermediate)
{
    if (NStringIsEmpty(path))
    {
        return false;
    }

    if (intermediate)
    {
        const WCHAR *chars = NStringU16Chars(path);
        int size = NStringU16Size(path);

        //SHCreateDirectory() isn't support relative path.
        if (size >= 2 * sizeof(WCHAR) && chars[1] != L':')
        {
            WCHAR *allChars = NAlloc(MAX_PATH * sizeof(WCHAR) + sizeof(WCHAR) + size);
            GetCurrentDirectoryW(MAX_PATH, allChars);
            _NConcatenatePath(allChars, chars);

            int error = SHCreateDirectoryExW(NULL, allChars, NULL);
            NFree(allChars);

            return error == ERROR_SUCCESS;
        }
        else
        {
            int error = SHCreateDirectoryExW(NULL, chars, NULL);
            return error == ERROR_SUCCESS;
        }
    }
    else
    {
        const WCHAR *chars = NStringU16Chars(path);
        return CreateDirectoryW(chars, NULL);
    }
}

NArray *NCopySubitems(NString *path, bool *success)
{
    if (NStringIsEmpty(path))
    {
        if (success) {*success = false;}
        return NULL;
    }

    //construct the pattern string "xx\xx\*".
    LPCWSTR chars = NStringU16Chars(path);
    int size = NStringU16Size(path);

    WCHAR *target = NAlloc(size + 3 * sizeof(WCHAR));
    wcscpy(target, chars);
    _NConcatenatePath(target, L"*");

    //traverse the subitems.
    WIN32_FIND_DATAW data = {0};
    HANDLE state = FindFirstFileW(target, &data);
    if (state == INVALID_HANDLE_VALUE)
    {
        NFree(target);
        if (success) {*success = false;}
        return NULL;
    }

    NArray *subitems= NArrayCreate();
    while (true)
    {
        //ignore ".", ".." and hidden files.
        bool startsWithDot = (data.cFileName[0] == L'.');
        bool hiddenFile = (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
        if (!startsWithDot && !hiddenFile)
        {
            NString *item = NStringCreateWithUTFChars(NUTF16, data.cFileName);
            NArrayPush(subitems, item);
            NRelease(item);
        }

        if (!FindNextFileW(state, &data))
        {
            break;
        }
    }
    FindClose(state);

    NFree(target);
    if (success) {*success = false;}
    return subitems;
}

bool NPathExists(NString *path, bool *isDirectory)
{
    bool directory = false;
    bool exists = false;

    if (!NStringIsEmpty(path))
    {
        const WCHAR *chars = NStringU16Chars(path);
        DWORD attributes = GetFileAttributesW(chars);
        if (attributes != INVALID_FILE_ATTRIBUTES)
        {
            directory = (attributes & FILE_ATTRIBUTE_DIRECTORY);
            exists = true;
        }
    }

    if (isDirectory) {*isDirectory = directory;}
    return exists;
}

void NRemovePath(NString *path)
{
    if (NStringIsEmpty(path))
    {
        return;
    }

    //SHFILEOPSTRUCT.pFrom need double null terminating.
    const WCHAR *chars = NStringU16Chars(path);
    int size = NStringU16Size(path);

    uint8_t *from = NAlloc(size + 2 * sizeof(WCHAR));
    NMoveMemory(from, chars, size);
    NMoveMemory(from + size, L"\0\0", 2 * sizeof(WCHAR));

    SHFILEOPSTRUCTW operation;
    memset(&operation, 0, sizeof(operation));
    operation.wFunc = FO_DELETE;
    operation.pFrom = (WCHAR *)from;
    operation.pTo = NULL;
    operation.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
    SHFileOperationW(&operation);

    NFree(from);
}
