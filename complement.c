#include <stdio.h>
#include <stdlib.h>

typedef struct header //header construction
{
    char name[2];
    unsigned int size;
    unsigned int skip;
    unsigned int pixel_start;
} header;

typedef struct info_header //header information construction
{
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short int plane;
    unsigned short int depth;
    unsigned int compression;
    unsigned int pic_size;
    unsigned int horizontal;
    unsigned int vertical;
    unsigned int color;
    unsigned int important_color;
} info_header;

typedef struct color_table //color table construction
{
    unsigned char data1;
    unsigned char data2;
    unsigned char data3;
    unsigned char data4;
} color_table;

int main()
{
    FILE* image = fopen("teduid.bmp","rb"); //opening the bmp file

    struct header new_header;
    struct info_header new_infoheader;
    struct color_table colorTable;

    fread(new_header.name, 2, 1, image); //reading data from header
    fread(&new_header.size, 3*sizeof(unsigned int), 1, image);
    fread(&new_infoheader.size, sizeof(info_header), 1, image); //reading data from header information

    if (new_infoheader.depth <= 8) { //if bit depth is less than 8 (which is the case, since image is monochrome)
        fread(&colorTable, sizeof(colorTable), 1, image); //reading data from color table
        fseek(image,new_header.pixel_start,SEEK_SET); //positioning the cursor to the start of the pixel array

        unsigned char** pixel_array = (unsigned char**)malloc(new_infoheader.height*sizeof(unsigned char*)); //dynamically allocating space for pixel array
        for(int i = 0; i<new_infoheader.height; i++)
            pixel_array[i] = (unsigned char*)malloc(sizeof(unsigned char)*new_infoheader.width);
        unsigned char value; //represents the pixel value read from original image

        for (int i = 0; i < new_infoheader.height; i++) {
            for (int j = 0; j < new_infoheader.width; j++) {
                fread(&value,sizeof(unsigned char),1,image); //reading the pixel value
                pixel_array[i][j] = 255-value; //taking the complement of the pixel value
            }
        }

        FILE* out = fopen("Q4.bmp","w"); //creating the output image
        fwrite(new_header.name, 2, 1, out); //writing the same header
        fwrite(&new_header.size, 3*sizeof(unsigned int), 1, out);
        fwrite(&new_infoheader,sizeof(info_header),1,out); //writing the same header information
        fwrite(&colorTable,sizeof(color_table),1,out); //writing the same color table

        for (int i = 0; i < new_infoheader.height; i++) {
            for (int j = 0; j < new_infoheader.width; j++) {
                fwrite(&pixel_array[i][j],sizeof(unsigned char),1,out); //writing the created pixel array
            }
        }

        for(int i = 0; i<new_infoheader.width; i++) free(pixel_array[i]); //deallocating the space reserved for pixel array
        free(pixel_array);
        fclose(image); //closing the files
        fclose(out); }

    return 0;
}
