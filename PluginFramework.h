// PluginFramework.h --- PluginFramework
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.
#ifndef PLUGIN_FRAMEWORK_H_
#define PLUGIN_FRAMEWORK_H_

#include "Plugin.h"
#include <vector>

// NOTE: This structure must be a POD (Plain Old Data).
struct PLUGIN_EX : public PLUGIN
{
    TCHAR plugin_pathname[MAX_PATH];
    PLUGIN_LOAD load;
    PLUGIN_UNLOAD unload;
    PLUGIN_ACT act;
};

BOOL PF_LoadOne(PLUGIN_EX *pi, const TCHAR *pathname);
BOOL PF_LoadAll(std::vector<PLUGIN_EX>& pis, const TCHAR *dir);
BOOL PF_IsLoaded(PLUGIN_EX *pi);
LRESULT PF_ActOne(PLUGIN_EX *pi, UINT uAct, WPARAM wParam, LPARAM lParam);
LRESULT PF_ActAll(std::vector<PLUGIN_EX>& pis, UINT uAct, WPARAM wParam, LPARAM lParam);
BOOL PF_UnloadOne(PLUGIN_EX *pi);
BOOL PF_UnloadAll(std::vector<PLUGIN_EX>& pis);

#endif  // ndef PLUGIN_FRAMEWORK_H_
