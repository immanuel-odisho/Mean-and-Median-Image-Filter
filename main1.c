#include <stdio.h>
#include "filter.h"
#include <stdlib.h>
#include <time.h>

int cmpfunc (const void * a, const void * b);
int main(int argc, char *argv[])
{
	clock_t  t,y,z;
	if (argc != 5)
	{
		printf("Error, Usage: denoise input_file output_file N F"); 
		return 0; 
	}
	
	
	int *width, *height, *max;
	width = malloc(sizeof(int)*10);
	height = malloc(sizeof(int)*10);
	max = malloc(sizeof(int)*3);			
	RGB *readfile;
	y = clock();
	printf("Reading file %s\n", argv[1]);
	readfile = readPPM(argv[1],width,height,max);
	y = clock() - y;
	double timetakey = ((double)y)/CLOCKS_PER_SEC;
	printf("*** image read in %1.1e seconds\n", timetakey);

	

	RGB *final;
	

	if (argv[4][0] == 'A')
	{
	printf("processing %d x %d image using %d window and mean filter...\n",*width,*height,atoi(argv[3]));
	t= clock();
	final = denoiseImage(*width,*height,readfile,atoi(argv[3]), 0); 
	t = clock() - t;
	double timetaket = ((double)t)/CLOCKS_PER_SEC;
	printf("*** image processed in %1.1e seconds\n", timetaket);	
	z = clock();
	printf("Writting file %s\n", argv[1]);
	writePPM(argv[2], *width, *height, *max, final);
	z = clock() - z;
	double timetakez = ((double)z)/CLOCKS_PER_SEC;
	printf("*** image written in %1.1e seconds\n", timetakez);

	}	
	else if (argv[4][0] == 'M')
	{
	printf("processing %d x %d image using %d window and median filter...\n",*width,*height,atoi(argv[3]));
	t = clock();
	final = denoiseImage(*width,*height,readfile,atoi(argv[3]), 1); 
	t = clock() - t;
	double timetaket = ((double)t)/CLOCKS_PER_SEC;
	printf("*** image processed in %1.1e seconds\n", timetaket);
	z = clock();
	printf("Writting file %s\n", argv[1]);
	writePPM(argv[2], *width, *height, *max, final);
	z = clock() - z;
	double timetakez = ((double)z)/CLOCKS_PER_SEC;
	printf("*** image written in %1.1e seconds\n", timetakez);

	}



	return 0;





}


RGB *readPPM(const char *file, int *width, int *height, int *max)
{
    FILE *fp;
    int  i = 0,tempw, temph, tempm;
    char pass[3];

    if((fp = fopen(file, "r")) == NULL)
    {
        printf("Error:file doesnt exist");
    }

    fgets(pass, sizeof(pass), fp);

    fscanf(fp, "%d %d %d", &tempw, &temph, &tempm);

    int numofPix = tempw * temph;

    RGB *pixels = malloc(numofPix * sizeof(RGB));



    while(fscanf(fp, "%hhd %hhd %hhd", &pixels[i].r, &pixels[i].g, &pixels[i].b) == 3)
    {
        i++;


    }

    fclose(fp);
 *max = tempm;
    *width = tempw;
    *height = temph;
   
    return pixels;
}



void writePPM(const char *file, int width, int height, int max, const RGB *image)
{
    FILE *fp;

    int size = width * height, i;
    fp = fopen(file, "w");

    fprintf(fp, "P3\n%d %d\n%d\n", width,height,max);
    /*runs through the code*/
    for(i = 0;i<size;i++)
    {
        fprintf(fp, "%d %d %d ",image[i].r, image[i].g, image[i].b);
    }


    fclose(fp);
}


RGB *denoiseImage(int width, int height, const RGB *image, int n, filter f)
{
	if (f == MEAN)
	{

	
	int j,i,x, index,count = 0,sumR = 0,sumG = 0,sumB = 0;
	RGB *noise;
	noise = malloc(sizeof(RGB)*(width*height));


	for (j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			count = 0, sumR = 0, sumG = 0, sumB = 0;
			index = i + (j * width);
			sumB += image[index].b;
			sumR += image[index].r;
			sumG += image[index].g;
			count++;

		for (x =1; x <= (n-1/2); x++)
		{
			if (j + x <= height)
			{
			sumB += image[index+(x*width)].b;
			sumR += image[index+(x*width)].r;
			sumG += image[index+(x*width)].g;				
			count++;
			}

			if (j - x >= 0)
			{
			sumB += image[index-(x*width)].b;
			sumR += image[index-(x*width)].r;
			sumG += image[index-(x*width)].g;			
			count++;
			}

			if ((j + x ) <= height && (i + x) <= width)
			{
			sumB += image[index+(x*width)+x].b;
			sumR += image[index+(x*width)+x].r;
			sumG += image[index+(x*width)+x].g;			
			count++;
			}

			if ((j - x ) >= 0 && (i - x) >= 0)
			{
			sumB += image[index-(x*width)-x].b;
			sumR += image[index-(x*width)-x].r;
			sumG += image[index-(x*width)-x].g;			
			count++;
			}

			if (j + x <= height)
			{
			noise[index+(x*width)].b = sumB/count;
			noise[index+(x*width)].g = sumB/count;
			noise[index+(x*width)].r = sumB/count;
			}

			if (j - x >= 0)
			{
			noise[index-(x*width)].b = sumB/count;
			noise[index-(x*width)].r = sumR/count;
			noise[index-(x*width)].g = sumG/count;
			}



			if ((j + x ) <= height && (i + x) <= width)
			{
			noise[index+(x*width)+x].b = sumB/count;
			noise[index+(x*width)+x].r = sumR/count;
			noise[index+(x*width)+x].g = sumG/count;			
			}

			if ((j - x ) >= 0 && (i - x) >= 0)
			{
			noise[index-(x*width)-x].b = sumB/count;
			noise[index-(x*width)-x].r = sumR/count;
			noise[index-(x*width)-x].g = sumG/count;			
			}


		}




		}	
		



	}
	return noise;
	}



else 
{

	int arrR[n*n];
	int arrG[n*n];
	int arrB[n*n];
		int j,i,x, index,count,medianG, medianB,medianR;
	RGB *noise;
	noise = malloc(sizeof(RGB)*(width*height));


	for (j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			count = 0;
			index = i + (j * width);
			arrB[count] = image[index].b;
			arrR[count] = image[index].r;
			arrG[count] = image[index].g;
			count++;

		for (x =1; x <= (n-1/2); x++)
		{
			if (j + x <= height)
			{
			arrB[count] = image[index+(x*width)].b;
			arrR[count] = image[index+(x*width)].r;
			arrG[count] = image[index+(x*width)].g;
			count++;
			}

			if (j - x >= 0)
			{
			arrB[count] = image[index-(x*width)].b;
			arrR[count] = image[index-(x*width)].r;
			arrG[count] = image[index-(x*width)].g;
			count++;
			}

			if ((j + x ) <= height && (i + x) <= width)
			{
			arrB[count] = image[index+(x*width)+x].b;
			arrR[count] = image[index+(x*width)+x].r;
			arrG[count] = image[index+(x*width)+x].g;
			count++;
			}

			if ((j - x ) >= 0 && (i - x) >= 0)
			{
			arrB[count] = image[index-(x*width)-x].b;
			arrR[count] = image[index-(x*width)-x].r;
			arrG[count] = image[index-(x*width)-x].g;
			count++;
			}

			qsort(arrG, count, sizeof(int), cmpfunc);
			qsort(arrR, count, sizeof(int), cmpfunc);
			qsort(arrB, count, sizeof(int), cmpfunc);

			if (count % 2 == 0)
			{
				medianG = (arrG[(count/2)]+arrG[(count/2)-1])/2;
				medianB = (arrB[(count/2)]+arrB[(count/2)-1])/2;
				medianR = (arrR[(count/2)]+arrR[(count/2)-1])/2;
			}
			else
			{
				medianG= arrG[(count-1)/2];
				medianB= arrB[(count-1)/2];
				medianR = arrR[(count-1)/2];			
			}
			



			if (j + x <= height)
			{
			noise[index+(x*width)].b = medianB;
			noise[index+(x*width)].g = medianG;
			noise[index+(x*width)].r = medianR;
			}

			if (j - x >= 0)
			{
			noise[index-(x*width)].b = medianB;
			noise[index-(x*width)].r = medianR;
			noise[index-(x*width)].g = medianG;
			}



			if ((j + x ) <= height && (i + x) <= width)
			{
			noise[index+(x*width)+x].b = medianB;
			noise[index+(x*width)+x].r = medianR;
			noise[index+(x*width)+x].g = medianG;
			}

			if ((j - x ) >= 0 && (i - x) >= 0)
			{
			noise[index-(x*width)-x].b = medianB;
			noise[index-(x*width)-x].r = medianR;
			noise[index-(x*width)-x].g = medianG;
			}
	

		}




		}	
}

return noise;
}

}





int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}