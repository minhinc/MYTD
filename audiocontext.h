/*
 * audiocontext.h
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
#ifndef AUDIOCONTEXT_H
#define AUDIOCONTEXT_H
#define GFDAUDIOBUFFERSIZE 1024*4*2*32
struct audiocontext{
int vwidth;
int vheight;
int freq;
int channel;
int sampleformat;
int sampleformatsize;
int64_t tt;
int frameratenum;
int framerateden;
int volume;
char filename[1024];
uint8_t *vptr;
uint8_t aptr[GFDAUDIOBUFFERSIZE*2+1024];
int begptr;
int n;
int video;
int audio;
int64_t videopts;
int64_t audiopts;
int64_t videostreampts;
int64_t audiostreampts;
};
#endif
