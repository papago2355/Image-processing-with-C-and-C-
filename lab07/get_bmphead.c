/***************************************************************************
 * File: get_bmphead.c                                                     *
 *                                                                         *
 * Desc: This program reads header part of bmp format image                *
 ***************************************************************************/
 
 
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


void ReadBitmapHeader(char *filename) {
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
 
