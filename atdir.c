/*------------------------------------------------------------------------------------
 * atdir.c
 *
 * Show disc directory of an *.ATR file image.
 *  
 * Part of the atools collection
 *
 * Revised: 16.3.2016
 *------------------------------------------------------------------------------------*/  

#define VERSION "\natdir V2.7 // 16.3.2016\n\n"

#include <stdio.h>

#include "_D2x_Base_Struct.h" /* Dos 2.x standarts */
#include "_D2x_fms_low.c"     /* Dos 2.x FMS low level fms- functions like read or write a single sector*/

#define PATHSIZE 500

/* Define's for 'trace' */

#define SIZE_ERR 1
#define FNO_ERR  2
#define NSEC_INV 4
#define NSEC_LOOP  8

/*
 * Global
 */

struct atr_dir mydir[128];  /* Structure of Atari DOS 2.x file directory */  
struct atr_image myimage;   /* Structure of *.ATR disk image file */                                                                                        
struct vtoc vtocp;          /* Structure of Atari DOS 2.x VTOC */

int i;

/*------------------------------------------------------------------------------------
 * main
 *------------------------------------------------------------------------------------*/

int main(int argc, const char *argv[])
{

  FILE *input;
    
  int 
    i,   
    error,
    vopt,
    fopt,
    sopt,
    files;

  char 
    c,
    file [PATHSIZE];
          
  /*                                
   * Check parameters
   */
    
  if (argc<=1) {
    version();
    descripe();
    usag();
    return(0); /*  No errors! */
  }
    
  /*
   * Check for options
   */
    
  vopt=sopt=fopt=0;
    
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
      sopt++;
      break;
    }
    --argc;
    ++argv;
  }
  
  /*
   * Check if file name was given.
   * This is the case, when argc is > than 1, then the second element 
   * contains what shpuld be the file name string (the argument at 0 is the 
   * programm name (atdir).
   *
   * If argc=1, then the user may have forgotten to pass a file name.
   */

  if (argc>1) argv++;
  else{ 
    printf ("No file name of atr- image given.\n\n");
    usag();
    return(0); /* Exit, no errors...*/
  }
  
  /*
   * Display program name and version, if no '-v' option
   */
    
  if (vopt==0) version();
    
  /*
   * Open Image and show dir
   */
  
  /* Process one file */

  if (!fopt){ 
    if ((image_open (*argv))!=1){
      if (sopt) sdir(*argv);
      else dir (*argv);
      return(0); /* Exit, no errors */
    }
  
  /* Batch processing */

  } else { 
    files=0;
    printf ("Input file:%s\n",*argv);

    if ((input=fopen (*argv,"r"))!=NULL){
      while (fgets(file,PATHSIZE,input)){
          
          /* Path names must be terminated by 'NULL'! Remove '\n'  */
          
          char *p=strchr(file,'\n');            
          if (p!=NULL) *p='\0';    
          
          if ((image_open(file))==0){
              if (sopt) sdir (file);
              
          else dir (file);
	}
	files++;
      }
      printf ("Processed %d image files\n\n",files);
      return (0); /* Exit, no errors! */

    } else {
      printf ("Could not read batch file:%s\n",*argv);
      return (1); /* Exit, error! */
    }

    /* End of Show- dir */
    
    fclose (input);
    return (0);     /* Exit, no errors! */
  }
  return (1);       /* Exit, error! */
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
 * Open image file
 *-----------------------------------------------------------------------------------*/

image_open (char *path[PATHSIZE])
{
  int 
    error,
    i;

  error=d2x_init_image(path,&myimage,&mydir,&vtocp);

  if (error!=0){
    printf ("%s\n",path);
    printf ("INIT: There is something wrong with your *.ATR file image\n\n");
    
    if (error==IMAGE_READ_ERR)
      printf ("- The image could not be read from the filesystem\n\n");

    if (error==IMAGE_NOT_VALID)
      printf ("- File could be read, but it is not a valid *.ATR disk image file\n\n");

    if (error==SECTORBYTES)
      printf ("- Bytes per sector > 256!\n\n");
    
    if (error==SECTORS)
      printf ("- # of sectors >1020\n\n");

    return(1);

  } else {

    /*
     * Image could be read and it is a valid *.ATR image file  
     * Check, is it a Dos 2.x formated disk?
     */
    
    if ((vtocp.sec_low+vtocp.sec_high*256!=1010) && /* Just a simple test, if total ' of sectors */
	(vtocp.sec_low+vtocp.sec_high*256!=707)){   /* does not match what we expect 
						    /* => no know dos format! */
      
      printf ("%s\n",path);
      printf ("Valid *.ATR disk image but, not DOS 2.x formated. No Directory!\n\n");
      return (1);
    }

  }
  return (0);                                       /* No error! */
}

/*------------------------------------------------------------------------------------      
 * Show dir
 * Basic version.
 *----------------------------------------------------------------------------------*/

sdir (char *path [PATHSIZE])
{
  int 
    fno=0,
    stat;

  char
    filename [9],
    ext [4];

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
 *
 * Long version.
 *------------------------------------------------------------------------------------*/
    
dir (char *path[PATHSIZE])
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
    
    printf ("Image file: %s\n\n",path); 
    printf ("# of useable data sectors on disk/ unused data sectors on disk:%d/ %d\n\n",sectors,free_sec);
    
    printf ("FN\tStat\tFilename\tStart\t#Sect.\t#Bytes\tSector chain\n");
    printf ("---\t------\t---------------\t-----\t------\t------\t------------\n");          
    
    fno=0;

    while ((mydir[fno].filename[0])!=0 && (fno<=64))     /* Read from image until last entry */
    {
	
      /*
       * Calculate file size => # of sectors
       */
	
      if (mydir[fno].count_high<=3){
	size=mydir[fno].count_low+256*mydir[fno].count_high;
      }else{
	printf ("DIR: File #%d -> Invalid # of sectors\n",fno);
	return (1);
      }
      
      /*
       * Calculate start sector
       */
        
      if (mydir[fno].ssn_high<=3){
	start=mydir [fno].ssn_lo+256*mydir[fno].ssn_high;
      } else{
	printf ("DIR: File #%d -> Invalid start sector. Start sector>1020\n",fno);
	return (1);
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
      
      filename [8]=0;  /* Terminate with \0 to make it a C compatible string */        
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
        
        if ((fstat & FNO_ERR) == FNO_ERR) printf ("\tF#!");         /* File number! */
        if ((fstat  & SIZE_ERR) == SIZE_ERR)printf ("\tSize!");     /* File size! */
	if ((fstat & NSEC_INV)== NSEC_INV)printf ("\tNSec!");       /* Invalif next sector! */
	
       
	//if ((fstat & NSEC_LOOP)== NSEC_LOOP){
	//  printf ("\tLoop!\n");                                     /* Next=Prev. sector! */
	//  break;
	//}
        

	if (fstat== 0) printf ("\tOK");                             /* File is OK! */

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
  int prev_sec[1024];                        /* Files sector chain */

  count=err=0; 

  do
    {
      prev_sec[count]=start_sec;
      next=d2x_secnext (&myimage,start_sec);        /* Get next sector of file */

      for (i=0;i<=count;i++){                       /* Check sector chain */
	if (next==prev_sec[i]) err=err | NSEC_LOOP;    /* trace would loop if we did not abord*/
      }
      if ((err & NSEC_LOOP)==NSEC_LOOP){
	printf ("LOOOOOOP");
	break;
      }

      if (next>=1020){
	err=err | NSEC_INV;
	break;
      }
      if ((d2x_secfile (&myimage,start_sec))!=fn) err=err | FNO_ERR; 
      
      start_sec=next;                       /* Get next sector in chain */      
      count++;
    }
  while (next!=0);                          /* If next sector=0 => end of file reached */

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
  int i,b;
  b=0;

  do{
    i=d2x_secnext (&myimage,sec);

    if (i>=1020)  break;

    b=b+d2x_secbyte (&myimage,sec);
    sec=i;
    }
  while (sec !=0);
  
return (b);
}
