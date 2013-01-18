/*
NOLAN MILLER
*/

#ifndef BMPOPS
#include "bmpops.h"
#endif

#ifndef BMPREADER
#include "bmpreader.h"
#endif

#ifndef STDIO
#include <stdio.h>
#define STDIO
#endif

#ifndef STDLIB
#include <stdlib.h>
#define STDLIB
#endif

#include <math.h>


int relprime(int,int);

int main(){
    // insert code here...
//    FILE* fin = fopen("./labbmp2.bmp", "rb");
    FILE* fout = fopen("./bmpout.bmp","wb");

    int i,ii;
//    bmpfile filein;
//    bmpfile newbmp;
    bmpfile fun;    
//    readinbmp(fin, &filein);    
printf("here\n");
    initBMP_w_h(&fun,5000,5000,32);
    for(i=0;i<fun.width;i++){
        for(ii=0;ii<fun.height;ii++){
            fun.pixlearray[i][ii].arrayval = ((relprime(i+1,ii+1)!=1)?0xffffffff:0x00000000);
            //0xffffffff - (relprime(i+1,ii+1)*0x11111111 - 0x11111111);
            //((relprime(i+1,ii+1)!=1)?0xffffffff:0x00000000);
        }
    }
    setcolors(&fun);
//    print_report(&fun);
//    initBMP_w_h(&newbmp,abs(filein.width),abs(filein.height),32);

//    downsample(&filein,&newbmp);

//    print_report(&filein);

    writeBMP(&fun,fout);        
    
//    dalloc(&newbmp);
//    dalloc(&filein);
    dalloc(&fun);

    fclose(fout);
//    fclose(fin);
    
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
