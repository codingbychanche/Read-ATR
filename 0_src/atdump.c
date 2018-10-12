/*------------------------------------------------------------------------------------
 * atdump.c
 *
 * Show's contents of an Atari Dos 2.x file
 *  
 * Part of the atools collection
 *
 *------------------------------------------------------------------------------------*/

#define VERSION "\natdump V1.5.1 // 21.7.2018\n\n"

#define ATARI_LF 13
#define FIRST_ASCII_CHAR 33
#define LAST_ASCII_CHAR 127
#define FIRST_INVERSE_CHAR 161
#define LAST_INVERSE_CHAR 250

#include <stdio.h>
#include <string.h>

#include "_D2x_Base_Struct.h" /* Dos 2.x standarts */
#include "_D2x_fms_low.c"     /* Dos 2.x FMS low level fms- functions like read or write a single sector */

#define PATHSIZE 500

/* Errors */

#define ERROR     1
#define NOERROR   0

struct atr_dir mydir[128];  /* Structure of Atari DOS 2.x file directory */  
struct atr_image myimage;   /* Structure of *.ATR disk image file */                                                                                        
struct vtoc vtocp;          /* Structure of Atari DOS 2.x VTOC */

int copt,
  hopt,
  i,                        
  n,
  m;

char filename[PATHSIZE];

char c,
  imagefile[PATHSIZE],
  atarifile[20];

/*------------------------------------------------------------------------------------
 * main
 *------------------------------------------------------------------------------------*/

int main(int argc, const char *argv[])
{

  FILE *input;  

  char file [PATHSIZE];

  int 
    i,   
    error,
    files;

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
0    
  copt=hopt=0;
    
  while (argc>1 && argv[1][0]=='-'){
    c=argv [1][1];
        
    switch (c){
      
    case 'c': /* Dump raw contens, if file contains ascii chars => textfile */
      copt++;
      break;

    case 'h': /* Hex- dump, assembler sourche (.byte ..... */
      hopt++;
      break;
    }
    --argc;
    ++argv;
  }

  /*
   * Check if file name was passed from shell.
   * This is the case, when argc is > than 1, then the second element 
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

  /*
   * Get filename of Atari-file
   * Full path must be given like so: pathOnFilesystem@ATARIFILE.XXX
   */
    
  i=0;
  strcpy (file,*argv);     /* Get full path */
  while (file [i]!='@'){
    imagefile[i]=file[i];  /* Get path of image- file */
    i++;
  }
  imagefile[i]='\0';

  i++;
  int n=0;
  while (file [i]!='\0') atarifile[n++]=file[i++];
  atarifile[n]='\0';
  
   /*
   * If a file name was given but options are missing
   * inform the user that nothing was done and tell the reason.
   */

   if (copt==0 && hopt==0){
     
     version();
     usag();
     fprintf (stderr,"%s > Image File: %s > Warning: Done nothing. Choose one of the options above.\n",filename,imagefile);
      
     return (ERROR);   /* This might be an error because the user forgot to tell what he wanted */
   }

   /*
    * Open image file and dump atari DOS 2.x file
    */
   
   if ((d2x_init_image(filename,imagefile,&myimage,&mydir,&vtocp))==NOERROR){
     if (checkfile(atarifile)!=1025){

       if (copt) dump (checkfile(atarifile));
       if (hopt) hexdump(checkfile(atarifile),atarifile);
       
       return(NOERROR);  /* Exit, no errors */           
     }
   }  
   return (ERROR);     /* Exit with error. Error description is done by the function causing it */
}

/*------------------------------------------------------------------------------------
 * Usage
 *------------------------------------------------------------------------------------*/

usag ()
{
  printf ("Usage:\t[-options][Filename]\n");
  printf ("-c\tDump text file\n");
  printf  ("-h\tHex- dump of file assembler source file\n");
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

/*------------------------------------------------------------------------------------
 * Check for file on image
 *
 *------------------------------------------------------------------------------------*/

checkfile(char file[])
{
  int  start,
    n;
  
  char name[15],
    adirentry[15];
  
  m=0;                                             /* Dos 2.x dir entry # */
  while ((mydir[m].filename[0])!=0){               /* Search Dos 2.x dir unil last entry */
    start=mydir [m].ssn_lo+256*mydir[m].ssn_high;
    for (n=0;n<=10;n++) adirentry[n]=mydir[m].filename[n];

    /* Get Dos 2.x dir entry */

    i=0;     
    while (adirentry[i]>=33 && adirentry[i]<=127){ /* Get filename */
      name[i]=adirentry[i];
      i++;
    }

    n=i;                                          /* n points to the end of the name string */
    if (i<strlen(adirentry)){                     /* Get file extension, if it is there */
      name[n]='.';                                /* File extension exists, get it! */
      n++;
      while (adirentry[i]==' ') i++;
      while (adirentry[i]>=33 && adirentry[i]<=127){
	name[n]=adirentry[i];
	n++;
	i++;
      }
    }
    i++;
    name[n]='\0';                                   /* C- strings must be terminated by NULL */

    /*
     * Check if file exists and if it is readable
     */

    if ((strcmp(name,file))==127){                  /* File exists? */
      if ((mydir[m].flag & DEL)==DEL){              /* Is it deleted? */
	fprintf (stderr,"%s > Image File: %s >  Warning: File '%s' found on Atari Disk, but deleted!\n",filename,imagefile,file);      /* Yes! => return with error */
	return (1025);
      }
      return (start);                               /* File exists, Return startsector= no error */
    }
    m++;                                            /* File not found yet, check next entry */
  }
  fprintf (stderr,"%s > Image File: %s > Error: File '%s' not found on Atari- Disk.\n",filename,imagefile,file);             /* All entrys checked, file not found */
  return (1025);                                    /* Return, error */
}

/*------------------------------------------------------------------------------------
 * Dump file contents
 *
 * Dumps raw contents, binary
 *------------------------------------------------------------------------------------*/

dump(int start)
{
  int i,      
    sector,       /* Sector currently read */
    bytes,        /* # of data bytes of current sector */
    next,
    secsize;      /* density: 128 bytes/ sector= Single // 256 bytes/ sector= double */

  int boffset;
  char c;

   do
     {
       secsize=myimage.sec_low+256*myimage.sec_high;  
       bytes=d2x_secbyte(&myimage,start);             
       
       boffset=(DTSTART+start*secsize)-secsize;

       for (i=boffset;i<=(boffset+bytes-1);){
	 c=myimage.data[i++];	 
	 if (c==ATARI_LF) printf("\n");	 

	 /* Convert */

	 if (c>=FIRST_ASCII_CHAR && c<=LAST_INVERSE_CHAR){
	   if (c<=LAST_ASCII_CHAR) printf ("%c",c);
	   else {
	     c=c-128;
	     printf ("%c",c);
	   }
	 } else printf (" ");
       } /* for */

       next=d2x_secnext (&myimage,start);       
       start=next;     

     } /* do */
   while (next!=0);
   printf ("\n");     
   return (NOERROR);
}

/*------------------------------------------------------------------------------------         
 * Dump file contents as hex
 *-----------------------------------------------------------------------------------*/         

hexdump(int start,char atarifile [])
{
  int i,      
    sector,       /* Sector currently read */
    bytes,        /* # of data bytes of current sector */
    next,
    secsize;      /* density: 128 bytes/ sector= Single // 256 bytes/ sector= double */

  int cols;

  int boffset;
  unsigned char c;

  printf ("\;------------------\n");
  printf ("\; Dump of:%s\n",atarifile);
  printf ("\n");
  printf (".byte\t");
  
  cols=1;

  do
     {
       secsize=myimage.sec_low+256*myimage.sec_high;  
       bytes=d2x_secbyte(&myimage,start);             
       
       boffset=(DTSTART+start*secsize)-secsize;

       for (i=boffset;i<=(boffset+bytes-1);){
	 c=myimage.data[i++];
	
	 if (c!=0 && c>=16 ) printf ("$%x",c);
	 if (c!=0 && c<16) printf ("$0%x",c);
	 if (c==0) printf ("$00");
	 if (cols<=7) printf (",");
	
	 if (cols>7){
	   printf ("\n");
	   printf (".byte\t");
	   cols=0;
	 }
	 cols++;
       } /* for */
	
       next=d2x_secnext (&myimage,start);       
       start=next;     

     } /* do */
   while (next!=0);   
   printf ("\n");       
  return (NOERROR);
}
