/*------------------------------------------------------------------------------------
 * atdir.c
 *
 * Show disc directory of an *.ATR file image.
 *  
 * Part of the atools collection
 *
 *------------------------------------------------------------------------------------*/  

#define VERSION "\natdir V2.8.8 // 30.08.2021\n\n"

#include <stdio.h>

#include "_D2x_Base_Struct.h" /* Dos 2.x standarts */
#include "_D2x_fms_low.c"     /* Dos 2.x FMS low level fms- functions like read or write a single sector*/

#define PATHSIZE 500

/* Define's for 'trace' */
 
#define SIZE_ERR  1
#define FNO_ERR   2
#define NSEC_INV  4
#define NSEC_LOOP 8


/* Errors */

#define ERROR     1
#define NOERROR   0

/*
 * Global
 */

struct atr_dir mydir[128];  /* Structure of Atari DOS 2.x file directory */  
struct atr_image myimage;   /* Structure of *.ATR disk image file */                                                                                        
struct vtoc vtocp;          /* Structure of Atari DOS 2.x VTOC */

int 
i,   
  error,
  vopt,
  fopt,
  sopt,
  aopt,
  jopt,
  files;
char 
c,
  file [PATHSIZE],
  thisfile [PATHSIZE];

char unused;

/*------------------------------------------------------------------------------------
 * main
 *------------------------------------------------------------------------------------*/

int main(int argc,const char *argv[])
{
  FILE *input;
   
  /*
   * Filename of this binary to 'filename' so that
   * it can be used gobally...
   */

  strcpy(thisfile,argv[0]);
       
  /*                                
   * Any parameters passed?
   * If not, show info, return to shell.
   */
    
  if (argc<=1) {
    version();
    descripe();
    usag();
    return(NOERROR);
  }

  /*
   * Check parameters passed and set options accordingly.
   * If error, return to shell!
   */

  vopt=sopt=fopt=aopt=jopt=0;
  
  while (argc>1 && argv[1][0]=='-'){
    c=argv [1][1];
        
    switch (c){
    case 'v':           /* Don't display copyright message */
      vopt++;
      break;
    
    case 'f':           /* Input is a batch file */
      fopt++;
    break;
 
    case 's':           /* Show short version of dir (like DUP does) */
      sopt++;           /* Don't check files read, do not show start, end sector # ..... */
      break;
      
    case 'a':           /* Show dir in an 'atdump' compatible input format */
      aopt++;
      break;

    case 'j':
      jopt++;
      break;
    } /* switch */
    --argc;
    ++argv;
  } /* while */
  
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
    fprintf (stderr,"%s > Error: No file name of atr- image given.\n\n",thisfile);
    usag();
    return(ERROR); 
  }
    
  /*
   * All went well, now, start.....
   *
   * Display program name and version, if no '-v' option
   */
    
  if (vopt==0) version();
    
  /*
   * Open Image and show dir
   */
  
   /* Process one file */

  if (!fopt){ 
    
    /*
     * Show either long,atdump compatible or short version of dir
     *
     * Long version checks each file for errors in the file
     * system and show it's detailed status.
     *
     * Short version shows just the file names and some very 
     * basic status info.
     */
    if ((d2x_init_image(thisfile,*argv,&myimage,&mydir,&vtocp))==NOERROR){
      if (!sopt && !aopt) dir (*argv);        /* Detailed version */
      if (sopt) sdir(*argv);                     /* Short version */
      if (aopt)	atdumpdir(*argv);                /* atdump compatible */
      if (jopt) atjsondir(*argv);
      return(NOERROR); 
    }
    return (ERROR);
  }

  /* Batch processing */

  if (fopt) { 
    files=0;
    printf ("Input file:%s\n",*argv);
    if ((input=fopen (*argv,"r"))!=NULL){
      while (fgets(file,PATHSIZE,input)){
	
	/* Path names must be terminated by 'NULL'! Remove '\n'  */
        
	char *p=strchr(file,'\n');            
	if (p!=NULL) *p='\0';    
        
	if ((d2x_init_image(thisfile,file,&myimage,&mydir,&vtocp))==NOERROR){
	  if (!sopt && !aopt) dir (file);           /* Detailed version */	  
	  if (sopt) sdir(file);                     /* Short version */
	  if (aopt) atdumpdir(file);                /* atdump compatible */
	  if (jopt) atjsondir(*argv);
	}
	files++;
      }
      printf ("Processed %d image files\n\n",files);
      return (0); /* Exit, no errors! */
    } else {
      fprintf (stderr,"Error: Could not read batch file:%s\n",*argv);
      return (ERROR); /* Exit, error! */
    }
    
    /* End of Show- dir */
    
    fclose (input);
    return (NOERROR);   
  }
  return (ERROR);       
  }

/*------------------------------------------------------------------------------------
 * Usage
 *------------------------------------------------------------------------------------*/

usag ()
{
  printf ("Usage : [-v][Filename]\n");
  printf ("-v:\t\tHide coyright message\n");
  printf ("-f:\t\tFilename contains path- list of *.ATR images. Batch processing\n");
  printf ("-s:\t\tShow short version of disk directory (same as DUP.SYS)\n");
  printf ("-a:\t\tDirectory dump can be used by 'atdump' as input\n");
  printf ("Filename:\tValid *.ATR disk image, Dos 2.x formated\n\n");
}

/*------------------------------------------------------------------------------------
 * Version
 *------------------------------------------------------------------------------------*/

version ()
{
  printf (VERSION);
  printf ("Made by retrozock\n");  
  printf ("www.retrozock.com\n\n");
}

/*------------------------------------------------------------------------------------
 * Description
 *------------------------------------------------------------------------------------*/

descripe ()
{
  printf ("'atdir' shows contents of an Atari 8- Bit Dos 2.x formated disk image\n");
  printf ("Works for single, enhanched and double density formated disk images\n\n");
}

/*------------------------------------------------------------------------------------      
 * Show dir
 * Basic version.
 *----------------------------------------------------------------------------------*/

sdir (char path [])
{
  int 
    fno=0,
    stat;

  char
    filename [9],
    ext [4];

  printf ("------------------------------------------------------------------------\n");
  printf ("Imagefile: %s\n",path);

  while ((mydir[fno].filename[0])!=0){               /* Read from image until last entry of directory */
    stat=mydir [fno].flag;
    strncpy (filename,mydir[fno].filename,8);
    strncpy (ext,mydir[fno].ext,3);
    
    if ((stat & DEL) ==DEL && filename[0]!=0){       /* Deleted? */
      printf ("#\t");
    }else{
      printf ("\t");
    }
    
    printf ("%s.%s\n",filename,ext);
    fno++;
  }
  return (0);
}

/*------------------------------------------------------------------------------------      
 * Show dir
 * Dumps the Atatri image's directory to stdout in an 'atdump' compatible format.
 * Lists created with this function can be used by 'atdump' as Input. 
 *----------------------------------------------------------------------------------*/

atdumpdir (char *path [PATHSIZE])
{
  int 
    fno=0,
    stat;

  char
    filename [9],
    ext [4];

   while ((mydir[fno].filename[0])!=0){                     /* Read from image until last entry of directory */
     i=0;
     while ((filename[i]=mydir[fno].filename[i])!=' ' && i<=7) 
       i++;
     filename[i]='\0';
       
    strncpy (ext,mydir[fno].ext,3);

    stat=mydir [fno].flag;
    if ((stat & DEL) ==DEL && filename[0]!=0){ 
      fprintf (stderr,"%s > Warning: The file '%s.%s' on image file '%s' has status 'deleted'\n",thisfile,filename,ext,path); /* If deleted, mark so */
    }else{
      printf ("%s@%s.%s\n",path,filename,ext);
    }
    fno++;
  }
  return (0);
}

/*------------------------------------------------------------------------------------      
 * Dumps the dir to stdout as a json- data structure
 * 
 * 
 *----------------------------------------------------------------------------------*/

atjsondir (char *path [PATHSIZE])
{
  int 
    fno,
    numberOfFiles,
    stat;

  char
    filename [9],
    ext [4];

  printf ("{path\":\"%s\",\n",path); /* Start of json- struct */
  printf ("\t\"files\":[\n"); /* Start of json array */

  /* Count files on image */
  numberOfFiles=0;
  while ((mydir[numberOfFiles].filename[0])!=0)
    numberOfFiles++;

  /* Get all files and build json- struct */
  fno=0;
  while ((mydir[fno].filename[0])!=0){   
    
    /* Copy filename */
    i=0;
    while ((filename[i]=mydir[fno].filename[i])!=' ' && i<=7) 
      i++;
    filename[i]='\0';    
    strncpy (ext,mydir[fno].ext,3);
    
    /* Insert filename into json array) */
    printf ("\t\"%s.%s\"",filename,ext); 
    
    fno++;

    /* As long as it is not the last filename, add a comma to separate array elements */
    if (fno<numberOfFiles)
      printf (",\n");
  }   

  /* Terminate json array */
  printf ("\n\t]\n}\n"); 
  return (0);
}

/*------------------------------------------------------------------------------------
 * Show dir
 *
 * Long version.
 *------------------------------------------------------------------------------------*/
    
dir (char path[])
{
 int   
   stat,               /* File status from directory entry */
   start,              /* Start sector of file */
   size,               /* File size */
   sectors,            /* # of sectors of file */
   free_sec,           /* Free sectors on disk */
   fno,                /* File number */
   f_size,             /* OBSOLETE? */
    error;
        
    int fstat;
        
    char 
      filename[9],   /* File name */
      ext[4];        /* File extension */

    /*
     * Get various info about the disk (from VTOC)
     */
       
    sectors=vtocp.sec_low+vtocp.sec_high*256;
    free_sec=vtocp.sec_unul+vtocp.sec_unuh*256;
       
    /*
     * Print header
     */
    
    printf ("--------------------------------------------------------------------------\n");
    printf ("Image file: %s\n\n",path); 
    printf ("# of useable data sectors on disk/ unused data sectors on disk:%d/ %d\n\n",sectors,free_sec);
    printf ("FN\tStat\tFilename\tStart\t#Sect.\t#Bytes\tSector chain\n");
    printf ("---\t------\t---------------\t-----\t------\t------\t------------------\n");          

    fno=0;
    	
    while ((mydir[fno].filename[0])!=0 && (fno<=63))     /* Read from image until last entry */
    {
	
      /*
       * Calculate file size => # of sectors
       */
	
      if (mydir[fno].count_high<=3){
	size=mydir[fno].count_low+256*mydir[fno].count_high;
      }else{
	fprintf (stderr,"%s > Error: DIR: File #%d -> Invalid # of sectors\n",thisfile,fno);
	return (ERROR);
      }
      
      /*
       * Calculate start sector
       */
        
      if (mydir[fno].ssn_high<=3){
	start=mydir [fno].ssn_lo+256*mydir[fno].ssn_high;
      } else{
	fprintf (stderr,"%s > Error: DIR: File #%d -> Invalid start sector. Start sector>1020\n",thisfile,fno);
	return (ERROR);
      }

      /*
       * Get file status
       */
      
      stat=mydir [fno].flag;
      
      /*
       * Get file name and file extension from disk image
       */
	
      strncpy (filename,mydir[fno].filename,8);
      strncpy (ext,mydir[fno].ext,3);
      filename [8]='\0';  /* Terminate with \0 to make it a C compatible string */        
      ext [3]=0;       
      
        /*
         * Get actual file- size in bytes
         */
	
	f_size=file_size (start);
	
        /*
         * Show dir entrys
         */
	
        printf("%d\t",fno);
	
        /*
         * Check and print status of file
         */
        
        if ((stat & DEL) ==DEL && filename[0]!=0)       /* Deleted? */
            printf ("#");
        
        if ((stat & LOCK)==LOCK && filename[0]!=0)      /* Write protected? */
            printf ("*");
        
        if ((stat & NUSD)==NUSD && filename[0]!=0)      /* Never been used? */
            printf ("N");
        
        if ((stat & OPEN)==OPEN && filename[0]!=0)      /* Is open? */
            printf ("O");
        
        if ((stat & INUS)==INUS && filename[0]!=0)      /* In use? */
            printf ("U");
	
        if ((stat & DOS2) ==DOS2  && filename[0]!=0)    /* DOS 2 File */
            printf ("2");
	
        printf ("\t");
	
        /*
         * Show filename, ext, startsector, # of sectors and file size in bytes
         */
	      
        printf ("%s.%s\t%d\t%d\t%d",filename,ext,start,size,f_size);	      
	
        /*
         * Check if file is damaged (trace sector chain) and 
         * show result
         */
	
	fstat=trace (start,size,fno);
        
        if ((fstat  & FNO_ERR)   == FNO_ERR)   printf ("\tF#!");         /* File number! */
        if ((fstat  & SIZE_ERR)  == SIZE_ERR)  printf ("\tSize!");       /* File size! */
	if ((fstat  & NSEC_INV)  == NSEC_INV)  printf ("\tNSec!");       /* Invalif next sector! */
	if ((fstat  & NSEC_LOOP) == NSEC_LOOP) printf (" LOOP");         /* Sector chain loops...*/
	
	if (fstat== 0) printf ("\tOK");                                  /* File is OK! */

        /*
         * Next entry
         */
	
        printf ("\n"); /* Next line */
	
        fno++;    
    } 
    return(0);  
}

/*------------------------------------------------------------------------------------
 * trace
 *
 * Trace and check a files sector chain
 *
 * Principle of Operation and valid contitions for an undammaged file:
 * - Total # of sectors counted is the same as # of sectors stored in directory
 * - Every sector belongs to the same file # 
 * - File # matches the order of the file in the directory (countig starts with 0!)
 * 
 * If the conditons above do not apply, the file is damaged!
 *------------------------------------------------------------------------------------*/

trace (int start_sec,int size,int fn)
{
  int next,count,err;

  count=err=0; 

  do
    {
      next=d2x_secnext (&myimage,start_sec);        /* Get next sector of file */

      if (count >= 1020){
	err=err | NSEC_LOOP;                        /* If the file has more sectors than the disk */
        break;                                      /* asume that the file 'loops' */
      }

      if (next>=1020){
	err=err | NSEC_INV;
	break;
      }

      if ((d2x_secfile (&myimage,start_sec))!=fn) err=err | FNO_ERR; 
      
      start_sec=next;                               /* Get next sector in chain */      
      count++;
    }
  while (next!=0);                    /* If next sector=0 => end of file reached */

  if (count!=size) err=err | SIZE_ERR;
  return (err); 
}

/*------------------------------------------------------------------------------------
 * file_size
 *
 * Returns file- size in bytes
 *------------------------------------------------------------------------------------*/

file_size(unsigned int sec)
{
  int i,b,sectors_read;
  b=0;
  sectors_read=0;


  do{
    i=d2x_secnext (&myimage,sec);

    if (i>=1020)  break;

    b=b+d2x_secbyte (&myimage,sec);
    sec=i;
    sectors_read++;

    if (sectors_read>1020){
      b=0;
      break;
    }
  }
  while (sec !=0);
  
return (b);
}
