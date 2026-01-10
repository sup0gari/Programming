#include <stdio.h>

int main() {
    char a = 127; // 1byte(-128~127)
    int b; // 4byteの整数
    long long c; // 8byteの整数
    unsigned char d = 255; // unsignedとは符号なしのこと(0~255)

    // sizeofとは指定した方がメモリを何バイト消費するか
    // %zuとはsizeofの出力
    printf("char size: %zu byte\n", sizeof(a));
    printf("int size: %zu byte\n", sizeof(b));
    printf("long long size: %zu byte\n", sizeof(c));

    printf("char a:          %d(hex: 0x%X)\n", a, a);
    // %dは整数の出力
    // %Xは16進数表記
    printf("unsigned char d: %d(hex: 0x%X)\n", d, d);
}