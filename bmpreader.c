//
//  main.c
//  bmpreader
//
//  Created by Nolan Miller on 10/25/12.
//  Copyright (c) 2012 Nolan Miller. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define READ4BYTESFROM(X) fgetc((X)) | fgetc((X))<<8 | fgetc((X))<<16 | fgetc((X))<<24
#define READ2BYTESFROM(X) fgetc((X)) | fgetc((X))<<8


typedef struct pix {
    int red;
    int blue;
    int yellow;
    int fourth;
    unsigned int arrayval;
}pixle;
#define RED 0
#define BLUE 1
#define YELLOW 2
#define VAL 3
#define FOURTH 4

typedef struct bmp {
    // bmp file headder
    char signature[2];      //0x0   (2 bytes)
    unsigned int size;      //0x2   (4 bytes)
    unsigned int imgaddress;//0xA   (4 bytes)
    // second headder
    unsigned int headersize;//0xE   (4 bytes)
    int width;              //0x12  (4 bytes)
    int height;             //0x16  (4 bytes)
    short int bitsperpixle; //0x1C  (2 bytes)
    //data
    //some sort of pallat data...
    pixle** pixlearray;
    // pixlearray[width][height]
    
}bmpfile;

int readinbmp(FILE*,bmpfile*);
void printbmp(bmpfile*,int);
void reducerange(bmpfile*);
void setcolors(bmpfile*);

int main(int argc, const char * argv[])
{

    // insert code here...
    FILE* fin = fopen("./bmptest.bmp", "rb");
    bmpfile filein;
    
    readinbmp(fin, &filein);
    
    printf("length: %u\n",filein.size);
    printf("offset: %u\n",filein.imgaddress);
    printf("size:   %i x %i\n",filein.height,filein.width);
    printf("bpp:    %hi\n",filein.bitsperpixle);
    
//    reducerange(&filein);
    setcolors(&filein);
    printf("blue:\n");
    printbmp(&filein,BLUE);
    printf("yellow:\n");
    printbmp(&filein,YELLOW);
    printf("red:\n");
    printbmp(&filein,RED);
    printf("fourth\n");
    printbmp(&filein,FOURTH);
    printbmp(&filein,VAL);
    
    
    fclose(fin);

    
    return 0;
}


int readinbmp(FILE* fin,bmpfile* filein){
    int i,ii;
    filein->signature[0] = fgetc(fin);
    filein->signature[1] = fgetc(fin);
    filein->size =READ4BYTESFROM(fin);
    
    fseek(fin, 0XA, SEEK_SET);
    filein->imgaddress = READ4BYTESFROM(fin);
    
    filein->headersize = READ2BYTESFROM(fin);
    
    
    //      BITMAPINFOHEADER (nearly ubiquitous)
    if (filein->headersize == 40) {
        fseek(fin, 0x12, SEEK_SET);
        filein->width =      READ4BYTESFROM(fin);
        filein->height =     READ4BYTESFROM(fin);
        
        fseek(fin, 0x1C, SEEK_SET);
        filein->bitsperpixle = READ2BYTESFROM(fin);
    }
    
    //      OS/2 BITMAPCOREHEADER (sometimes used)
    else if (filein->headersize == 12) {
        fseek(fin, 0x12, SEEK_SET);
        filein->width =      (unsigned int)READ2BYTESFROM(fin);
        filein->height =     (unsigned int)READ2BYTESFROM(fin);
        
        fseek(fin, 0x18, SEEK_SET);
        filein->bitsperpixle = READ2BYTESFROM(fin);
    }
    
    else if (filein->headersize == 64){
        printf("BITMAPCOREHEADER2 not supported");
        printf("\nData aquisition failed.");
        return 1;
    }
    else {
        printf("Header size:%i not supported.",filein->headersize);
        return 2;
    }
    
    fseek(fin, filein->imgaddress, SEEK_SET);
    if (filein->bitsperpixle == 32) {
        //4 bytes per pixel
        filein->pixlearray = (pixle**)malloc(sizeof(pixle*)*abs(filein->width));
        for (i =0; i<abs(filein->width); i++) {
            filein->pixlearray[i] = (pixle*)malloc(sizeof(pixle)*abs(filein->height));
            for (ii=0; ii<abs(filein->height); ii++) {
                filein->pixlearray[i][ii].arrayval = READ4BYTESFROM(fin);
            }
        }
    }
    
    else if (filein->bitsperpixle == 16) {
        //2 bytes per pixel
        filein->pixlearray = (pixle**)malloc(sizeof(pixle*)*abs(filein->width));
        for (i =0; i<abs(filein->width); i++) {
            filein->pixlearray[i] = (pixle*)malloc(sizeof(pixle)*abs(filein->height));
            for (ii=0; ii<abs(filein->height); ii++) {
                filein->pixlearray[i][ii].arrayval = READ2BYTESFROM(fin);
            }
        }
    }
    else if (filein->bitsperpixle ==8) {
        //1 byte per pixel
        filein->pixlearray = (pixle**)malloc(sizeof(pixle*)*abs(filein->width));
        for (i =0; i<abs(filein->width); i++) {
            filein->pixlearray[i] = (pixle*)malloc(sizeof(pixle)*abs(filein->height));
            for (ii=0; ii<abs(filein->height); ii++) {
                filein->pixlearray[i][ii].arrayval = fgetc(fin);
            }
        }
    }
    else {
        printf("given BPP not supported");
        return 3;
    }
    
    return 0;
}

void setcolors(bmpfile* bmp){
    int i,ii;
    if(bmp->bitsperpixle ==32||bmp->bitsperpixle == 24){
        for(i=0;i<abs(bmp->width);i++){
            for(ii=0;ii<abs(bmp->height);ii++){
                bmp->pixlearray[i][ii].red = ((bmp->pixlearray[i][ii].arrayval) & (0xFF<< 0));
                bmp->pixlearray[i][ii].blue = ((bmp->pixlearray[i][ii].arrayval) & (0xFF<< 8))>>8;
                bmp->pixlearray[i][ii].yellow = ((bmp->pixlearray[i][ii].arrayval) & (0xFF<< 16))>>16;
                bmp->pixlearray[i][ii].fourth = ((bmp->pixlearray[i][ii].arrayval) & (0xFF<< 24))>>24;
            }
        }
    }
    else if(bmp->bitsperpixle == 16){
        for(i=0;i<abs(bmp->width);i++){
            for(ii=0;ii<abs(bmp->height);ii++){
                bmp->pixlearray[i][ii].red = (bmp->pixlearray[i][ii].arrayval) & (0xF<< 0);
                bmp->pixlearray[i][ii].blue = ((bmp->pixlearray[i][ii].arrayval) & (0xF<< 4))>>4;
                bmp->pixlearray[i][ii].yellow = ((bmp->pixlearray[i][ii].arrayval) & (0xF<< 8))>>8;
                bmp->pixlearray[i][ii].fourth = ((bmp->pixlearray[i][ii].arrayval) & (0xF<<12))>>12;
            }
        }
    }
    else
        printf("color mapping not yet supported.");
}

void printbmp(bmpfile* bmp,int type){
    int i,ii;
    for(i = 0; i<abs(bmp->height);i++){
        for (ii = 0; ii<abs(bmp->width); ii++) {
            switch(type){
                case RED:
                printf("%x ",bmp->pixlearray[i][ii].red);
                break;
                case BLUE:
                printf("%x ",bmp->pixlearray[i][ii].blue);
                break;
                case YELLOW:
                printf("%x ",bmp->pixlearray[i][ii].yellow);
                break;
                case VAL:
                printf("%x ",bmp->pixlearray[i][ii].arrayval);
                break;
                case FOURTH:
                printf("%x ",bmp->pixlearray[i][ii].fourth);
                break;
                default:
                printf("failure");
                break;
            }
        }
        printf("\n");
    }
}

void reducerange(bmpfile* bmp){
    unsigned int old;
    int k =0;
    int i,ii;
    int l,ll;
    for(i = 0; i<abs(bmp->height);i++){
        for (ii = 0; ii<abs(bmp->width); ii++) {
            if ((old = bmp->pixlearray[i][ii].arrayval)>abs(bmp->height*bmp->width)) {
                k++;
                for(l = i; l<abs(bmp->height);l++){
                    for (ll = ii; ll<abs(bmp->width); ll++) {
                        if (bmp->pixlearray[l][ll].arrayval == old) {
                            bmp->pixlearray[l][ll].arrayval = k;
                        }
                    }
                }
            }
        }
    }
}
