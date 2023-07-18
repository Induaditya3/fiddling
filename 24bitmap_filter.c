// BMP-related data types based on Microsoft's own
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/**
 * Common Data Types
 *
 * The data types in this section are essentially aliases for C/C++
 * primitive data types.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.
 */
typedef struct
{
    WORD   bfType;
    DWORD  bfSize;
    WORD   bfReserved1;
    WORD   bfReserved2;
    DWORD  bfOffBits;
} __attribute__((__packed__))
BITMAPFILEHEADER;

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.
 */
typedef struct
{
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} __attribute__((__packed__))
BITMAPINFOHEADER;

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */
typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;

// Sobel kernels
int Gx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
int Gy[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};

//function for applying sobel operator
float* loop(int height,int width,int i,int j,int timesi,int timesj,int sobeli,int sobelj, RGBTRIPLE arr[height][width])
{
    int sumxb=0;
    int sumxg=0;
    int sumxr=0;

    int sumyb=0;
    int sumyg=0;
    int sumyr=0;

    int Si=sobeli;
    int Sj=sobelj;
    for(int ii=i;ii<i+timesi;ii++)
    {
        for (int jj=j;jj<j+timesj;jj++)
        {
            //applying Gx kernel on RGB channels
            sumxb+=Gx[Si][Sj]*(arr[ii][jj].rgbtBlue);
            sumxg+=Gx[Si][Sj]*(arr[ii][jj].rgbtGreen);
            sumxr+=Gx[Si][Sj]*(arr[ii][jj].rgbtRed);

            //applying Gy kernel on RGB channels
            sumyb+=Gy[Si][Sj]*(arr[ii][jj].rgbtBlue);
            sumyg+=Gy[Si][Sj]*(arr[ii][jj].rgbtGreen);
            sumyr+=Gy[Si][Sj]*(arr[ii][jj].rgbtRed);

            Sj++;
        }
        Si++;
        Sj=sobelj;
    }
    float red=sqrt(pow(sumyr,2)+pow(sumxr,2));
    float green=sqrt(pow(sumyg,2)+pow(sumxg,2));
    float blue=sqrt(pow(sumyb,2)+pow(sumxb,2));

    static float rgb[3];
    rgb[0]=red;
    rgb[1]=green;
    rgb[2]=blue;

    return rgb;
}
BYTE limit(float x)
{
    if (x>255)
    {
        x=255;
        return x;
    }
    x= round(x);
    return x;
}


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE Blue,Green,Red,avg;
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            Blue=image[i][j].rgbtBlue;
            Green=image[i][j].rgbtGreen;
            Red=image[i][j].rgbtRed;
            avg=round((Red+Green+Blue)/3.0);
            image[i][j].rgbtBlue=avg;
            image[i][j].rgbtGreen=avg;
            image[i][j].rgbtRed=avg;

        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int n=(int)(width/2);
    //Iterate over rows
    for(int i=0;i<height;i++)
    {
        //iterate over columns
      for(int j=0;j<n;j++)
      {
        BYTE tempBlue=image[i][width-j-1].rgbtBlue;
        image[i][width-j-1].rgbtBlue=image[i][j].rgbtBlue;
        image[i][j].rgbtBlue=tempBlue;

        BYTE tempGreen=image[i][width-j-1].rgbtGreen;
        image[i][width-j-1].rgbtGreen=image[i][j].rgbtGreen;
        image[i][j].rgbtGreen=tempGreen;

        BYTE tempRed=image[i][width-j-1].rgbtRed;
        image[i][width-j-1].rgbtRed=image[i][j].rgbtRed;
        image[i][j].rgbtRed=tempRed;
      }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE Blue,Green,Red;
    RGBTRIPLE copy[height][width];
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            //corner cases
            if(i==0 && j==0)
            {
                Blue=(BYTE)round((image[0][0].rgbtBlue+image[0][1].rgbtBlue+image[1][0].rgbtBlue+image[1][1].rgbtBlue)/4.0);
                Green=(BYTE)round((image[0][0].rgbtGreen+image[0][1].rgbtGreen+image[1][0].rgbtGreen+image[1][1].rgbtGreen)/4.0);
                Red=(BYTE)round((image[0][0].rgbtRed+image[0][1].rgbtRed+image[1][0].rgbtRed+image[1][1].rgbtRed)/4.0);
            }
            else if (i==height-1 && j==0)
            {
                Blue=(BYTE)round((image[height-1][0].rgbtBlue+image[height-1][1].rgbtBlue+image[height-1-1][1].rgbtBlue+image[height-1-1][0].rgbtBlue)/4.0);
                Green=(BYTE)round((image[height-1][0].rgbtGreen+image[height-1][1].rgbtGreen+image[height-1-1][1].rgbtGreen+image[height-1-1][0].rgbtGreen)/4.0);
                Red=(BYTE)round((image[height-1][0].rgbtRed+image[height-1][1].rgbtRed+image[height-1-1][1].rgbtRed+image[height-1-1][0].rgbtRed)/4.0);
            }
            else if (i==0 && j==width-1)
            {
                Blue=(BYTE)round((image[0][width-1].rgbtBlue+image[0][width-1-1].rgbtBlue+image[1][width-1-1].rgbtBlue+image[1][width-1].rgbtBlue)/4.0);
                Green=(BYTE)round((image[0][width-1].rgbtGreen+image[0][width-1-1].rgbtGreen+image[1][width-1-1].rgbtGreen+image[1][width-1].rgbtGreen)/4.0);
                Red=(BYTE)round((image[0][width-1].rgbtRed+image[0][width-1-1].rgbtRed+image[1][width-1-1].rgbtRed+image[1][width-1].rgbtRed)/4.0);
            }
            else if (i==height-1 && j==width-1)
            {
                Blue=(BYTE)round((image[height-1][width-1].rgbtBlue+image[height-1][width-1-1].rgbtBlue+image[height-1-1][width-1-1].rgbtBlue+image[height-1-1][width-1].rgbtBlue)/4.0);
                Green=(BYTE)round((image[height-1][width-1].rgbtGreen+image[height-1][width-1-1].rgbtGreen+image[height-1-1][width-1-1].rgbtGreen+image[height-1-1][width-1].rgbtGreen)/4.0);
                Red=(BYTE)round((image[height-1][width-1].rgbtRed+image[height-1][width-1-1].rgbtRed+image[height-1-1][width-1-1].rgbtRed+image[height-1-1][width-1].rgbtRed)/4.0);
            }
            //edge cases
            else if (i==0)
            {
                Blue=(BYTE)round((image[0][j-1].rgbtBlue+image[0][j].rgbtBlue+image[0][j+1].rgbtBlue+image[1][j-1].rgbtBlue+image[1][j].rgbtBlue+image[1][j+1].rgbtBlue)/6.0);
                Green=(BYTE)round((image[0][j-1].rgbtGreen+image[0][j].rgbtGreen+image[0][j+1].rgbtGreen+image[1][j-1].rgbtGreen+image[1][j].rgbtGreen+image[1][j+1].rgbtGreen)/6.0);
                Red=(BYTE)round((image[0][j-1].rgbtRed+image[0][j].rgbtRed+image[0][j+1].rgbtRed+image[1][j-1].rgbtRed+image[1][j].rgbtRed+image[1][j+1].rgbtRed)/6.0);
            }
            else if (i==height-1)
            {
                Blue=(BYTE)round((image[height-1][j-1].rgbtBlue+image[height-1][j].rgbtBlue+image[height-1][j+1].rgbtBlue+image[height-1-1][j-1].rgbtBlue+image[height-1-1][j].rgbtBlue+image[height-1-1][j+1].rgbtBlue)/6.0);
                Green=(BYTE)round((image[height-1][j-1].rgbtGreen+image[height-1][j].rgbtGreen+image[height-1][j+1].rgbtGreen+image[height-1-1][j-1].rgbtGreen+image[height-1-1][j].rgbtGreen+image[height-1-1][j+1].rgbtGreen)/6.0);
                Red=(BYTE)round((image[height-1][j-1].rgbtRed+image[height-1][j].rgbtRed+image[height-1][j+1].rgbtRed+image[height-1-1][j-1].rgbtRed+image[height-1-1][j].rgbtRed+image[height-1-1][j+1].rgbtRed)/6.0);
            }
            else if (j==0)
            {
                Blue=(BYTE)round((image[i-1][0].rgbtBlue+image[i-1][1].rgbtBlue+image[i][1].rgbtBlue+image[i+1][1].rgbtBlue+image[i+1][0].rgbtBlue+image[i][0].rgbtBlue)/6.0);
                Green=(BYTE)round((image[i-1][0].rgbtGreen+image[i-1][1].rgbtGreen+image[i][1].rgbtGreen+image[i+1][1].rgbtGreen+image[i+1][0].rgbtGreen+image[i][0].rgbtGreen)/6.0);
                Red=(BYTE)round((image[i-1][0].rgbtRed+image[i-1][1].rgbtRed+image[i][1].rgbtRed+image[i+1][1].rgbtRed+image[i+1][0].rgbtRed+image[i][0].rgbtRed)/6.0);
            }
            else if (j==width-1)
            {
                Blue=(BYTE)round((image[i][width-1].rgbtBlue+image[i-1][width-1].rgbtBlue+image[i+1][width-1].rgbtBlue+image[i][width-1-1].rgbtBlue+image[i-1][width-1-1].rgbtBlue+image[i+1][width-1-1].rgbtBlue)/6.0);
                Green=(BYTE)round((image[i][width-1].rgbtGreen+image[i-1][width-1].rgbtGreen+image[i+1][width-1].rgbtGreen+image[i][width-1-1].rgbtGreen+image[i-1][width-1-1].rgbtGreen+image[i+1][width-1-1].rgbtGreen)/6.0);
                Red=(BYTE)round((image[i][width-1].rgbtRed+image[i-1][width-1].rgbtRed+image[i+1][width-1].rgbtRed+image[i][width-1-1].rgbtRed+image[i-1][width-1-1].rgbtRed+image[i+1][width-1-1].rgbtRed)/6.0);
            }
            //catch all
            else
            {
                Blue=(BYTE)round((image[i-1][j].rgbtBlue+image[i][j].rgbtBlue+image[i+1][j].rgbtBlue+image[i-1][j-1].rgbtBlue+image[i][j-1].rgbtBlue+image[i+1][j-1].rgbtBlue+image[i-1][j+1].rgbtBlue+image[i][j+1].rgbtBlue+image[i+1][j+1].rgbtBlue)/9.0);
                Green=(BYTE)round((image[i-1][j].rgbtGreen+image[i][j].rgbtGreen+image[i+1][j].rgbtGreen+image[i-1][j-1].rgbtGreen+image[i][j-1].rgbtGreen+image[i+1][j-1].rgbtGreen+image[i-1][j+1].rgbtGreen+image[i][j+1].rgbtGreen+image[i+1][j+1].rgbtGreen)/9.0);
                Red=(BYTE)round((image[i-1][j].rgbtRed+image[i][j].rgbtRed+image[i+1][j].rgbtRed+image[i-1][j-1].rgbtRed+image[i][j-1].rgbtRed+image[i+1][j-1].rgbtRed+image[i-1][j+1].rgbtRed+image[i][j+1].rgbtRed+image[i+1][j+1].rgbtRed)/9.0);
            }
            copy[i][j].rgbtBlue=Blue;
            copy[i][j].rgbtGreen=Green;
            copy[i][j].rgbtRed=Red;
        }
    }
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            image[i][j]=copy[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE Blue,Green,Red;
    RGBTRIPLE edge[height][width];
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            float *rgb;
            rgb = (float*) malloc(3 * sizeof(float));
            //corner cases
            if(i==0 && j==0)
            {
                rgb=loop(height,width,i,j,2,2,1,1,image);
                Blue=(BYTE) limit(rgb[2]);
                Green=(BYTE) limit(rgb[1]);
                Red=(BYTE) limit(rgb[0]);
            }
            else if (i==height-1 && j==0)
            {
                rgb=loop(height,width,i-1,j,2,2,0,1,image);
                Blue=(BYTE) limit(rgb[2]);
                Green=(BYTE) limit(rgb[1]);
                Red=(BYTE) limit(rgb[0]);
            }
            else if (i==0 && j==width-1)
            {
                rgb=loop(height,width,i,j-1,2,2,1,0,image);
                Blue=(BYTE) limit(rgb[2]);
                Green=(BYTE) limit(rgb[1]);
                Red=(BYTE) limit(rgb[0]);
            }
            else if (i==height-1 && j==width-1)
            {
                rgb=loop(height,width,i-1,j-1,2,2,0,0,image);
                Blue=(BYTE) limit(rgb[2]);
                Green=(BYTE) limit(rgb[1]);
                Red=(BYTE) limit(rgb[0]);
            }
            //edge cases
            else if (i==0)
            {
                rgb=loop(height,width,i,j-1,2,3,1,0,image);
                Blue=(BYTE) limit(rgb[2]);
                Green=(BYTE) limit(rgb[1]);
                Red=(BYTE) limit(rgb[0]);
            }
            else if (i==height-1)
            {
                rgb=loop(height,width,i-1,j-1,2,3,0,0,image);
                Blue=(BYTE) limit(rgb[2]);
                Green=(BYTE) limit(rgb[1]);
                Red=(BYTE) limit(rgb[0]);
            }
            else if (j==0)
            {
                rgb=loop(height,width,i-1,j,3,2,0,1,image);
                Blue=(BYTE) limit(rgb[2]);
                Green=(BYTE) limit(rgb[1]);
                Red=(BYTE) limit(rgb[0]);
            }
            else if (j==width-1)
            {
                rgb=loop(height,width,i-1,j-1,3,2,0,0,image);
                Blue=(BYTE) limit(rgb[2]);
                Green=(BYTE) limit(rgb[1]);
                Red=(BYTE) limit(rgb[0]);
            }
            //catch all
            else
            {
                rgb=loop(height,width,i-1,j-1,3,3,0,0,image);
                Blue=(BYTE) limit(rgb[2]);
                Green=(BYTE) limit(rgb[1]);
                Red=(BYTE) limit(rgb[0]);
            }
            edge[i][j].rgbtBlue=Blue;
            edge[i][j].rgbtGreen=Green;
            edge[i][j].rgbtRed=Red;
        }
    }
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            image[i][j]=edge[i][j];
        }
    }
}

void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float Blue,Green,Red;
    RGBTRIPLE copy[height][width];
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            Blue=image[i][j].rgbtBlue;
            Green=image[i][j].rgbtGreen;
            Red=image[i][j].rgbtRed;
            copy[i][j].rgbtBlue=limit((0.272 *Red + 0.534 * Green + 0.131 *Blue));
            copy[i][j].rgbtGreen=limit((0.349 * Red + 0.686 * Green + 0.168 * Blue));
            copy[i][j].rgbtRed=limit((0.393 *Red + 0.769 *Green + 0.189 *Blue));

        }
    }
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            image[i][j]=copy[i][j];
        }
    }
}

int main(int argc, char *argv[])
{
    // Define allowable filters
    char *filters = "begrs";

    // Get filter flag and check validity
    char filter = getopt(argc, argv, filters);
    if (filter == '?')
    {
        printf("Invalid filter.\n");
        return 1;
    }

    // Ensure only one filter
    if (getopt(argc, argv, filters) != -1)
    {
        printf("Only one filter allowed.\n");
        return 2;
    }

    // Ensure proper usage
    if (argc != optind + 2)
    {
        printf("Usage: ./filter [flag] infile outfile\n");
        return 3;
    }

    // Remember filenames
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 5;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 6;
    }

    // Get image's dimensions
    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // Allocate memory for image
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // Skip over padding
        fseek(inptr, padding, SEEK_CUR);
    }

    // Filter image
    switch (filter)
    {
        // Blur
        case 'b':
            blur(height, width, image);
            break;

        // Edges
        case 'e':
            edges(height, width, image);
            break;

        // Grayscale
        case 'g':
            grayscale(height, width, image);
            break;

        // Reflect
        case 'r':
            reflect(height, width, image);
            break;
        // Sepia
        case 's':
            sepia(height, width, image);
            break;
    }

    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Write new pixels to outfile
    for (int i = 0; i < height; i++)
    {
        // Write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // Free memory for image
    free(image);

    // Close files
    fclose(inptr);
    fclose(outptr);
    return 0;
}
