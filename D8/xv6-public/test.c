#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    int i;
    for (i = 0; i < 2; i++) {
        printf(1, "\n\n\nHello, world!\n\n");
        sleep(20);
    }
    exit();
}
