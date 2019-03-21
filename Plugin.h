#ifndef PLUGIN_H_
#define PLUGIN_H_

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

typedef BOOL (APIENTRY *PLUGIN_LOAD)(struct PLUGIN *pi, LPARAM lParam);
typedef BOOL (APIENTRY *PLUGIN_UNLOAD)(struct PLUGIN *pi, LPARAM lParam);
typedef LRESULT (APIENTRY *PLUGIN_ACT)(struct PLUGIN *pi, UINT uAct, WPARAM wParam, LPARAM lParam);

typedef struct PLUGIN
{
    // Don't change:
    DWORD framework_version;
    TCHAR framework_name[32];
    HINSTANCE framework_instance;
    HWND framework_window;

    // Please change:
    DWORD plugin_version;
    TCHAR plugin_product_name[32];
    TCHAR plugin_filename[32];
    TCHAR plugin_company[64];
    TCHAR plugin_copyright[128];
    HINSTANCE plugin_instance;
    HWND plugin_window;

    // Use freely:
    void *p_user_data;
    LPARAM l_user_data;
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

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // ndef PLUGIN_H_
