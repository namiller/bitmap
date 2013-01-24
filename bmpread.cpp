/*
NOLAN MILLER
*/

#ifndef BMPREADER
#include "cppbmp.h"
#endif

#ifndef STDIO
#include <stdio.h>
#define STDIO
#endif

#ifndef STDLIB
#include <stdlib.h>
#define STDLIB
#endif



int relprime(int,int);

int main(){
    FILE* fin  = fopen("./bmptest.bmp","rb");
    FILE* fout = fopen("./bmpout.bmp","wb");

    int i,ii;

    printf("here\n");
    bmp opened = bmp(fin);
    printf("here too!\n");
    bmp fun = bmp(100,100);
    
    for(i=0;i<fun.width;i++){
        for(ii=0;ii<fun.height;ii++){
            fun.pixlearray[i][ii].arrayval = ((relprime(i+1,ii+1)!=1)?0xffffffff:0x00000000);
            //0xffffffff - (relprime(i+1,ii+1)*0x11111111 - 0x11111111);
            //((relprime(i+1,ii+1)!=1)?0xffffffff:0x00000000);
        }
    }
    printf("heretoo!\n");
//    setcolors(&fun);
//    print_report(&fun);
//    initBMP_w_h(&newbmp,abs(filein.width),abs(filein.height),32);

//    downsample(&filein,&newbmp);

    fun.print_report();

//    fun.writeBMP(fout);        
    opened.writeBMP(fout);
//    dalloc(&newbmp);
//    dalloc(&filein);

    fclose(fout);
    fclose(fin);
    
    return 0;
}

int relprime(int a,int b){
    while(1){
    if(a>b)
        a-=b;
    if(b>a)
        b-=a;
    if(a==b)
        return a;
    }
}
