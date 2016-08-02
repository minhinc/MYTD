/*
 * griddb.cpp
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
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QStyle>
#include <QLabel>
#include "widget.h"
#include "debug.h"
griddb::griddb(int idp,widget* n):widget(idp,n){
debug("griddb::gridgb")
int i=0;
QGridLayout* glt=new QGridLayout;
QHBoxLayout *hlt=new QHBoxLayout;
QVBoxLayout *vlt=new QVBoxLayout;
rbcache=new QRadioButton[TYPE_MAX];
hlt->addWidget((((QPushButton*)(dpb=new QPushButton))->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton)),(QPushButton*)dpb),0,Qt::AlignLeft);
hlt->addWidget((((QPushButton*)(cpb=new QPushButton))->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton)),(QPushButton*)cpb),1,Qt::AlignLeft);
for(i=0;i<TYPE_MAX;i++){
glt->addWidget((QRadioButton*)((QRadioButton*)rbcache+i),i%MW_ROW,i/MW_ROW);
((QRadioButton*)((QRadioButton*)rbcache+i))->setVisible(false);
}
vlt->addWidget((QLabel*)(lbl=new QLabel));
vlt->addLayout(glt);
vlt->addLayout(hlt);
setLayout(vlt);
connect((QPushButton*)dpb,SIGNAL(clicked()),this,SLOT(slotdpbclicked()));
connect((QPushButton*)cpb,SIGNAL(clicked()),this,SLOT(slotcpbclicked()));
debug("~griddb::gridgb")
}
int griddb::handle(){
debug("griddb::handle")
QStringList slist;
int i=0;
slist=((QString*)dt)->split("><",QString::SkipEmptyParts);
((QLabel*)lbl)->setText(slist.at(slist.size()-1));
for(i=0;i<slist.size()-1;i++){
((QRadioButton*)((QRadioButton*)rbcache+i))->setText(slist.at(i));
((QRadioButton*)((QRadioButton*)rbcache+i))->setVisible(true);
}
for(i=slist.size()-1;i<TYPE_MAX;i++)
((QRadioButton*)((QRadioButton*)rbcache+i))->setVisible(false);
((QRadioButton*)((QRadioButton*)rbcache))->setChecked(true);
show();
debug("~griddb::handle")
return exec();
}
void griddb::slotdpbclicked(){
debug("/griddb::slotdpbclicked()")
int i=0;
for(i=0;i<((QString*)dt)->split("><",QString::SkipEmptyParts).size()-1;i++)
if(((QRadioButton*)((QRadioButton*)rbcache+i))->isChecked()) break;
done(i);
}
void griddb::slotcpbclicked(){
debug("/griddb::slotcpbclicked()")
done(CANCEL);
}
griddb::~griddb(){
debug("/griddb::~griddb")
delete[] (QRadioButton*)rbcache;
}
