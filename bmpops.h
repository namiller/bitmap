#define BMPOPS

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

void filter(bmpfile*,bmpfile*,pixle, int);
int distance(pixle,pixle);


int distance(pixle a ,pixle b){
    return (a.red-b.red)*(a.red-b.red)+(a.blue-b.blue)*(a.blue-b.blue)+(a.yellow-b.yellow)*(a.yellow-b.yellow);
}

void filter(bmpfile* source,bmpfile* target,pixle color,int radius){
    int i,ii;
    for (i=0; i<abs(source->width); i++) {
        for(ii=0;ii<abs(source->height);ii++){
            target->pixlearray[i][ii].arrayval = (distance(source->pixlearray[i][ii],color));// <=radius*radius);
        }
    }
}
