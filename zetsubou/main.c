/*
 *  main.c
 *  Zetsubou
 *
 *  Created by Alex Marshall on 09/03/17.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "tpl.h"

void bad_args(char progname[])
{
	printf("Invalid usage. Usage:\n\t%s r <file.tpl>\nOr:\n\t%s w <type> <file1.bmp> [file2.bmp...] <out.tpl>\n", progname, progname);
	exit(1);
}

int main(int argc, char *argv[])
{
	printf("Zetsubou v0.1. Written by SquidMan (Alex Marshall)\nRGB5A3 code was contributed by booto.\nBased off of gentpl.\n\n");
	if(argc < 3)
		bad_args(argv[0]);
	char type = toupper(argv[1][0]);
	if(type == 'R') {	// Read TPL.
		if(argc != 3)
			bad_args(argv[0]);
		FILE* fp = fopen(argv[2], "rb");
		if(fp == NULL) {
			printf("Error opening %s.\n", argv[2]);
			exit(1);
		}
		char *basename = (char*)calloc(strlen(argv[2]) + 1, 1);
		int arg1strlen = strlen(argv[2]);
		int i;
		for(i = 0; (i < arg1strlen) && (argv[2][i] != '.'); i++) basename[i] = argv[2][i];
		fseek(fp, 0, SEEK_END);
		u32 tplsize = ftell(fp);
		u8* tplbuf  = (u8*)malloc(tplsize);
		fseek(fp, 0, SEEK_SET);
		fread(tplbuf, tplsize, 1, fp);
		fclose(fp);
		int ret = TPL_ConvertToBMP(tplbuf, tplsize, basename);
		if(ret < 0) {
			printf("Error converting to BMP.\n");
			exit(1);
		}
	}else if(type == 'W') {	// Write TPL.
		if(argc < 5)
			bad_args(argv[0]);
		char** names = (char**)calloc(sizeof(char*), argc - 4);
		int i;
		for(i = 0; i < argc - 4; i++)
			names[i] = argv[i + 3];
		printf("%d\n", argc);
		int format;
		char formatstr[256];
		memset(formatstr, 0, 256);
		for(i = 0; i < strlen(argv[2]); i++) formatstr[i] = toupper(argv[2][i]);
		if(strcmp(formatstr, "I4") == 0) format = TPL_FORMAT_I4;
		else if(strcmp(formatstr, "I8") == 0) format = TPL_FORMAT_I8;
		else if(strcmp(formatstr, "IA4") == 0) format = TPL_FORMAT_IA4;
		else if(strcmp(formatstr, "IA8") == 0) format = TPL_FORMAT_IA8;
		else if(strcmp(formatstr, "RGB565") == 0) format = TPL_FORMAT_RGB565;
		else if(strcmp(formatstr, "RGB5A3") == 0) format = TPL_FORMAT_RGB5A3;
		else if(strcmp(formatstr, "RGBA8") == 0) format = TPL_FORMAT_RGBA8;
		else if(strcmp(formatstr, "CI4") == 0) format = TPL_FORMAT_CI4;
		else if(strcmp(formatstr, "CI8") == 0) format = TPL_FORMAT_CI8;
		else if(strcmp(formatstr, "CI14X2") == 0) format = TPL_FORMAT_CI14X2;
		else if(strcmp(formatstr, "CMP") == 0) format = TPL_FORMAT_CMP;
		int ret = TPL_ConvertFromBMPs(argc - 4, names, argv[argc - 1], format);
		if(ret < 0) {
			printf("Error converting BMPs.\n");
			exit(1);
		}
	}
	return 0;
}
