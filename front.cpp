/*
 * front.cpp
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
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include "front.h"
#include "fetcher.h"
#include "fpbttn.h"
#include "widget.h"
#include "debug.h"
front::front(){
QFile file;
debug("front::front")
setWindowTitle(tr("MYTD"));
setWindowIcon(QIcon(":/topicon.png"));
setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
dt=new QString;
cd=new QString(DOWNLOAD_DIR);
nam=new QNetworkAccessManager;
rear=new fetcher(dt,this);
widget::dt=dt;
//dbw=new labelgb(LABELGB, new griddb(GRIDDB, new videowd(VIDEOWD, new webview(WEBVIEW,0))));
dbw=new labelgb(LABELGB, new griddb(GRIDDB, new videowd(VIDEOWD,0)));
cgb=dbw;
QVBoxLayout *flt=new QVBoxLayout;
QHBoxLayout *hlt=new QHBoxLayout;
hlt->addWidget((((QLineEdit*)(fle=new QLineEdit))->setPlaceholderText("https://www.youtube.com/watch?v=dgYGRiVjQ4w"),(QLineEdit*)fle));
hlt->addWidget((((fpbttn*)(fpb=new fpbttn))->setIcon(style()->standardIcon(QStyle::SP_ArrowDown)),(fpbttn*)fpb));
flt->addLayout(hlt);
flt->addWidget(dynamic_cast<QWidget*>(cgb));
(dynamic_cast<QWidget*>(cgb))->hide();
QVBoxLayout *bvlt=new QVBoxLayout;
QHBoxLayout *bhlt=new QHBoxLayout;
bhlt->addWidget((((QPushButton*)(pcpb=new QPushButton))->setIcon(style()->standardIcon(QStyle::SP_BrowserStop)),(QPushButton*)pcpb));
bhlt->addWidget((((QPushButton*)(ppb=new QPushButton))->setIcon(style()->standardIcon(QStyle::SP_MediaStop)),(QPushButton*)ppb),1,Qt::AlignLeft);
((QPushButton*)pcpb)->setDisabled(true);
((QPushButton*)ppb)->setDisabled(true);
bhlt->addWidget((((QLabel*)(dlb=new QLabel))->setText(QString("<a href=\" \">")+DOWNLOAD_DIR+QString("</a>")),(QLabel*)dlb),0,Qt::AlignRight);
if(QFile::exists(DIR_FILE)){
file.setFileName(DIR_FILE);
file.open(QIODevice::ReadOnly);
*(QString*)cd=file.readAll();
((QLabel*)dlb)->setText(QString("<a href=\" \">")+(*(QString*)cd)+QString("</a>"));
file.close();
}
bvlt->addLayout(bhlt);
bvlt->addWidget((listwidget*)(lw=new listwidget));
((listwidget*)lw)->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
bvlt->addWidget((QLabel*)(alb=new QLabel("<a href=\" \" style=\"color:#004000;font-family:Tw Cen MT;font-size:8pt\">Minh Inc</a>")));
flt->addLayout(bvlt);
setLayout(flt);
flt->setSizeConstraint(QLayout::SetFixedSize);
connect((fpbttn*)fpb,SIGNAL(clicked()),this,SLOT(slotclicked()));
connect((QPushButton*)pcpb,SIGNAL(clicked()),this,SLOT(slotclicked()));
connect((QPushButton*)ppb,SIGNAL(clicked()),this,SLOT(slotclicked()));
connect((QLabel*)alb,SIGNAL(linkActivated(QString)),this,SLOT(slotclicked()));
connect((QLabel*)dlb,SIGNAL(linkActivated(QString)),this,SLOT(slotclicked()));
connect((listwidget*)lw,SIGNAL(currentRowChanged(int)),this,SLOT(slotlwcurrentrowchanged(int)));
debug("~front::front")
}
front::~front() {
debug("front::~front")
widget* wd=dbw;
widget* twd;
delete (QString*)cd;
while(wd!=0){
if(wd==cgb)
wd=wd->next;
else{
twd=wd;
wd=wd->next;
delete twd;
}
}
delete (fetcher*)rear;
delete (QNetworkAccessManager*)nam;
delete (QString*)dt;
debug("~front::~front")
}
void front::slotclicked(){
debug("front::slotclicked")
if(((fpbttn*)sender())==fpb){
debug("front::slotclicked, fpb")
if(!((fpbttn*)fpb)->old){
paintold();
((fetcher*)rear)->handle(PAUSE);
bwdtodb(LABELGB,&(*(QString*)dt=QString("%1><%2").arg("Info").arg("<p style=\"color:'#ff0000'\">Fetch cancelled</p>")));
}else
if((li=((listwidget*)lw)->count())==DEV_MAX){
paintold(true,false);
bwdtodb(LABELGB,&(*(QString*)dt=QString("%1><%2").arg("Info").arg("<p style=\"color:'#ff0000'\">MAX COUNT REACHED</p>")));
}else{
paintold(false,true);
bwdtodb(LABELGB,&(*(QString*)dt=QString("%1><%2").arg("Info").arg("Fetching...")));
if(!((fetcher*)rear)->handle(FETCH,&(*(QString*)dt=((QLineEdit*)fle)->text()))){
paintold();
bwdtodb(LABELGB,&(*(QString*)dt=QString("%1><%2").arg("Info").arg("<p style=\"color:#ff0000\">FILE FORMAT NOT SUPPORTED</p>")));
}
}
}else
if((QPushButton*)sender()==pcpb){
debug("front::slotclicked, pcpb")
if(((fetcher*)rear)->sl[li=((listwidget*)lw)->currentRow()].state==PROGRESS){
((fetcher*)rear)->sl[li].state=PAUSE;
if(((fetcher*)rear)->handle(PAUSE))
updatelist(li,3,&(*(QString*)dt="pause"));
else
((fetcher*)rear)->sl[li].state=COMPLETE;
}else
if(((fetcher*)rear)->handle(GET,&(*(QString*)dt=*(QString*)((fetcher*)rear)->sl[li].url+"><"+*(QString*)((fetcher*)rear)->sl[li].fnm))){
((fetcher*)rear)->sl[li].state=PROGRESS;
updatelist(li,3,&(*(QString*)dt="progress"));
}
slotlwcurrentrowchanged(li);
}else
if((QPushButton*)sender()==ppb){
debug("front::slotclicked, ppb")
//*(QString*)dt=QString("%1><%2").arg(((listwidget*)lw)->currentRow()).arg(*(QString*)((fetcher*)rear)->sl[((listwidget*)lw)->currentRow()].fnm);
*(QString*)dt=*(QString*)((fetcher*)rear)->sl[((listwidget*)lw)->currentRow()].fnm;
bwdtodb(VIDEOWD);
}else
if((QLabel*)sender()==dlb){
debug("front::slotclicked, dlb")
QString str;
QFile file(DIR_FILE);
str=QFileDialog::getExistingDirectory(this, tr("Open Directory"),*(QString*)cd,QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
if(!str.isEmpty()){
((QLabel*)dlb)->setText(QString("<a href=\" \">")+str+QString("</a>"));
*(QString*)cd=str;
file.open(QIODevice::WriteOnly);
file.write(str.toUtf8());
file.close();
}
}else
if((QLabel*)sender()==alb){
debug("front::slotclicked, alb")
bwdtodb(LABELGB,&(*(QString*)dt=QString("%1><%2").arg("About").arg("Minh YouTube Downloader.<br>Developed at Minh Inc.<br>www.minhinc.com develop@minhinc.com<br>Based on Qt5 APIs")));
}
debug("~front::slotclicked")
}
void front::updatelist(int row,int col,void*){
QString str(FILENAME_TAB+ITAG_TAB+SIZE_TAB+STATUS_TAB,' ');
debug("front::updatelist")
if(((listwidget*)lw)->item(row)==NULL && col==0){
((listwidget*)lw)->addItem(new QListWidgetItem(str.replace(0,FILENAME_TAB,((QString*)dt)->leftJustified(FILENAME_TAB,' ',true))));
((listwidget*)lw)->setCurrentRow(((listwidget*)lw)->count()-1);
}else
if(((listwidget*)lw)->item(row)!=NULL){
str=((listwidget*)lw)->item(row)->text();
if(col==1)
str.replace(FILENAME_TAB,ITAG_TAB,((QString*)dt)->rightJustified(ITAG_TAB,' ',true));
else
if(col==2)
str.replace(FILENAME_TAB+ITAG_TAB,SIZE_TAB,((QString*)dt)->rightJustified(SIZE_TAB,' ',true));
else
str.replace(FILENAME_TAB+ITAG_TAB+SIZE_TAB,STATUS_TAB,((QString*)dt)->rightJustified(STATUS_TAB,' ',true));
((listwidget*)lw)->item(row)->setText(str);
slotlwcurrentrowchanged(row);
}
debug("~front::updstelist")
}
void front::slotlwcurrentrowchanged(int row){
debug("front::slotcurrentrowchanged, row, state, old"<<row<<" "<<((fetcher*)rear)->sl[row].state<<" "<<((fpbttn*)fpb)->old)
((QPushButton*)pcpb)->setDisabled(true);
((QPushButton*)ppb)->setDisabled(true);
((QLabel*)dlb)->setDisabled(true);
((QLabel*)alb)->setDisabled(true);
if(((fetcher*)rear)->sl[row].state==PROGRESS){
((QPushButton*)pcpb)->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
if(((fpbttn*)fpb)->old)((QPushButton*)pcpb)->setDisabled(false);
}else
if(((fetcher*)rear)->sl[row].state==PAUSE && ((fetcher*)rear)->sl[row].size){
((QPushButton*)pcpb)->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
if(((fpbttn*)fpb)->old)((QPushButton*)pcpb)->setDisabled(false);
}else
((QPushButton*)pcpb)->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));

if(QFile::exists(*(QString*)((fetcher*)rear)->sl[row].fnm) && *(QString*)((fetcher*)rear)->sl[row].fnm != TMP_FILE){
//if(QFile::exists(*(QString*)((fetcher*)rear)->sl[row].fnm) && (((fetcher*)rear)->sl[row].state==COMPLETE) && *(QString*)((fetcher*)rear)->sl[row].fnm != TMP_FILE){
((QPushButton*)ppb)->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
if(((fpbttn*)fpb)->old)((QPushButton*)ppb)->setDisabled(false);
}
if(((fpbttn*)fpb)->old){
((QLabel*)dlb)->setDisabled(false);
((QLabel*)alb)->setDisabled(false);
}
debug("~front::slotcurrentrowchanged")
}
int front::bwdtodb(int ids,void*) {
debug("front::bwdtodb")
widget* nw=dbw;
while(nw!=0)
if(nw->id==ids)
break;
else
nw=nw->next;
(dynamic_cast<QWidget*>(cgb))->hide();
if(cgb!=nw){
((QVBoxLayout*)layout())->removeWidget(dynamic_cast<QWidget*>(cgb));
((QVBoxLayout*)layout())->insertWidget(1,dynamic_cast<QWidget*>(nw));
}
cgb=nw;
debug("~front::bwdtodb")
return nw->handle();
}
void front::paintold(bool option,bool enable, bool hidecgb){
debug("front::paintold")
((fpbttn*)fpb)->paintold(option,enable);
if(option){
((QLabel*)dlb)->setDisabled(false);
((QLabel*)alb)->setDisabled(false);
}else{
((QLabel*)dlb)->setDisabled(true);
((QLabel*)alb)->setDisabled(true);
}
if(hidecgb)
(dynamic_cast<QWidget*>(cgb))->hide();
debug("~front::paintold")
}
