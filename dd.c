/*
 * dd.c
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
 * Generic Media Player
 * Copyright (c) 2015-1016 Minh Inc
 *
 * This file is part of Mgmp.
 *
 * Mgmp is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Mgmp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Mgmp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <sys/stat.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "audiocontext.h"
/*fmt_ctx for video and afmt_ctx for audio*/
static AVFormatContext *fmt_ctx = NULL;
static AVFormatContext *afmt_ctx=NULL;
static AVCodecContext *video_dec_ctx = NULL, *audio_dec_ctx=NULL;
/*user defined*/
char errorstr[512];
#ifndef NNODEBUG
#define traceo(arg) fprintf(stdout,arg);
#define tracee(arg) fprintf(stderr,arg);
#else
#define traceo(arg);
#define tracee(arg);
#endif
static AVPicture m_Rgb;
static struct SwsContext *swsContext;
struct audiocontext ac;
int nopen();
int ninit();
int readframe();
//int readaframe(int,int64_t*);
int readaframe(int);
void nclose();
void ndeinit();
int seekfileframe(int64_t val);
void newwidthheight(int,int);

static enum AVPixelFormat pix_fmt;
static uint8_t *video_dst_data[4] = {NULL};
static int video_dst_linesize[4];
static int video_dst_bufsize;
static AVStream *video_stream=NULL,*audio_stream=NULL;
static int video_stream_idx = -1, audio_stream_idx = -1;
static AVFrame *frame = NULL;
static AVPacket pkt;
static AVPacket pktA;
int ninit(){
traceo("ninit()\n")
int ret=0;
int i,j;
int k;
ac.vwidth=200;
ac.vheight=50;
ac.vptr=calloc(ac.vwidth*ac.vheight,4);
for(i=0;i<ac.vheight;i++)
for(j=0;j<ac.vwidth;j++)
*((uint32_t*)ac.vptr+i*ac.vwidth+j)=0xff000000;
//j=k=0;
//for(i=ac.vheight*0.65;i>ac.vheight*0.20;i--){
//*((uint32_t*)ac.vptr+i*ac.vwidth+(int)(ac.vwidth*0.32)+j)=0xff004000;
//j=++k/2.8;
//}
//for(i=ac.vheight*0.20;i<ac.vheight*0.65;i++){
//*((uint32_t*)ac.vptr+i*ac.vwidth+(int)(ac.vwidth*0.32)+j)=0xff004000;
//j=++k/2.8;
//}
//for(i=ac.vheight*0.65;i>ac.vheight*0.20;i--){
//*((uint32_t*)ac.vptr+i*ac.vwidth+(int)(ac.vwidth*0.32)+j)=0xff004000;
//j=++k/2.8;
//}
//for(i=ac.vheight*0.20;i<ac.vheight*0.65;i++){
//*((uint32_t*)ac.vptr+i*ac.vwidth+(int)(ac.vwidth*0.32)+j)=0xff004000;
//j=++k/2.8;
//}
frame=av_frame_alloc();
if(!frame){
tracee("Could not allocate frame\n");
ret=AVERROR(ENOMEM);
av_frame_free(&frame);
}
traceo("~ninit()\n")
return ret;
}
void ndeinit(){
traceo("ndeinit()\n")
free(ac.vptr);
av_frame_free(&frame);
traceo("~ndeinit()\n")
}
static int decode_packet_video(int *got_frame){
int ret=0;
*got_frame=0;
//traceo("decode_packet_video()\n");
if((ret=avcodec_decode_video2(video_dec_ctx,frame,got_frame,&pkt))<0)
tracee("error:decode_packet_video():avcodec_decode_video2\n")
else if(*got_frame){
//printf("video frame pts:%d\n",(int)frame->pkt_pts);
sws_scale(swsContext,frame->data,frame->linesize,0,ac.vheight,m_Rgb.data,m_Rgb.linesize);
}
//sprintf(errorstr,"~decode_packet_video(),got_frame:%d\n",*got_frame);
//traceo(errorstr);
return ret;
}
static int decode_packet_audio(int *got_frame){
int ret=0;
int decoded=pktA.size;
*got_frame=0;
//traceo("decode_packet_audio()\n");
if((decoded=ret=avcodec_decode_audio4(audio_dec_ctx,frame,got_frame,&pktA))<0)
tracee("deocde_packet_(),avcodec_decode_audio4()\n")
else
decoded=FFMIN(ret,pktA.size);
//traceo("~decoded_packet_audio()\n");
//printf("audio frame pts:%d\n",(int)frame->pkt_pts);
return decoded;
}
static int open_codec_context(int *stream_idx,AVFormatContext *fmt_ctx, enum AVMediaType type){
int ret, stream_index;
AVStream *st;
AVCodecContext *dec_ctx = NULL;
AVCodec *dec = NULL;
AVDictionary *opts = NULL;
traceo("open_codec_context()\n")
ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
if (ret < 0) {
sprintf(errorstr, "Could not find %s stream in input file '%s'\n",av_get_media_type_string(type), ac.filename);
tracee(errorstr)
return ret;
} else {
stream_index = ret;
st = fmt_ctx->streams[stream_index];
/* find decoder for the stream */
dec_ctx = st->codec;
dec = avcodec_find_decoder(dec_ctx->codec_id);
if (!dec) {
sprintf(errorstr, "Failed to find %s codec\n",av_get_media_type_string(type));
tracee(errorstr)
return AVERROR(EINVAL);
}
/* Init the decoders, with or without reference counting */
av_dict_set(&opts, "refcounted_frames", "0", 0);
if ((ret = avcodec_open2(dec_ctx, dec, &opts)) < 0) {
sprintf(errorstr, "Failed to open %s codec\n",av_get_media_type_string(type));
tracee(errorstr)
return ret;
}
*stream_idx = stream_index;
}
sprintf(errorstr,"~open_codec_context ret:%d\n",ret);
traceo(errorstr)
return 0;
}
void nclose(){
traceo("nclose()\n")
//avcodec_close(video_dec_ctx);
//avcodec_close(audio_dec_ctx);
traceo("here")
avformat_close_input(&fmt_ctx);
avformat_close_input(&afmt_ctx);
traceo("~nclose()\n")
}
int nopen(){
traceo("nopen()\n")
int ret=0;
int i,j;
int previousvwidthvheight;
ac.video=ac.audio=1;
for(i=0;i<ac.vheight;i++)
for(j=0;j<ac.vwidth;j++)
*((uint32_t*)ac.vptr+i*ac.vwidth+j)=0xff000000;
av_register_all();
//fmt_ctx=avformat_alloc_context();
sprintf(errorstr,"new filename to open:%s\n",ac.filename);
traceo(errorstr)
if(avformat_open_input(&fmt_ctx, ac.filename, NULL, NULL) < 0){
tracee("error:avformat_open_input\n");
avformat_close_input(&fmt_ctx);
//nclose();
return -1;
}
if(avformat_find_stream_info(fmt_ctx, NULL)<0){
tracee("error:avformat_find_stream_info\n");
nclose();
return -1;
}
if(avformat_open_input(&afmt_ctx, ac.filename, NULL, NULL) < 0){
tracee("error:afmt avformat_open_input\n");
nclose();
return -1;
}
if(avformat_find_stream_info(afmt_ctx, NULL)<0){
tracee("error: afmtavformat_find_stream_info\n");
nclose();
return -1;
}
if(open_codec_context(&video_stream_idx, fmt_ctx, AVMEDIA_TYPE_VIDEO) < 0){
tracee("Could not open video stream\n");
avformat_close_input(&fmt_ctx);
ac.video=0;
ac.frameratenum=ac.framerateden=0;
}else{
video_stream=fmt_ctx->streams[video_stream_idx];
video_dec_ctx=fmt_ctx->streams[video_stream_idx]->codec;
pix_fmt=video_dec_ctx->pix_fmt;
ac.tt=fmt_ctx->duration;
previousvwidthvheight=ac.vwidth*ac.vheight;
ac.frameratenum=video_stream->r_frame_rate.num;
ac.framerateden=video_stream->r_frame_rate.den;
ac.videostreampts=video_stream->time_base.den/video_stream->time_base.num;
if((ret=av_image_alloc(video_dst_data,video_dst_linesize,video_dec_ctx->width,video_dec_ctx->height,pix_fmt,1))<0){
tracee("Could not allocate image buffer\n");
av_free(video_dst_data[0]);
nclose();
return -1;
}
video_dst_bufsize=ret;
sprintf(errorstr,"videobufsize:%d\n",video_dst_bufsize);
traceo(errorstr)
ac.vwidth=video_dec_ctx->width;
ac.vheight=video_dec_ctx->height;
swsContext=sws_getContext(ac.vwidth,ac.vheight,video_dec_ctx->pix_fmt,ac.vwidth,ac.vheight,AV_PIX_FMT_RGB32,SWS_BICUBIC,NULL,NULL,NULL);
m_Rgb.linesize[0]=ac.vwidth*4;
if(ac.vwidth*ac.vheight>previousvwidthvheight){
traceo("width height different, malloc again\n")
ac.vptr=(uint8_t*)realloc(ac.vptr,ac.vwidth*ac.vheight*4);
}
m_Rgb.data[0]=ac.vptr;
}
//if(open_codec_context(&audio_stream_idx, fmt_ctx, AVMEDIA_TYPE_AUDIO) <0){
if(open_codec_context(&audio_stream_idx, afmt_ctx, AVMEDIA_TYPE_AUDIO) <0){
tracee("Could not open codec context for AVMEDIA_TYPE_AUDIO\n");
avformat_close_input(&afmt_ctx);
ac.channel=ac.sampleformat=ac.sampleformatsize=ac.freq=0;
ac.audio=0;
}else{
audio_stream=afmt_ctx->streams[audio_stream_idx];
audio_dec_ctx=afmt_ctx->streams[audio_stream_idx]->codec;
ac.sampleformat=audio_dec_ctx->sample_fmt;
ac.sampleformatsize=av_get_bytes_per_sample(audio_dec_ctx->sample_fmt);
ac.channel=audio_dec_ctx->channels;
ac.freq=audio_dec_ctx->sample_rate;
ac.audiostreampts=audio_stream->time_base.den/audio_stream->time_base.num;
}
av_init_packet(&pkt);
pkt.data=NULL;
pkt.size=0;
av_init_packet(&pktA);
pktA.data=NULL;
pktA.size=0;
if(ac.freq==0 || ac.sampleformat==0 || ac.channel==0)
ac.freq=ac.sampleformat=ac.sampleformatsize=ac.channel=0;
if(ac.frameratenum==0 || ac.framerateden==0)
ac.frameratenum=ac.framerateden=0;
if(ac.frameratenum==0 && ac.freq==0){
tracee("not video audio data\n")
return -1;
}
sprintf(errorstr,"tt:%d\nfreq:%d\nsampleformat:%d\nsamplformatsize:%d\nchannel:%d\nwidth:%d\nheight:%d\nframeratenum:%d\nframerateden:%d\n",(int)ac.tt,(int)ac.freq,(int)ac.sampleformat,(int)ac.sampleformatsize,(int)ac.channel,(int)ac.vwidth,(int)ac.vheight,(int)ac.frameratenum,(int)ac.framerateden);
if(ac.frameratenum>(ac.framerateden*100))ac.frameratenum=ac.framerateden=0;
if(!ac.frameratenum && ac.freq){
ac.frameratenum=8;
ac.framerateden=1;
}
//seekfileframe(0);
traceo(errorstr)
sprintf(errorstr,"~init();%d\n",ret);
traceo(errorstr);
return ret<0;
}
int readframe(){
int ret=-1;
int got_frame=0;
//traceo("readframe()\n")
while(!got_frame && (ret=av_read_frame(fmt_ctx,&pkt))>=0){
if(pkt.stream_index==video_stream_idx){
ret=decode_packet_video(&got_frame);
av_packet_unref(&pkt);
}
}
ac.videopts=frame->pkt_pts;
//sprintf(errorstr,"~readframe(),ret:%d\n",ret);
//traceo(errorstr)
return ret;
}
int readaudioframe(){
int ret=-1;
int got_frame=0;
while(!got_frame && (ret=av_read_frame(afmt_ctx,&pktA))>=0){
if(pktA.stream_index==audio_stream_idx){
ret=decode_packet_audio(&got_frame);
av_packet_unref(&pktA);
}
}
return ret;
}
//int readaframe(int tweak,int64_t* rfd){
int readaframe(int tweak){
int ret=-1;
int sampleIndex,channelIndex;
int got_frame=0;
int first=1;
//sprintf(errorstr,"readaframe,tweak:%d\n",tweak);
//traceo(errorstr)
while((ret=av_read_frame(afmt_ctx,&pktA))>=0){
if(pktA.stream_index==audio_stream_idx){
AVPacket orig_pkt=pktA;
do{
if((ret=decode_packet_audio(&got_frame))<0)break;
pktA.data += ret;
pktA.size -= ret;
if(got_frame){
for(sampleIndex=0;sampleIndex<frame->nb_samples;sampleIndex++)
for(channelIndex=0;channelIndex<audio_dec_ctx->channels;channelIndex++){
if(audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_FLTP||audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_FLT)
memcpy(ac.aptr+ac.n,(float*)frame->extended_data[channelIndex]+sampleIndex,av_get_bytes_per_sample(frame->format));
else if(audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_S16 || audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_S16P)
memcpy(ac.aptr+ac.n,(int16_t*)frame->extended_data[channelIndex]+sampleIndex,av_get_bytes_per_sample(frame->format));
else if(audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_DBLP||audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_DBL)
memcpy(ac.aptr+ac.n,(double*)frame->extended_data[channelIndex]+sampleIndex,4);
else if(audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_U8 || audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_U8P)
memcpy(ac.aptr+ac.n,(uint8_t*)frame->extended_data[channelIndex]+sampleIndex,1);
else if(audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_S32 || audio_dec_ctx->sample_fmt==AV_SAMPLE_FMT_S32P)
memcpy(ac.aptr+ac.n,(int32_t*)frame->extended_data[channelIndex]+sampleIndex,4);
if((ac.n+av_get_bytes_per_sample(frame->format))>(GFDAUDIOBUFFERSIZE*2))
memcpy(ac.aptr,ac.aptr+2*GFDAUDIOBUFFERSIZE,ac.n+av_get_bytes_per_sample(frame->format)-GFDAUDIOBUFFERSIZE*2);
ac.n=(ac.n+av_get_bytes_per_sample(frame->format))%(GFDAUDIOBUFFERSIZE*2);
}
//*rfd=*rfd+frame->nb_samples;
}//got_frame
}while (pktA.size>0);
av_packet_unref(&orig_pkt);
}else
av_packet_unref(&pktA);
if( first && ((tweak && ac.n>=(GFDAUDIOBUFFERSIZE*3)/2)||(!tweak && ac.n>=GFDAUDIOBUFFERSIZE/2))){first=0;ac.audiopts=frame->pkt_pts;}
if((tweak && ac.n<GFDAUDIOBUFFERSIZE) || (!tweak && ac.n>=GFDAUDIOBUFFERSIZE))break;
}
//sprintf(errorstr,"~readaframe(),ret,ac.n,rfd:%d,%d,%d\n",ret,ac.n,(int)*rfd);
//traceo(errorstr);
return ret;
}
int seekfileframe(int64_t val){
int ret1=-1,ret2=-1;
int got_frame=0;
int64_t video_pts,audio_pts;
sprintf(errorstr,"seekfileframe:%d,%d,%d\n",(int)val,ac.video,ac.audio);
traceo(errorstr)
if(ac.video){
ret1=avformat_seek_file(fmt_ctx,-1,INT64_MIN,val*1000000,INT64_MAX,0);
avformat_flush(fmt_ctx);
}
if(ac.audio){
ret2=avformat_seek_file(afmt_ctx,-1,INT64_MIN,val*1000000,INT64_MAX,0);
avformat_flush(afmt_ctx);
}
if(ac.video){
pkt.data=NULL;
pkt.size=0;
do{
decode_packet_video(&got_frame);
ac.videopts=frame->pkt_pts;
sprintf(errorstr,"do loop frame pts:%d\n",(int)frame->pkt_pts);
traceo(errorstr)
}while(got_frame);
}
if(ac.audio){
pktA.data=NULL;
pktA.size=0;
do{
decode_packet_audio(&got_frame);
ac.audiopts=frame->pkt_pts;
sprintf(errorstr,"audio do loop frame pts:%d\n",(int)frame->pkt_pts);
traceo(errorstr)
}while(got_frame);
}
//printf("video num,den audio num,den,%d,%d,%d,%d\n",(int)video_stream->time_base.num,(int)video_stream->time_base.den,(int)audio_stream->time_base.num,(int)audio_stream->time_base.den);
//if(ac.video && ac.audio){
//if(readframe()>=0)
//video_pts=frame->pkt_pts;
//else return ret>=0;
//if(readaudioframe()>=0)
//audio_pts=frame->pkt_pts;
//else return ret>=0;
//if(audio_pts < (video_pts*(int64_t)audio_stream->time_base.den*(int64_t)video_stream->time_base.num)/((int64_t)audio_stream->time_base.num*(int64_t)video_stream->time_base.den))
//do{
//readaudioframe();
//audio_pts=frame->pkt_pts;
//}while((int)audio_pts && audio_pts < (video_pts*(int64_t)audio_stream->time_base.den*(int64_t)video_stream->time_base.num)/((int64_t)audio_stream->time_base.num*(int64_t)video_stream->time_base.den));
//else
//do{
//readframe();
//video_pts=frame->pkt_pts;
//}while((int)video_pts && video_pts < (audio_pts*(int64_t)video_stream->time_base.den*(int64_t)audio_stream->time_base.num)/((int64_t)video_stream->time_base.num*(int64_t)audio_stream->time_base.den));
//}
sprintf(errorstr,"~seekfileframeret1,ret2:%d,%d\n",ret1,ret2);
traceo(errorstr)
return (ret1>=0 || ret2>=0);
}
