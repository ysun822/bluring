/**************************************************************

	The program reads an BMP image file and creates a new
	image that is the negative of the input file.

**************************************************************/

#include "qdbmp.h"
#include <stdio.h>
#include <stdlib.h>
/* Creates a negative image of the input bitmap file */
int main( int argc, char* argv[] )
{
	UCHAR	r, g, b;
	UINT	width, height;
	UINT	x, y;
	BMP*	bmp;
    BMP*    bmp1;
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
	/* Check arguments */
	if ( argc != 4 )
	{
		fprintf( stderr, "Usage: %s <input file> <output file><box size>\n", argv[ 0 ] );
		return 0;
	}

	/* Read an image file */
	bmp = BMP_ReadFile( argv[ 1 ] );
	BMP_CHECK_ERROR( stdout, -1 );

    bmp1=BMP_ReadFile(argv[1]);
    BMP_CHECK_ERROR( stdout, -1 );
    //check the boxsize
    int boxsize=atoi(argv[3]);
    if(boxsize<=0){
        printf("error:box size cannot be negative or zero.");
        return 0;
    }
	/* Get image's dimensions */
	width = BMP_GetWidth( bmp );
	height = BMP_GetHeight( bmp );
 
    

    
   
	/* Iterate through all the image's pixels */
	for ( x = 0 ; x < width ; ++x )
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
            //record the ymin 
           int yminrec=ymin;
            //calculate the color of each pixel
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

	/* Save result */
	BMP_WriteFile( bmp1, argv[ 2 ] );
	BMP_CHECK_ERROR( stdout, -2 );


	/* Free all memory allocated for the image */
	BMP_Free( bmp );
    BMP_Free( bmp1 );

	return 0;
}

