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

/**
 * help functions to create a result folder (if necessary),
 * store the result times in a text file
 */
void createResultFolder(char* folder_path, struct stat buf);
char* createResultFile(char* folder_path, int Matrix_Dimension_N, int BS, int mm_repetitions,
                      double* overall_times, bool* calcs);
char* createFileName(char* folder_path);

/**
 * simple function to print some information about the used parameters etc.
 */
void printHeader(int N, int BS, int REPETITIONS);

#endif //FPS_MATRIXMULTIPLIKATION_HELPFUNCTIONS_H
