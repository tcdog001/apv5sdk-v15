/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	       *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.                *
 *                                                                                         *
 * You should have received a copy of the GNU General Public License along with this       *
 * program; if not, write to the:                                                          *
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,                    *
 * MA  02111-1307, USA.                                                                    *
 *                                                                                         *
 * --------------------------------------------------------------------------------------- *
 * Project:  Capwap                                                                        *
 *                                                                                         *
 * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                          *  
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *******************************************************************************************/


#ifndef __CAPWAP_CWLog_HEADER__
#define __CAPWAP_CWLog_HEADER__

extern char gLogFileName[];
extern char gWTPLogFileName[];



#define WID_SYSLOG_EMERG	0
#define WID_SYSLOG_ALERT	1
#define WID_SYSLOG_CRIT		2
#define WID_SYSLOG_ERR		3
#define WID_SYSLOG_WARNING	4
#define WID_SYSLOG_NOTICE	5
#define WID_SYSLOG_INFO		6
#define WID_SYSLOG_DEBUG	7
#define WID_SYSLOG_DEFAULT	0


#define WID_SYSLOG_DEBUG_NONE		0
#define WID_SYSLOG_DEBUG_INFO		1
#define WID_SYSLOG_DEBUG_DEBUG		8
#define WID_SYSLOG_DEBUG_ALL		15
#define WID_SYSLOG_DEBUG_DEFAULT	15
//if the syslog system forbidden showing the debug_info,we should change the default value to 0
extern int gWIDLogdebugLevel;

extern int gWIDLOGLEVEL;

__inline__ void CWVLog(const char *format, va_list args);
__inline__ void CWLog(const char *format, ...);
__inline__ void CWDebugLog(const char *format, ...);
__inline__ void CWWTPVLog(const char *format, va_list args);
__inline__ void CWWTPDebugLog(const char *format, ...);
void CWLogInitFile(char *fileName);
void CWWTPLogInitFile(char *fileName);
__inline__ void WID_Log(int level,const char *format, ...);
__inline__ void WIDVLog(int level,const char *format,va_list args);
void wid_syslog_emerg(char *format,...);
void wid_syslog_alert(char *format,...);
void wid_syslog_crit(char *format,...);
void wid_syslog_err(char *format,...);
void wid_syslog_warning(char *format,...);
void wid_syslog_notice(char *format,...);
void wid_syslog_info(char *format,...);
void wid_syslog_debug(char *format,...);
void wid_syslog_debug_info(char *format,...);
void wid_syslog_debug_debug(char *format,...);







#endif
