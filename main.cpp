#include <windows.h>
#include <fstream>
#include <string>
#include "resource.h"

// دالة لاستخراج الصورة المضمنة من الموارد (Resource) إلى مسار محدد
bool ExtractResourceToFile(WORD resourceID, const std::wstring& outputPath) {
    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(resourceID), RT_RCDATA);
    if (!hRes) return false;

    HGLOBAL hMem = LoadResource(hModule, hRes);
    if (!hMem) return false;

    DWORD size = SizeofResource(hModule, hRes);
    LPVOID pData = LockResource(hMem);
    if (!pData || size == 0) return false;

    HANDLE hFile = CreateFileW(
        outputPath.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) return false;

    DWORD bytesWritten = 0;
    BOOL result = WriteFile(hFile, pData, size, &bytesWritten, NULL);
    CloseHandle(hFile);

    return (result && bytesWritten == size);
}

// دالة للحصول على المسار الكامل داخل مجلد Temp للنظام
std::wstring GetTempFilePath(const std::wstring& filename) {
    wchar_t tempPath[MAX_PATH];
    DWORD pathLen = GetTempPathW(MAX_PATH, tempPath);
    if (pathLen == 0 || pathLen > MAX_PATH) return L"";

    return std::wstring(tempPath) + filename;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // تحديد مسار حفظ الصورة المؤقتة
    std::wstring tempImagePath = GetTempFilePath(L"temp_wallpaper.png");

    if (tempImagePath.empty()) {
        MessageBoxW(NULL, L"فشل في الوصول إلى مجلد الملفات المؤقتة.", L"خطأ", MB_OK | MB_ICONERROR);
        return 1;
    }

    // استخراج الصورة المضمنة داخل EXE إلى Temp
    if (!ExtractResourceToFile(IDR_WALLPAPER_IMG, tempImagePath)) {
        MessageBoxW(NULL, L"فشل في استخراج صورة الخلفية المضمنة من البرنامج.", L"خطأ", MB_OK | MB_ICONERROR);
        return 1;
    }

    // تطبيق الصورة كخلفية لسطح المكتب باستعمال Windows API
    BOOL success = SystemParametersInfoW(
        SPI_SETDESKWALLPAPER,
        0,
        (PVOID)tempImagePath.c_str(),
        SPIF_UPDATEINIFILE | SPIF_SENDCHANGE
    );

    if (success) {
        MessageBoxW(NULL, L"tm tshfyr byanatk mn m7kmh al3dl w gary atlaf ns5h alwyndwz", L"M7kmh_Al3dl", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBoxW(NULL, L"gary atlaf ns5h alwyndwz", L"M7kmh_Al3dl", MB_OK | MB_ICONERROR);
    }
      else {
                MessageBoxW(NULL, L"la t7awl ay8af al3mlyh lan sytm tsry3 3mlyh atlaf alwyndwz", L"M7kmh_Al3dl", MB_OK | MB_ICONERROR);
      }
    // تنظيف وحذف النسخة المؤقتة للصورة
    DeleteFileW(tempImagePath.c_str());

    return success ? 0 : 1;
}