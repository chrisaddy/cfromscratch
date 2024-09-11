#include <stdio.h>
#include <time.h>


int main(int argc, char *argv[]) {
  // DONE: read file in from args so can do cmd data.csv
  if (argc < 2) {
    printf("Missing argument for input file\n");
    // TODO: does it matter if I exit or return? or if I use 0/1?
    return 1;
  }

  // TODO: how do I check that the file name ends in .csv?
  FILE* file_ptr = fopen(argv[1], "r");
  if (file_ptr == NULL) {
    printf("Error opening file\n");
    return 0;
  }


  // TODO: be able to parse each row into the Ticker Struct
  // and combine the ticker structs into an array
  int i = 0;
  while (i < 1000) {
    char row[128];
    fscanf(file_ptr, "%s", &row);
    printf("%s\n", row);
    i++;
  }

  char c = fgetc(file_ptr);
  fclose(file_ptr);
}


struct Ticker {
  char timestamp[128];
  double ticker;
  double open_price;
  double high_price;
  double low_price;
  double close_price;
  double volume;
  char source[8];
};


// EXAMPLE INPUT
// timestamp,ticker,open_price,high_price,low_price,close_price,volume,source
// 2017-05-26 00:00:00+00:00,CRM,90.81,90.98,90.34,90.71,6086180.0,ALPACA
// 2017-05-30 00:00:00+00:00,CRM,90.38,90.95,90.35,90.74,4947895.0,ALPACA
// // 2017-05-31 00:00:00+00:00,CRM,90.57,90.85,89.36,89.52,7462753.0,ALPACA
// // 2017-06-01 00:00:00+00:00,CRM,89.88,90.52,89.2,89.93,5000359.0,ALPACA
// // 2017-06-02 00:00:00+00:00,CRM,90.1,90.97,89.71,90.74,5243170.0,ALPACA
// // 2017-06-05 00:00:00+00:00,CRM,90.87,91.76,90.63,91.26,5781959.0,ALPACA
// // 2017-06-06 00:00:00+00:00,CRM,90.88,91.83,90.68,90.98,4619877.0,ALPACA
// 2017-06-07 00:00:00+00:00,CRM,91.33,91.87,90.5,91.27,3713724.0,ALPACA
// 2017-06-08 00:00:00+00:00,CRM,91.22,91.5,90.47,91.13,4033683.0,ALPACA
