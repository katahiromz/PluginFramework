#include "PluginFramework.h"
#include <cstdio>

int main(int argc, char **argv)
{
    std::vector<PLUGIN_EX> pis;
    if (!PF_LoadAll(pis, argc >= 2 ? argv[1] : TEXT(".")))
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
    }

    PF_ActAll(pis, 1, 2, 3);
    PF_ActAll(pis, 2, 2, 3);

    PF_UnloadAll(pis);

    return 0;
}
