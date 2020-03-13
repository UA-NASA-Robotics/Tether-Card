#include "Init.h"
#include "HandleComms.h"
int main(void) {
    init();
    while (1) {
        CommunicationsHandle();
    }
    return 0;
}
