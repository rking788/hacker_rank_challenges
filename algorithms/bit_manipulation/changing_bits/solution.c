#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#define DEBUG       0
#define SEG_COUNT   3126
#define BUFF_LEN    32
#define IND1(i)     (i/32)
#define IND2(i)     (i%32)

void readBitString(const char* bitString, int len, uint64_t* ab);
void handleQuery(uint64_t* a, uint64_t* b);
void setBit(uint64_t* ab, int index, int val);
void getC(uint64_t* a, uint64_t* b, int index);

int main() {

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */    
    // Get N and Q
    unsigned int n = 0, q = 0;
    scanf("%d", &n);
    scanf("%d", &q);

    if (DEBUG)
        printf("Found N=%d, Q=%d\n", n, q);

    // A and B will have to be a multidimenaional array
    // of sizes A[3125]. The upper 32 bits are used for 
    // overflow, only the lower 32 hold the actual a/b values
    uint64_t* a = calloc(SEG_COUNT, sizeof(uint64_t));
    uint64_t* b = calloc(SEG_COUNT, sizeof(uint64_t));
    
    // Read A
    char* abuf = malloc(100000);
    char* bbuf = malloc(100000);

    scanf("%s", abuf);
    readBitString(abuf, n, a);

    // Read B
    scanf("%s", bbuf);
    readBitString(bbuf, n, b);

    free(abuf);
    free(bbuf);

    // Handle each query
    int qind = 0;
    for (qind = 0; qind < q; ++qind) {
        handleQuery(a, b);
    }

    free(a);
    free(b);

    return 0;
}

void readBitString(const char* bitString, int len, uint64_t* ab)
{
    for (int i = 0; i < len; i++) {

        if (bitString[i] == '1') {
            ab[IND1(i)] |= (1 << IND2(i));
        }
    }
}

void handleQuery(uint64_t* a, uint64_t* b)
{
    char cmd[32] = {0};
    scanf("%s", cmd);

    if(DEBUG)
        printf("A=%lu ,, B=%lu ,, cmd=%s\n", a[1], b[1], cmd);

    if (strcmp(cmd, "set_a") == 0){
        int ind = 0, val = 0;
        scanf("%d %d", &ind, &val);

        setBit(a, ind, val);
    }
    else if(strcmp(cmd, "set_b") == 0) {
        int ind = 0, val = 0;
        scanf("%d %d", &ind, &val);

        setBit(b, ind, val);
    }
    else if(strcmp(cmd, "get_c") == 0) {
        int ind = 0;
        scanf("%d", &ind);

        getC(a, b, ind);
    }
    else {
        if(DEBUG)
            printf("Unknown Command: %s\n", cmd);
    }
        
}

void setBit(uint64_t* ab, int index, int val)
{
    if (val == 0) {
        ab[IND1(index)] = ab[IND1(index)] & (~(0x0000000000000001 << IND2(index)));
    }
    else {
        ab[IND1(index)] = ab[IND1(index)] | (1 << IND2(index));
    }
}

void getC(uint64_t* a, uint64_t* b, int index)
{
//    unsigned long long val = (a + b) & (1 << index);
    uint64_t overflow = 0;
    int tempIndex = IND1(index);
    for (int i = 0; i < tempIndex; ++i) {
//        overflow = ((a[i] + b[i] + overflow) >> 32) & 0x00000000FFFFFFFF;
        overflow = ((a[i] + b[i] + overflow) >> 32);
    }
    
    uint64_t val = (a[tempIndex] + b[tempIndex] + overflow) & (1 << IND2(index));

    if(DEBUG)
        printf("Val = %lu\n", val);
    
    printf("%u", (val == 0) ? 0 : 1);
}
