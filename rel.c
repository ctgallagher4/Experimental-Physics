#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "twister.h"

double PI = 3.14159265;
double E = 2.71828;

//not my code

double **make_empty(int rows, int cols)
{
    double **x = (double **)malloc(sizeof(double *) * rows);
    for (int j = 0; j < rows; j++){
        x[j] = (double*)malloc(sizeof(double)*cols);
        for (int i = 0; i < cols; i++){
            x[j][i] = 0;
        }
    }
    return x;
}

void free_array(double **array, int rows, int cols)
{
    for (int j = 0; j < rows; j++){
        free(array[j]);
    }
    free(array);
}

void print_array(double **array, int rows, int cols)
{
    for (int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            printf("%f ", array[j][i]);
        }
        printf("\n");
    }
}

double **gen_kernel(double **array, double mux, double muy, double peak, double sigma, int rows, int cols)
{
    for (int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            double distance = pow(pow((mux - i),2) + pow((muy - j), 2), .5);
            double gaussian = pow(E, (-.5 * pow(distance/sigma,2)));
            array[j][i] = gaussian * peak;
        }
    }
    return array;
}

double find_kernel_HWHM(double **array, double value, double locx, double locy, int rows, int cols)
{   
    double total = 0;
    double count = 0;
    for (int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            total += array[j][i];
        }
    }

    return total / count;
}

double total_array(double **array, int rows, int cols)
{
    double total = 0;
    for (int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            total += array[j][i];
        }
    }
    return total;
}

double **add_peaks(double **array1, double **array2, double **array3, int rows, int cols)
{
    double **empty = make_empty(rows, cols);
    for (int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            empty[j][i] = array1[j][i] + array2[j][i] + array3[j][i];
        }
    }
    return empty;
}

double HWHM(double **array, double value, int x, int y, int l_x,\
            int u_x, int l_y, int u_y, int rows, int cols)
{
    double count = 0;
    double total = 0;
    double distance;
    for (int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            if ((l_x < i < u_x) && (l_y < j < u_y)) {
                if (value/2 * .97 < array[j][i]) {
                    if (value/2 * 1.03 > array[j][i]){
                        distance = pow((x - i), 2)+pow((y - j), 2);
                        distance = pow(distance, .5);
                        total += distance;
                        count += 1;
                    }
                }
            }
        }
    }
    if (total > 0) {
        return (total / count);
    } else (total = 0);
        printf("ERROR HWHM NULL");
        return 0;
    
}

double integrate(double **array, int rows, int cols)
{
    double total = 0;
    for (int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            total += array[j][i];
        }
    }
    return total;
}

double **add_background(double **array, int background, int rows, int cols)
{
    for (int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            array[j][i] += background;
        }
    }
    return array;
}

int main()
{ 
    int cols = 112;
    int rows = 112;
    /*Generating peak arrays*/
    double **peak1 = gen_kernel(make_empty(rows, cols), 39, 43, 5078-879, 9.4, rows, cols);
    double **peak2 = gen_kernel(make_empty(rows, cols), 62, 31, 4948-879, 6.2, rows, cols);
    double **peak3 = gen_kernel(make_empty(rows, cols), 53, 74, 2594-879, 4.26, rows, cols);

    /*Finding the associated HWHMs*/
    double HWHM1 = HWHM(peak1, 5078-879, 39, 43, 0, 110, 0, 110, rows, cols);
    double total1 = integrate(peak1, rows, cols);
    printf("Total Counts: %f \n", total1);
    printf("HWHM: %f \n", HWHM1);
    double HWHM2 = HWHM(peak2, 3948-879, 62, 31, 0, 110, 0, 110, rows, cols);
    double total2 = integrate(peak2, rows, cols);
    printf("Total Counts: %f \n", total2);
    printf("%f \n", HWHM2);
    double HWHM3 = HWHM(peak3, 1594-879, 53, 74, 0, 110, 0, 110, rows, cols);
    double total3 = integrate(peak3, rows, cols);
    printf("Total Counts: %f \n", total3);
    printf("%f \n", HWHM3);


    /*Computing the total array*/
    double **total = add_peaks(peak1, peak2, peak3, rows, cols);
    //total = add_background(total, 1000, rows, cols);
    //print_array(total, rows, cols);

    /*write total to a file */
    FILE *fpt;
    fpt = fopen("data_test.csv", "w+");
    for (int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            fprintf(fpt,"%f ", total[j][i]);
        }
        fprintf(fpt, " \n");
    }

    /*Free array*/
    free_array(total, rows, cols);
    free_array(peak1, rows, cols);
    free_array(peak2, rows, cols);
    free_array(peak3, rows, cols);
}
