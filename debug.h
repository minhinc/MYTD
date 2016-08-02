/*
 * debug.h
 * Minh YouTube Downloader
 * Copyright (c) 2015-2016 Minh Inc www.minhinc.com
 *
 * This file is part of MYTD.
 *
 * MYTD is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * MYTD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Mgmp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef DEBUG_H
#define DEBUG_H
#include <QtDebug>
#ifndef GFDNODEBUG
#define debug(arg) qDebug()<< arg ;
#else
#define debug(arg)
#endif
#define DEV_MAX 3
#define TYPE_MAX 32
#define MW_ROW 4
#define MW_ROW_HEIGHT 15
#define BUTTON_WIDTH 55
#define BUTTON_HEIGHT 22
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 200
#define FILENAME_TAB WINDOW_WIDTH/12
#define ITAG_TAB 4+10
#define SIZE_TAB 4+4
#define STATUS_TAB 4+10
#define VIEW_COL_COUNT 4

#define HEAD 0
#define GET 1
#define CHECK 2
#define FETCH 3

#define FREE 21
#define PROGRESS 22
#define PAUSE 23
#define COMPLETE 24

#define OK TYPE_MAX+1
#define CANCEL TYPE_MAX+2

#define LABELGB 51
#define QUESTIONDB 52
#define GRIDDB 53
#define VIDEOWD 54
#define WEBVIEW 55

#define FILE_FETCH_DURATION 10000

#define DIR_FILE QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+"/dir"
#define SYNC_FILE QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+"/syncfile"
#define TMP_FILE QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+"/tmp"
#define KEY_FILE QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+"/key"
#define DOWNLOAD_DIR QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+"/"
#endif
