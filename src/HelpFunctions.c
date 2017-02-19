//
// Created by matthias on 31.01.17.
//

#include "include/HelpFunctions.h"

void createResultFolder(char* folder_path, struct stat buf) {
    // "results" folder does not exist -> create it
    if (stat(folder_path, &buf) == -1) {
        mkdir(folder_path, 0700);
    }
}

char* createResultFile(char* folder_path, int Matrix_Dimension_N, int mm_repetitions,
                      double* overall_times, bool* calcs) {

    // create new file, mode: append extended
    char* filename = createFileName(folder_path);
    FILE *fp = fopen(filename, "ab+");

    // write content to the file
//    char* header = "Matrix-Multiplication, using square matrices (N x N)\n";
    double avg_std = overall_times[0] / mm_repetitions;
    double avg_cache = overall_times[1] / mm_repetitions;
    double avg_parall = overall_times[2] / mm_repetitions;
    double avg_blas = overall_times[3] / mm_repetitions;

    fprintf(fp,
            "Matrix-Multiplication (C = A * B) using square matrices (N x N)\n"
            "4 different algorithms: naive, cache-optimized, parallel, BLAS-lib\n"
            "------------------------------------------------------------------\n"
            "N = %d\n"
            "Count of MM-repetitions = %d\n"
            "------------------------------------------------------------------\n\n"
            "1) standard algorithm:\n"
            "overall time: %.6f sec. -- average time: %.6f sec.\n\n"
            "2) cache-optimized algorithm:\n"
            "calculation correct? %s\n"
            "overall time: %.6f sec. -- average time: %.6f sec.\n\n"
            "3) parallel algorithm:\n"
            "calculation correct? %s\n"
            "overall time: %.6f sec. -- average time: %.6f sec.\n\n"
            "4) BLAS-lib algorithm:\n"
            "calculation correct? %s\n"
            "overall time: %.6f sec. -- average time: %.6f sec.\n\n",
            Matrix_Dimension_N, mm_repetitions,
            overall_times[0], avg_std, calcs[0]? "true":"false", overall_times[1], avg_cache,
            calcs[1]? "true":"false", overall_times[2], avg_parall,
            calcs[2]? "true":"false", overall_times[3], avg_blas);

    // TODO: comparison overall, avg
    fprintf(fp,
            "------------------------------------------------------------------\n"
            "Comparison between the 4 algorithms (N = %d):\n"
            "------------------------------------------------------------------\n\n"
            "Standard algorithm average time: %.3f\n"
            "------------------------------------------------------------------\n"
            "Speedups in comparison to standard algorithm:\n"
            "cache-optimized algorithm: %.3f x\n"
            "parallel algorithm: %.3f x\n"
            "BLAS-lib algorithm: %.3f x\n",
            Matrix_Dimension_N, avg_std, avg_std/avg_cache, avg_std/avg_parall,
            avg_std/avg_blas);

    // close the file
    fclose(fp);
    return filename;
}

char* createFileName(char* folder_path) {
    // pseudo random number
    srand(time(0)); //use current time as seed for random generator
    int pseudo_random_number = rand();
    char* pseudo_rand = malloc(sizeof(char) * 50);
    sprintf(pseudo_rand, "%d", pseudo_random_number);

    // date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char year[10];
    sprintf(year, "%d", tm.tm_year + 1900);
    char mon[10];
    sprintf(mon, "%d", tm.tm_mon + 1);
    char day[10];
    sprintf(day, "%d", tm.tm_mday);

    char* filename = malloc(100 * sizeof(char)); // make space for the new string
    strcpy(filename, folder_path);

    // add:
    strcat(filename, day);
    strcat(filename, "-");
    strcat(filename, mon);
    strcat(filename, "-");
    strcat(filename, year);
    strcat(filename, "_");
    strcat(filename, pseudo_rand);
    strcat(filename, ".txt");


//    free(filename);
    // TODO: return sth.......
    return filename;
}

void printHeader(int N, int REPETITIONS) {
    printf("|====================================================================|\n");
    printf("| matrix multiplication (C = A * B) using square matrices (N x N)    |\n");
    printf("| 4 different algorithms: naive, cache-optimized, parallel, BLAS-lib |\n");
    printf("|====================================================================|\n");
    printf("| N = %d, REPETITIONS for each algorithm: %d \n", N, REPETITIONS);
    printf("|====================================================================|\n");
}

