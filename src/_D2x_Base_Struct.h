/*
 * _D2x_Base_Struct.h
 *
 * Part of the atools collection
 *
 */

#define     BYTE    unsigned char
#define     ADRESS  int
#define     BYTESEC 128                                 /* Bytes/ Sektor einer im Single Desnsity Format formatierten Diskette */
#define     VTOCSEC 360                                 /* Startsektor der VTOC */
#define     VTOC2SEC 1024                               /* Enhanched density disks haben eine 2. VTOC ab Sektor 1024 */
#define     DIRSEC  361                                 /* Startsektor des Directorys */
#define     DTSTART 16                                  /* Erstes Datenbyte im ATR- FIle */
#define     DSEKTOR (BYTESEC*DIRSEC+DTSTART)-BYTESEC    /* Offset im ATR- File auf das erste Byte des Directorys (Sektor 361) */
#define     VTOC    (BYTESEC*VTOCSEC+DTSTART)-BYTESEC   /* Offset im ATR- File auf das erste Byte der Vtoc (Sektor 360)*/
#define     VTOC2   (BYTESEC*VTOC2SEC+DTSTART)-BYTESEC  /* Offset im ATR- File auf das erste Byte der Vtoc2 bei Enhanced density disks (1040 Sektoren/ 128 Bytes je Sektor */

/* DOS 2.x File Stats */

#define     NUSD    0       /* File has never been used */
#define     OPEN    1       /* File has been opened */
#define     DOS2    2       /* File created by DOS 2.0 */
#define     LOCK    32      /* File write protected */
#define     INUS    64      /* File is in use */
#define     DEL     128     /* File has been deleted */

/* *.ATR Image Format */

struct atr_image
{
  BYTE signature;         /* Nick Atari signature=96 */
  BYTE e1;                
  BYTE p_low;             /* Paragraphs low + high */
  BYTE p_high;
  BYTE sec_low;           /* Bytes/ Sektor (128= single/ enhached denstity  */
  BYTE sec_high;          /* 256= (true) double density */
  BYTE p_extented;        /* Paragraphs extented */
  BYTE u1;                /* Unused */
  BYTE u2;
  BYTE u3;
  BYTE u4;
  BYTE u5;
  BYTE u6;
  BYTE u7;
  BYTE u8;
  BYTE u9;
  BYTE data [200000];      
};                         


/* Dos 2.x dir entry */

struct atr_dir 
{
  BYTE flag;              /* File Status */
  BYTE count_low;         /* Number of sectors used by this file */
  BYTE count_high;
  BYTE ssn_lo;            /* First data sector of this file */
  BYTE ssn_high;
  BYTE filename [8];      /* Filename */
  BYTE ext [3];           /* Extension */
};                        

/* VTOC Volume Table of Contents */

struct vtoc
{
  BYTE   dos;      /* Dos type 0. 0: Dos 2.0 */
  BYTE   sec_low;  /* Total number of usable sectors on disk */
  BYTE   sec_high;  
  BYTE   sec_unul; /* Total number of unused sectors */
  BYTE   sec_unuh;
  BYTE   reserved; 
  BYTE   u1;   
  BYTE   u2;
  BYTE   u3;
  BYTE   u4;       
  
  BYTE   sector_bm [90]; /* Sector bitmap. */
  
  BYTE   u7[28];
};                       
    
    
    
    




