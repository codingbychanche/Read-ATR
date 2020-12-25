/*------------------------------------------------------------------------------------
 * atsvtoc
 *
 * Show disc's volume table of contents
 *  
 * Part of the atools collection
 *
 * Revised: 20.5.2018
 *------------------------------------------------------------------------------------*/  

#define VERSION "\natsvtoc 1.0.9  // 11/2018\n\n"

#include <stdio.h>

#include "_D2x_Base_Struct.h" /* Dos 2.x standarts */
#include "_D2x_fms_low.c"     /* Dos 2.x FMS low level fms- functions like read or write a single sector*/
#define PATHSIZE 500

/* The error */

#define ERROR     1
#define NOERROR   0
#define TRUE      1
#define FALSE     0

/*
 * Global
 */

struct atr_dir mydir[128];   /* Structure of Atari DOS 2.x file directory */  
struct atr_image myimage;    /* Structure of *.ATR disk image file */                                                                                        
struct vtoc vtocp;           /* Structure of Atari DOS 2.x VTOC */

int k,l;
int c,vopt,copt;

char filename [PATHSIZE];

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

  strcpy(filename,argv[0]);
          
  /*                                
   * Any parameters passed?
   * If not, show info, return to shell.
   */
    
  if (argc==1) {
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
    fprintf (stderr,"%s > Error: No file name of atr- image given.\n\n",filename);
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

  printf ("\n\nChecking disk image:%s\n\n",*argv);
  if ((d2x_init_image(filename,*argv,&myimage,&mydir,&vtocp))==NOERROR){
    shvtoc(myimage.data);
    return (NOERROR); 
  } else return (ERROR);
}

/*------------------------------------------------------------------------------------
 * Usage
 *------------------------------------------------------------------------------------*/

usag ()
{
  printf ("Usage : [-v][Filename]\n");
  printf ("-v:\t\tHide coyright message\n");
  printf ("-c:\t\tMark VTOC map with colors\n");
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
 *------------------------------------------------------------------------------------*/

shvtoc (unsigned char daten[])
{
  int offset=VTOC;    /* Points to the start of VTOC */
  BYTE dosbyte,       /* Dos Version */    
    scountl,          /* Total #of sectors */
    scounth,        
    sfreel,           /* Total # of free sectors */
    sfreeh,        
    bytes_sec,        /* Bytes / Sector */
    byte;
  int totalsektors,totalused;      
  int taken,empty,sector;    
  int enhanced;
  
  /*
   * Get Disk status
   */
       
  dosbyte=daten [offset];            /* Dos version */
  enhanced=FALSE;             

  bytes_sec=myimage.sec_low+256*myimage.sec_high;
  
  scountl=daten [offset+1];          /* # of data sectors */ 
  scounth=daten [offset+2];
  totalsektors=scountl+256*scounth;
  
  sfreel=daten [offset+3];           /* # of free sectors */ 
  sfreeh=daten [offset+4];
  totalused=sfreel+256*sfreeh;       /* # of sectors used */
  
  /*
   * Print header for sector map
   */
  
    printf ("Volume Table of Contents (VTOC)\n");
    printf ("-------------------------------\n");
    printf ("Dos Version:%d\n",dosbyte);
    
    printf ("Bytes/ sector:%d and # of usable sectors=%d =>",bytes_sec,totalsektors);
    if(bytes_sec==128 && totalsektors<1000) printf ("Single density\n");
    if(bytes_sec==255 && totalsektors<1000) printf ("(True) double density\n");
    if(bytes_sec==128 && totalsektors>1000){
      printf ("Enhanced density\n");
      enhanced=TRUE;
    }

    /* Print sector map */

    printf ("\n");
    offset=offset+10;   /* Sector bitmap starts at byte 10 of VTOC */
    showsectormap(daten,offset,1);

    if (enhanced==TRUE){
      offset=VTOC2+84;  
      showsectormap(daten,offset,721); 
    }
    
    printf ("Total number of free Sectors:%d\n\n",totalused);
}
    
/*------------------------------------------------------------------------------------
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
 * Works for:
 * - single density formated disks (720 sectors, 128 bytes/ sector) 
 * - enhanced density formated disks (1040 sectors.128 bytes/ sector)
 *
 * VTOC
 * ---- 
 * Is loceted at sector 360. VTOC begins with 10 bytes of status information
 * and 90 bytes for the sector map for sectors 0 thru 720
 * Drive numbers sectors 1 - 720. Dos: 0 - 719 (a well known bug). 
 * Since there is no sector 0, it can not be alloceted, so, it is always marked as in use.
 * For sector 720 there is no allocation bit in the table, so this sector is out of reach
 * for the FMS (physically it is present!).
 *
 * VTOC2 
 * -----
 * Is used for enhanched density and is located at sector 1024.
 * Bytes 0 - 83 repeat VTOC bitmap for sectors 83 - 719.
 * Bytes 84 - 121 is the VTOC bitmap for sectors 720 - 1023
 * Bytes 122 - 123 Current # of free sectors above sector 719 (should be 303).
 * Bytes 124 - 127 Unused 
 *
 *------------------------------------------------------------------------------------*/
    
showsectormap(unsigned char daten[],int offset,int sector)
{

#define ROWS           30
#define COLUMNS        2
#define SEC_PER_COLUMN 8

  int m,i,j;                                /* Some counters.... */
  int byte,empty,taken;
  BYTE fno;

  for (m=0;m<ROWS;m++){                     /* Sector map displayed will have 30 rows */
    for (i=0;i<COLUMNS;i++){                /* and 3 columns, each 8 sectors. Thats 3 x 30 = 90 Vtoc Bytes   */
      byte=daten[offset];
      if (sector < 1041) printf ("%d\t",sector);             /* Show first sector in row */
      for (j=0;j<SEC_PER_COLUMN;j++){
	fno=d2x_secfile(&myimage,sector);                    /* Get # of file that this sectors data belong to */
	
	if(copt==1)
	  printf ("\e[40;38;5;%dm",fno+100);                 /* Set color accordig to file # */
	
	if (sector >3 &&(sector<360 || sector > 368) && sector !=1024 && sector < 1025 && sector !=720){
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
	  
	  if (sector<4) printf("BSC ");
	  if (sector>360 && sector<369) printf ("DIR "); 
	  if (sector==360)   printf ("VTC ");
	  if (sector==1024)  printf ("VT2 ");
	  if (sector==720) printf ("--- ");  /* Dos 2.0: Single density discs, sector 720 is not writable. Dos 2.5 enhanched density marks it as used */ 
	  if (sector>1024 && sector<1041) printf ("--- ");  /* Enhached density discs, sectors above 1024 can not be adressed (next sector: 10 bit number */
	}
	printf (" \e[0m");                   /* Reset all escape sequences */
	
	byte=byte<<1;
	sector++;
      }
      offset++;
      printf ("\t");
    }
    if (sector <1041){
      printf ("%d",sector-1);                /* Show last sector in row... */
      printf ("\n");                         /* Next row. */
    }
  }     
  printf ("\n");
  return (NOERROR);
}
