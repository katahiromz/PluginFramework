// Plugin.h --- PluginFramework Plugin interface
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.
#ifndef PLUGIN_H_
#define PLUGIN_H_
// TODO: Rename this file

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif

// TODO: Change me!
#define FRAMEWORK_NAME TEXT("PluginFramework")

// TODO: Change me!
#define FRAMEWORK_SPEC TEXT("*.plugin")

// TODO: Change me!
#define FRAMEWORK_VERSION 1

struct PLUGIN;
struct PLUGIN_FRAMEWORK_IMPL;

typedef BOOL (APIENTRY *PLUGIN_LOAD)(struct PLUGIN *pi, LPARAM lParam);
typedef BOOL (APIENTRY *PLUGIN_UNLOAD)(struct PLUGIN *pi, LPARAM lParam);
typedef LRESULT (APIENTRY *PLUGIN_ACT)(struct PLUGIN *pi, UINT uAct, WPARAM wParam, LPARAM lParam);

// NOTE: This structure must be a POD (Plain Old Data).
typedef struct PLUGIN
{
    // Don't change:
    DWORD framework_version;
    TCHAR framework_name[32];
    HINSTANCE framework_instance;
    HWND framework_window;
    TCHAR plugin_pathname[MAX_PATH];
    struct PLUGIN_FRAMEWORK_IMPL *framework_impl;

    // Please fill them in Plugin_Load:
    DWORD plugin_version;
    TCHAR plugin_product_name[64];
    TCHAR plugin_filename[32];
    TCHAR plugin_company[64];
    TCHAR plugin_copyright[128];
    HINSTANCE plugin_instance;

    // Use freely:
    HWND plugin_window;
    void *p_user_data;
    LPARAM l_user_data;

    // TODO: Add more members...
} PLUGIN;

#ifdef PLUGIN_BUILD
    #define PLUGIN_API __declspec(dllexport)
#else
    #define PLUGIN_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// API Name: Plugin_Load
// Purpose: The framework loads the plugin component.
// TODO: Load the plugin.
PLUGIN_API BOOL APIENTRY Plugin_Load(PLUGIN *pi, LPARAM lParam);

// API Name: Plugin_Unload
// Purpose: The framework unloads the plugin component.
// TODO: Unload the plugin.
PLUGIN_API BOOL APIENTRY Plugin_Unload(PLUGIN *pi, LPARAM lParam);

// API Name: Plugin_Act
// Purpose: Act something on the plugin.
// TODO: Act something on the plugin.
PLUGIN_API LRESULT APIENTRY Plugin_Act(PLUGIN *pi, UINT uAct, WPARAM wParam, LPARAM lParam);

// TODO: Add more APIs

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // ndef PLUGIN_H_
