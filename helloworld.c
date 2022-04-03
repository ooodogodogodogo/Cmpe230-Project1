#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define _GNU_SOURCE

int main(int argc, char* argv[])
{
//read any text file from currect directory

char const* const fileName = "cmpe230test.txt";
 FILE *fp;

   fp = fopen("cmpe230write.txt", "w+");
   fprintf(fp, "This is testing for fprintf...\n");
   fputs("This is testing for fputs...\n", fp);
FILE* file = fopen(fileName, "r"); 

char delim[] = " ";

if(!file){
printf("\n Unable to open : %s ", fileName);
return -1;
}



char line[500];
char vars[256][256];
int size1[256];
int size2[256];
bool isassigned[256];
char scalars[256][256];
int varcount = 0 ;
int linecount = 0 ;
int scalarcount = 0 ;

while (fgets(line, sizeof(line), file)) {
  line[sizeof(line)-1] = 0;
  char thisline[500]; // string of iterated line, stable

  printf("newline \n" ); 
  memcpy(thisline, line, sizeof(line));
  printf(thisline);

  char *ptr = strtok(line, delim);
  char types[8];
  strncpy(types,thisline,7);
  types[7] = 0; 

  bool definition = (strstr("scalar ",types) || strstr("vector ",types) || strstr("matrix ",types)) ; // true if line is a definition line
  bool assignment = (strstr( thisline, "{") && strstr(thisline, "}"));  // true if line is a matrix or vector
  bool hascomments = (strstr(thisline, "#"));
  bool isscalardef = (strcmp(ptr,"scalar") == 0);
  bool isfloat = (strstr(thisline,"."));
  printf("%d   assignment \n", assignment);
/*
  printf("---- \n") ;
  printf(types);
  printf("%d\n",definition);
*/
  bool addline = true ;
    while(ptr != NULL)
    {
      printf(thisline);
      printf(" thisline \n");
      if ( strcmp("scalar",ptr) == 0 && isscalardef){  // case: scalar definition
        fputs("int ", fp);
        printf(ptr);
      }

      else if(strstr(thisline, "[") && definition ){ // case non-scalar definition
        addline = false;
        ptr = strtok(NULL, delim);
        if (strstr(thisline, ",")){ // case 2d array definition

          printf("comma \n") ;
          char *comma;
          int commaindex;
          char *bracket;
          int bracketindex;  

          comma = strchr(ptr, ',');
          commaindex = (int)(comma - ptr);

          bracket = strchr(ptr, '[');
          bracketindex = (int)(bracket - ptr);

          char firstdimension[256];
          strncpy(firstdimension, &ptr[bracketindex+1], commaindex-bracketindex-1) ;
          char varname[256];
          strncpy(varname,ptr,bracketindex);
          varname[bracketindex] = 0;
         // fputs(varname,fp);
          firstdimension[commaindex-bracketindex-1] = 0 ;
        // printf (firstdimension) ;

        //  fputs(firstdimension,fp); 

          int len = strlen(ptr) ;

          char seconddimension[256]  ;
          strncpy(seconddimension,&ptr[commaindex+1], strlen(ptr)- commaindex -3);
          seconddimension[strlen(ptr)- commaindex -3] = 0;
        //  fputs(seconddimension,fp ) ;
          
          strncpy(vars[varcount], varname, strlen(varname));
          printf(vars[varcount]);
          printf("\n\n\n");
        
          size1[varcount] = atoi(firstdimension);
          size2[varcount] = atoi(seconddimension);  
          varcount = varcount + 1 ;
          break ;
        }
        else {  // case 1d array definition

          char *e;
          int index;
          e = strchr(ptr, '[');
          index = (int)(e - ptr);
          char varname[256];
          strncpy(varname,ptr,index);
          varname[index] = 0;
          printf("varname");
          strcpy(vars[varcount],varname);
        //  fputs(varname,fp);
          printf("varnamedone");
          int len = strlen(ptr)  ;
          char vectorsize[256];
          strncpy(vectorsize,&ptr[index+1],len-index-3);
          printf("vectorsize");
          int vectsizeint = atoi(vectorsize);
          size1[varcount] = vectsizeint ;
          size2[varcount] = 1;
        //  fputs(vectorsize,fp);
          
          varcount = varcount + 1 ;
          printf("%d ", ptr);
          break;
        }

      }
      else if (assignment){  // case assign vector or matrix
          int i ;
          printf("\n    ---Fine---   \n");
          if (isfloat){
            fputs("float " , fp);
          }
          else {
            fputs("int ",fp);
          }
          for(i = 0; i < varcount; ++i)
          {
              if(!strcmp(vars[i], ptr))
              {
                printf("\n    ---Fine---   \n");
                fputs(vars[i],fp);
                fputs("[",fp);
                fprintf(fp,"%d",size1[i]);
                fputs("][",fp);
                fprintf(fp,"%d",size2[i]);
                fputs("] = {",fp);

                char *e;
                int index;
                e = strchr(thisline, '{');
                index = (int)(e - thisline);
                char nums[256];
                strncpy(nums,&thisline[index+1],strlen(thisline) -index -3);
                char *commaptr = strtok(nums,delim);
                int numofelements = size1[i] * size2[i];
                int elementcounter = 0 ;
                while (commaptr != NULL){
                  elementcounter = elementcounter + 1 ;
                  fputs(commaptr,fp);
                  if (elementcounter<numofelements){
                    fputs (",",fp);
                  }
                  commaptr = strtok(NULL,delim);
                }
                //fputs(nums,fp);
                fputs("}",fp);
                break;
              }
          }
          break;
      }
      else if (strstr(ptr,"printsep()")) { // case printsep
        fputs("printf(\"------------\n\")",fp);
      }
      else {
        if (strstr(thisline, "{") && assignment){ // case not used

          break; 
          char *e;
          int index;
          e = strchr(ptr, '{');
          index = (int)(e - ptr);

          char allelements [256];
          strncpy(allelements,thisline,index);
          fputs("float",fp);
          fputs(ptr,fp) ;
          fputs(" = ", fp);
          char *elementptr = strtok(allelements, delim);
            
          while(elementptr != NULL)
      {
          fputs(ptr,fp);
          fputs(",",fp);
          elementptr = strtok(NULL,delim);
      }
          printf(thisline);
          break ;
        }
        else {
          if (isscalardef){ // case scalar initialize to 0 
            strncpy(scalars[scalarcount], ptr, strlen(ptr));
            scalarcount = scalarcount + 1 ; 
            fputs(ptr,fp); 
            fputs( " = 0", fp);

          }
          else {
            fputs("bad choice \n",fp);
          }
          printf("%d ", ptr);
          break;
        }

      }
      printf(" ");
      fputs(" ",fp); 

      ptr = strtok(NULL, delim); // get new token
    }
    if (addline){
    fputs(";\n",fp);
    } // end of line
    printf(";\n");
  }

fclose(file);
fclose(fp);


return 0;
}

    
