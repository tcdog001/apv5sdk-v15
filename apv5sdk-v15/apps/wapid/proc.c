/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：proc.c
* 摘    要：解析.ini格式配置文件的相关函数的实现
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <error.h>
#include "proc.h"

char *strip (char *string, char delimiter)
{
  register int x, y;

  y = strlen (string);
  x = 0;
  while ((x < y) && (string[x] == delimiter))
    {
      x++;
    };
  while ((y > 0) && (string[y - 1] == delimiter))
    {
      y--;
    };
  string[y] = 0;
  return string + x;
}


int get_pr (char type, char *line, prop_data *prop)
{
    char *index = NULL;	
    char *keyptr = NULL;

    if (line[0] != '#')
    {
        // modified by kWlW, separator changed to this  vvv  from "="
        keyptr = (char *)strstr(line, "setenv ");	
		//finds the first occurrence of the substring 'setenv' in line
        if (keyptr != NULL) line = keyptr + 7;   
		/* Add size of 'setenv ' string */
		index =  strstr (line, type == SEP_EQUAL ? "=" : type == SEP_SPACE ? " " : "\t");	
        if (((char *)index) != NULL)
        {
            *((char *) index) = '\0';
            if ((prop->key = (char *)malloc (strlen (line) + 1)) != NULL &&
                (prop->value =(char *) malloc (strlen ((char *) (index + 1)) + 1)) != NULL)
            {
                strcpy (prop->key, line);
                strcpy (prop->value, (char *) (index + 1));
                return 1;
            }
        }
    }
    return 0;
}

int load_prop (char type, char *file_name, prop_data properties[])
{
	FILE *fid;
	char line[1024];//jwchang
	int prop_count = 0;

	if ((fid = fopen (file_name, "r")) == NULL)  
	{
		printf(" fopen == NULL\n");
		return 0;
	}
	while (!feof (fid))
	{
		fgets(line, 255, fid);
		if(feof(fid)) break;
		if (strchr(line, '\n'))  *(strchr(line, '\n')) = '\0';
		if (get_pr (type, line, (prop_data *) & properties[prop_count]))
		{
			prop_count++;
		}
	}
	fclose (fid);
	return prop_count;
}


char *get_prop (char *key, char *result, prop_data properties[], int count)
{
  int index;
  char tmp_str[256] = "";
  for (index = 0; index < count; index++)
    {
      if (strcmp (properties[index].key, key) == 0)
	{
	  strcpy(tmp_str, properties[index].value);	
	  strcpy (result, strip(tmp_str, SEP_Q_MARK));
	  return result;
	}
    }
  return NULL;
}


int hex2int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return -1;
}

int str2byte( unsigned char *str, int len,  unsigned char *byte_out)
{
	int i, val, val2;
	char *pos = (char *)str;
	for (i = 0; i < len/2; i++) {
		val = hex2int(*pos++);
		if (val < 0)
			return -1;
		val2 = hex2int(*pos++);
		if (val2 < 0)
			return -1;
		byte_out[i] = (val * 16 + val2) & 0xff;
	}

	return 0;
}
int hwaddr_aton(const char *txt, u8 *addr)
{
	int i;

	for (i = 0; i < 6; i++) {
		int a, b;

		a = hex2int(*txt++);
		if (a < 0)
			return -1;
		b = hex2int(*txt++);
		if (b < 0)
			return -1;
		*addr++ = (a << 4) | b;
		if (i < 5 && *txt++ != ':')
			return -1;
	}

	return 0;
}

void *get_buffer(int len)
{
	char *buffer=NULL;
	buffer = (char *)malloc(len);
	if(buffer)
		memset(buffer, 0, len);
	else
		buffer = NULL;
	return buffer;
}
void *free_buffer(void *buffer, int len)
{
	char *tmpbuf = (char *)buffer;

	if(tmpbuf != NULL)
	{
		memset(tmpbuf, 0, len);
		free(tmpbuf);
		return NULL;
	}
	else
		return NULL;
}

void free_item(item_data_t *zap, int freeit)
{
    if (zap) {
	free(zap->data);
	zap->data = 0;
	zap->length= 0;
	if (freeit) {
	    free(zap);	
	}
    }
}

int free_prop (prop_data properties[], int count)
{
  int index;
  for (index = 0; index < count; index++)
    {
      free (properties[index].key);
      free (properties[index].value);
    }
  return 1;
}

int save_global_conf(char type, char* filename, char *command, char *key, char *value)
{
	FILE* fidIn, *fidOut;
	char line[1024], tmp_line[1024];
        char tmp_fname[255], *index;
        char separator[2];
        int i=0;
		
	if ((fidIn=fopen(filename,"r"))==NULL) return 0;
	strcpy(tmp_fname,filename);	
	strcat(tmp_fname,".wapid");
	
        if ((fidOut=fopen(tmp_fname,"w"))==NULL) 
        {
        	fclose(fidIn);
		return 0;
	}

        strcpy(separator,(type==SEP_EQUAL?"=":type==SEP_SPACE?" ":"\t"));
        type=separator[0];

	while(!feof(fidIn))
	{
		fgets(line,1024,fidIn);
                strcpy(tmp_line,command);
                strcat(tmp_line,key);
                if (((index=strstr(line,tmp_line))!=NULL) && (index==line)) {
                    i=1;
                    if (!feof(fidIn)) {
                        if (type != '0')
                            fprintf(fidOut,"%s%s%c%c%s%c\n",command,key,type,SEP_Q_MARK, value, SEP_Q_MARK);
                        else
                            fprintf(fidOut,"%s%s%c%s%c\n",command,key,SEP_Q_MARK,value,SEP_Q_MARK);
                    }
                } else
                    if (!feof(fidIn)) fprintf(fidOut,"%s",line);
        }

        if (i==0) {
            if (type != '0') { fprintf(fidOut,"%s%s%c%s\n",command,key,type,value); }
            else fprintf(fidOut,"%s%s%s\n",command,key,value);
        }
             
	fclose(fidIn);
	fclose(fidOut);
	unlink(filename);
	rename(tmp_fname,filename);
	return 1;
}


