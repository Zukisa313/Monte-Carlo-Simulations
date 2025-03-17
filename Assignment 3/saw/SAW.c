#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define randomint(n) (rand() % n) //Random integer from 0...n-1.

char *outputfilename;
FILE *outputfile;

int areCoordinatesEqual(int x1, int y1, int x2, int y2) {
    return (x1 == x2) && (y1 == y2);
}

void freeCoordinateArray(int** coordinates, int N) {
    for (int i = 0; i < N; i++) {
        free(coordinates[i]);
    }
    free(coordinates);
}

int isCoordinateInArray(int** coordinates, int x, int y, int size) {
    for (int i = 0; i < size; i++) {
        if (areCoordinatesEqual(coordinates[i][0], coordinates[i][1], x, y)) {
            return 1;
        }
    }
    return 0; 
}

int** generateSelfAvoidingWalk(int N, int maxAttempts) {
    int x = 0;
    int y = 0;
    int n = 0;
    int all_directions[][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int** coordinates_SAW = malloc((N + 1) * sizeof(int*));

    if (coordinates_SAW == NULL) {
        return NULL;
    }

    for (int i = 0; i < N + 1; i++) {
        coordinates_SAW[i] = malloc(2 * sizeof(int));
        if (coordinates_SAW[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(coordinates_SAW[j]);
            }
            free(coordinates_SAW);
            return NULL;
        }
    }

    coordinates_SAW[0][0] = 0;
    coordinates_SAW[0][1] = 0;

    int attempts = 0;

    while (n < N) {
        int num_possible_directions = 0;
        int possible_directions[4][2];

        for (int i = 0; i < 4; i++) {
            int new_x = x + all_directions[i][0];
            int new_y = y + all_directions[i][1];

            if (!isCoordinateInArray(coordinates_SAW, new_x, new_y, n + 1)) {
                possible_directions[num_possible_directions][0] = new_x;
                possible_directions[num_possible_directions][1] = new_y;
                num_possible_directions++;
            }
        }

        if (num_possible_directions == 0) {
            attempts++;
            if (attempts >= maxAttempts) {
                freeCoordinateArray(coordinates_SAW, N + 1);
                return NULL;
            }
            n--;
            if (n < 0) {
                freeCoordinateArray(coordinates_SAW, N + 1);
                return NULL; 
            }
            x = coordinates_SAW[n][0];
            y = coordinates_SAW[n][1];
        } else {
            int random_index = rand() % num_possible_directions;
            x = possible_directions[random_index][0];
            y = possible_directions[random_index][1];
            n++;
            coordinates_SAW[n][0] = x;
            coordinates_SAW[n][1] = y;
        }
    }

    return coordinates_SAW;
}


void generateData() {
    int sample_size = 100000;
    int N = 10;
    int sum = 0;
    int sumRms = 0;
    while(N < 80){
        int completeWalk = 0;
        int sum = 0;
        for(int k = 0; k < sample_size; k++)
        {
            int** p = generateSelfAvoidingWalk(N, 2);
            if(p != NULL){
                completeWalk = completeWalk + 1;
                int x = p[N-1][0];
                int y = p[N-1][1];
                int d = x*x + y*y;
                sum = sum + d;
                sumRms = sumRms + pow((d - pow(N, 1.5)), 2);
            }
        }
        double average = (double)sum /completeWalk;
        double rms = sqrt((1/(double)completeWalk)*sumRms)/sqrt((double)completeWalk);
        N = N + 2;
        fprintf(outputfile, "%d ",N);
        fprintf(outputfile, "%f ",average);
        fprintf(outputfile, "%f ",rms);
        fprintf(outputfile, "\n");
    }
     
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (argc < 2) {
        printf("Usage: %s <output_filename>\n", argv[0]);
        return 1;
    }

    outputfilename = argv[1];
    outputfile = fopen(outputfilename, "w");

    if (!outputfile) {
        printf("Cannot create %s.\n", outputfilename);
        return 1;
    }

    generateData();

    fflush(outputfile);
    fclose(outputfile);

    printf("Done!\n");
    return 0;
}
