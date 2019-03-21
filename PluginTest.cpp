// PluginTest.cpp --- PluginFramework tests
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.
#include "PluginFramework.h"
#include <cstdio>
#include <shlwapi.h>

int main(int argc, char **argv)
{
    TCHAR szDir[MAX_PATH];
    GetModuleFileName(NULL, szDir, ARRAYSIZE(szDir));
    *PathFindFileName(szDir) = 0;
    PathRemoveBackslash(szDir);

    std::vector<PLUGIN> pis;
    if (!PF_LoadAll(pis, szDir))
    {
        printf("unable to load\n");
        return 1;
    }

    printf("loaded: %d\n", (int)pis.size());

    for (size_t i = 0; i < pis.size(); ++i)
    {
        printf("---\n");
        printf("#%d:\n", (int)i);
        printf("plugin_version: 0x%08lX\n", pis[i].plugin_version);
        printf("plugin_product_name: '%s'\n", pis[i].plugin_product_name);
        printf("plugin_filename: '%s'\n", pis[i].plugin_filename);
        printf("plugin_company: '%s'\n", pis[i].plugin_company);
        printf("plugin_copyright: '%s'\n", pis[i].plugin_copyright);
        printf("plugin_instance: %p\n", pis[i].plugin_instance);
        printf("plugin_pathname: '%s'\n", pis[i].plugin_pathname);
    }

    PF_ActAll(pis, 1, 2, 3);
    PF_ActAll(pis, 2, 2, 3);

    PF_UnloadAll(pis);

    return 0;
}
