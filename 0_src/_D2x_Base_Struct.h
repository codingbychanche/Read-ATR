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
#define     DIRSEC  361                                 /* Startsektor des Directorys */
#define     DTSTART 16                                  /* Erstes Datenbyte im ATR- FIle */
#define     DSEKTOR (BYTESEC*DIRSEC+DTSTART)-BYTESEC    /* Offset im ATR- File auf das erste Byte des Directorys (Sektor 361) */
#define     VTOC    (BYTESEC*VTOCSEC+DTSTART)-BYTESEC   /* Offset im ATR- File auf das erste Byte der Vtoc (Sektor 360)*/

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
  BYTE u1;                /* Unbenutzt */
  BYTE u2;
  BYTE u3;
  BYTE u4;
  BYTE u5;
  BYTE u6;
  BYTE u7;
  BYTE u8;
  BYTE u9;
  BYTE data [200000];      /* Statisch => schmutzig! */
};                         /* 16 Bytes */


/* Dos 2.x Directory Eintrag */

struct atr_dir 
{
  BYTE flag;              /* File Status */
  BYTE count_low;         /* Anzahl Sektoren, die die Datei belegt */
  BYTE count_high;
  BYTE ssn_lo;            /* Erster Sektor der Datei */
  BYTE ssn_high;
  BYTE filename [8];      /* Dateiname */
  BYTE ext [3];           /* Extension */
};                        /* 16 Bytes */

/* VTOC Volume Table of Contents */

struct vtoc
{
  BYTE   dos;      /* DOS Type. 0 zeigt an, es ist eine DOS 2.0 Disk */
  BYTE   sec_low;  /* Anzahl der Sektoren insgesammt */
  BYTE   sec_high;  
  BYTE   sec_unul; /* Anzahl der nicht benutzten Sektoren */
  BYTE   sec_unuh;
  BYTE   reserved; 
  BYTE   u1;   
  BYTE   u2;
  BYTE   u3;
  BYTE   u4;       /* 10 Bytes */
  
  BYTE   sector_bm [90]; /* 90 Bytes */
  
  BYTE   u7[28];
};                       /* 128 Bytes */
    
    
    
    




