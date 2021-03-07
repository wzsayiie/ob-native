#include "nfiles.h"
#include <ShlObj.h>
#include <Windows.h>

static void GetProgramName(WCHAR *outName)
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

static void ConcatenatePath(WCHAR *buffer, const WCHAR *item)
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

static NString *CreateUserDirectory(const WCHAR *parent, const WCHAR *sub)
{
    WCHAR name[MAX_PATH] = L"\0";
    GetProgramName(name);

    WCHAR buffer[MAX_PATH] = L"\0";
    ConcatenatePath(buffer, parent);
    ConcatenatePath(buffer, name);
    ConcatenatePath(buffer, sub);

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

        path = CreateUserDirectory(directory, NULL);
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

        path = CreateUserDirectory(directory, L"caches");
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

        path = CreateUserDirectory(directory, L"temporary");
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
            WCHAR *allChars = NAllocMemory(MAX_PATH * sizeof(WCHAR) + sizeof(WCHAR) + size);
            GetCurrentDirectoryW(MAX_PATH, allChars);
            ConcatenatePath(allChars, chars);

            int error = SHCreateDirectoryExW(NULL, allChars, NULL);
            NFreeMemory(allChars);

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

static void SetOutBool(bool *out, bool value) {
    if (out) {
        *out = value;
    }
}

NArray *NCopySubItems(NString *path, bool *outSuccess)
{
    if (NStringIsEmpty(path))
    {
        SetOutBool(outSuccess, false);
        return NULL;
    }

    //construct the pattern string "xx\xx\*".
    LPCWSTR chars = NStringU16Chars(path);
    int size = NStringU16Size(path);

    WCHAR *target = NAllocMemory(size + 3 * sizeof(WCHAR));
    wcscpy(target, chars);
    ConcatenatePath(target, L"*");

    //traverse the subitems.
    WIN32_FIND_DATAW data = {0};
    HANDLE state = FindFirstFileW(target, &data);
    if (state == INVALID_HANDLE_VALUE)
    {
        NFreeMemory(target);
        SetOutBool(outSuccess, false);
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

    NFreeMemory(target);
    SetOutBool(outSuccess, true);
    return subitems;
}

bool NPathExists(NString *path, bool *outIsDirectory)
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

    SetOutBool(outIsDirectory, directory);
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

    uint8_t *from = NAllocMemory(size + 2 * sizeof(WCHAR));
    NMoveMemory(from, chars, size);
    NMoveMemory(from + size, L"\0\0", 2 * sizeof(WCHAR));

    SHFILEOPSTRUCTW operation;
    memset(&operation, 0, sizeof(operation));
    operation.wFunc = FO_DELETE;
    operation.pFrom = (WCHAR *)from;
    operation.pTo = NULL;
    operation.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
    SHFileOperationW(&operation);

    NFreeMemory(from);
}
