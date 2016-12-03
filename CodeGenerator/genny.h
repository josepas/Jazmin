#ifndef __MYWRAPPER_H
#define __MYWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Generator Generator;

Generator* newGenerator();

void deleteGenerator(Generator* v);






#ifdef __cplusplus
}
#endif




#endif