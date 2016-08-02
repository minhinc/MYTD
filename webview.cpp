/*
 * webview.cpp
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
#include <QContextMenuEvent>
#include <QWebFrame>
#include <QWebSettings>
#include "widget.h"
#include "debug.h"
webview::webview(int i,widget* n):widget(i,n){
settings()->globalSettings()->setFontSize(QWebSettings::DefaultFontSize, 10);}
int webview::handle(){
debug("webview::handle"<<*(QString*)dt)
load(QUrl(*(QString*)dt));
show();
return 0;
}
QSize webview::sizeHint() const{
debug("/webview::sizeHint")
//return QSize(WINDOW_WIDTH*3/2,WINDOW_HEIGHT*3/2);
return QSize(WINDOW_WIDTH*2,WINDOW_HEIGHT*3/2);
//return QSize(200,100);
}
void webview::hideEvent(QHideEvent *e){
debug(".webview::hideevent")
stop();
debug("~webview::hideevent")
}
