//
// Created by matthias on 31.01.17.
//

#ifndef FPS_MATRIXMULTIPLIKATION_HELPFUNCTIONS_H
#define FPS_MATRIXMULTIPLIKATION_HELPFUNCTIONS_H

#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void createResultFolder(char* folder_path, struct stat buf);
char* createResultFile(char* folder_path, int Matrix_Dimension_N, int mm_repetitions,
                      double* overall_times, bool* calcs);
char* createFileName(char* folder_path);
void printHeader(int N, int REPETITIONS);

#endif //FPS_MATRIXMULTIPLIKATION_HELPFUNCTIONS_H
