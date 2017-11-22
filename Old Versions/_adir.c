/*------------------------------------------------------------------------------------
 * adir.c
 *
 * Show disc directory of an *.ATR file image.
 *  
 * Part of the atools collection
 *
 * V´2.0 // Revised: 27.11.2014
 *------------------------------------------------------------------------------------*/  

#define VERSION "\nadir V2.0 // 27.11.2014\n\n"
#include <stdio.h>
#include "_Base_Struct.h"
#include "Dos2X.c"

// Define's for 'trace'

#define SIZE_ERR 1
#define FNO_ERR  2

/*-------------------------------------------------------------------------------------
 * main
 *------------------------------------------------------------------------------------*/

int main(int argc, const char *argv[])
{
    
  int i;                        // Are what they look alike, they are counters....
  int error;
    
  int vopt,copt;                 // Options
  char c;
          
  struct atr_dir mydir[128];  // Structure of Atari DOS 2.x file directory
  struct atr_image myimage;   // Structure of *.ATR disk image file
  struct vtoc vtocp;          // Structure of Atari DOS 2.x VTOC

  /*                                
   * Check parameters
   */
    
  if (argc<=1) {
    version();
    descripe();
    usag();
    return(0);
  }
    
  /*
   * Check for options
   */
    
  vopt=copt=0;
    
  while (argc > 1 && argv[1][0]=='-'){
    c=argv [1][1];
        
    switch (c){
    case 'v':   // Don't display copyright message
      vopt++;
      break;
      
    case 'c':   // Demo
      copt++;
      break;
    }
    --argc;
    ++argv;
  } 

  argv++;
  if (argv [1][0]==' '){
    printf ("empty");
    return(0);
  }

  /*
   * Display program name and version, if no '-v' option
   */
    
  if (vopt==0){
    version();
  }
    
  /*
   * Open image file
   */

  error=init_image(*argv,&myimage,&mydir,&vtocp);
  
  if (error!=0){
    printf ("%s\n",*argv);
    printf ("There is something wrong with your *.ATR file image\n\n");
    
    if (error==IMAGE_READ_ERR)
      printf ("- The image could not be read from the filesystem\n\n");
    
    if (error==IMAGE_NOT_VALID)
      printf ("- File could be read, but it is not a valid *.ATR disk image file\n\n");
        
  } else {
    
    /*
     * Image could be read and it is a valid *.ATR image file
     * Check, is it a Dos 2.x formated disk?
     */
    
    if ((vtocp.sec_low+vtocp.sec_high*256!=1010) && (vtocp.sec_low+vtocp.sec_high*256!=707)){
      printf ("%s\n",*argv);
      printf ("Valid *.ATR disk image but, not DOS 2.x formated. No Directory!\n\n");
      return (0);
      
    }
        
    /*
     * OK, show dir
     */

    dir (*argv,&mydir,&myimage,&vtocp);
        
  }

  return (1);
}

/*------------------------------------------------------------------------------------
 * Usage
 *------------------------------------------------------------------------------------*/

usag ()
{
  printf ("Usage : [-v][Filename]\n");
  printf ("-v:\t\tHide coyright message\n");
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
  printf ("'adir' shows contents of an Atari 8- Bit Dos 2.x formated disk image\n");
  printf ("Works for single, enhanched and double density formated disk images\n\n");
}

/*------------------------------------------------------------------------------------
 * Show dir
 *------------------------------------------------------------------------------------*/
    
dir (char *path[],struct atr_dir *mydir, struct atr_image *myimage, struct vtoc *vtocp)
{
  unsigned int   
    stat,               // File status from directory entry
    start,              // Start sector of file
    size,               // File size
    sectors,            // # of sectors of file
    free_sec,           // Free sectors on disk
    fno,                // File number
    f_size;             // OBSOLETE?
        
    int fstat;
        
    char filename[50],  //File name
         ext[4];        //File extension

    /*
     * Get various info about the disk (from VTOC)
     */
       
    sectors=vtocp->sec_low+vtocp->sec_high*256;
    free_sec=vtocp->sec_unul+vtocp->sec_unuh*256;
       
    /*
     * Print header
     */
    
    printf ("Image file: %s\n\n",path); 
    printf ("# of useable data sectors on disk/ unused data sectors on disk:%d/ %d\n\n",sectors,free_sec);
    
    printf ("FN\tStat\tFilename\tStart\t#Sect.\t#Bytes\tSector chain\n");
    printf ("---\t------\t---------------\t-----\t------\t------\t------------\n");          
    
    fno=0;

    while ((mydir[fno].filename[0])!=0)     // Read from image until last entry of directory is reached 
    {
	
        /*
         * Calculate file size => # of sectors
         */
	
        size=mydir[fno].count_low+256*mydir[fno].count_high;

        /*
         * Calculate start sector
         */
        
        start=mydir [fno].ssn_lo+256*mydir[fno].ssn_high;
	
        /*
         * Get file status
         */
	
        stat=mydir [fno].flag;
	
        /*
         * Get file name and file extension from disk image
         */
	
        strncpy (filename,mydir[fno].filename,8);
        strncpy (ext,mydir[fno].ext,3);
	
        filename [8]=0;  // Terminate with \0 to make it a C compatible string        
        ext [3]=0;       
        
        /*
         * Get actual file- size in bytes
         */
	
        f_size=file_size (myimage,start);
	
        /*
         * Show dir entrys
         */
	
        printf("%d\t",fno);
	
        /*
         * Check and print status of file
         */
        
        if ((stat & DEL) ==DEL && filename[0]!=0)       // Deleted?
            printf ("#");
        
        if ((stat & LOCK)==LOCK && filename[0]!=0)      // Write protected?
            printf ("*");
        
        if ((stat & NUSD)==NUSD && filename[0]!=0)      // Never been used?
            printf ("N");
        
        if ((stat & OPEN)==OPEN && filename[0]!=0)      // Is open?
            printf ("O");
        
        if ((stat & INUS)==INUS && filename[0]!=0)      // In use?
            printf ("U");
	
        if ((stat & DOS2) ==DOS2  && filename[0]!=0)    // DOS 2 File?
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
	
        fstat=trace (myimage,start,size,fno);
        
        if ((fstat & FNO_ERR) == FNO_ERR) printf ("\tF#!");         // File number!
        if ((fstat  & SIZE_ERR) == SIZE_ERR)printf ("\tSize!");     // File size!
        if (fstat== 0) printf ("\tOK");                             // File is OK!

        /*
         * Next entry
         */
	
        printf ("\n"); // Next line
	
        fno++;
    
    } // While
 
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

trace (struct atr_image *image, int start_sec,int size,int fn)
{
  int next,count,err;

  count=err=0; 

  do
    {
      next=secnext (image,start_sec); // Get next sector of file

      if ((secfile (image,start_sec))!=fn) err=err | FNO_ERR; 

      start_sec=next; // Get next sector in chain      
      count++;
    }
  while (next!=0); // If next sector=0 => end of file reached

  if (count!=size) err=err | SIZE_ERR;

  return (err); 
}

/*------------------------------------------------------------------------------------
 * bytes
 *
 * Returns # of data bytes in specified sector
 *------------------------------------------------------------------------------------*/

bytes (struct atr_image *image,int sec)
{ 
  return(secbyte (image,sec));
}

/*------------------------------------------------------------------------------------
 * file_size
 *
 * Returns file- size in bytes
 *------------------------------------------------------------------------------------*/

file_size(struct atr_image *image,unsigned int sec)
{
  int i,b;
  b=0;

  do{
    i=secnext (image,sec);
    b=b+bytes(image,sec);
    sec=i;
    }
  while (sec !=0);
  
return (b);
}
