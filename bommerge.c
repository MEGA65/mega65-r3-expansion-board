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
  char id[8192],ds[8192],url[8192],number[8192],mfgnumber[8192],source[8192];
  int line_num=0;
  line[0]=0; fgets(line,8192,f);
  int errors=0;
  while(line[0]) {
    if (sscanf(line,"%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]",id,ds,url,number,mfgnumber,source)!=6) {
      fprintf(stderr,"ERROR: Could not scan line #%d of parts list file '%s'\n",line_num,argv[2]);
      errors++;
    } else {
      partids[part_count]=strdup(id);
      partds[part_count]=strdup(ds);
      parturl[part_count]=strdup(url);
      partnumbers[part_count]=strdup(number);
      partmfgnumbers[part_count]=strdup(mfgnumber);
      partsource[part_count]=strdup(source);
      part_count++;
    }
    line_num++;
    line[0]=0; fgets(line,8192,f);
  }
  fclose(f);
  fprintf(stderr,"INFO: Read library of %d parts.\n",part_count);
  if (errors) {
    fprintf(stderr,"WARN: Could not parse %d part definitions.\n",errors);
  }

  int count=0;
  errors=0;
  f=fopen(argv[1],"r");
  line[0]=0; fgets(line,8192,f);
  line_num=0;
  while(line[0]) {
    // "Id";"Designator";"Package";"Quantity";"Designation";"Supplier and ref";
    char index[8192],id[8192],pkg[8192],num[8192],des[8192],supplier[8192];
    int r=sscanf(line,"%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]",index,id,pkg,num,des,supplier);
    if (r<5) {
      fprintf(stderr,"ERROR: Could not scan line #%d of parts list file '%s' (r=%d)\n",line_num,argv[2],r);
      errors++;
    } else {
      count++;
    }    

    line_num++;
    line[0]=0; fgets(line,8192,f);
  }
  fclose(f);

  fprintf(stderr,"INFO: Read schematic with %d unique parts.\n",count);
  if (errors) {
    fprintf(stderr,"WARN: Could not parse %d unique components.\n",errors);
  }
  
  return 0;
}
