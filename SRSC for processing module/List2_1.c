



/***************************************************************************
 * File: frame.c                                                           *
 *                                                                         *
 * Desc: This program performs frame operations between 2 images           *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include "ip.h" 


#define frame_operation(X1, X2)  X1-X2

#include <fcntl.h>

typedef unsigned short int USHORT;
typedef unsigned long int  ULONG;

struct BITMAPHEADER {
    USHORT bmpType;                       // BMP ������ ������ ��Ÿ���� �ĺ���
    ULONG  bmpSize;                       // BMP ������ ũ��(����Ʈ ����)
    USHORT bmpReserved1;                  // ����
    USHORT bmpReserved2;                  // ����
    ULONG  bmpOffset;                     // �̹��� ������ ���۵Ǵ� ��ġ(Offset)
    ULONG  bmpHeaderSize;                 // Header�� ũ��(����Ʈ ����)
    ULONG  bmpWidth;                      // ���� ũ��(�ȼ� ����)
    ULONG  bmpHeight;                     // ���� ũ��(�ȼ� ����)
    USHORT bmpPlanes;                     // Color Plane�� ��(1�� ����)
    USHORT bmpBitCount;                   // �� �ȼ��� �Ҵ�� ��Ʈ��
    ULONG  bmpCompression;                // ���� ��� ��뿩��
    ULONG  bmpBitmapSize;                 // ��Ʈ�� �̹����� ũ��
    ULONG  bmpXPelsPerMeter;              // ���� �ػ�
    ULONG  bmpYPelsPerMeter;              // ���� �ػ�
    ULONG  bmpColors;                     // ���� �����
    ULONG  bmpClrImportant;
};

struct BITMAPHEADER bmp_header;                // Bitmap ������ Header ����ü


void ReadBitmapHeader(char* filename) {
    int          read_fd;                              // ������ �а� ���µ� ����� Descriptor

    read_fd = open(filename, O_RDONLY);     // bmp������ open�Ѵ�.
    if (read_fd == -1) {
        printf("Can't open file %s\n", filename);
        exit(1);
    }

    read(read_fd, &bmp_header.bmpType, sizeof(bmp_header.bmpType));
    read(read_fd, &bmp_header.bmpSize, sizeof(bmp_header.bmpSize));
    read(read_fd, &bmp_header.bmpReserved1, sizeof(bmp_header.bmpReserved1));
    read(read_fd, &bmp_header.bmpReserved2, sizeof(bmp_header.bmpReserved2));
    read(read_fd, &bmp_header.bmpOffset, sizeof(bmp_header.bmpOffset));
    read(read_fd, &bmp_header.bmpHeaderSize, sizeof(bmp_header.bmpHeaderSize));
    read(read_fd, &bmp_header.bmpWidth, sizeof(bmp_header.bmpWidth));
    read(read_fd, &bmp_header.bmpHeight, sizeof(bmp_header.bmpHeight));
    read(read_fd, &bmp_header.bmpPlanes, sizeof(bmp_header.bmpPlanes));
    read(read_fd, &bmp_header.bmpBitCount, sizeof(bmp_header.bmpBitCount));
    read(read_fd, &bmp_header.bmpCompression, sizeof(bmp_header.bmpCompression));
    read(read_fd, &bmp_header.bmpBitmapSize, sizeof(bmp_header.bmpBitmapSize));
    read(read_fd, &bmp_header.bmpXPelsPerMeter, sizeof(bmp_header.bmpXPelsPerMeter));
    read(read_fd, &bmp_header.bmpYPelsPerMeter, sizeof(bmp_header.bmpYPelsPerMeter));
    read(read_fd, &bmp_header.bmpColors, sizeof(bmp_header.bmpColors));
    read(read_fd, &bmp_header.bmpClrImportant, sizeof(bmp_header.bmpClrImportant));

    close(read_fd);
}


extern int getnum(FILE* fp);

int main(int argc, char* argv[])
{
    char file1[100];                    /* name of first input file */
    char file2[100];                    /* name of second input file */
    char fileout[100];                  /* name of output file */
    int rows, cols;                     /* image rows and columns */
    unsigned long i, j;                  /* x and y loop variables */
    unsigned long bytes_per_pixel;      /* number of bytes per image pixel */
    unsigned char* buff1, * buff2;       /* input line buffers */
    unsigned char* buff_out;            /* output line buffer */
    int file_type;                      /* what type of image data */
    FILE* fp1, * fp2;                    /* input file pointers */
    FILE* fpout;                        /* output file pointer */
    int firstchar, secchar;             /* first and second characters of file */
    int line;                           /* size of line in bytes */
    int temp;                           /* temporary variable */

    /* set input filename and output file name */
    if (argc == 4)
    {
        strcpy(file1, argv[1]);
        strcpy(file2, argv[2]);
        strcpy(fileout, argv[3]);
    }
    else
    {
        printf("Input name of first input file\n");
        gets(file1);
        printf("Input name of second input file\n");
        gets(file2);
        printf("\nInput name of output file\n");
        gets(fileout);
        printf("\n");
    }

    if ((fpout = fopen(fileout, "wb")) == NULL)
    {
        printf("Unable to open %s for writing\n", fileout);
        exit(1);
    }

    if ((fp1 = fopen(file1, "rb")) == NULL)
    {
        printf("Unable to open %s for reading\n", file1);
        exit(1);
    }

    firstchar = getc(fp1);
    secchar = getc(fp1);

    if (firstchar != 'P')
    {
        printf("You silly goof... This is not a PPM file!\n");
        exit(1);
    }

    cols = getnum(fp1);
    rows = getnum(fp1);
    getnum(fp1);     /* consume max value from input file header */

    if ((fp2 = fopen(file2, "rb")) == NULL)
    {
        printf("Unable to open %s for reading\n", file2);
        exit(1);
    }

    firstchar = getc(fp2);
    if (getc(fp2) != secchar)
    {
        printf("Input files are different types...exiting\n");
        exit(1);
    }

    if (firstchar != 'P')
    {
        printf("You silly goof... This is not a PPM file!\n");
        exit(1);
    }

    if (getnum(fp2) != cols)
    {
        printf("different column sizes...exiting\n");
        exit(1);
    }

    if (getnum(fp2) != rows)
    {
        printf("different row sizes...exiting\n");
        exit(1);
    }

    getnum(fp2);     /* consume max value from input file header */

    /* determine bytes_per_pixel, 3 for color, 1 for gray-scale */
    if (toupper(secchar) == 'P')
    {
        bytes_per_pixel = 3;
        file_type = PPM;
    }
    else
    {
        bytes_per_pixel = 1;
        file_type = PGM;
    }

    line = bytes_per_pixel * cols;
    buff1 = malloc(line);
    buff2 = malloc(line);
    buff_out = malloc(line);

    if ((buff1 == NULL) || (buff2 == NULL) || (buff_out == NULL))
    {
        printf("unable to malloc enough memory for measley frame process\n");
        exit(1);
    }

    /* print output file header */
    fprintf(fpout, "P%c\n%d %d\n255\n", secchar, cols, rows);
    /* operate on two images and write output one line at a time */
    for (i = 0; i < rows; i++)
    {
        fread(buff1, 1, line, fp1);
        fread(buff2, 1, line, fp2);
        for (j = 0; j < line; j++)
        {
            temp = frame_operation(buff1[j], buff2[j]);
            CLIP(temp, 0, 255);
            buff_out[j] = temp;
        }
        fwrite(buff_out, 1, line, fpout);
    }

    free(buff1);
    free(buff2);
    free(buff_out);
    fclose(fp1);
    fclose(fp2);
    fclose(fpout);
    return 0;
}