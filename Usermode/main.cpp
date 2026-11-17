#include <Windows.h>
#include <iostream>
#include <print>

bool EnableSystemEnvironmentPrivilege()
{
    void* token;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
        return false;

    TOKEN_PRIVILEGES tp{};
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!LookupPrivilegeValueW(nullptr, SE_SYSTEM_ENVIRONMENT_NAME, &tp.Privileges[0].Luid))
    {
        CloseHandle(token);
        return false;
    }

    if (!AdjustTokenPrivileges(token, 0, &tp, sizeof(tp), nullptr, nullptr))
    {
        CloseHandle(token);
        return false;
    }

    int error = GetLastError();

    CloseHandle(token);

    return error == ERROR_SUCCESS;
}

int main()
{
    if (!EnableSystemEnvironmentPrivilege())
    {
        std::println("Failed to enable privilege.\nRun as administrator.");
        std::cin.get();
        return 1;
    }

    int magic_number{};
    int bytes_returned = GetFirmwareEnvironmentVariableW( L"EFB", L"{4F4E5958-58B7-4940-9C72-1226390518F8}", &magic_number, sizeof(int));

    std::println("Magic number: {}\nBytes returned: 0x{}", magic_number, bytes_returned);

    std::cin.get();

    return 0;
}