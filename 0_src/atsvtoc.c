/*------------------------------------------------------------------------------------
 * atsvtoc
 *
 * Show disc's volume table of contents
 *  
 * Part of the atools collection
 *
 * Revised: 20.5.2018
 *------------------------------------------------------------------------------------*/  


#define VERSION "\natsvtoc BETA // 20.5.2018\n\n"

#include <stdio.h>

#include "_D2x_Base_Struct.h" /* Dos 2.x standarts */
#include "_D2x_fms_low.c"     /* Dos 2.x FMS low level fms- functions like read or write a single sector*/

#define PATHSIZE 500


/* The error */

#define ERROR     1
#define NOERROR   0

/*
 * Global
 */

struct atr_dir mydir[128];  /* Structure of Atari DOS 2.x file directory */  
struct atr_image myimage;   /* Structure of *.ATR disk image file */                                                                                        
struct vtoc vtocp;          /* Structure of Atari DOS 2.x VTOC */

int k,l;
int c,vopt,copt;

/*------------------------------------------------------------------------------------
 * main
 *------------------------------------------------------------------------------------*/

int main(int argc,const char *argv[])
{

  FILE *input;

          
  /*                                
   * Any parameters passed?
   * If not, show info, return to shell.
   */
    
  if (argc<=1) {
    version();
    descripe();
    usag();
    return(NOERROR); /*  No errors! */
  }

  /*
   * Check parameters passed and set options accordingly.
   * If error, return to shell!
   */

  vopt=copt==0;
  
  while (argc>1 && argv[1][0]=='-'){
    c=argv [1][1];
        
    switch (c){
    case 'v':           /* Don't display copyright message */
      vopt++;
      break;
    case 'c':            /* Markup VTOC map  with colors */
      copt++;
    break;
    } /* switch */
    --argc;
    ++argv;
  } /* while */
  
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
    printf ("No file name of atr- image given.\n\n");
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
   * Open ATR- disc image
   */
  if ((image_open (*argv))!=1){
    printf ("\n\nChecking disk image:%s\n\n",*argv);
    shvtoc(myimage.data);
  }

  return (NOERROR);
}

/*------------------------------------------------------------------------------------
 * Usage
 *------------------------------------------------------------------------------------*/

usag ()
{
  printf ("Usage : [-v][Filename]\n");
  printf ("-v:\t\tHide coyright message\n");
  printf ("-c:\t\tMar VTOC map with colors\n");
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
  printf ("'atsvtoc' shows volume table of contents (VTOC) of an Atari DOS 2.x formated disc image\n");
  printf ("Works for single, enhanched and double density formated disc images\n\n");
}

/*------------------------------------------------------------------------------------
 * Show contents of VTOC
 *
 *-------------------------------------------------------------------------------------/*/

shvtoc (unsigned char *daten)
{
    int                 offset=VTOC;    // Offset im ATR- File
    
    unsigned    char    dosbyte,        // Info zum Dos.
                        scountl,        // Anzahl der Sektoren der Diskette Low- Byte
                        scounth,        // Anzahl der Sektoren der Diskette High- Byte
                        sfreel,         // Anzahl der freien Sektoren Low- Byte
                        sfreeh,         // Anzahl der freien Sektoren High- Byte
                        bytes_sec,      // Bytes/ Sektor
                        byte,
                        fno;
    
    int                 m,i,j,            // Zähler
                        totalsektors,   // 16 Bit für die Anzahl der Sektoren
                        totalused;      // Und nochmal 16 Bit für die Anzahl der benutzten Sektoren

    int taken,empty,sector;
    
    
    //
    // Vtoc Bytes ermitteln
    //
       
    dosbyte=daten [offset];

    bytes_sec=myimage.sec_low+256*myimage.sec_high;
    
    scountl=daten [offset+1];           // Anzahl der nutzbaren Sektoren
    scounth=daten [offset+2];
    totalsektors=scountl+256*scounth;
    
    sfreel=daten [offset+3];            // Anzahl der freien Sektoren
    sfreeh=daten [offset+4];
    totalused=sfreel+256*sfreeh;

    
    /*
     * Some info...
     */

    printf ("Volume Table of Contents (VTOC)\n");
    printf ("-------------------------------\n");
    printf ("Dos Version:%d\n",dosbyte);
    
    printf ("Bytes/ sector:%d and # of usable sectors=%d =>",bytes_sec,totalsektors);
    if(bytes_sec==128 && totalsektors<1000) printf ("Single density\n");
    if(bytes_sec==128 && totalsektors>1000) printf ("Enhanced density\n");
    if(bytes_sec==256 && totalsektors<1000) printf ("(True) double density\n");
    
    printf ("Total number of free Sectors:%d\n\n",totalused);
    
    /*
     * Show sector map
     * 
     * Reads from the VTOC. Bit set means the sector can be overwritten.
     * This is the case if it belongs to an deleted file.
     *
     * Bit not set means the sector can not be overwritten. Reason for that 
     * is that it contains data of an active (not deleted file).
     *
     * Status of sector and file # to which the sectors data belong to
     * are displayed. 
     * 
     */
    
    offset=offset+10;
    taken=empty=0;
    sector=1;

    for (m=0;m<30;m++){                     /* Sector map deisplayed will have 30 rows */
      for (i=0;i<3;i++){                    /* and 3 columns, each 8 sectors. Thats 3 x 30 = 90 Vtoc Bytes   */
	byte=daten[offset];
	printf ("%d\t",sector);             /* Show first sector in row */
	for (j=0;j<8;j++){
	  fno=d2x_secfile(&myimage,sector); /* Get # of file that this sectors data belong to */

	  if(copt==1)
	    printf ("\e[40;38;5;%dm",fno+100);   /* Set color accordig to file # */
	 
	  if (sector >3 &&(sector<360 || sector > 368)){
	    if ((byte & 128)==128){
	      printf ("E");             
	      empty++;
	    } else {
	      printf ("T");
	      taken++;
	    }
	    if (fno<10) printf ("0");        /* Print file #, leading 0 if only one digit */
	    printf ("%d ",fno);

	  } else {

	    if (copt==1){
	      printf ("\e[0m");              /* Reset all escape sequences */
	      printf ("\e[1m");              /* Bold */
	    }

	    if (sector>360) printf ("DIR "); 
	    if (sector==360)  printf ("VTC ");
	    if (sector<4) printf("BSC ");
	  }
	  printf (" \e[0m");                 /* Reset all escape sequences */

	  byte=byte<<1;
	  sector++;
	}
	offset++;
	printf ("\t");
      }
      printf ("%d",sector-1);                /* Show last sector in row... */
      printf ("\n");                         /* Next row. */
    }     
    printf ("\n");
    printf ("Data sectors:\t%d\n",taken);
    printf ("Empty sectors:\t%d\n\n",empty);

    return (0);
}

/*------------------------------------------------------------------------------------     
 * Open image file
 *-----------------------------------------------------------------------------------*/

image_open (char path[PATHSIZE])
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
