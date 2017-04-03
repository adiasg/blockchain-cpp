#include "id.h"
#include <stdio.h>
#include <string.h>
char host_id[8];

void set_host_id() {
    printf("Set host_id:\n");
    scanf("%s", host_id);
    printf("host_id set to: %s\n", host_id);
    for(int i=strlen(host_id); i<8; i++) {
        host_id[i] = '\0';
    }
}
