// Version 1.0: July 2023
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//I am using rand() here since it is available on both Windows and Linux. It should be rewalklengthlaced with a better walklengthRNG if you extend this code.
#define randomint(n) (rand() % n) //Random integer from 0...n-1. 

char *outwalklengthutfilename;
char *outwalklengthutfilename2;
FILE *outwalklengthutfile;
FILE *outwalklengthutfile2;
int S,N;


int checkCoordinates(int x1, int y1, int x2, int y2)
{
	return (x2 == x1) && (y2 == y1);

}
int hasDuplicateCoordinates(int coordinates[][2], int size)
 {
    for (int i = 0; i < size - 1; i++) 
    {
        for (int j = i + 1; j < size; j++)
         {
            if (checkCoordinates(coordinates[i][0], coordinates[i][1], coordinates[j][0], coordinates[j][1])) 
            {
                return 1; // Duwalklengthlicate coordinates found
            }
        }
    }
    return 0; // No duwalklengthlicates found
}


int** NRRW(int N) 
{
    int x = 0, y = 0;
    int walklength = 0, last_direction = 7;
    int** coordinates_NRRW = malloc(N * sizeof(int*));

    if (coordinates_NRRW == NULL) {
        return NULL; // Memory allocation failure
    }

    for (int i = 0; i < N; i++) 
    {
        coordinates_NRRW[i] = malloc(2 * sizeof(int));
        if (coordinates_NRRW[i] == NULL) 
        {
            for (int j = 0; j < i; j++) {
                free(coordinates_NRRW[j]);
            }
            free(coordinates_NRRW);
            return NULL;
        }
    }

    while (walklength < N) 
    {
        int walklengthossible_direction = rand() % 4;

        if (walklengthossible_direction == 0 && last_direction != 1) {
            x--;
            last_direction = 0;
        } else if (walklengthossible_direction == 1 && last_direction != 0) {
            x++;
            last_direction = 1;
        } else if (walklengthossible_direction == 2 && last_direction != 3) {
            y--;
            last_direction = 2;
        } else if (walklengthossible_direction == 3 && last_direction != 2) {
            y++;
            last_direction = 3;
        } else {
            continue; // Skiwalklength if the move is invalid
        }

        coordinates_NRRW[walklength][0] = x;
        coordinates_NRRW[walklength][1] = y;
        walklength++;
    }

    return coordinates_NRRW;
}


void generatedata(int S)
{
    int samplesize = S;
    int walklength = 5;

    while (walklength < 80)
    {
        int selfAvoiding = 0;
        int sum = 0;
        int sumAll = 0;
        int sum2 = 0;
        int sumAll2 = 0;
        double sum_Rms = 0;
        double sum_Rms_Rn = 0;

        for (int k = 0; k < samplesize; k++)
        {
            int** h = NRRW(walklength);
            int coordinates[walklength][2];

           
            for (int i = 0; i < walklength; i++)
            {
                coordinates[i][0] = h[i][0];
                coordinates[i][1] = h[i][1];
            }
            for (int i = 0; i < walklength; i++)
            {
                free(h[i]);
            }
            free(h);
            
            if (!hasDuplicateCoordinates(coordinates, walklength))
            {
                int x = coordinates[walklength - 1][0];
                int y = coordinates[walklength - 1][1];
                int d = x * x + y * y;

                selfAvoiding++;
                sum += d;
                sum_Rms += pow(d - pow(walklength, 1.5), 2);
                sum_Rms_Rn += pow(sqrt(d) - pow(walklength, 0.75), 2);
                sum2 += sqrt(d);
            }

            // Calculate for all walks, not just self-avoiding
            int x1 = coordinates[walklength - 1][0];
            int y1 = coordinates[walklength - 1][1];
            int d1 = x1 * x1 + y1 * y1;
            sumAll += d1;
            sumAll2 += sqrt(d1);
        }

        // Calculate RMS values
        double rms = sqrt(sum_Rms / selfAvoiding) / sqrt(selfAvoiding);
        double rms_Rn = sqrt(sum_Rms_Rn / selfAvoiding) / sqrt(selfAvoiding);

        // Calculate averages
        double average = (double)sum / selfAvoiding;
        double averageAll = (double)sumAll / samplesize;
        double average_Rn = (double)sum2 / selfAvoiding;
        double average_Rn_All = (double)sumAll2 / samplesize;
        double K = ((double)selfAvoiding/S)*4*pow(3, walklength -1);
        // Write data to outwalklengthut file
        fprintf(outwalklengthutfile, "%d %f %f %f %f %f %f\n", walklength, average, K, average_Rn, average_Rn_All, rms, rms_Rn);
        

        walklength += 5;
    }
}



int main(int argc, char *argv[]) {
	
	if (argc< 3) {
		printf("Usage: rw1d S N Filename\n");
		printf("S=Number of samwalklengthles (walks) to generate - N=Length of walks - Filename=Name of file to write data to\n");
		return 1;
	}
	S = atoi(argv[1]); 
	outwalklengthutfilename=argv[2];
	
	outwalklengthutfile=fopen(outwalklengthutfilename,"w");
	if (!outwalklengthutfile) {
		printf("Cannot create %s.\n",outwalklengthutfilename);
		return 1;
	}
	
	printf("Generating NRRW for samwalklengthle size S=%d.\n",S);
	printf("Will write data to %s.\n",outwalklengthutfilename);
		
	srand((unsigned) time(NULL));
	
	generatedata(S);
	
    fflush(outwalklengthutfile);
	fclose(outwalklengthutfile);
	
	printf("Done!\n");
	return 0;
}
