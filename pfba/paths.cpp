
// Paths module
#include <unistd.h>
#include <sys/stat.h>
#include "burner.h"

#ifdef __PSP2__
#include <psp2/io/dirent.h>
char szAppHomePath[MAX_PATH] = "ux0:/data/pfba/";
char szAppSavePath[MAX_PATH] = "ux0:/data/pfba/saves";
char szAppConfigPath[MAX_PATH] = "ux0:/data/pfba/configs";
char szAppHiscorePath[MAX_PATH] = "ux0:/data/pfba/hiscore";
char szAppSamplesPath[MAX_PATH] = "ux0:/data/pfba/samples";
char szAppPreviewPath[MAX_PATH] = "ux0:/data/pfba/previews";
char szAppBlendPath[MAX_PATH] = "ux0:/data/pfba/blend/";
char szAppNvPath[MAX_PATH] = "ux0:/data/pfba/config/games";
char szAppSkinPath[MAX_PATH] = "app0:/skin";
#else
char szAppHomePath[MAX_PATH];
char szAppSavePath[MAX_PATH];
char szAppConfigPath[MAX_PATH];
char szAppHiscorePath[MAX_PATH];
char szAppSamplesPath[MAX_PATH];
char szAppPreviewPath[MAX_PATH];
char szAppBlendPath[MAX_PATH];
char szAppNvPath[MAX_PATH];
char szAppSkinPath[MAX_PATH];
#endif

void BurnPathsInit()
{
#ifndef __PSP2__ // TODO : crash on psp2 ?!
#ifdef __3DS__
    strncpy(szAppHomePath, "/pfba", MAX_PATH);
#else
    getcwd(szAppHomePath, MAX_PATH);
#endif
    strcat(szAppHomePath, "/");
    mkdir(szAppHomePath, 0777);
    //printf("szAppHomePath: %s\n", szAppHomePath);

    snprintf(szAppSavePath, MAX_PATH, "%s%s", szAppHomePath, "saves");
    mkdir(szAppSavePath, 0777);
    //printf("szAppSavePath: %s\n", szAppSavePath);

    snprintf(szAppConfigPath, MAX_PATH, "%s%s", szAppHomePath, "configs");
    mkdir(szAppConfigPath, 0777);
    //printf("szAppConfigPath: %s\n", szAppConfigPath);

    snprintf(szAppHiscorePath, MAX_PATH, "%s%s", szAppHomePath, "hiscore");
    mkdir(szAppHiscorePath, 0777);
    //printf("szAppHiscorePath: %s\n", szAppHiscorePath);

    snprintf(szAppSamplesPath, MAX_PATH, "%s%s", szAppHomePath, "samples");
    mkdir(szAppSamplesPath, 0777);
    //printf("szAppSamplesPath: %s\n", szAppSamplesPath);

    snprintf(szAppPreviewPath, MAX_PATH, "%s%s", szAppHomePath, "previews");
    mkdir(szAppPreviewPath, 0777);
    //printf("szAppPreviewPath: %s\n", szAppPreviewPath);

    snprintf(szAppBlendPath, MAX_PATH, "%s%s/", szAppHomePath, "blend");
    mkdir(szAppBlendPath, 0777);
    //printf("szAppBlendPath: %s\n", szAppBlendPath);

    snprintf(szAppSkinPath, MAX_PATH, "%s%s", szAppHomePath, "skin");
    mkdir(szAppSkinPath, 0777);
    //printf("szAppSkinPath: %s\n", szAppSkinPath);

    snprintf(szAppNvPath, MAX_PATH, "%sconfig", szAppHomePath);
    mkdir(szAppNvPath, 0777);
    strncat(szAppNvPath, "/games", MAX_PATH);
    mkdir(szAppNvPath, 0777);
#endif
}
