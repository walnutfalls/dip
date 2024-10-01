#include <os/browse_dialog.hpp>

#ifdef __APPLE__
#include "TargetConditionals.h"
#ifdef TARGET_OS_MAC



std::optional<std::string> os::browse_dialog() {
    throw std::runtime_error("unsupported platform");
}



#endif
#elif defined _WIN32 || defined _WIN64

#define WIN32_LEAN_AND_MEAN

#include <windows.h>      // For common windows data types and function headers
#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <shtypes.h>

const COMDLG_FILTERSPEC c_rgSaveTypes[] =
{
    {L"PPM (*.ppm)",                   L"*.ppm"},
    {L"Web Page (*.htm; *.html)",    L"*.htm;*.html"},
    {L"Text Document (*.txt)",       L"*.txt"},
    {L"All Documents (*.*)",         L"*.*"}
};

// Indices of file types
#define INDEX_PPM 1
#define INDEX_WEBPAGE 2
#define INDEX_TEXTDOC 3

std::optional<std::string> BasicFileOpen()
{
    // CoCreate the File Open Dialog object.
    IFileDialog *pfd = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        // Set the options on the dialog.
        DWORD dwFlags;

        // Before setting, always get the options first in order not to override existing options.
        hr = pfd->GetOptions(&dwFlags);
        if (SUCCEEDED(hr))
        {
            // In this case, get shell items only for file system items.
            hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
            if (SUCCEEDED(hr))
            {
                // Set the file types to display only. Notice that, this is a 1-based array.
                hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                if (SUCCEEDED(hr))
                {
                    // Set the selected file type index to Word Docs for this example.
                    hr = pfd->SetFileTypeIndex(INDEX_PPM);
                    if (SUCCEEDED(hr))
                    {
                        // Set the default extension to be ".ppm" file.
                        hr = pfd->SetDefaultExtension(L"ppm");
                        if (SUCCEEDED(hr))
                        {
                            // Show the dialog
                            hr = pfd->Show(nullptr);
                            if (SUCCEEDED(hr))
                            {
                                // Obtain the result, once the user clicks the 'Open' button.
                                // The result is an IShellItem object.
                                IShellItem *psiResult;
                                hr = pfd->GetResult(&psiResult);
                                if (SUCCEEDED(hr))
                                {
                                    // Convert to normal string and return if all went well
                                    PWSTR pszFilePath = nullptr;
                                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                                    if (SUCCEEDED(hr))
                                    {
                                        auto wstr = std::wstring(pszFilePath);
                                        std::string s(wstr.begin(), wstr.end());
                                        CoTaskMemFree(pszFilePath);
                                        psiResult->Release();
                                        pfd->Release();
                                        return s;
                                    }
                                    psiResult->Release();
                                }
                            }
                        }
                    }
                }
            }
        }


        pfd->Release();
    }

    return std::optional<std::string>();
}

std::optional<std::string> os::browse_dialog() {
    return BasicFileOpen();
}
#elif defined linux

std::optional<std::string> os::browse_dialog() {
    throw std::runtime_error("unsupported platform");
}

#endif
