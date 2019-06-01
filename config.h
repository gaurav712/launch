/*
 * Configuration file for launch
 *
 * Copyright (c) 2019 Gaurav Kumar Yadav <gaurav712@protonmail.com>
 * for license and copyright information, see the LICENSE file distributed with this source
 */

const char *fileAssociations[] = {
    "/usr/bin/mpv", /* first entry is program name and following are extentions to open with it until "END" */
    "mkv",
    "mp4",
    "END",

    "/usr/bin/feh",
    "jpg",
    "png",
    "END",

    "/usr/bin/zathura",
    "pdf",
    "END",

    /*
     * Add you stuff here
     */

    "ENDALL",
};
