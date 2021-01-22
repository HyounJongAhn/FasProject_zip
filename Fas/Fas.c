#define _CRT_SECURE_NO_WARNINGS

// 아쉬운점, 보완할점
/*
1. If문을 사용하여 Signature에 해당하는 값을 찾아서 개수를 세었는데, 만약 파일의 데이터에 Signature

값과 같은 값이 들어있는 경우 프로그램이 오작동 한다.

2. Eocd를 찾아서 파일의 개수를 구해야 하는데 그러지 못해서 아쉽다.

3. 2번째와 4번째 파일에서 파일 데이터가 이상하게 나왔고, 파일 데이터 뒤에 03 00이라는 값이 생겨났다.이는 2번째와 4번

째의 bit flags값이 08 00이어서다. (Bit flags의 값이 08 00인 경우 파일 데이터 뒤에 data descriptor이라는 값이 생겨난다.)

코드를 짤 때 이와 같은 부분을 몰랐고, 결국 반영되지 않아서 결과가 제대로 나오지 않았다.

왜 2번째, 4번째 파일만 bit flags값이 08 00인지, bit flags값이 08 00인 경우 unused인데 나온 이유가 무엇인지 더 공부할 것

이다.
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* in;
    FILE* datafile;
    int lis[99999];
    int lis2[99999];
    int ch;
    int num = 0;
    int i, j, k;
    int a, b;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;
    int g = 0;
    int l = 0;
    char name;
    char filename[99999] = {0x00,};
    char dataname[99999] = { 0x00, };
    int val = 0;
    int sum = 0;
    int next = 0;

// 파일 경로 입력
    printf("파일 경로를 입력하세요 : ");
    
// 파일 열기    
    gets(filename);
    in = fopen(filename, "rb");

    if (in == NULL) {
        fprintf(stderr, "fopen() error\n");
    }

// 16진수 복사
    while (1) {
        ch = fgetc(in);
        if (ch < 0) {
            break;
        }

        lis[num] = ch;
        lis2[num] = ch;
        num += 1;
    }
    printf("총 바이트 : %d\n", num);

// 파일 개수 찾기

    for (i = 0; i < num; i++) {
        if ((int)lis[i] == 80 && (int)lis[i + 1] == 75 && (int)lis[i + 2] == 3 && (int)lis[i + 3] == 4)
        {
            sum += 1;
        }
    }

    printf("파일 개수 : %d\n\n", sum);
    
// 파일 구조 출력
    for (k = 0; k < sum; k++) {

        printf("%d-th [시작 offset] : %d\n",k+1, next);
        for (i = 0; i < num; i++) {
            
            if (i == 26 + next) {
                a = lis[i];
            }
            else if (i == 27 + next) {
                b = lis[i];
                c = a + b;
            }
            else if (i == 28 + next) {
                e = lis[i];
            }
            else if (i == 29 + next) {
                f = lis[i];
                g = e + f;
            }

            else if (i == 30 + next) {
                printf("%d-th [길이] : %d\n",k+1 , c);
                printf("%d-th [파일 이름 - 16진수] : ", k + 1);
                for (j = 30 + next; j < 30 + c + next; j++) {
                    printf("%02X ", lis2[j]);
                }
                printf("\n");

                printf("%d-th [파일 이름] : ", k + 1);
                for (j = 30 + next; j < 30 + c + next; j++) {
                    name = lis2[j];
                    printf("%c", name);
                }
            }
        }

        d = 31 + c + g + next;
        next = 0;
        printf("\n%d-th [파일 데이터가 시작하는 offset] : %d\n",k+1 , d);

// 다음 시작지점 찾기
        val = 0;
        
        for (i = d-1; i < num; i++) {
            if ((int)lis[i] == 80 && (int)lis[i + 1] == 75 && (int)lis[i + 2] == 3 && (int)lis[i + 3] == 4)
            {
                break;
            }
            else if ((int)lis[i] == 80 && (int)lis[i + 1] == 75 && (int)lis[i + 2] == 1 && (int)lis[i + 3] == 2)
            {
                break;
            }
            val += 1;
        }
        printf("%d-th [데이터의 크기] : %d\n\n",k+1, val);

// 데이터 따로 저장
        printf("데이터를 저장할 파일 이름과 형식을 입력하세요 : ");
        gets(dataname);

        datafile = fopen(dataname, "a+");
        if (datafile == NULL) {
            fprintf(stderr, "fopen() error\n");
        }
        
        fputs("file data\n", datafile);
        for (i = d - 1; i < d + val - 1; i++) {
            fputc(lis2[i], datafile);
        }
        fputs("\n", datafile);

        
        if (datafile != NULL) {
            fclose(datafile);
        }

        
// 초기화
        next = d + val -1;
        a = 0;
        b = 0;
        c = 0;
        e = 0;
        f = 0;
        g = 0;
        d = 0;
        printf("\n");
    }

    

    if (in != NULL) {
        fclose(in);
    }
    


    return 0;
}

