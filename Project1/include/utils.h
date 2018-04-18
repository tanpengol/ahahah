#pragma once
#include <stdio.h>
#include <stdarg.h>


#ifdef _WIN32
#define SEP "\\"
#else
#define SEP "/"
#endif 

#define DEBUG_TO_FILE
#define LOGFILE "log"SEP"log.txt" 
#define SHADERSOURCE_MAXLENTH 10240


BOOL isFileOpen = FALSE;
FILE *outputLOGfile = NULL;


// -! This function will malloc a memory of 512 byte
// -! You need to free it by yourself
char * getResourceName(const char *fileName, const char *type)
{
	char * fullfilename = (char *)malloc(512);
	memset(fullfilename, 0, 512);
	sprintf_s(fullfilename, 512, "resource"SEP"%s"SEP"%s",type, fileName);
	return fullfilename;
}

// -! This function will alloc a memory for shadersource
// -! You need to free it by your self
char * LoadShader(const char *filename)
{
	char *shaderSource = NULL;
	char * shadername = getResourceName(filename, "shaders");
	FILE *fp;
	fopen_s(&fp, shadername,"rb");
	if (fp == NULL)
	{
		printf_s("can't open file %s\n", shadername);
		free(shadername);
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);
	shaderSource = (char *)malloc(size+1);
	if (shaderSource == NULL)
	{
		return NULL;
	}
	memset(shaderSource, 0, size+1);
	fread_s(shaderSource, size+1, 1, size, fp);
	fclose(fp);
	free(shadername);
	return shaderSource;
}

int openLogFile()
{
	if (outputLOGfile != NULL)
	{
		if (fopen_s(&outputLOGfile, LOGFILE, "at,ccs=encoding") != 0)
		{
			printf_s("Can't write to logfile");
			return -1;
		}
	}
	return 0;
}

int closeLogFile()
{
	if(outputLOGfile != NULL)
		return fclose(outputLOGfile);
	return 0;
}


int DebugToFile(const char *content)
{
	if (outputLOGfile!= NULL)
		return fprintf_s(outputLOGfile, content);
	return -1;
}

int DebugToStdout(const char *content)
{
	if (outputLOGfile != NULL)
		return printf_s(content);
	return -1;
}

int DebugLog(const char *format, ...)
{
	va_list formatList;
	va_start(formatList, format);
	char outputstring[4096];
	vsnprintf_s(outputstring, 4096, 4096, format, formatList);
#ifndef DEBUG_TO_FILE
	return DebugToStdout(outputstring);
#else
	return DebugToFile(outputstring);
#endif
}