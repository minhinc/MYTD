/*
 * fetcher.h
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
#ifndef FETCHER_H
#define FETCHER_H
#include <QObject>
#include "debug.h"
struct QNetworkReply;
struct get{
get();
~get();
void* fnm;
void* url;
int size;
int state;
QNetworkReply* reply;
};
struct fetcher:QObject{
fetcher(void* dt=0,void* fnt=0);
~fetcher();
void* dt;
void* dv;
void* cdv;
void* frnt;
void* timer;
void* hr[TYPE_MAX];
get sl[DEV_MAX];
QNetworkReply* tn[TYPE_MAX];
void* handle(int,void* d=0);
private slots:
void slottimer();
void slotcomplete(QNetworkReply*);
private:
Q_OBJECT
};
#endif
