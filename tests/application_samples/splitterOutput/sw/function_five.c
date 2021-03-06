typedef unsigned char Xuint8;
typedef char Xint8;
typedef unsigned short Xuint16;
typedef short Xint16;
typedef unsigned long Xuint32;
typedef long Xint32;
typedef float Xfloat32;
typedef double Xfloat64;
typedef unsigned long Xboolean;
typedef struct {
    Xuint32 Upper;
    Xuint32 Lower;
}Xuint64;
typedef Xuint32 u32;
typedef Xuint16 u16;
typedef Xuint8 u8;
typedef void (*XInterruptHandler)(void *);
typedef void (*XExceptionHandler)(void *);
typedef void (*XAssertCallback)(char *, int);
typedef int XStatus;
typedef u32 XIo_Address;
typedef struct {
    u16 DeviceId;
    u32 BaseAddress;
}XTmrCtr_Config;
typedef void (*XTmrCtr_Handler)(void *, u8);
typedef struct {
    u32 Interrupts;
}XTmrCtrStats;
typedef struct {
    XTmrCtrStats Stats;
    u32 BaseAddress;
    u32 IsReady;
    XTmrCtr_Handler Handler;
    void *CallBackRef;
}XTmrCtr;
int abs();
void function_five(int n1, int n2, int n3, int *out1, int *out2) {
    int i, a, b1, b2, b3, c;
    a = 0;
    b1 = 1;
    for (i = 1; i <= n1; i++) {
        c = a + b1;
        a = b1;
        b1 = c;
    }
    a = 0;
    b2 = 1;
    for (i = 1; i <= n2; i++) {
        c = a + b2;
        a = b2;
        b2 = c;
    }
    a = 0;
    b3 = 1;
    for (i = 1; i <= n3; i++) {
        c = a + b3;
        a = b3;
        b3 = c;
    }
    b1 = abs(b1 + b2) % n1;
    b2 = abs(b2 + b3) % n2;
    *out1 = b1;
    *out2 = b2;
}

