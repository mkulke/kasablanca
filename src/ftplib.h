/***************************************************************************
                          ftplib.h  -  description
                             -------------------
    begin                : Son Jul 27 2003
    copyright            : (C) 2003 by mkulke
    email                : sikor_sxe@radicalapproach.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* original unmodified copyright notes from Thomas Pfau */

/***************************************************************************/
/*									   */
/* ftplib.c - callable ftp access routines				   */
/* Copyright (C) 1996, 1997, 1998 Thomas Pfau, pfau@cnj.digex.net	   */
/*	73 Catherine Street, South Bound Brook, NJ, 08880		   */
/*									   */
/* This library is free software; you can redistribute it and/or	   */
/* modify it under the terms of the GNU Library General Public		   */
/* License as published by the Free Software Foundation; either		   */
/* version 2 of the License, or (at your option) any later version.	   */
/* 									   */
/* This library is distributed in the hope that it will be useful,	   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of	   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	   */
/* Library General Public License for more details.			   */
/* 									   */
/* You should have received a copy of the GNU Library General Public	   */
/* License along with this progam; if not, write to the			   */
/* Free Software Foundation, Inc., 59 Temple Place - Suite 330,		   */
/* Boston, MA 02111-1307, USA.						   */
/* 									   */
/***************************************************************************/
 
#ifndef FTPLIB_H
#define FTPLIB_H

/* FtpAccess() type codes */
#define FTPLIB_DIR 1
#define FTPLIB_DIR_VERBOSE 2
#define FTPLIB_FILE_READ 3
#define FTPLIB_FILE_WRITE 4
#define FTPLIB_FILE_READ_APPEND 5
#define FTPLIB_FILE_WRITE_APPEND 6
#define FTPLIB_ASCII 'A'
#define FTPLIB_IMAGE 'I' 

#include <sys/time.h>
#include <openssl/ssl.h>

using namespace std;

/**
  *@author mkulke
  */

typedef int (*FtpCallbackXfer)(off64_t xfered, void *arg);
typedef int (*FtpCallbackIdle)(void *arg);
typedef void (*FtpCallbackLog)(char *str, void* arg, bool out);

struct netbuf {
	char *cput,*cget;
	int handle;
	int cavail,cleft;
	char *buf;
	int dir;
	netbuf *ctrl;
	int cmode;
	int tlsctrl;
	int tlsdata;
	struct timeval idletime;
	FtpCallbackXfer xfercb;
	FtpCallbackIdle idlecb;
	FtpCallbackLog logcb;
	void *cbarg;
	off64_t xfered;
	off64_t cbbytes;
	off64_t xfered1;
	char response[256];
	SSL* ssl;
	SSL_CTX* ctx;
	BIO* sbio;
	off64_t offset;
	bool correctpasv;
};
  
class ftplib {
public:
    enum ftp
    {
        pasv = 1,
        port = 2,
        defaultconnmode = 1,
        ascii = 'A',
        image = 'I',
        unencrypted = 0,
        secure = 1,
        defaultfxp = 0,
        alternativefxp = 1
    };
	ftplib();
	~ftplib();
    char* LastResponse();
    int Connect(const char *host);
    int Login(const char *user, const char *pass);
    int Site(const char *cmd);
    int Raw(const char *cmd);
    int SysType(char *buf, int max);
    int Mkdir(const char *path);
    int Chdir(const char *path);
    int Cdup();
    int Rmdir(const char *path);
    int Pwd(char *path, int max);
    int Nlst(const char *outputfile, const char *path);
    int Dir(const char *outputfile, const char *path);
    int Size(const char *path, int *size, ftplib::ftp mode);
    int ModDate(const char *path, char *dt, int max);
    int Get(const char *outputfile, const char *path, ftplib::ftp mode, off64_t offset = 0);
    int Put(const char *inputfile, const char *path, ftplib::ftp mode, off64_t offset= 0);
    int Rename(const char *src, const char *dst);
    int Delete(const char *fnm);
    int SetDataEncryption(ftplib::ftp flag);
    int NegotiateEncryption();
    int Quit();
    void SetCallbackIdleFunction(FtpCallbackIdle pointer);
    void SetCallbackLogFunction(FtpCallbackLog pointer);
	 void SetCallbackXferFunction(FtpCallbackXfer pointer);
	 void SetCallbackArg(void *arg);
    void SetCallbackBytes(off64_t bytes);
	 void SetCorrectPasv(bool b) { mp_netbuf->correctpasv = b; };
    void SetCallbackIdletime(int time);
    void SetConnmode(ftplib::ftp mode);
    static int Fxp(ftplib* src, ftplib* dst, const char *pathSrc, const char *pathDst, ftplib::ftp mode, ftplib::ftp method);
    netbuf* mp_netbuf;
    int FtpRead(void *buf, int max, netbuf *nData);
    int FtpWrite(void *buf, int len, netbuf *nData);
    int FtpAccess(const char *path, int typ, int mode, netbuf *nControl, netbuf **nData);
    int FtpClose(netbuf *nData);
private:
    int FtpXfer(const char *localfile, const char *path, netbuf *nControl, int typ, int mode);
    int FtpOpenPasv(netbuf *nControl, netbuf **nData, int mode, int dir, char *cmd);
    int FtpSendCmd(const char *cmd, char expresp, netbuf *nControl);
    int FtpAcceptConnection(netbuf *nData, netbuf *nControl);
    int FtpOpenPort(netbuf *nControl, netbuf **nData, int mode, int dir, char *cmd);
    int socket_wait(netbuf *ctl);
    int readline(char *buf,int max,netbuf *ctl);
    int writeline(char *buf, int len, netbuf *nData);
    int readresp(char c, netbuf *nControl);
	 void ClearNetbuf();
	 int CorrectPasvResponse(unsigned char *v);
};

#endif
