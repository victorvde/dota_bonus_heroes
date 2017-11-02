#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>

#include "heroes.h"

typedef uint8_t STREAM[256];

int cmp(const void* a, const void* b) {
    return strcmp(*((char**) a), *((char**) b));
}

int main() {
    HINSTANCE vstdlib = LoadLibrary("vstdlib.dll");
    int64_t (*randomint)(STREAM *stream, int min, int max) = (void *)GetProcAddress(vstdlib, "?RandomInt@CUniformRandomStream@@UEAAHHH@Z");
    void (*setseed)(STREAM *stream, int seed) = (void *)GetProcAddress(vstdlib, "?SetSeed@CUniformRandomStream@@UEAAXH@Z");
    void (*randomstream)(STREAM *stream) = (void *)GetProcAddress(vstdlib, "??0CUniformRandomStream@@QEAA@XZ");

    for(uint64_t i = 0x443D; i < 0x443D + 365 * 4; i++) {
        STREAM s;
	randomstream(&s);
	setseed(&s, i);
#define NBONUS 10
	int64_t bonus[NBONUS];
	for(int j = 0; j < NBONUS; j++) {
	retry:;
	    int64_t b = randomint(&s, 0, 114);
	    for(int k = 0; k < j; k++) {
	        if(bonus[k] == b) goto retry;
	    }
	    bonus[j] = b;
	}
	char date[20];
	time_t t1 = i * 86400;
	struct tm t2 = *gmtime(&t1);
        strftime(date, 20, "%Y-%m-%d", &t2);
	printf("%s: ", date);
	const char *hero_names[NBONUS];
	for(int j = 0; j < NBONUS; j++) {
	    hero_names[j] = heroes[bonus[j]];
	}
        qsort(hero_names, NBONUS, sizeof(hero_names[0]), cmp);
	for(int j = 0; j < NBONUS; j++) {
            printf("%s", hero_names[j]);
            if(j != NBONUS - 1) {
                printf(", ");
            } else {
                printf("\n");
            }
	}
    }
}
