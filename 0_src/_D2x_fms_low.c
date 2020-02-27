/*----------------------------------------------------------------------------------------------------------------
 *  _D2x_fms_low.c
 *
 * Basic Dos functions.
 *              
 *----------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#define NO_ERR          0  
#define IMAGE_READ_ERR  1  /* The image- file could not be read  */
#define IMAGE_NOT_VALID 2  /* File format not valid */
#define DOUBLE_DENSITY  3  /* Disk image contains a double density disk */
#define SECTORBYTES     4  /* Number of bytes per sectors  not valid */
#define SECTORS         5  /* Total number of secotors is not valid */

/*----------------------------------------------------------------------------------------------------------------
 * init_image
 *
 * Checks the disk- image to be read and inits data structures needed to work with the image.
 * 
 * Parameters:  char    filename  Filename.
 *              char    image     Path of disk- image file.
 *              struct  header    File header of *.ATR image- file.
 *              struct  dir       Dir entry.
 *              struct  vtoc      VTOC
 *
 * Returns:     error code
 *              
 *----------------------------------------------------------------------------------------------------------------*/

d2x_init_image (char filename[],char image [],
            struct atr_image *headerp, 
            struct dir *dirp,
            struct vtoc *vtocp)
{
    
  FILE *f;
  int sectors,        /* Total number of sectors on disk- image. */
      sector_bytes,   /* Bytes per sector */
      totalbytes;     /* Total size of image- file in bytes */
  
  char error;         /* Error Code */
    
  /* 
   * Open file- image. 
   *
   * 90  kb image:  720 sect. + 128 bytes/sect. = Single Density
   * 130 kb image: 1040 sect. + 128 bytes/sect. = (1050)Enhanched Denstity
   * 180 kb image:  720 sect. + 256 bytes/sect. = (true)Double Density
   */

    f=fopen(image,"r");
    
    error=0;

    if (f!=0)
    {     

      /* Read and check file header */

      fread(headerp,16,1,f);

      sector_bytes=headerp->sec_low+256*headerp->sec_high;
      if (sector_bytes>=257){
	fprintf (stderr,"%s > Image File: %s > Error: Bytes per sector > 256!\n",filename,image);
	return(SECTORBYTES);
      }
	
      if (headerp->signature != 0x96){
	fprintf (stderr,"%s > Image File: %s > Error: File could be read, but it is not a valid *.ATR disk image file\n",filename,image);
	return (IMAGE_NOT_VALID);       
      }

      /* Read directory */

      fseek (f,(16+128*3+sector_bytes*358)-sector_bytes,SEEK_SET);
      fread(dirp,(8*sector_bytes),1,f);
      
      /* Read VTOC */

      fseek (f,(16+128*3+sector_bytes*357)-sector_bytes,SEEK_SET);
      fread(vtocp,sector_bytes,1,f);
      
      sectors=vtocp->sec_low+256*vtocp->sec_high;
      
      /* Read complete disk- image file. */

        fseek (f,0,SEEK_SET); 
        fread (headerp->data,sector_bytes*sectors,1,f);
        
        fclose (f);

        return (NO_ERR); 
    }
    fprintf (stderr,"%s > Image File: %s > Error: The image could not be read from the filesystem\n",filename,image);
    return (IMAGE_READ_ERR); 
}

/*----------------------------------------------------------------------------------------------------------------
 * secread  
 *
 * Reads one sector from the disk- image.
 *
 *----------------------------------------------------------------------------------------------------------------*/

d2x_secread (struct atr_image *headerp, BYTE *daten,int sector,int boffset)
{
  int offset,         /* Points to the first data byte of the sector to be read */ 
      secsize,        /* Bytes/ sector. */
      i;              /* Counter. */
    
  secsize=headerp->sec_low+256*headerp->sec_high;      
  offset=(secsize*sector+DTSTART)-secsize;             
 
    for (i=offset;i<=(offset+secsize);i++) daten[boffset++]=headerp->data[i];
    
  return(NO_ERR);
}

/*----------------------------------------------------------------------------------------------------------------
 * secwrite 
 *
 * Writes a sector.
 *
 * Parameters:  daten       ATR- File Rohdaten
 *              buffer      daher kommen die zu schreibenden Daten
 *              boffset     Offsetim Buffer, das heist: ab hier werden die Daten aus dem Buffer gelesen
 *              sector      der Sector soll geschrieben werden
 *              bytes       Anzahl der zu schreibenden Bytes, man besorge sich diese vorzugsweise über die Funktion "secbytes"
 *
 * Rückgabe:    -
 *
 *----------------------------------------------------------------------------------------------------------------*/

secwrite (unsigned char daten[], unsigned char *buffer[],int boffset,int sector, int bytes)
{
  int offset;                                                         /* Offset auf das erste Datenbyte */
  int i;                                                              /* Zähler */
    
  offset=(BYTESEC*sector+DTSTART)-128;                                /* Offset auf das erste Datenbyte von "sector" verbiegen */
    
  for (i=offset;i<=(offset+bytes);i++) daten[i]=buffer[boffset++];    /* Datenbytes in den Buffer schreiben*/
    
}

/*----------------------------------------------------------------------------------------------------------------
 * secnext
 *
 * Liefert die Nummer des Sektors zurück, auf den "sektor" zeigt
 *
 * Aufruf:          secnext (daten,sektor)
 *                  daten   ATR File Rohdaten
 *                  sektor  gewünschter Sektor
 *
 * Rückgabe:        Zeiger auf den nächsten Daten Sektor
 * 
 * Aufbau eines Daten Sektors (DOS 2.X):
 * - Byte 0-124:    Data Bytes
 * - Byte 125:      Die ersten 6 Bits enthalten die Nummer des zugehörigen Files im Verzeichniss.
 *                  Eintrag 0 hat die Nummer 0. Eintrag 1 hat die Nummer 1 und so weiter....
 *                  Die  reslichen 2 Bits bilden zusammen mit allen 8 Bit von Byte 126 eine 10- Bit große Zahl...
 * - Byte 126:      ..welche auf den nächsten Data Sektor des Files zeigt. Gültige Werte reichen von 0 - 719
 *                  Eine 0 zeigt an, dass das Ende des Files erreicht wurde.
 * - Byte 127:      Sektor Byte Count. Enthält die Anzahl der Bytes im Sektor
 *
 *----------------------------------------------------------------------------------------------------------------*/

d2x_secnext (struct atr_image *headerp,int sektor)
{
  int offset=DTSTART,                         /* Offset auf das erstes Daten Byte im ATR- File */
    i,j,k,l;                                  /* Zähler und Zeiger */
  int nsektor;                                /* Nächster Sektor */
  int secsize;                                /* 128 oder 256 bytes? */

  secsize=headerp->sec_low+256*headerp->sec_high;      /* Sector Größe berechnen */

  i=(secsize*sektor+DTSTART)-secsize;         /* i=Zeiger auf das erste Daten Byte des gew. Sektors */
  k=i+secsize-3;                              /* j+l Zeiger auf den nächsten Sektor */
  j=i+secsize-2;                                                                   
    
    /*
    * File- Nummer und nächster Sektor
    * Sind in Byte 125 und 126 des Sektors enthalten. Format:
    *
    * Byte         /             1                 //               2               /
    *              --------------------------------//--------------------------------
    * Bit          /128/64 /32 /16 /8  /4  /2  /1  //128/64 /32 /16 /8  /4  /2  /1  /
    *              --------------------------------//--------------------------------
    * Bit          /128/64 /32 /16 /8  /4  //512/256/128/64 /32 /16 /8  /4  /2  /1  /
    *              ------------------------------------------------------------------
    *              /    File #             // Nächster Sektor                       / 
    *
    * File # ist eine Zahl aus 6- bit und die Sektor # eine Zahl im 10- Bit Format.
    */
    
    nsektor=0;
    if ((((int)headerp->data [k])& 2)==2) nsektor=nsektor+512;        /* Sektor aus Bit 1 und 2 des Byte 1 und weiter aus */
    if ((((int)headerp->data [k])& 1)==1) nsektor=nsektor+256;        /* Bit 1 bis 8 des Byte 2 auslesen */
    if ((((int)headerp->data [j])& 1)==1) nsektor=nsektor+1;
    if ((((int)headerp->data [j])& 2)==2) nsektor=nsektor+2;
    if ((((int)headerp->data [j])& 4)==4) nsektor=nsektor+4;
    if ((((int)headerp->data [j])& 8)==8) nsektor=nsektor+8;
    if ((((int)headerp->data [j])& 16)==16) nsektor=nsektor+16;
    if ((((int)headerp->data [j])& 32)==32) nsektor=nsektor+32;
    if ((((int)headerp->data [j])& 64)==64) nsektor=nsektor+64;
    if ((((int)headerp->data [j])& 128)==128)nsektor=nsektor+128;
    
    return (nsektor);
}

/*----------------------------------------------------------------------------------------------------------------
* secbyte
*
* Liefert die Anzhal der belegten Datenbytes des Sektors zurück
*
* Aufruf:          secbyte (daten,sektor)
*                  daten   ATR File Rohdaten
*                  sektor  gewünschter Sektor
*
* Rückgabe:        Anzahl der Datenbytes
*----------------------------------------------------------------------------------------------------------------*/

d2x_secbyte (struct atr_image *headerp,int sector)
{
  int offset=DTSTART;                         /* Offset auf das erstes Daten Byte im ATR- File */
  int secsize;                                /* 128 oder 256 Byte? */
    BYTE byte;
    
    secsize=headerp->sec_low+256*headerp->sec_high;      /* Sector Größe berechnen */
    
    offset=offset+(secsize*sector)-secsize;     /* Offset auf den Beginn des Sektors verbiegen */
    offset=offset+secsize-1;                    /* Offset auf Anzahl Bytes des gewählten Sektors verbiegen */
    
    byte=headerp->data [offset];                /* Anzahl der Datenbytes auslesen */
    
    return (byte);
}

/*----------------------------------------------------------------------------------------------------------------
 * secfile 
 *
 * Liefert die Nummer der Datei zurück, zu der der Sektor gehört
 *
 * Aufruf:          secfile (daten,sektor)
 *                  daten   ATR File Rohdaten
 *                  sektor  gewünschter Sektor
 *
 * Rückgabe:        Die Filenummer
 * 
 * Aufbau eines Daten Sektors:
 * - Byte 0-124:    Data Bytes
 * - Byte 125:      Die ersten 6 Bits enthalten die Nummer des zugehörigen Files im Verzeichniss.
 *                  Eintrag 0 hat die Nummer 0. Eintrag 1 hat die Nummer 1 und so weiter....
 *                  Die  reslichen 2 Bits bilden zusammen mit allen 8 Bit von Byte 126 eine 10- Bit große Zahl...
 * - Byte 126:      ..welche auf den nächsten Data Sektor des Files zeigt. Gültige Werte reichen von 0 - 719
 *                  Eine 0 zeigt an, dass das Ende des Files erreicht wurde.
 * - Byte 127:      Sektor Byte Count. Enthält die Anzahl der Bytes im Sektor
 *
 *----------------------------------------------------------------------------------------------------------------*/

unsigned int d2x_secfile (struct atr_image *headerp, int sektor)
{
  int offset=DTSTART;                           /* Offset auf das erstes Daten Byte im ATR- File */
    int secsize;
    int i,j,k,l;                                /* Zähler und Zeiger */
    unsigned int fno;                           /* Filenummer */
 
    secsize=headerp->sec_low+256*headerp->sec_high;      /* Sector Größe berechnen */
  
    i=(secsize*sektor+DTSTART)-secsize;         /* i=Zeiger auf das erste Daten Byte des gew. Sektors */
    k=i+secsize-3;                              /* j+l Zeiger auf den nächsten Sektor */
    j=i+secsize-2;                                                                       
    
    /*
     * File- Nummer und nächster Sektor
     * Sind in Byte 125 und 126 des Sektors enthalten. Format:
     *
     * Byte         /             125               //               126             /
     *              --------------------------------//--------------------------------
     * Bit          /128/64 /32 /16 /8  /4  /2  /1  //128/64 /32 /16 /8  /4  /2  /1  /
     *              --------------------------------//--------------------------------
     * Bit          /32 /16 /8 /4  /2  /1  //512/256//128/64 /32 /16 /8  /4  /2  /1  /
     *              ------------------------------------------------------------------
     *              /    File #           // Nächster Sektor                        / 
     *
     * File # ist eine Zahl aus 6- bit und die Sektor # eine Zahl im 10- Bit Format.
     */
    
    fno=0;
    if ((((unsigned int)headerp->data [k])& 4)==4) fno=fno+1;   
    if ((((unsigned int)headerp->data [k])& 8)==8) fno=fno+2;
    if ((((unsigned int)headerp->data [k])& 16)==16) fno=fno+4;
    if ((((unsigned int)headerp->data [k])& 32)==32) fno=fno+8;
    if ((((unsigned int)headerp->data [k])& 64)==64) fno=fno+16;
    if ((((unsigned int)headerp->data [k])& 128)==128) fno=fno+32;
  
    return (fno);
}



