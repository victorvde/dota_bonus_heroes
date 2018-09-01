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

#define START_DATE 0x443D // 2017-10-30
#define BONUS_HEROES_CHANGE_DATE 0x44B7 // 2018-03-01
#define END_DATE 0x4A31 // 2022-01-01
#define NBONUS_MAX 12

int main() {
    HINSTANCE vstdlib = LoadLibrary("vstdlib.dll");
    int64_t (*randomint)(STREAM *stream, int min, int max) = (void *)GetProcAddress(vstdlib, "?RandomInt@CUniformRandomStream@@UEAAHHH@Z");
    void (*setseed)(STREAM *stream, int seed) = (void *)GetProcAddress(vstdlib, "?SetSeed@CUniformRandomStream@@UEAAXH@Z");
    void (*randomstream)(STREAM *stream) = (void *)GetProcAddress(vstdlib, "??0CUniformRandomStream@@QEAA@XZ");

    for(uint64_t i = START_DATE; i < END_DATE; i++) {
        STREAM s;
	randomstream(&s);
	setseed(&s, i);

	int64_t bonus[NBONUS_MAX];
        int nbonus = i < BONUS_HEROES_CHANGE_DATE ? 10 : 12;

	for(int j = 0; j < nbonus; j++) {
	retry:;
	    int64_t b = randomint(&s, 0, 115);
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
	const char *hero_names[NBONUS_MAX];
	for(int j = 0; j < nbonus; j++) {
	    hero_names[j] = heroes[bonus[j]];
	}
        qsort(hero_names, nbonus, sizeof(hero_names[0]), cmp);
	for(int j = 0; j < nbonus; j++) {
            printf("%s", hero_names[j]);
            if(j != nbonus - 1) {
                printf(", ");
            } else {
                printf("\n");
            }
	}
    }
}
