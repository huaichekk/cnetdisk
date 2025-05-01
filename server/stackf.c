#include"head.h"
int initstack(stackf_t *stackf){
    stackf->topd=-1;
    return 0;
}
int enstack(stackf_t *stackf,int id){
    stackf->topd++;
    stackf->id[stackf->topd]=id;
    return 0;
}
int popstack(stackf_t *stackf){
    if(stackf->topd==-1){
        return 0;
    }
    stackf->topd--;
    return 0;
}
int stacktop(stackf_t *stackf){
    return stackf->id[stackf->topd];
}
