#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

bool DeleteDirectory(const std::wstring& dir) {
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((dir + L"\\*").c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Не удалось открыть директорию: " << GetLastError() << std::endl;
        return false;
    }

    do {
        const std::wstring fileOrDir = findData.cFileName;

        // Игнорируем текущую и родительскую директории
        if (fileOrDir == L"." || fileOrDir == L"..") {
            continue;
        }

        std::wstring fullPath = dir + L"\\" + fileOrDir;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Рекурсивно удаляем поддиректорию
            DeleteDirectory(fullPath);
        }
        else {
            // Удаляем файл
            if (!DeleteFile(fullPath.c_str())) {
                std::wcerr << L"Не удалось удалить файл: " << fullPath << L" Код ошибки: " << GetLastError() << std::endl;
            }
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);

    // Удаляем саму директорию
    if (!RemoveDirectory(dir.c_str())) {
        std::wcerr << L"Не удалось удалить директорию: " << dir << L" Код ошибки: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const std::wstring dirPath = L"C:\\Program Files (x86)\\Adobe"; // Укажите вашу директорию
    if (DeleteDirectory(dirPath)) {
        std::wcout << L"Директория успешно удалена.\n";
    }
    else {
        std::wcerr << L"Не удалось удалить директорию.\n";
    }

    return 0;
}
