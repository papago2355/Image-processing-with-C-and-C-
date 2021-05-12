/*************************************************************************** 
 * File: iplib.c                                                           *
 *                                                                         *
 * Desc: general purpose image processing routines                         * 
 ***************************************************************************/


#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "ip.h"

//아래 함수들은 대부분 다 특정 파일을 읽고, 쓰는 (read,write)용 함수이다
image_ptr read_pnm(char *filename, int *rows, int *cols, int *type);
int getnum(FILE *fp);

/***************************************************************************
 * Func: read_pnm                                                          *
 *                                                                         *
 * Desc: reads a portable bitmap file                                      *
 *                                                                         *
 * Params: filename - name of image file to read                           *
 *         rows - number of rows in the image                              *
 *         cols - number of columns in the image                           *
 *         type - file type                                                *
 *                                                                         *
 * Returns: pointer to the image just read into memory                     *
 ***************************************************************************/

//pnm 이미지 파일을 읽는 함수
image_ptr read_pnm(char *filename, int *rows, int *cols, int *type)
    {
	//필요한 변수들, 사이즈, 바이트, 처음 2개의 character와 file 포인터등을 설정해준다.
    int i;                     /* index variable */
    int row_size;              /* size of image row in bytes */
    int maxval;                /* maximum value of pixel */
    FILE *fp;                  /* input file pointer */
    int firstchar, secchar;    /* first 2 characters in the input file */
    image_ptr ptr;             /* pointer to image buffer */
    unsigned long offset;      /* offset into image buffer */
    unsigned long total_size;  /* size of image in bytes */
    unsigned long total_bytes; /* number of total bytes written to file */
    float scale;               /* number of bytes per pixel */

    /* open input file */
	//rb를 통해 읽기모드로 열수 있는가를 체크
    if((fp = fopen(filename, "rb")) == NULL)
	{
	printf("Unable to open %s for reading\n",filename);
	exit(1);
	}
	//처음 2개의character를 받음
    firstchar = getc(fp);
    secchar = getc(fp);
	//pnm을 체크하는 구간
    if(firstchar != 'P')
	{
	printf("You silly goof... This is not a PPM file!\n");
	exit(1);
	}
	//타입과 cols,rows를 받음
    *cols = getnum(fp);
    *rows = getnum(fp);
    *type = secchar - '0';

	//4,5,6일때 각각 PBM PGM PPM 으로 switch문 실행
    switch(secchar)
	{
	case '4':            /* PBM */
	    scale = 0.125;
	    maxval = 1;
	    break;
	case '5':            /* PGM */
	    scale = 1.0;
	    maxval = getnum(fp);
	    break;
	case '6':             /* PPM */
	    scale = 3.0;
	    maxval = getnum(fp);
	    break;
		//위 아무것도 해당안될경우 (PNM이 아니다)
	default :             /* Error */
	    printf("read_pnm: This is not a Portable bitmap RAWBITS file\n");
	    exit(1);
	    break;
	}

	//total size를 설정
    row_size = (*cols) * scale;
    total_size = (unsigned long) (*rows) * row_size;

    ptr = (image_ptr) IP_MALLOC(total_size);
	//메모리 할당을 못 받았을 경우 체크
    if(ptr == NULL)
	{
	printf("Unable to malloc %lu bytes\n",total_size);
	exit(1);
	}
	//rows를 기준으로 byte를 읽어감
    total_bytes=0;
    offset = 0;
    for(i=0; i<(*rows); i++)
	{
	total_bytes+=fread(ptr+offset, 1, row_size, fp);
	offset += row_size;
	}
	//사이즈와 읽은 바이트 사이에 괴리 발생 체크
    if(total_size != total_bytes)
	{
	printf("Failed miserably trying to read %ld bytes\nRead %ld bytes\n",
		total_size, total_bytes);
	exit(1);
	}

    fclose(fp);
    return ptr;
    }

/***************************************************************************
 * Func: getnum                                                            *
 *                                                                         *
 * Desc: reads an ASCII number from a portable bitmap file header          *
 *                                                                         *
 * Param: fp - pointer to file being read                                  *
 *                                                                         *
 * Returns: the number read                                                *
 ***************************************************************************/
//비트맵 파일로 부터 ASCII 숫자를 읽음
int getnum(FILE *fp)
    {
    char c;               /* character read in from file */
    int i;                /* number accumulated and returned */
	//아래 종결 신호 전까지 입력을 c에 받음
    do
	{
	c = getc(fp);
	}
    while((c==' ') || (c=='\t') || (c=='\n') || (c=='\r'));
	//Garabe체크를 실행
    if((c<'0') || (c>'9'))
	if(c == '#')                   /* chew off comments */
	    {
	    while(c == '#')
		{
		while(c != '\n')
		    c = getc(fp);
		c = getc(fp);
		}
	    }
	else
	    {
	    printf("Garbage in ASCII fields\n");
	    exit(1);
	    }

    i=0;
	//ascii를 int로 변경
    do
	{
	i=i*10+(c-'0');         /* convert ASCII to int */
	c = getc(fp);
	}
    while((c>='0') && (c<='9'));

    return i;
    }

/***************************************************************************
 * Func: write_pnm                                                         *
 *                                                                         *
 * Desc: writes out a portable bitmap file                                 *
 *                                                                         *
 * Params: ptr - pointer to image in memory                                *
 *         filename _ name of file to write image to                       *
 *         rows - number of rows in the image                              *
 *         cols - number of columns in the image                           *
 *         magic_number - number that defines what type of file it is      *
 *                                                                         *
 * Returns: nothing                                                        *
 ***************************************************************************/
//pnm write를 실행함
void write_pnm(image_ptr ptr, char *filename, int rows, 
	       int cols, int magic_number)
    {
	//필요한 변수들을 선언
    FILE *fp;             /* file pointer for output file */
    long offset;          /* current offset into image buffer */
    long total_bytes;     /* number of bytes written to output file */
    long total_size;      /* size of image buffer */
    int row_size;         /* size of row in bytes */
    int i;                /* index variable */
    float scale;          /* number of bytes per image pixel */

	//switch문을 통해 파일 종류를 구분
    switch(magic_number)
	{
	case 4:            /* PBM */
	    scale = 0.125;
	    break;
	case 5:            /* PGM */
	    scale = 1.0;
	    break;
	case 6:             /* PPM */
	    scale = 3.0;
	    break;
		//전부 아닌경우 아래를 실행
	default :             /* Error */
	    printf("write_pnm: This is not a Portable bitmap RAWBITS file\n");
	    exit(1);
	    break;
	}

    /* open new output file */
    if((fp=fopen(filename, "wb")) == NULL)
	{
	printf("Unable to open %s for output\n",filename);
	exit(1);
	}

    /* print out the portable bitmap header */
    fprintf(fp, "P%d\n%d %d\n", magic_number, cols, rows);
    if(magic_number != 4)
	fprintf(fp, "255\n");

	//이제 사이즈를 구함 , total size를 위주로
    row_size = cols * scale;
    total_size = (long) row_size *rows;
    offset = 0;
    total_bytes = 0;
	//total byte를 구함
    for(i=0; i<rows; i++)
	{
	total_bytes += fwrite(ptr+offset, 1, row_size, fp);
	offset += row_size;
	}
	//total bytees != total_size일때 아래를 실행
    if(total_bytes != total_size)
	printf("Tried to write %ld bytes...Only wrote %ld\n",
		total_size, total_bytes);

    fclose(fp);
    }

/****************************************************************************
 * Func: pnm_open                                                           *
 *                                                                          *
 * Desc: opens a pnm file and determines rows, cols, and maxval             *
 *                                                                          *
 * Params: rows- pointer to number of rows in the image                     *
 *         cols - pointer number of columns in the image                    *
 *         maxval - pointer to max value                                    *
 *         filename - name of image file                                    *
 ****************************************************************************/
//pnm파일을 열고, rows,cols, maxval을 정하는 함수
FILE *pnm_open(int *rows, int *cols, int *maxval, char *filename)
    {
	//처음 2개의 character와 기타 변수들을 설정
    int firstchar, secchar;
    float scale;
    unsigned long row_size;
    FILE *fp;
	//파일을 read 가능한지 확인
    if((fp = fopen(filename, "rb")) == NULL)
	{
	printf("Unable to open %s for reading\n",filename);
	exit(1);
	}
	//처음 2개의 character를 받음
    firstchar = getc(fp);
    secchar = getc(fp);
	//ppm이 아닐경우
    if(firstchar != 'P')
	{
	printf("You silly goof... This is not a PPM file!\n");
	exit(1);
	}

    *cols = getnum(fp);
    *rows = getnum(fp);
	//pnm 파일 확장자 체크
    switch(secchar)
	{
	case '4':            /* PBM */
	    scale = 0.125;
	    *maxval = 1;
	    break;
	case '5':            /* PGM */
	    scale = 1.0;
	    *maxval = getnum(fp);
	    break;
	case '6':             /* PPM */
	    scale = 3.0;
	    *maxval = getnum(fp);
	    break;
		//전부 아닐경우
	default :             /* Error */
	    printf("read_pnm: This is not a Portable bitmap RAWBITS file\n");
	    exit(1);
	    break;
	}

    row_size = (*cols) * scale;
    return fp;
    }


/****************************************************************************
 * Func: read_mesh                                                          *
 *                                                                          *
 * Desc: reads mesh data into a mesh structure				    *
 *                                                                          *
 * Params: filename - name of input mesh file                               *
 *                                                                          *
 * Returns: mesh structure storing width, height, x data  and y data        *
 ****************************************************************************/
//mesh 구조체에서 mesh를 읽음
mesh *read_mesh(char *filename)
    {
    FILE *fp;
    mesh *mesh_data;
    int width, height, mesh_size;

    /* open mesh file for input */
    if((fp = fopen(filename, "rb")) == NULL)
	{
	printf("Unable to open mesh file %s for reading\n", filename);
	exit(1);
	}
	//메모리 할당
    mesh_data = malloc(sizeof(mesh));
    /* read dimensions of mesh */
    fread(&width, sizeof(int), 1, fp);
    fread(&height, sizeof(int), 1, fp);
    mesh_data->width = width;
    mesh_data->height = height;
    mesh_size = width * height;
	//메모리 할당을 위해 설정
    /* allocate memory for mesh data */
    mesh_data->x_data = malloc(sizeof(float) * mesh_size);
    mesh_data->y_data = malloc(sizeof(float) * mesh_size);
	//메쉬 정보를 읽어들임
    fread(mesh_data->x_data, sizeof(float), mesh_size, fp);
    fread(mesh_data->y_data, sizeof(float), mesh_size, fp);

    return(mesh_data);
    }
