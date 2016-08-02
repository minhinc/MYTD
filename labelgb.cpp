/*
 * labelgb.cpp
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
#include <QVBoxLayout>
#include <QLabel>
#include "widget.h"
#include "debug.h"

labelgb::labelgb(int idp,widget* n):widget(idp,n){
debug("labelgb::labelgb")
QVBoxLayout* vlt=new QVBoxLayout;
vlt->addWidget((QLabel*)(lbl=new QLabel));
setLayout(vlt);
debug("~labelgb::labelgb")
}
int labelgb::handle(){
debug("labelgb::handle")
setTitle(((QString*)dt)->split("><",QString::SkipEmptyParts).at(0));
((QLabel*)lbl)->setText(((QString*)dt)->split("><",QString::SkipEmptyParts).at(1));
show();
debug("~labelgb::handle")
return 0;
}
