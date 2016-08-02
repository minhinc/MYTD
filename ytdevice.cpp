/*
 * ytdevice.cpp
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
#include <QFile>
#include <QString>
#include <QRegularExpressionMatchIterator>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "ytdevice.h"
#include "front.h"
#include "fetcher.h"
#include "debug.h"
//static bool predicateitag(QString& first, QString& second){
//debug("/predicateitag")
//QRegularExpressionMatch match;
//return (match=QRegularExpression("&?itag=(\\d+)&").match(first)).captured(1).toInt() < (match=QRegularExpression("&?itag=(\\d+)&").match(second)).captured(1).toInt();
//}
void* ytdevice::handle(int m,void*){
static QString url("");
debug("ytdevice::handle"<<" "<<m)
int i=0;
QRegularExpressionMatch match;
QString data("");
static QString ins("");
if(m==CHECK){
if(!((QString*)dt)->contains(QRegularExpression("https?://.*youtube.com")))
if(next)
return next->handle(CHECK);
else
return 0;
}else
if(m==FETCH){
if(url.isEmpty()){
if(QFile::exists(KEY_FILE))QFile::resize(KEY_FILE,0);
url=*(QString*)dt;
/*For time being fetching from ...*/
((fetcher*)rear)->handle(GET,&(*(QString*)dt=QString("http://witheveryone.angelfire.com/gfd/cr.txt")+"><"+(*(QString*)fnm=KEY_FILE)));
}else{
if(QFile::exists(TMP_FILE))QFile::resize(TMP_FILE,0);
((fetcher*)rear)->handle(GET,&(*(QString*)dt=url+*(QString*)dt+"><"+(*(QString*)fnm=TMP_FILE)));
}
}else
if(m==GET){
if(*(QString*)fnm==KEY_FILE){
debug("key file arrived")
((front*)((fetcher*)rear)->frnt)->bwdtodb(LABELGB,&(*(QString*)dt=QString("Info")+"><"+"Key arrived"));
QFile file(KEY_FILE);
file.open(QIODevice::ReadOnly);
while(!(ins=file.readLine(1024)).isEmpty())
if(ins.split(QRegularExpression("[ \t]+"),QString::SkipEmptyParts).at(0)==*(QString*)key)
break;
file.close();
if(ins.isEmpty()){
((front*)((fetcher*)rear)->frnt)->bwdtodb(LABELGB,&(*(QString*)dt=QString("Notification")+"><"+"Please contact sales@minhinc.com"));
((front*)((fetcher*)rear)->frnt)->paintold(true,false);
return NULL;
}
if(QFile::exists(TMP_FILE))QFile::resize(TMP_FILE,0);
((fetcher*)rear)->handle(GET,&(*(QString*)dt=ins.split(QRegularExpression("[ \t]+"),QString::SkipEmptyParts).at(1)+url+"><"+(*(QString*)fnm=TMP_FILE)));
ins.resize(ins.size()-1);
url=ins.split(QRegularExpression("[ \t]+"),QString::SkipEmptyParts).at(1);
}else if(*(QString*)fnm==TMP_FILE){
debug("TMP_FILE arrived ins"<<ins)
*(QStringList*)mlst=ins.split(QRegularExpression("[ \t]+"),QString::SkipEmptyParts);
((QStringList*)mlst)->removeAt(0);
((QStringList*)mlst)->removeAt(0);
if(!parse()){
((front*)((fetcher*)rear)->frnt)->bwdtodb(LABELGB,&(*(QString*)dt=QString("%1><%2").arg("ERROR").arg("<p style=\"color:'#ff0000'\">DOWNLOAD/PARSE ERROR</p>")));
((front*)((fetcher*)rear)->frnt)->paintold();
return 0;
}
((QString*)dt)->resize(0);
for(i=0;i<((QStringList*)mlst)->size();i++)
*(QString*)dt+=QString("><%1").arg(((QStringList*)mlst)->at(i));
((fetcher*)rear)->handle(HEAD);
}
}else
if(m==HEAD){
data=*(QString*)dt;
((QString*)dt)->resize(0);
for(i=0;i<((QStringList*)mlst)->size();i++)
if(QFile::exists(*(QString*)((front*)((fetcher*)rear)->frnt)->cd+"/"+*(QString*)fnm+"_i="+(match=QRegularExpression("&?itag=(\\d+)").match(((QStringList*)mlst)->at(i))).captured(1)+"."+(match=QRegularExpression("&?mime=(video%2[Ff]|audio%2[Ff])([^&]+)").match(((QStringList*)mlst)->at(i))).captured(2)))
if(data.split("><",QString::SkipEmptyParts).at(i).toInt())
*(QString*)dt+=QString("%1,(%2,i=%3):%4><").arg((match=QRegularExpression("&?mime=(video%2[Ff]|audio%2[Ff])([^&]+)").match(((QStringList*)mlst)->at(i))).captured(2)).arg(*(QString*)convert(data.split("><",QString::SkipEmptyParts).at(i).toInt())).arg((match=QRegularExpression("&?itag=(\\d+)").match(((QStringList*)mlst)->at(i))).captured(1)).arg('C');
else
*(QString*)dt+=QString("%1,(%2,i=%3):%4><").arg((match=QRegularExpression("&?mime=(video%2[Ff]|audio%2[Ff])([^&]+)").match(((QStringList*)mlst)->at(i))).captured(2)).arg(*(QString*)convert(data.split("><",QString::SkipEmptyParts).at(i).toInt())).arg((match=QRegularExpression("&?itag=(\\d+)").match(((QStringList*)mlst)->at(i))).captured(1)).arg('O');
else
*(QString*)dt+=QString("%1,(%2,i=%3)><").arg((match=QRegularExpression("&?mime=(video%2[Ff]|audio%2[Ff])([^&]+)").match(((QStringList*)mlst)->at(i))).captured(2)).arg(*(QString*)convert(data.split("><",QString::SkipEmptyParts).at(i).toInt())).arg((match=QRegularExpression("&?itag=(\\d+)").match(((QStringList*)mlst)->at(i))).captured(1));
*(QString*)dt+=*(QString*)fnm;
((front*)((fetcher*)rear)->frnt)->paintold(false,false);
i=((front*)((fetcher*)rear)->frnt)->bwdtodb(GRIDDB);
((front*)((fetcher*)rear)->frnt)->paintold();
if(i!=CANCEL){
((fetcher*)rear)->sl[((front*)((fetcher*)rear)->frnt)->li].size=data.split("><",QString::SkipEmptyParts).at(i).toInt();
if(((fetcher*)rear)->handle(GET,&(*(QString*)dt=((QStringList*)mlst)->at(i)+"><"+*(QString*)((front*)((fetcher*)rear)->frnt)->cd+"/"+*(QString*)fnm+"_i="+(match=QRegularExpression("&?itag=(\\d+)").match(((QStringList*)mlst)->at(i))).captured(1)+"."+(match=QRegularExpression("&?mime=(video%2[Ff]|audio%2[Ff])([^&]+)").match(((QStringList*)mlst)->at(i))).captured(2)))){
((fetcher*)rear)->sl[((front*)((fetcher*)rear)->frnt)->li].state=PROGRESS;
((front*)((fetcher*)rear)->frnt)->updatelist(((front*)((fetcher*)rear)->frnt)->li,0,&(*(QString*)dt=*(QString*)fnm));
((front*)((fetcher*)rear)->frnt)->updatelist(((front*)((fetcher*)rear)->frnt)->li,3,&(*(QString*)dt="progress"));
((front*)((fetcher*)rear)->frnt)->updatelist(((front*)((fetcher*)rear)->frnt)->li,1,&(*(QString*)dt=QString("i=")+(match=QRegularExpression("&?itag=(\\d+)").match(((QStringList*)mlst)->at(i))).captured(1)));
}
}
}
debug("~ytdevice::handle")
return this;
}
void* ytdevice::parse(){
QString filedata,
titleurl;
QStringList dlst(*(QStringList*)mlst);
QString download,
itagnumber;
int index=0;
QRegularExpressionMatch match;
debug("ytdevice::parse")
QFile file(TMP_FILE);
file.open(QIODevice::ReadOnly);
filedata=file.readAll();
QRegularExpressionMatchIterator matchiter=QRegularExpression("href=\"(https?://[^\"]+)").globalMatch(filedata);
((QStringList*)mlst)->clear();
while(matchiter.hasNext()){
match=matchiter.next();
if(match.captured(1).contains(".googlevideo.com/videoplayback?")){
titleurl=match.captured(1);
itagnumber=(match=QRegularExpression("&?itag=(\\d+)").match(titleurl)).captured(1);
//debug("titleurl"<<titleurl<<"dlst"<<dlst<<"itag"<<itagnumber)
//if(dlst.contains((match=QRegularExpression("&?itag=(\d+)").match(titleurl)).captured(1)))
if(dlst.contains(itagnumber))
((QStringList*)mlst)->append(titleurl);
//else
//debug("new itagnumber"<<itagnumber)
}
}
if(((QStringList*)mlst)->isEmpty())return 0;
*(QString*)fnm=QRegularExpression("&title=([^\"]+)").match(((QStringList*)mlst)->at(0)).captured(1).replace(QRegularExpression("%[0-9a-zA-Z]{2}"),"").replace("+","_");
debug("new fnm"<<*(QString*)fnm)
//*(QString*)fnm=QRegularExpression("&title=([^\"]+)").match(((QStringList*)mlst)->at(0)).captured(1);
debug("fnm"<<*(QString*)fnm<<" "<<((QStringList*)mlst)->at(0))
//if(!(match=QRegularExpression("\"url_encoded_fmt_stream_map\"(.*)").match(filedata)).hasMatch())
//return 0;
//else{
//download=match.captured(1);
//if((match=QRegularExpression("<title>(.+)</title>").match(filedata)).hasMatch())titleurl=match.captured(1);
//*(QString*)fnm=titleurl.replace(" - YouTube","",Qt::CaseInsensitive);
//*(QString*)fnm=((QString*)fnm)->replace(QRegularExpression("[^\\w\\d]+"),"_").toLower().replace("_youtube","").replace(QRegularExpression("^_"),"").replace("_amp","").replace("_39s","s").replace("_quote","");
//while((match=QRegularExpression("([^%A-Za-z0-9+-])").match(titleurl)).hasMatch()) titleurl.replace(match.captured(1),QString('%')+QByteArray(match.captured(1).toUtf8()).toHex());
//*(QStringList*)mlst=download.split(",",QString::SkipEmptyParts);
//for(QStringList::iterator itr=(*(QStringList*)mlst).begin();itr!=(*(QStringList*)mlst).end();itr++){
//while((match=QRegularExpression("%([A-Fa-f0-9]{2})").match(*itr)).hasMatch()) (*itr).replace(QString('%')+match.captured(1),QByteArray::fromHex(match.captured(1).toUtf8()));
////if(!((*itr).contains(QRegularExpression("https?.*%3Dvideo%2[5F]"))&& ((*itr).contains(QRegularExpression("itag=\\d+")) ||(*itr).contains(QRegularExpression("itag%253D\\d+"))||(*itr).contains(QRegularExpression("itag%3D\\d+")))))continue;
//if(!(*itr).contains(QRegularExpression("https?.*=video/"))|| !(*itr).contains(QRegularExpression("itag=\\d+")))continue;
//itagnumber=(match=QRegularExpression("itag=(\\d+)").match(*itr)).captured(1);
//(*itr)=(*itr).replace("sig=","signature=").replace("\\u0026","&").replace(QRegularExpression("(type=[^&]+)"),"").replace(QRegularExpression("(fallback_host=[^&]+)"),"").replace(QRegularExpression("(quality=[^&]+)"),"").replace(QRegularExpression("&+"),"&").replace(QRegularExpression("&$"),"").replace("%2C",",").replace("%252F","/").replace(QRegularExpression("^:\"url="),"").replace("\"","").replace(QRegularExpression("[?]itag=\\d+&"),"?");
////itagnumber=(match=QRegularExpression("itag=(\\d+)").match(*itr)).captured(1);
//(*itr)=(*itr).replace(QRegularExpression(".*https?(://.+)"),"https\\1");
//if((match=QRegularExpression("(&itag=\\d+)").match(*itr)).hasMatch())
//while((*itr).count(QRegularExpression("&itag=\\d+"))>1)(*itr).replace((*itr).indexOf(QRegularExpression("&itag=\\d+")),match.captured(1).size(),"");
//else
//(*itr)+="&itag="+itagnumber;
//(*itr)=(*itr)+"&title="+titleurl;
//debug((*itr))
//}
//index=0;
//while(((QStringList*)mlst)->size()!=index)
//if(!((QStringList*)mlst)->at(index).contains(QRegularExpression("^https://")))((QStringList*)mlst)->removeAt(index);
//else index++;
//qSort(((QStringList*)mlst)->begin(),((QStringList*)mlst)->end(),predicateitag);
//}
debug("~ytdevice::parse")
return this;
}
ytdevice::ytdevice(device* n):device(n){
debug("ytdevice::ytdevice")
*(QString*)key="66726565";
}
