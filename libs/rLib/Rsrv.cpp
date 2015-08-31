#include "Rsrv.h"


#ifdef MAIN
int isByteSexOk() {
    int i;
    i=itop(0x12345678);
    return (*((char*)&i)==0x78);
}
#endif
