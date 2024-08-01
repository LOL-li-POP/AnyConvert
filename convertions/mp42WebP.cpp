#include <string>
#include <windows.h>
#include "mp42WebP.h"

std::string getExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

bool convertToWebP(const std::string& inputPath, const std::string& outputPath) {
    std::string ffmpegPath = getExecutablePath() + "\\ffmpeg\\bin\\ffmpeg.exe";

    std::string command = "\"" + ffmpegPath + "\" -i \"" + inputPath + "\" -vcodec libwebp \"" + outputPath + "\"";

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return exitCode == 0;
}
