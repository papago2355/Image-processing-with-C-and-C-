/****************************************************************************
 * file - ip.h                                                              *
 ****************************************************************************/

/* typedefs */
//image 포인터 double 포인터 설정
typedef unsigned char *image_ptr;
typedef double *double_ptr;
//rgb pixel구조체
typedef struct
    {
    unsigned char r,g,b;
    } pixel;

typedef pixel *pixel_ptr;
//mesh 구조체
typedef struct
    {
    int width;
    int height;
    float *x_data;
    float *y_data;
    } mesh;

typedef struct
    {
    double re;
    double im;
    } COMPLEX;

typedef COMPLEX *complex_ptr;

typedef struct
    {
    int x;
    int y;
    } POINT;

typedef struct 
    {
    POINT P;
    POINT Q;
    int dx, dy;
    float length;
    long length_squared;
    } LINE;

typedef struct 
    {
    POINT P;
    POINT Q;
    } LINE_SEGMENT;

typedef struct 
    {
    int number;             /* number of segments to follow */
    LINE_SEGMENT line[100];
    char *filename; /* name of file holding the line list */
    } LINE_LIST;

/* defines */

#define PI   3.14159265358979323846 //파이
#define CLIP(val, low, high) {if(val<low) val=0; if(val>high) val=0;} //LUT용 일정값이상 이하 값 설정 함수
#define CLAMP(val, low, high) ((val<low) ? low : ((val>high) ? high : val))
#define MAX(A,B)        ((A) > (B) ? (A) : (B)) //큰 값을 찾는 MAX
#define MIN(A,B)        ((A) < (B) ? (A) : (B)) //작은 값을 찾는 MIN
#define IP_MALLOC(X) malloc(X) //메모리 할당
#define IP_FREE(X) free(X) //메모리 해제
#define PBM 4 //PBM 파일을 위한 번호
#define PGM 5 //PGM 파일을 위한 번호
#define PPM 6 //PPM 파일을 위한 번호

