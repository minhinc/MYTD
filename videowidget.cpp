/*
 * videowidget.cpp
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
#include <QStyle>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>

#include "videowidget.h"
#include "debug.h"

videowidget::videowidget(QWidget *p):QWidget(p){
QVideoWidget *vw=new QVideoWidget;
QVBoxLayout *vlt=new QVBoxLayout;
vlt->addWidget(vw);
QHBoxLayout *hlt=new QHBoxLayout;
hlt->setMargin(0);
hlt->addWidget(pb=new QPushButton);
hlt->addWidget(sl=new QSlider(Qt::Horizontal));
sl->setRange(0,0);
pb->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
vlt->addLayout(hlt);
setLayout(vlt);
mp=new QMediaPlayer(0, QMediaPlayer::VideoSurface);
mp->setVideoOutput(vw);
connect(mp,SIGNAL(positionChanged(qint64)),this,SLOT(slotslvaluechanged(qint64)));
connect(mp,SIGNAL(durationChanged(qint64)),this,SLOT(slotsldurationchanged(qint64)));
connect(pb,SIGNAL(clicked()),this,SLOT(slotpbclicked()));
connect(sl,SIGNAL(sliderMoved(int)),this,SLOT(slotslslidermoved(int)));
debug("~videowidget::videowidget")
}

videowidget::~videowidget(){
debug("videowidget::~videowidget")
delete (QMediaPlayer*)mp;
debug("~videowidget::~videowidget")
}
void videowidget::slotsldurationchanged(qint64 val){
debug("videowidget::slotsldurationchanged")
sl->setRange(0,val);
debug("videowidget::slotsldurationchanged")
}
void videowidget::slotslvaluechanged(qint64 val){
debug("videowidget::slotslvaluechanged")
sl->setValue(val);
debug("~videowidget::slotslvaluechanged")
}
void videowidget::slotslslidermoved(int val){
debug("videowidget::slotslslidermoved")
mp->setPosition(val);
debug("~videowidget::slotslslidermoved")
}
void videowidget::slotpbclicked(){
debug("videowidget::slotpbclicked")
if(mp->isMuted()){
mp->setMuted(false);
pb->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
}else{
mp->setMuted(true);
pb->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
}
debug("~videowidget::slotpbclicked")
}
