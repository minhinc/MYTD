/*
 * device.cpp
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
#include <QStringList>
#include <QString>
#include "device.h"
#include "debug.h"
void* device::rear=0;
void* device::dt=0;
device::device(device* n):next(n){
debug("/device::device")
mlst=new QStringList;
fnm=new QString;
key=new QString;
}
device::~device(){
debug("/device::~device")
delete (QString*)fnm;
delete (QStringList*)mlst;
delete (QString*)key;
}
void* device::convert(int size){
int filesize;
char typechar='M';
static QString str;
debug("device::convert")
str="?";
if(size<1024*1024){
typechar='K';
filesize=size/1024;
}else
filesize=size/(1024*1024);
if(size>0)
str=QString("%1%2").arg(filesize).arg(typechar);
debug("~device::convert")
return &str;
}
