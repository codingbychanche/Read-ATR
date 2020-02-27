/*------------------------------------------------------------------------------------
 * atdbasic.c
 *
 * Dumps an Atari Basic listing (translates it from tokenized format to haman readable)
 *  
 * Part of the atools collection
 *
 *------------------------------------------------------------------------------------*/

#define VERSION "\natdbasic V1.0B  // 28.11.2018\n\n"

#include <stdio.h>
#include <string.h>

#define ERROR     1
#define NOERROR   0

#define MAXPATH_LENGTH 255
#define MAX_BASIC_FILE_LENGTH 64000    /* Max size of a basic program */

int i,m,n;
char filename [MAXPATH_LENGTH];

/*------------------------------------------------------------------------------------
 * Main
 *------------------------------------------------------------------------------------*/

main (int argc,const char *argv[])
{
  char c;
  int aopt,bopt,copt;

  char atrimagefile [MAXPATH_LENGTH];

  /*
   * Atari Basic tokenized file structurre
   *
   * The file is divided into two parts:
   * - Part 1 Header
   * - Part 2 Variable name table, Variable value table and the statement table.
   *
   * Part one contains displacement values+256 (d...) which point from 
   * the begining of Part 2 to the starting byte of the three tables.
   *  
   * One must substract 256 from each displacement value to obtain the 
   * true displacement.
   *
   * dvnt= Variable name table
   * dvvt= Variable value table
   * dst=  Statement table
   * dend= 
   */

  struct basicfile 
  {
    int null;
    int vnt;
    int not_used_1;
    int vvt;
    int dst;
    int not_used_2;
    int dend;
    unsigned char token_area[MAX_BASIC_FILE_LENGTH];
  } basic;
     
  FILE *f;

  /*
   * Filename of this binary to 'filename' so that
   * it can be used gobally...
   */

  strcpy(filename,argv[0]);
          
  /*                                
   * Check parameters
   * If no paramerters where given, show program version, abstract and 
   * usage. Then abord
   */
  
  if (argc<=1) {
    version();
    descripe();
    usag();
    return(NOERROR); 
  }
    
  /*
   * Check for options
   */
    
  aopt=bopt=copt=0;
    
  while (argc>1 && argv[1][0]=='-'){
    c=argv [1][1];
        
    switch (c){
      
    case 'a': 
      aopt++;
      break;

    case 'b': 
      bopt++;
      break;

    case 'c': 
      copt++;
    break;
    }

    --argc;
    ++argv;
  }

  /*
   * Check if file name was passed from shell.
   * This is the case, when argc is >1 then the second element 
   * contains what should be the file name string (the argument at 0 is the 
   * programm name (atdir).
   *
   * If argc=1, then the user may have forgotten to pass a file name.
   */

  if (argc>1) argv++;
  else{ 
    fprintf (stderr,"%s > Error: No file name of atr- image given.\n\n",filename);
    usag();
    return(ERROR); 
  }

  /* Get path of atr- image file */

  strcpy (atrimagefile,*argv);

  /*
   * If a file name was given but options are missing
   * inform the user that nothing was done and tell the reason.
   */

   if (aopt==0 && bopt==0 && copt==0){
     version();
     usag();
     fprintf (stderr,"%s > Warning: Done nothing. Choose one of the options above.\n",filename);      
     return (ERROR);   
   }

   /*
    * Open image file and check if file exists.
    */
   
   f=fopen(atrimagefile,"r");

   if (f==NULL){
     fprintf (stderr,"%s > Error: Image file (%s) not found in disk\n",filename,atrimagefile);
     fclose(f);
     return (ERROR);
   }

   /*
    * All set, let's go
    */

   printf ("Dumping:%s\n",atrimagefile);

   /*
    * Read file header
    */

   fread (&basic.null,2,1,f);
   fread (&basic.vnt,2,1,f);
   fread (&basic.not_used_1,2,1,f);
   fread (&basic.vvt,2,1,f);
   fread (&basic.dst,2,1,f);
   fread (&basic.not_used_2,2,1,f);
   fread (&basic.dend,2,1,f);


   printf ("null:%d\n",basic.null);
   printf ("vnt:%d\n",basic.vnt);
   printf ("not used:%d\n",basic.not_used_1);
   printf ("vvt:%d\n",basic.vvt);
   printf ("dst:%d\n",basic.dst);
   printf ("not used:%d\n",basic.not_used_2);
   printf ("dend:%d\n",basic.dend);

   /* Move fp to the start of the statement table */

   fseek(f,14+basic.dst-256,SEEK_SET);

   /* Read statement table (the basic program */

   int size=fread(&basic.token_area,1,basic.dend,f);
   fclose (f);

   /* output....*/

   unsigned char line [255];
   int linenumber;
   i=0;
   while (i<=basic.dend){
     m=0;
     while (basic.token_area[i++]!=22){
       line [m++]=basic.token_area[i];
     }
     line[m]="\0";
     linenumber=line[0]*256+line[1];
     printf ("\n%d %s\n",linenumber,line);
   }

   return (NOERROR);
}

/*------------------------------------------------------------------------------------
 * Usage
 *------------------------------------------------------------------------------------*/

usag ()
{
  printf ("Usage:\t[-options][Filename]\n");
  printf ("-c\tDump text file\n");
  printf ("-h\tHex- dump of file assembler source file\n");
  printf ("-r\tDump raw contents (e.g. if file is a binary the result is an an XEX- file)\n");
}

/*------------------------------------------------------------------------------------
 * Version
 *------------------------------------------------------------------------------------*/

version ()
{
  printf (VERSION);
  printf ("Made by retrozock\n");  
  printf ("www.retrozock.com\n\n");
  return (NOERROR);
}

/*------------------------------------------------------------------------------------
 * Description
 *------------------------------------------------------------------------------------*/

descripe ()
{
  printf ("Dumps the contents of an file, saved on an Atari Dos 2.x formated disc to the standart output.\n");
  printf ("Works for single density (720 sectors, 128 bytes/ sector) enhanched density (1040 sectors, 128\n");
  printf ("bytes/ sector) and double density (720 sectors/ 256 bytes/ sector).\n");
  printf ("Debugging has been done, using the native 'Disk Wizzard II' tool on an emulated Atari 800 XL (Atari800MacX 13.26.26).\n\n");
  return (NOERROR);
}
