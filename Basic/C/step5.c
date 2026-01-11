#include <stdio.h>

void swap(int *x, int *y) {
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

int main() {
    int num = 777;
    // numのアドレスをコピー
    int *num_pointer = &num;

    // *はアドレスから値へジャンプする 
    // *xはxがアドレス、*xがxのアドレスへジャンプして値を取りに行く
    // &は値のアドレスを指す
    printf("num                : %d\n", num);
    printf("*num_pointer       : %d\n", *num_pointer);
    printf("&num               : %p\n", (void*)&num);
    printf("num_pointer address: %p\n\n", (void*)num_pointer);

    // *num_pointerはnumのアドレスからジャンプした値を指すため、numが書き換わる
    *num_pointer = 999;
    printf("*num_pointer = 999;\n");
    printf("num                : %d\n", num);
    printf("num_pointer address: %p\n\n", (void*)&num_pointer);

    int a = 100;
    int b = 200;

    printf("a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("a = %d, b = %d\n\n", a, b);
    
    int nums[5] = {1, 2, 3, 4, 5};
    // nums[0]のアドレスをpへ代入。numsと書くことで&nums[0]と同じ
    int *p = nums;

    printf("nums[0] address: %p\n", (void*)p);
    for (int i = 0; i < 5; i++) {
        // 最初の一周だけ*p, pで出力
        printf("Value: %d | Address: %p\n\n", *p, (void*)p);
        // ポインタをインクリメントする
        p++;
    }
    
    char msg[] = "Security";
    char *ptr = msg;
    
    // ヌルバイトに当たるまでループ
    while (*ptr != '\0') {
        printf("Char: %c | Address: %p\n", *ptr, (void*)ptr);
        ptr++;
    }
    return 0;
}