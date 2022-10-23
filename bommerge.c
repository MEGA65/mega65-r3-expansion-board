#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define MAX_PARTS 65536
int part_count=0;
char *partids[MAX_PARTS];
char *partds[MAX_PARTS];
char *parturl[MAX_PARTS];
char *partnumbers[MAX_PARTS];
char *partmfgnumbers[MAX_PARTS];
char *partsource[MAX_PARTS];


int main(int argc,char **argv)
{
  if (argc!=4) {
    fprintf(stderr,"usage: bommerge <kicad bom CSV file as input> <parts library CSV as input> <merged BOM as output>\n");
    exit(-1);
  }

  char line[8192];
  FILE *f=fopen(argv[2],"r");
  line[0]=0; fgets(line,8192,f);
  char id[8192],ds[8192],url[8192],number[8192],mfgnumber[8192],source[8192];
  int line_num=0;
  while(line[0]) {
    if (sscanf(line,"%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]",id,ds,url,number,mfgnumber,source)!=6) {
      fprintf(stderr,"ERROR: Could not scan line #%d of parts list file '%s'\n",line_num,argv[2]);
      exit(-1);
    }
    partids[part_count]=strdup(id);
    partds[part_count]=strdup(ds);
    parturl[part_count]=strdup(url);
    partnumbers[part_count]=strdup(number);
    partmfgnumbers[part_count]=strdup(mfgnumber);
    partsource[part_count]=strdup(source);
    line_num++;
  }
  fclose(f);
}
