// PluginFramework.cpp --- PluginFramework
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#include "PluginFramework.h"
#include <cstdio>
#include <cassert>
#include <shlwapi.h>
#include <strsafe.h>

static void PF_Init(PLUGIN_EX *pi)
{
    assert(pi);
    ZeroMemory(pi, sizeof(*pi));
    pi->framework_version = FRAMEWORK_VERSION;
    StringCbCopy(pi->framework_name, sizeof(pi->framework_name), FRAMEWORK_NAME);
    pi->framework_instance = GetModuleHandle(NULL);
    pi->framework_window = NULL;
}

static BOOL PF_Validate(PLUGIN_EX *pi)
{
    if (!pi)
    {
        assert(0);
        return FALSE;
    }
    if (pi->framework_version < FRAMEWORK_VERSION)
    {
        assert(0);
        return FALSE;
    }
    if (lstrcmpi(pi->framework_name, FRAMEWORK_NAME) != 0)
    {
        assert(0);
        return FALSE;
    }
    if (pi->framework_instance == NULL)
    {
        assert(0);
        return FALSE;
    }

    pi->plugin_product_name[ARRAYSIZE(pi->plugin_product_name) - 1] = 0;
    pi->plugin_filename[ARRAYSIZE(pi->plugin_filename) - 1] = 0;
    pi->plugin_company[ARRAYSIZE(pi->plugin_company) - 1] = 0;
    pi->plugin_copyright[ARRAYSIZE(pi->plugin_copyright) - 1] = 0;

    StrTrim(pi->plugin_product_name, TEXT(" \t\n\r\f\v"));

    if (pi->plugin_product_name[0] == 0)
    {
        assert(0);
        return FALSE;
    }
    if (pi->plugin_filename[0] == 0)
    {
        assert(0);
        return FALSE;
    }
    if (pi->plugin_company[0] == 0)
    {
        assert(0);
        return FALSE;
    }
    if (pi->plugin_copyright[0] == 0)
    {
        assert(0);
        return FALSE;
    }

    LPCTSTR filename = PathFindFileName(pi->plugin_pathname);
    if (!filename || lstrcmpi(filename, pi->plugin_filename) != 0)
    {
        assert(0);
        return FALSE;
    }

    if (!pi->plugin_instance)
    {
        assert(0);
        return FALSE;
    }

    return TRUE;
}

BOOL PF_IsLoaded(PLUGIN_EX *pi)
{
    return pi && !!pi->plugin_instance;
}

LRESULT PF_ActOne(PLUGIN_EX *pi, UINT uAct, WPARAM wParam, LPARAM lParam)
{
    if (!pi || !pi->act)
    {
        assert(0);
        return FALSE;
    }

    LRESULT ret = pi->act(pi, uAct, wParam, lParam);
    return ret;
}

LRESULT PF_ActAll(std::vector<PLUGIN_EX>& pis, UINT uAct, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 0;
    for (size_t i = 0; i < pis.size(); ++i)
    {
        ret = PF_ActOne(&pis[i], uAct, wParam, lParam);
    }
    return ret;
}

BOOL PF_UnloadOne(PLUGIN_EX *pi)
{
    if (!pi || !pi->unload)
    {
        assert(0);
        return FALSE;
    }

    BOOL ret = pi->unload(pi, 0);
    pi->plugin_instance = NULL;
    pi->load = NULL;
    pi->unload = NULL;
    pi->act = NULL;
    return ret;
}

BOOL PF_UnloadAll(std::vector<PLUGIN_EX>& pis)
{
    for (size_t i = 0; i < pis.size(); ++i)
    {
        PF_UnloadOne(&pis[i]);
    }
    pis.clear();
    return TRUE;
}

BOOL PF_LoadOne(PLUGIN_EX *pi, const TCHAR *pathname)
{
    assert(pathname != NULL);
    GetFullPathName(pathname, ARRAYSIZE(pi->plugin_pathname),
                    pi->plugin_pathname, NULL);

    DWORD attrs = GetFileAttributes(pi->plugin_pathname);
    if (attrs == 0xFFFFFFFF || (attrs & FILE_ATTRIBUTE_DIRECTORY))
    {
#ifndef NDEBUG
        //printf("'%s'\n", pi->plugin_pathname);
        assert(0);
#endif
        return FALSE;
    }

    HINSTANCE hInstDLL = LoadLibrary(pi->plugin_pathname);
    if (hInstDLL)
    {
        pi->load = (PLUGIN_LOAD)GetProcAddress(hInstDLL, "Plugin_Load");
        pi->unload = (PLUGIN_UNLOAD)GetProcAddress(hInstDLL, "Plugin_Unload");
        pi->act = (PLUGIN_ACT)GetProcAddress(hInstDLL, "Plugin_Act");

        if (pi->load && pi->unload && pi->act)
        {
            pi->plugin_instance = hInstDLL;

            if (pi->load(pi, 0))
            {
                if (PF_Validate(pi))
                {
                    return TRUE;
                }
                else
                {
                    assert(0);
                }
            }
            else
            {
                assert(0);
            }

            pi->unload(pi, 0);
        }

        FreeLibrary(hInstDLL);
    }
    assert(0);
    return FALSE;
}

BOOL PF_LoadAll(std::vector<PLUGIN_EX>& pis, const TCHAR *dir)
{
    pis.clear();

    if (!dir || dir[0] == 0)
    {
        dir = TEXT(".");
    }

    TCHAR szSpec[MAX_PATH];
    StringCbCopy(szSpec, sizeof(szSpec), dir);
    PathAppend(szSpec, FRAMEWORK_SPEC);
#ifndef NDEBUG
    //printf(szSpec: "'%s'\n", szSpec);
#endif

    TCHAR szPath[MAX_PATH];
    WIN32_FIND_DATA find;
    HANDLE hFind = FindFirstFile(szSpec, &find);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            PLUGIN_EX plugin;
            PF_Init(&plugin);

            StringCbCopy(szPath, sizeof(szPath), dir);
            PathAppend(szPath, find.cFileName);
#ifndef NDEBUG
            //printf("szPath: %s\n", szPath);
#endif
            if (PF_LoadOne(&plugin, szPath))
            {
                pis.push_back(plugin);
            }
        } while (FindNextFile(hFind, &find));

        FindClose(hFind);
    }

    return !pis.empty();
}
