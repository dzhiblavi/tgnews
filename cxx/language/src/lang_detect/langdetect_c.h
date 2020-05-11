//
// Created by Роман Агеев on 11.05.2020.
//
#ifndef __LANGDETECT_DETECTOR_C_H_
#define __LANGDETECT_DETECTOR_C_H_

#ifdef __cplusplus
extern "C" {
#endif

void langdetect_detect(char const *data, unsigned int const length, char lang[6]);
void langdetect_detect_with_score(char const *data, unsigned int const length, char lang[6], double *score);

#ifdef __cplusplus
}
#endif

#endif
