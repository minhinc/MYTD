/*
 * fetcher.cpp
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
/*
              <<draws main widget>>   ----------------------------------
                         ---------    |---------   --------   ---------|
                         | front | -->||labelwd|-->|gridwd|-->|videowd||
                         ---------    |---------   --------   ---------|
                            ^ |       ----------------------------------
                            | |               <--- widget chain ----->
---------------------       | v
|         ----------|    --------- 
|. . . <--|ytdevice||<-- | rear  |
|         ----------|    ---------
---------------------    <<makes http request>>
<-- device chain -->
*/
#include <QFile>
#include <QTimer>
#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "front.h"
#include "fetcher.h"
#include "ytdevice.h"
#include "urldevice.h"
#include "debug.h"
fetcher::fetcher(void* dtp,void* fnt):dt(dtp),frnt(fnt){
int i=0;
debug("fetcher::fetcher")
timer=new QTimer;
device::dt=dt;
//cdv=dv=new ytdevice(new urldevice(0));
cdv=dv=new ytdevice(0);
((device*)dv)->rear=this;
for(i=0;i<TYPE_MAX;i++){
tn[i]=0;
hr[i]=new QString;
}
connect((QTimer*)timer,SIGNAL(timeout()),this,SLOT(slottimer()));
connect((QNetworkAccessManager*)((front*)frnt)->nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(slotcomplete(QNetworkReply*)));
debug("~fetcher::fetcher")
}
get::get(){
debug("get::get")
fnm=new QString;
url=new QString;
reply=0;
size=0;
state=FREE;
debug("~get::get")
}
get::~get(){
debug("get::~get")
delete (QString*)url;
delete (QString*)fnm;
if(reply!=0)reply->abort();
debug("~get::~get")
}
fetcher::~fetcher(){
int i=0;
debug("fetcher::~fethcer")
slottimer();
for(i=0;i<TYPE_MAX;i++){
if(tn[i]) tn[i]->abort();
tn[i]=0;
delete (QString*)hr[i];
}
delete (QTimer*)timer;
disconnect((QNetworkAccessManager*)(((front*)frnt)->nam),SIGNAL(finished(QNetworkReply*)),this,SLOT(slotcomplete(QNetworkReply*)));
device* tdv=(device*)dv;
while(dv!=0){
dv=((device*)dv)->next;
debug("deleted one")
delete tdv;
tdv=(device*)dv;
}
debug("~fetcher::~fetcher")
}
void* fetcher::handle(int m,void*){
int i=0;
QNetworkRequest rq;
debug("fetcher::handler"<<" "<<m<<" "<<((front*)frnt)->li)
rq.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute,true);
if(m==HEAD)
for(i=0;i<((QString*)dt)->split("><",QString::SkipEmptyParts).size();i++){
tn[i]=((QNetworkAccessManager*)((front*)frnt)->nam)->head((rq.setUrl(((QString*)dt)->split("><",QString::SkipEmptyParts).at(i)),rq));
*(QString*)hr[i]=((QString*)dt)->split("><",QString::SkipEmptyParts).at(i);
}
else
if(m==GET){
rq.setUrl(*(QString*)sl[((front*)frnt)->li].url=((QString*)dt)->split("><",QString::SkipEmptyParts).at(0));
QFile file(*(QString*)sl[((front*)frnt)->li].fnm=((QString*)dt)->split("><",QString::SkipEmptyParts).at(1));
debug("fetcher::handle get ,url"<<*(QString*)sl[((front*)frnt)->li].url)
if(file.exists())
rq.setRawHeader("Range","bytes="+QByteArray::number(file.size())+"-");
if(!(sl[((front*)frnt)->li].reply=((QNetworkAccessManager*)((front*)frnt)->nam)->get(rq))){
debug("fetcher::handle get error,url"<<*(QString*)sl[((front*)frnt)->li].url)
return 0;
}
if(!((QTimer*)timer)->isActive())
((QTimer*)timer)->start(FILE_FETCH_DURATION);
}else
if(m==PAUSE){
slottimer();
if(sl[((front*)frnt)->li].reply)
sl[((front*)frnt)->li].reply->abort();
for(i=0;i<TYPE_MAX;i++)
if(tn[i]!=0)
tn[i]->abort();
}else
if(m==FETCH)
if((cdv=((device*)dv)->handle(CHECK)))
((device*)cdv)->handle(FETCH);
else
return 0;
debug("~fetcher::handle")
return this;
}
void fetcher::slotcomplete(QNetworkReply *reply){
int i=0;
QNetworkRequest rq;
int size=0;
QString data;
static int sz[TYPE_MAX];
debug("fetcher::slotcomplete")
for(i=0;i<((QString*)dt)->split("><",QString::SkipEmptyParts).size();i++)
if(reply==tn[i])break;
if(i!=((QString*)dt)->split("><",QString::SkipEmptyParts).size()){
QVariant attribute=reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
if((!reply->error()) && attribute.isValid()){
tn[i]=((QNetworkAccessManager*)((front*)frnt)->nam)->head((rq.setUrl(QUrl(*(QString*)hr[i]).resolved(attribute.toUrl())),rq));
debug("head request is redirectable"<<i<<"new url"<<rq.url().toString())
return;
}
data=*(QString*)dt;
((front*)frnt)->bwdtodb(LABELGB,&(*(QString*)dt=QString("%1><header ack recieved for %2/%3").arg("Info").arg(i+1).arg(((QString*)dt)->split("><",QString::SkipEmptyParts).size())));
*(QString*)dt=data;
sz[i]=0;
//if(!tn[i]->error() && tn[i]->hasRawHeader("Accept-Ranges") && tn[i]->hasRawHeader("Content-Length"))
if(!tn[i]->error() && tn[i]->hasRawHeader("Content-Length"))
sz[i]=reply->rawHeader("Content-Length").toInt();
tn[i]=0;
for(i=0;i<((QString*)dt)->split("><",QString::SkipEmptyParts).size();i++)
if(tn[i]!=0) break;
if(i==((QString*)dt)->split("><",QString::SkipEmptyParts).size()){
size=i;
((QString*)dt)->resize(0);
for(i=0;i<size;i++)
*(QString*)dt+=QString("%1><").arg(sz[i]);
((device*)cdv)->handle(HEAD);
}
}else{
for(i=0;i<DEV_MAX;i++)
if(sl[i].reply==reply)break;
if(i==DEV_MAX)
return;
QVariant attribute=reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
if((!reply->error()) && attribute.isValid()){
handle(GET,&(*(QString*)dt=(*(QString*)sl[i].url=QUrl(*(QString*)sl[i].url).resolved(attribute.toUrl()).toString())+"><"+*(QString*)sl[i].fnm));
debug("fetcher::slotcomplete redirection, url"<<*(QString*)sl[i].url)
}else{
if(sl[i].state!=PAUSE)
sl[i].state=COMPLETE;
if(reply->error()){
if(sl[i].state==COMPLETE){
((front*)frnt)->updatelist(i,3,&(*(QString*)dt=QString("%1.%2").arg("error").arg(reply->error())));
((front*)frnt)->bwdtodb(LABELGB,&(*(QString*)dt=QString("%1><error#%2").arg("Error").arg(reply->error())));
((front*)frnt)->paintold();
}
}else{
slottimer();
((front*)frnt)->updatelist(i,3,&(*(QString*)dt="finished"));
//((device*)cdv)->handle(GET);
}
sl[i].reply=0;
for(i=0;i<DEV_MAX;i++)
if(sl[i].reply!=0)
break;
if(i==DEV_MAX)
((QTimer*)timer)->stop();
if(!reply->error())
((device*)cdv)->handle(GET);
}
}
debug("~fetcher::slotcomplete")
}

void fetcher::slottimer(){
int i=0;
QString data=*(QString*)dt;
debug("fetcher::slottimer")
for(i=0;i<DEV_MAX;i++)
if(sl[i].reply && sl[i].reply->bytesAvailable()){
QFile file(*(QString*)sl[i].fnm);
if(!file.open(QIODevice::WriteOnly|QIODevice::Append)){
debug("fetcher::slottimer file could not be opened"<<i<<" "<<*(QString*)sl[i].fnm)
return;
}
debug("fetcher::slottimer i,fnm"<<i<<" "<<*(QString*)sl[i].fnm)
file.write(sl[i].reply->readAll());
if(sl[i].size)
((front*)frnt)->updatelist(i,2,&(*(QString*)dt=QString("%1%2").arg(((qint64)(100*file.size()))/sl[i].size).arg('%')));
else
((front*)frnt)->updatelist(i,2,&(*(QString*)dt=*(QString*)device::convert(file.size())));
((front*)frnt)->updatelist(i,3,&(*(QString*)dt="progress"));
file.close();
}
*(QString*)dt=data;
debug("~fetcher::slottimer")
}
