#ifndef __MYWRAPPER_H
#define __MYWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Generator Generator;

Generator* newGenerator();

void Generator_int_set(Generator* v, int i);

int Generator_int_get(Generator* v);

void deleteGenerator(Generator* v);

#ifdef __cplusplus
}
#endif
#endif