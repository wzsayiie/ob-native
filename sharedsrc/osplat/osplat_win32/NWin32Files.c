#include "nfiles.h"
#include <ShlObj.h>
#include <Windows.h>

NString *NCopyDocumentPath(void)
{
    return NULL;
}

NString *NCopyCachesPath(void)
{
    return NULL;
}

NString *NCopyTemporaryPath(void)
{
    return NULL;
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
            WCHAR *allChars = NAlloc(MAX_PATH * sizeof(WCHAR) + size);
            GetCurrentDirectoryW(MAX_PATH, allChars);
            if (chars[0] != L'\\' && chars[0] != '/')
            {
                wcscat(allChars, L"\\");
            }
            wcscat(allChars, chars);

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
    int     size  = NStringU16Size(path);
    WCHAR   last  = *(WCHAR *)((uint8_t *)chars + size - sizeof(WCHAR));

    WCHAR *target = NAlloc(size + 3 * sizeof(WCHAR));
    NMoveMemory(target, chars, size);
    if (last != L'\\' && last != L'/')
    {
        NMoveMemory((uint8_t *)target + size, L"\\*", 2 * sizeof(WCHAR));
    }
    else
    {
        NMoveMemory((uint8_t *)target + size, L"*", sizeof(WCHAR));
    }

    //traverse the subitems.
    WIN32_FIND_DATAW data;
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
