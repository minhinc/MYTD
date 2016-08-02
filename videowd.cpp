/*
 * videowd.cpp
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
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QLabel>
#include <QString>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStyle>
#include <QMouseEvent>
#include <stdint.h>
#include <SDL.h>
#include <SDL_audio.h>
#include "widget.h"
#include "debug.h"
#include "audiocontext.h"
extern "C" qint64 av_gettime();
extern "C" int av_usleep(unsigned);
extern "C" int seekfileframe(int64_t);
extern "C" int nopen();
extern "C" int ninit();
extern "C" void nclose();
extern "C" void ndeinit();
extern "C" int readframe();
//extern "C" int readaframe(int,int64_t*);
extern "C" int readaframe(int);
extern "C" void newwidthheight(int,int);
extern struct audiocontext ac;
static void audio_callback(void*,Uint8* _stream,int _length){
//trace("callback,lenght"<<_length<<" "<<ac.begptr)
memset(_stream,'\0',_length);
if((ac.begptr+_length)>(GFDAUDIOBUFFERSIZE*2)){
//memcpy(_stream,ac.aptr+ac.begptr,(GFDAUDIOBUFFERSIZE*2)-ac.begptr);
//memcpy(_stream,ac.aptr,_length+ac.begptr-(GFDAUDIOBUFFERSIZE*2));
SDL_MixAudio(_stream,ac.aptr+ac.begptr,(GFDAUDIOBUFFERSIZE*2)-ac.begptr,ac.volume);
SDL_MixAudio(_stream,ac.aptr,_length+ac.begptr-(GFDAUDIOBUFFERSIZE*2),ac.volume);
ac.begptr=_length+ac.begptr-(GFDAUDIOBUFFERSIZE*2);
}else{
//memcpy(_stream,ac.aptr+ac.begptr,_length);
SDL_MixAudio(_stream,ac.aptr+ac.begptr,_length,ac.volume);
ac.begptr+=_length;
}
//trace("~callback")
}
videowd::videowd(int idn,widget *n):widget(idn,n){
debug("videowd::widget")
//setWindowIcon(QIcon(":/topicon.png"));
//setWindowTitle("GMP(Beta)");
SDL_Init(SDL_INIT_AUDIO);
setAutoFillBackground(false);
//setAttribute(Qt::WA_NoSystemBackground,true);
QPalette palette=this->palette();
palette.setColor(QPalette::Background, Qt::black);
setPalette(palette);
QVBoxLayout* vlt=new QVBoxLayout;
vlt->addStretch(1);
QHBoxLayout* hlt=new QHBoxLayout;
hlt->addWidget(lbl=new QLabel("<p style=\"color:#004000;font-family:Tw Cen MT;font-size:8pt\">minhinc.com<br/>Based upon Qt5</p>"));
hlt->addSpacerItem(new QSpacerItem(60,0,QSizePolicy::Expanding,QSizePolicy::Fixed));
hlt->addWidget(sldr=new QSlider(Qt::Horizontal));
//hlt->addWidget(fpb=new QPushButton(this));
//fpb->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
vlt->addLayout(hlt);
setLayout(vlt);
sldr->setDisabled(true);
tmr=new QTimer;
connect(tmr,SIGNAL(timeout()),this,SLOT(pushframe()));
//connect(fpb,SIGNAL(clicked()),this,SLOT(slotclicked()));
connect(sldr,SIGNAL(valueChanged(int)),this,SLOT(slotsldrvaluechanged(int)));
if(ninit()){
debug("error in avcoded iniitalization, exiting")
exit(-1);
}
ac.volume=40;
audio=video=0;
debug("~videowd::widget")
}
videowd::~videowd(){
debug("videowd::~widget")
ndeinit();
debug("~videowd::~widget")
}
void videowd::pushframe(){
int ret=0;
//trace("videowd::pushframe,tweak,framecount,audiosamplecount,video,audio"<<tweak<<" "<<framecount<<" "<<audiosamplecount<<"video"<<video<<"audio"<<audio)
if(video && bt+(((qint64)framecount*(qint64)ac.framerateden*(qint64)1000000)/(qint64)ac.frameratenum)<av_gettime())
if(readframe()<0){
video=0;
if(!video && !audio){
tmr->stop();
SDL_PauseAudio(1);
}
}else{
++framecount;
repaint();
}
else
if(!video && audio && bt+(((qint64)framecount*(qint64)ac.framerateden*(qint64)1000000)/(qint64)ac.frameratenum)<av_gettime())++framecount;
if((audio||video) && !(framecount%(ac.frameratenum/ac.framerateden))){
seekfile=false;
sldr->setValue(((qint64)framecount*(qint64)ac.framerateden)/(qint64)ac.frameratenum);
}
if(audio && ((!tweak && ac.begptr>=GFDAUDIOBUFFERSIZE/2 && ac.begptr <GFDAUDIOBUFFERSIZE) || (tweak && ac.begptr>=(GFDAUDIOBUFFERSIZE*3)/2))){
//debug("diff in frame"<<(float)(((int64_t)ac.videopts-((int64_t)ac.videostreampts*(int64_t)ac.audiopts)/(int64_t)ac.audiostreampts)*(int64_t)ac.frameratenum)/((int64_t)ac.framerateden*(int64_t)ac.videostreampts))
//trace("videopts"<<(qint64)ac.videopts<<"videostreampts"<<(qint64)ac.videostreampts<<"audiopts"<<(qint64)ac.audiopts<<"audiostreampts"<<(qint64)ac.audiostreampts<<"frameratenum"<<ac.frameratenum<<"framerateden"<<ac.framerateden)
if(video && ac.videopts && (((int64_t)ac.videopts-((int64_t)ac.videostreampts*(int64_t)ac.audiopts)/(int64_t)ac.audiostreampts)*(int64_t)ac.frameratenum)/((int64_t)ac.framerateden*(int64_t)ac.videostreampts)>=1){
//debug("MORE videopts"<<ac.videopts<<" "<<ac.videostreampts<<" "<<ac.audiopts<<" "<<ac.audiostreampts<<" "<<((int64_t)ac.videopts-((int64_t)ac.videostreampts*(int64_t)ac.audiopts)/(int64_t)ac.audiostreampts))
bt=bt+(((int64_t)ac.videopts-((int64_t)ac.videostreampts*(int64_t)ac.audiopts)/(int64_t)ac.audiostreampts)*(int64_t)1000000)/(int64_t)ac.videostreampts;
//ac.videopts=((int64_t)ac.videostreampts*(int64_t)ac.audiopts)/(int64_t)ac.audiostreampts;
ac.videopts=0;
}else if(video && ac.videopts && ((((int64_t)ac.videostreampts*(int64_t)ac.audiopts)/(int64_t)ac.audiostreampts-(int64_t)ac.videopts)*(int64_t)ac.frameratenum)/((int64_t)ac.framerateden*(int64_t)ac.videostreampts)>=1){
//debug("LESS videopts"<<ac.videopts<<" "<<ac.videostreampts<<" "<<ac.audiopts<<" "<<ac.audiostreampts<<" "<<((int64_t)ac.videopts-((int64_t)ac.videostreampts*(int64_t)ac.audiopts)/(int64_t)ac.audiostreampts))
bt=bt-((((int64_t)ac.videostreampts*(int64_t)ac.audiopts)/(int64_t)ac.audiostreampts-(int64_t)ac.videopts)*(int64_t)1000000)/(int64_t)ac.videostreampts;
}
if(readaframe(tweak=(tweak+1)%2)<0){
audio=0;
if(!video && !audio){
tmr->stop();
SDL_PauseAudio(1);
}
}
}
//trace("~videowd::pushframe"<<framecount<<" "<<audiosamplecount)
}
void videowd::paintEvent(QPaintEvent*){
//trace("videowd::paintEvent()")
QPainter p(this);
p.drawImage(QRect((width()-((width()*ac.vheight)/ac.vwidth>height()?(height()*ac.vwidth)/ac.vheight:width()))/2,(height()-((width()*ac.vheight)/ac.vwidth>height()?height():(ac.vheight*width())/ac.vwidth))/2,((width()*ac.vheight)/ac.vwidth>height()?(height()*ac.vwidth)/ac.vheight:width()),((width()*ac.vheight)/ac.vwidth>height()?height():(ac.vheight*width())/ac.vwidth)),QImage((uchar*)ac.vptr,ac.vwidth,ac.vheight,QImage::Format_RGB32).scaled(((width()*ac.vheight)/ac.vwidth>height()?(height()*ac.vwidth)/ac.vheight:width()),((width()*ac.vheight)/ac.vwidth>height()?height():(ac.vheight*width())/ac.vwidth),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
//trace("~videowd::paintEvent")
}
int videowd::handle(){
debug("videowd::slotclicked")
static QString prevfilename="/temp/dump";
QString filename;
filename=*(QString*)dt;
if(!filename.isEmpty() && !filename.isNull()){
strcpy(ac.filename,filename.toUtf8().data());
prevfilename=filename;
//setWindowTitle(filename);
tmr->stop();
SDL_CloseAudio();
nclose();
audio=video=0;
sldr->setDisabled(true);
if(!nopen()){
sldr->setDisabled(false);
pause=false;
resize(ac.vwidth,ac.vheight);
seekfile=false;
sldr->setRange(0,ac.tt/1000000);
seekfile=false;
sldr->setValue(0);
video=ac.video;
audio=ac.audio;
framecount=0;
if(ac.freq){
SDL_AudioSpec desiredSpec;
desiredSpec.freq=ac.freq;
if(ac.sampleformat==0||ac.sampleformat==5)
desiredSpec.format=AUDIO_U8;
else if(ac.sampleformat==1||ac.sampleformat==6)
desiredSpec.format=AUDIO_S16SYS;
else if(ac.sampleformat==2||ac.sampleformat==7)
desiredSpec.format=AUDIO_S32SYS;
else if(ac.sampleformat==3||ac.sampleformat==8)
desiredSpec.format=AUDIO_F32SYS;
else if(ac.sampleformat==4||ac.sampleformat==9)
desiredSpec.format=AUDIO_F32SYS;
desiredSpec.channels=ac.channel;
desiredSpec.samples=2048;
desiredSpec.callback=audio_callback;
desiredSpec.userdata=0;
SDL_AudioSpec obtainedSpec;
SDL_OpenAudio(&desiredSpec,&obtainedSpec);
ac.n=0;
ac.begptr=0;
if(readaframe(tweak=0)>=0)
SDL_PauseAudio(0);
else
audio=0;
}
bt=av_gettime();
tmr->start(((qint64)1000*(qint64)ac.framerateden)/((qint64)ac.frameratenum*3));
}else{
debug("aopen error")
}
}//filename
debug("~videowd::slotclicked()")
show();
return 0;
}
void videowd::slotsldrvaluechanged(int val){
//trace("videowd::slotclicked,sldr"<<val<<" "<<seekfile<<" "<<pause)
if(!pause && seekfile && seekfileframe(val)){
debug("val:"<<val)
video=ac.video;
audio=ac.audio;
framecount=((qint64)val*(qint64)ac.frameratenum)/(qint64)ac.framerateden;
if(audio){
SDL_PauseAudio(1);
ac.n=0;
ac.begptr=0;
if(readaframe(tweak=0)>=0)
SDL_PauseAudio(0);
else
audio=0;
}
bt=av_gettime()-(int64_t)val*1000000;
if(!tmr->isActive())
tmr->start(((qint64)1000*(qint64)ac.framerateden)/((qint64)ac.frameratenum*3));
}else if(pause)
ac.volume=val;
seekfile=true;
//trace("~videowd::slotsldrvaluechanged()")
}
void videowd::enterEvent(QEvent *e){
debug("videowd::enterEvent")
sldr->setVisible(true);
//fpb->setVisible(true);
lbl->setVisible(true);
QWidget::enterEvent(e);
debug("~videowd::enterEvent")
}
void videowd::leaveEvent(QEvent *e){
debug("videowd::leaveEvent")
sldr->setVisible(false);
//fpb->setVisible(false);
lbl->setVisible(false);
QWidget::leaveEvent(e);
debug("~videowd::leaveEvent")
}
void videowd::mousePressEvent(QMouseEvent *e){
static int64_t pausetime=0;
debug("videowd::mousePressEvent"<<pause<<"a"<<audio<<" "<<video)
pause=!pause;
if(pause){
if(audio)
SDL_PauseAudio(1);
tmr->stop();
if(sldr->isEnabled()){
seekfile=false;
sldr->setValue(ac.volume);
seekfile=false;
sldr->setMaximum(120);
}
pausetime=av_gettime();
}else{
if(sldr->isEnabled()){
seekfile=false;
sldr->setMaximum(ac.tt/1000000);
seekfile=false;
sldr->setValue(framecount/(ac.frameratenum/ac.framerateden));
}
if(audio)
SDL_PauseAudio(0);
bt=bt+av_gettime()-pausetime;
if(audio || video)
tmr->start(((qint64)1000*(qint64)ac.framerateden)/((qint64)ac.frameratenum*3));
}
//QWidget::mousePressEvent(e);
debug("~videowd::mousePressEvent")
}
void videowd::mouseDoubleClickEvent(QMouseEvent *e){
debug("videowd::mouseDoubleClickEvent")
static bool fullscreen=false;
if((fullscreen=!fullscreen)){
showFullScreen();
sldr->setVisible(false);
//fpb->setVisible(false);
lbl->setVisible(false);
}else
showNormal();
QWidget::mouseDoubleClickEvent(e);
}
void videowd::hideEvent(QHideEvent* e){
debug("videowd::hideEvent"<<pause)
if(!pause)
mousePressEvent(0);
QWidget::hideEvent(e);
}
QSize videowd::sizeHint()const{
return QSize(WINDOW_WIDTH,WINDOW_HEIGHT);
}

