/*--------------------------------------------------------------------------------------------------------------------------------
 *  _D2x_fms_low.c
 *
 * Bietet die Funktionalität des Atari DOS 2.X File Manager Systems (FMS)
 * Enthält alle 'low- level' Funtionen (lese einen Sektor/ schreibe einen Sektor usw...)
 *
 *              
 *--------------------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#define NO_ERR          0  /* Alles OK! */
#define IMAGE_READ_ERR  1  /* Lesefeler, die Datei konnte nicht gelesen werden */
#define IMAGE_NOT_VALID 2  /* Lesen möglich, aber, kein gültiges *.ATR Image File */
#define DOUBLE_DENSITY  3  /* Double Density */
#define SECTORBYTES     4  /* Sektorgröße > 256 Bytes, das gint es nicht!  */
#define SECTORS         5  /* Mehr als 1020 Sektoren! */

/*-------------------------------------------------------------------------------------------------------------------------------- 
 * init_image
 *
 * Prüft das Disk- Image auf Gültigkeit und initialisiert wichtige Strukturen:
 * 
 * Übergabe:    char    image   Pfad zum Disk- Image
 *              struct  header  File Header des *.ATR images
 *              struct  dir     Grundlegende Struktur zum Directory Eintrag des dos 2.x
 *              struct  vtoc    Die VTOC
 *
 * Rückgabe:    error code
 *              
 *--------------------------------------------------------------------------------------------------------------------------------*/

d2x_init_image (char image [],
            struct atr_image *headerp, 
            struct dir *dirp,
            struct vtoc *vtocp)
{
    
  FILE *f;
  int sectors,      /* Anzahl der Sektoren des Images */
    sector_bytes,   /* Bytes/ Sektor */
    totalbytes;     /* Größe des Images in Bytes */
  
  char error;       /* Error Code */
    
  /* ATR File- Image öffnen
   *
   * 90  kb image:  720 sect. + 128 bytes/sect. = Single Density
   * 130 kb image: 1040 sect. + 128 bytes/sect. = (1050)Enhanched Denstity
   * 180 kb image:  720 sect. + 256 bytes/sect. = (true)Double Density
   */

    f=fopen(image,"r");
    
    error=0;

    if (f!=0)
    {     

      /* ATR- Datei header lesen und prüfen.*/

      fread(headerp,16,1,f);

      sector_bytes=headerp->sec_low+256*headerp->sec_high;
      if (sector_bytes>=257){
	printf ("- Bytes per sector > 256!\n\n");
	return(SECTORBYTES);
      }
	
      if (headerp->signature != 0x96){
	printf ("- File could be read, but it is not a valid *.ATR disk image file\n\n");
	return (IMAGE_NOT_VALID);       /* Ist kein *.ATR Image! */
      }

      /* Directory lesene */

      fseek (f,(16+128*3+sector_bytes*358)-sector_bytes,SEEK_SET);
      fread(dirp,(8*sector_bytes),1,f);
      
      /* VTOC lesen */

      fseek (f,(16+128*3+sector_bytes*357)-sector_bytes,SEEK_SET);
      fread(vtocp,sector_bytes,1,f);
      
      sectors=vtocp->sec_low+256*vtocp->sec_high;
      if (sectors>=1021){
	printf ("- # of sectors >1020\n\n");
	return (SECTORS);
      }

      /* Last but not least, komplettes imgae lesen */

        fseek (f,0,SEEK_SET); 
        fread (headerp->data,sector_bytes*sectors,1,f);
        
        fclose (f);

        return (NO_ERR); /* Erfolg! */
    }
    printf ("- The image could not be read from the filesystem\n\n");
    return (IMAGE_READ_ERR); /* Datei- Lesefehler */
}

/*----------------------------------------------------------------------------------------------------------------
 * secread  
 *
 * Liest einen Sektor
 *
 * Aufruf:
 * headerp=> Zeiger auf die Struktur des Diskimages
 * daten=> Zeiger auf den buffer, in den die Daten geschrieben werden
 * sector=> Erster Sektor der Datei
 *----------------------------------------------------------------------------------------------------------------*/

d2x_secread (struct atr_image *headerp, BYTE *daten,int sector,int boffset)
{
  int offset,         /* Offset auf das erste Datenbyte */ 
    secsize,          /* Größe eines Sektors in Bytes */
    i;                /* Zähler */
    
  secsize=headerp->sec_low+256*headerp->sec_high;      /* Sector Größe berechnen */
  offset=(secsize*sector+DTSTART)-secsize;             /* Offset auf das erste Datenbyte von "sector" verbiegen */
 
    for (i=offset;i<=(offset+secsize);i++) daten[boffset++]=headerp->data[i];            /* Datenbytes in den Buffer schreiben */
    
  return(0);
}

/*----------------------------------------------------------------------------------------------------------------
 * secwrite >>>>>> ANPASSEN
 *
 * Schreibt einen Sektor
 *
 * Aufruf:      secwrite   (daten,buffer,sector,bytes)
 *
 *              daten       ATR- File Rohdaten
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



