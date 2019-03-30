/**************************************************************

	The program reads an BMP image file and creates a new
	image that is the negative of the input file.

**************************************************************/
#include <pthread.h>
#include "qdbmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
UINT	width, height;
pthread_mutex_t lock;
int pix_per_thread=0;
BMP*	bmp;
BMP*    bmp1;
//build the struct to store the necessary info
typedef struct pixel pixel;
struct pixel{
  int boxsize;
  int start;
  int end;
};
//blur the image
void* blur(void *argu){
    UCHAR	r, g, b;
	UINT	x, y;
    pixel*a=(pixel*)argu;
    int xmin=0;
    int xmax=0;
    int ymin=0;
    int ymax=0;
    int count=0;
    int rsum=0;
    int gsum=0;
    int bsum=0;
    int rsum_aver=0;
    int gsum_aver=0;
    int bsum_aver=0;
    //get the boxsize, start x value and end x value
    int boxsize=a->boxsize;
    int start_place=a->start;
    int end_place=a->end;
//blur the pixel based on the x value
    for ( x = start_place ; x <=end_place ; ++x )
	{
		for ( y = 0 ; y < height ; ++y )
		{ 
           
            rsum=0;
            gsum=0;
            bsum=0;
            rsum_aver=0;
            gsum_aver=0;
            bsum_aver=0;
            count=0;

            
            xmin=x-boxsize;
            if(xmin<=0){
                xmin=0;
            }
            xmax=x+boxsize;
            if(xmax>=width){
                xmax=width-1;
            }
            ymin=y-boxsize;
            if(ymin<=0){
                ymin=0;
            }
            
            ymax=y+boxsize;
            if(ymax>=height){
                ymax=height-1;
            }
            
           int yminrec=ymin;
            while(xmin<=xmax){
                 while(ymin<=ymax){
                  BMP_GetPixelRGB(bmp, xmin, ymin, &r, &g, &b );
                  rsum+=r;
                  gsum+=g;
                  bsum+=b;
                  count++;
                  ymin++;
            }
                xmin++;
                ymin=yminrec;
            }
            rsum_aver=rsum/count;
            gsum_aver=gsum/count;
            bsum_aver=bsum/count;
		
            
			/* Invert RGB values */
			BMP_SetPixelRGB(bmp1, x, y, rsum_aver, gsum_aver, bsum_aver);
         
		}
       
	}
     void* v = NULL;
    //free the malloc 
    free(argu);
    pthread_exit(v);
}

int main( int argc, char* argv[] )
{

    
    int ret=0;
    
	/* Check arguments */
	if ( argc != 5 )
	{
		fprintf( stderr, "Usage: %s <input file> <output file><box size><input thread number>\n", argv[ 0 ] );
		return 0;
	}

	/* Read an image file */
	bmp = BMP_ReadFile( argv[ 1 ] );
	BMP_CHECK_ERROR( stdout, -1 );
    bmp1 = BMP_ReadFile( argv[ 1 ] );
	BMP_CHECK_ERROR( stdout, -1 );
    int boxsize=atoi(argv[3]);
    //check if boxsize and thread_num is right
    if(boxsize<=0){
        printf("error:box size cannot be negative or zero.");
        return 0;
    }
    int thread_num=atoi(argv[4]);
    if(thread_num<=0){
       printf("error:thread number cannot be negative or zero.");
        return 0; 
    }
    
	/* Get image's dimensions */
	width = BMP_GetWidth( bmp );
	height = BMP_GetHeight( bmp );
    
   //get the per thread of the x value
    pix_per_thread=width/thread_num;
    
    pthread_t thread[thread_num];
    void * res[thread_num];
    

    for(int i=0;i<thread_num;i++){
         pixel* info=malloc(sizeof(pixel));
        info->boxsize=boxsize;
        info->start=pix_per_thread*i;
       
        if(i==thread_num-1){
            info->end=width-1;
        }else {info->end=pix_per_thread*(i+1)-1;}
        
   
       ret=pthread_create(&thread[i],NULL,&blur,(void*)info);
      
         if(ret!=0){
             printf("error to create thread");
             return 0;
         }
      
    }
 
    for(int i=0;i<thread_num;i++){
   ret=pthread_join(thread[i],&res[i]);
 	 if(ret!=0){
             printf("error to join thread");
             return 0;
         }
    }
	/* Save result */
	BMP_WriteFile( bmp1, argv[ 2 ] );
	BMP_CHECK_ERROR( stdout, -2 );

    pthread_exit(NULL);
	/* Free all memory allocated for the image */
	BMP_Free( bmp );
    BMP_Free( bmp1 );
   // free(info);
	return 0;
}

