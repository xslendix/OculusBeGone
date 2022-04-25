#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <windows.h>
#include <winuser.h>
#include <commdlg.h>

char*
strccat(char* s1, char* s2)
{
    char* result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char*
GetRegVal(HKEY key, const char* subkey, const char* valuename)
{
    DWORD keyType;
    DWORD dataSize;

    LONG result = RegGetValue(key, subkey, valuename, RRF_RT_REG_SZ, &keyType, NULL, &dataSize);

    if (result != ERROR_SUCCESS) {
        return NULL;
    }

    char* data = malloc(dataSize);

    result = RegGetValue(key, subkey, valuename, RRF_RT_REG_SZ, &keyType, data, &dataSize);

    if (result != ERROR_SUCCESS) {
        free(data);
        return NULL;
    }

    return data;
}

char*
PickEXE()
{
    char* file_name = malloc(MAX_PATH);
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Executable Files\0*.exe\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return file_name;
    } else {
        return NULL;
    }
}

bool
FileExists(const char* path)
{
    DWORD attrib = GetFileAttributes(path);
    return (attrib != INVALID_FILE_ATTRIBUTES);
}

int
main(void)
{
    char* steamvr = NULL;

    bool steamvr_found = FALSE;
    char* steam_path = GetRegVal(HKEY_CURRENT_USER, "SOFTWARE\\Valve\\Steam", "SteamPath");
    if (steam_path != NULL) {
        steamvr = strccat(steam_path, "\\steamapps\\common\\SteamVR\\bin\\win64\\vrstartup.exe");

        if (GetFileAttributes(steamvr) != INVALID_FILE_ATTRIBUTES) {
            steamvr_found = TRUE;
        }

        free(steam_path);
    }
    
    if (!steamvr_found) {
        // Check if our own registry value has the steamvr path set
        free(steamvr);
        steamvr = GetRegVal(HKEY_CURRENT_USER, "SOFTWARE\\Valve\\SteamVR", "SteamVRPath");
        if (steamvr != NULL) {
            if (GetFileAttributes(steamvr) != INVALID_FILE_ATTRIBUTES && strstr(steamvr, "vrstartup.exe") != NULL) {
                steamvr_found = TRUE;
            }
        }
    }

    if (!steamvr_found) {
        MessageBox(NULL, "It seems that we couldn't automatically find a SteamVR installation. Please select it manually. You shouldn't be prompted again.\nThe file you are looking for should be in steamapps\\common\\SteamVR\\bin\\win64\\vrstartup.exe", "Uh oh!", MB_OK);

        steamvr = PickEXE();
        if (strstr(steamvr, "vrstartup.exe") == NULL) {
            MessageBox(NULL, "The file you selected doesn't seem to be a valid SteamVR vrstartup.exe file. Exiting.", "Uh oh!", MB_OK);
            free(steamvr);
            return 1;
        }

        HKEY key;
        LONG result = RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Valve\\SteamVR", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);
        if (result != ERROR_SUCCESS) {
            printf("Couldn't create registry key.\n");
        }

        result = RegSetValueEx(key, "SteamVRPath", 0, REG_SZ, (BYTE*)steamvr, strlen(steamvr));
        if (result != ERROR_SUCCESS) {
            printf("Couldn't set registry value.\n");
        }

        RegCloseKey(key);
    }

    if (!FileExists(steamvr)) {
        MessageBox(NULL, "The SteamVR executable path seems to be invalid. Exiting.", "Uh oh!", MB_OK);
        free(steamvr);
        return 1;
    }

    printf("SteamVR path: %s\n", steamvr);

    // Start it and wait for it to exit
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(steamvr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        MessageBox(NULL, "Couldn't start SteamVR. Exiting.", "Uh oh!", MB_OK);
        free(steamvr);
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    free(steamvr);

    return 0;
}
