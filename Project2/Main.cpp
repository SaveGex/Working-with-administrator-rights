#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

bool DeleteDirectory(const std::wstring& dir) {
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((dir + L"\\*").c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::wcerr << L"�� ������� ������� ����������: " << GetLastError() << std::endl;
        return false;
    }

    do {
        const std::wstring fileOrDir = findData.cFileName;

        // ���������� ������� � ������������ ����������
        if (fileOrDir == L"." || fileOrDir == L"..") {
            continue;
        }

        std::wstring fullPath = dir + L"\\" + fileOrDir;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // ���������� ������� �������������
            DeleteDirectory(fullPath);
        }
        else {
            // ������� ����
            if (!DeleteFile(fullPath.c_str())) {
                std::wcerr << L"�� ������� ������� ����: " << fullPath << L" ��� ������: " << GetLastError() << std::endl;
            }
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);

    // ������� ���� ����������
    if (!RemoveDirectory(dir.c_str())) {
        std::wcerr << L"�� ������� ������� ����������: " << dir << L" ��� ������: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const std::wstring dirPath = L"C:\\Program Files (x86)\\Adobe"; // ������� ���� ����������
    if (DeleteDirectory(dirPath)) {
        std::wcout << L"���������� ������� �������.\n";
    }
    else {
        std::wcerr << L"�� ������� ������� ����������.\n";
    }

    return 0;
}
