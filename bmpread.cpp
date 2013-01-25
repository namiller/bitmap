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
    bmp fun = bmp(16,16);
    
    for(i=0;i<fun.width;i++){
        for(ii=0;ii<fun.height;ii++){
//            fun.pixlearray[i][ii].setArrayval(((relprime(i+1,ii+1)!=1)?0xffffffff:0x00000000));
              fun.pixlearray[i][ii].setBlue(16*i);
              fun.pixlearray[i][ii].setRed(16*ii);
              fun.pixlearray[i][ii].setGreen(0x00);
              fun.pixlearray[i][ii].setFourth(0xff);
        }
    }
    printf("heretoo!\n");
//    setcolors(&fun);
//    print_report(&fun);
//    initBMP_w_h(&newbmp,abs(filein.width),abs(filein.height),32);


    fun.print_report();
    fun.writeBMP(fout);

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
