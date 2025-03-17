
#include <stdio.h>
#include <math.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define randomint(n) (lrand48() % n) 
char *outputfilename;
FILE *outputfile;
int S,N;

int dowalk(int N) {
    int steps, direction, x = 0, y = 0;

    for (steps = 0; steps < N; steps++) {
        direction = randomint(4);
        if (direction == 0) {
            x = x - 1;
        } else if (direction == 1) {
            x = x + 1;
        } else if (direction == 2) {
            y = y - 1;
        } else if (direction == 3) {
            y = y + 1;
        }
    }
    return (x * x + y * y);
}

void generatedata(int S) {
    double R2average;
    double Raverage;
    int walkLength = 30;
    double pi = M_PI;
    while (walkLength <= 600) {
        int run;
        double R2;
        double rms_error = 0.0;
        double rms_error_Rn = 0.0;
        double rms_sum = 0.0;
        double rms_sum_Rn = 0.0;
        double sum_displacement = 0.0; 
        
        R2average = 0.0;
        for (run = 0; run < S; run++) {
            R2 = dowalk(walkLength);
            sum_displacement += sqrt(R2);
            rms_sum += pow(sqrt(R2) - walkLength, 2);
            rms_sum_Rn += pow(sqrt(R2) - sqrt(2/pi*walkLength),2);
            R2average += R2;
        }

        R2average /= S;
        Raverage = sum_displacement / S; 
        rms_error = sqrt(rms_sum / (S - 1)) / sqrt(S);
        rms_error_Rn = sqrt(rms_sum_Rn/(S -1))/ sqrt(S);

        fprintf(outputfile, "\n");
        fprintf(outputfile, "%d ", walkLength);
        fprintf(outputfile, "%.5f ", R2average); 
        fprintf(outputfile, "%.5f ", Raverage); 
        fprintf(outputfile, "%.5f ", rms_error);
        fprintf(outputfile, "%.5f",rms_error_Rn);

        walkLength += 30; 
    }
}

int main(int argc, char *argv[]) {
	
	if (argc<4) {
		printf("Usage: rw1d S N Filename\n");
		printf("S=Number of samples (walks) to generate - N=Length of walks - Filename=Name of file to write data to\n");
		return 1;
	}

	S = atoi(argv[1]); 
    N = atoi(argv[2]); 
	outputfilename=argv[3];
	
	outputfile=fopen(outputfilename,"w");
	if (!outputfile) {
		printf("Cannot create %s.\n",outputfilename);
		return 1;
	}
	
	printf("Generating S=%d samples of walks with length N=%d.\n",S,N);
	printf("Will write data to %s.\n",outputfilename);
		
	srand((unsigned) time(NULL));
	
	generatedata(S);
	
    fflush(outputfile);
	fclose(outputfile);
	
	printf("Done!\n");
	return 0;
}