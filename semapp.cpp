/*
 * semapp.cpp
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
#include <QStandardPaths>
#include <QSharedMemory>
#include <QFileSystemWatcher>
#include <QFile>
#include <QDir>
#include <QString>
#include <QDate>
#include "semapp.h"
#include "front.h"
#include "debug.h"
semapp::semapp(int& argc,char **argv):QApplication(argc,argv){
debug("semapp::semapp"<<SYNC_FILE)
shm=new QSharedMemory("sangf");
QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
if(!dir.exists()){
debug("creating "<<QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
QFile::resize(SYNC_FILE,0);
}
filewatcher=new QFileSystemWatcher(QStringList()<<SYNC_FILE);
connect(((QFileSystemWatcher*)filewatcher),SIGNAL(fileChanged(QString)),this,SLOT(slotfilechanged()));
debug("~semapp::semapp")
}
bool semapp::first(){
debug("/semapp::first")
QFile file(SYNC_FILE);
if(!file.open(QIODevice::WriteOnly|QIODevice::Append)){
debug("semapp::first could not create sync file")
return false;
}
if(!((QSharedMemory*)shm)->create(1)){
debug("could not create shm"<<((QSharedMemory*)shm)->error())
file.write(QString("%1 %2:%3\n").arg("instance failed at ").arg(QTime::currentTime().toString()).arg(QDate::currentDate().toString()).toUtf8().data());
file.write("try... ipcs|egrep -e '\s+1\s+0'|awk '{print \"ipcrm -M \"$1}'|sh");
file.close();
return false;
}
file.write(QString("%1 %2:%3\n").arg("instance started at ").arg(QTime::currentTime().toString()).arg(QDate::currentDate().toString()).toUtf8().data());
file.close();
return true;
}
void semapp::setfront(void* f){
debug("/semapp::front")
frnt=(front*)f;
}
void semapp::slotfilechanged(){
debug("semapp::slotfilechanged")
((front*)frnt)->activateWindow();
((front*)frnt)->showNormal();
((front*)frnt)->raise();
debug("~semapp::slotfilechanged")
}

semapp::~semapp(){
debug("/semapp::~semapp")
delete (QSharedMemory*)shm;
delete (QFileSystemWatcher*)filewatcher;
}
