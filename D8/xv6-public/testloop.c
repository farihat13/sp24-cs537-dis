#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    int i;
    int sum = 0;
    for (i = 0; i < 100000000; i++) {
        sum += i;
    }
    exit();
    return sum;
}
