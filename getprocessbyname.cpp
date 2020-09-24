#include "getprocessbyname.h"

std::vector<DWORD> GetProcessByName(const wchar_t* pName, const wchar_t* pName2, std::vector<int>& keys)
{
    std::vector<DWORD> ids;
    PROCESSENTRY32 entry;
    DWORD pID = 0;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (_wcsicmp(entry.szExeFile, pName) == 0)
            {
                pID = entry.th32ProcessID;
                ids.push_back(pID);
                keys.push_back(0);
            }
            if (_wcsicmp(entry.szExeFile, pName2) == 0)
            {
                pID = entry.th32ProcessID;
                ids.push_back(pID);
                keys.push_back(1);
            }
        }
    }

    CloseHandle(snapshot);
    return ids;
}
