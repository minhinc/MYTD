/*
 * front.h
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
#ifndef FRONT_H
#define FRONT_H
#include <QWidget>
struct widget;
struct front:QWidget {
front();
~front();
int li;
void* dt;
widget* cgb;
void *cd;
void* nam;
void* rear;
widget* dbw;
void* fle;
void* fpb;
void* pcpb;
void* ppb;
void* dlb;
void* lw;
void* alb;
void updatelist(int,int,void* d=0);
int bwdtodb(int,void* d=0);
void paintold(bool option=true,bool enable=true,bool hidecgb=false);
public slots:
void slotclicked();
void slotlwcurrentrowchanged(int);
private:
Q_OBJECT
};
#endif
