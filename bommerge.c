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

int lrtrim(char *s)
{
  int in=0;
  int o=0;
  for(int i=0;s[i];i++) {
    if (s[i]>' ') in=1;
    if (in) s[o++]=s[i];
  }
  s[o]=0;
  while(s[strlen(s)-1]<=' ') s[strlen(s)-1]=0;
  return 0;
}

int dequote(char *des)
{
  if (des[0]=='"') {
    int len=strlen(des)-2;
    for(int i=0;i<len;i++) des[i]=des[i+1];
    des[len]=0;
  }
  return 0;
}

int main(int argc,char **argv)
{
  if (argc!=4) {
    fprintf(stderr,"usage: bommerge <kicad bom CSV file as input> <parts library CSV as input and output> <merged BOM as output>\n");
    exit(-1);
  }

  char line[8192];
  FILE *f=fopen(argv[2],"r");
  char id[8192],ds[8192],url[8192],number[8192],mfgnumber[8192],source[8192];
  int line_num=0;
  line[0]=0; fgets(line,8192,f);
  int errors=0;
  while(line[0]) {
    if (sscanf(line,"%[^;];%[^;];%[^;];%[^;];%[^;];%[^;\n\r]",id,ds,url,number,mfgnumber,source)!=6) {
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

  // Remove merged BOMs
  unlink("mouser.com.csv");
  unlink("digikey.com.csv");
  unlink("element14.com.csv");
  
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

      // Skip header line
      if (count) {
	int part_id=-1;
	dequote(des);
	for(int i=0;i<part_count;i++) {
	  if (!strcmp(des,partids[i])) {
	    part_id=i; break;
	  }
	}
	if (part_id!=-1) {
	  char filename[8192];
	  snprintf(filename,8192,"%s.csv",partsource[part_id]);
	  FILE *f=fopen(filename,"w");
	  dequote(num);
	  fprintf(f,"%s;%s;%s\n",
		  partnumbers[part_id],
		  partmfgnumbers[part_id],
		  num);
	  fclose(f);
	  fprintf(stderr,"INFO: Sourcing '%s' from %s\n",des,partsource[part_id]);
	} else
	  {
	    fprintf(stderr,"INFO: I don't yet know about part '%s'\n",des);
	    fprintf(stderr,"      %s x for %s\n",num,id);
	    fprintf(stderr,"      Package: %s\n",pkg);
	    fprintf(stderr,"      Designation: %s\n",des);
	    fprintf(stderr,"      Supplier: %s\n",supplier);
	    unlink("temp.html");
	    fprintf(stderr,">>> Deposit HTML page containing the appropriate part into temp.html\n");
	    FILE *f=NULL;
	    while(!f) {
	      sleep(1);
	      f=fopen("temp.html","r");
	    }

	    fprintf(stderr,"INFO: Parsing teml.html...\n");
	    sleep(1);
	    line[0]=0; fgets(line,8192,f);
	    int mouser=0;
	    char partnumber[8192],mfgpartnumber[8192],desc[8192];
	    partnumber[0]=0; mfgpartnumber[0]=0; desc[0]=0;
	    while(line[0]) {
	      if (strstr(line,"<span id=\"spnMouserPartNumFormattedForProdInfo\">")) {
		line[0]=0; fgets(line,8192,f);
		lrtrim(line);
		strcpy(partnumber,line);
		mouser=1;
	      }	      
	      if (strstr(line,"<span id=\"spnManufacturerPartNumber\">")) {
		line[0]=0; fgets(line,8192,f);
		lrtrim(line);
		strcpy(mfgpartnumber,line);
	      }	      
	      if (strstr(line,"<span id=\"spnDescription\">")) {
		line[0]=0; fgets(line,8192,f);
		lrtrim(line);
		strcpy(desc,line);
	      }
	      char datasheet[8192];
	      if (sscanf(line,"<a id=\"pdp-datasheet_0\" href=\"%[^\"]",datasheet)==1) {
		fprintf(stderr,"      Datasheet: '%s'\n",datasheet);
	      }
	      
	      line[0]=0; fgets(line,8192,f);
	    }
	    fclose(f);
	    unlink("temp.html");
	    if (mouser) {
	      fprintf(stderr,"      Mouser Part: '%s'\n",partnumber);
	      fprintf(stderr,"      Manufacturer Part: '%s'\n",mfgpartnumber);
	      fprintf(stderr,"      Description: '%s'\n",desc);

	      FILE *p=fopen(argv[2],"a");
	      // id,ds,url,number,mfgnumber,source
	      fprintf(p,"%s;%s;%s;%s;%s;%s\n",
		      des,
		      "<no datasheet>",
		      "<no URL>",
		      partnumber,
		      mfgpartnumber,
		      "mouser.com");
	      fclose(p);
	      
	    }
	  }
      }
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
