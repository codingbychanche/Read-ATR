//----------------------------------------------------------------------------------------------------------------
// DOS2XFunc.c                                                                                      BF 27.6.2013
// Version 0.1
//
// Einige grundlegende Funktionen zur Behandlung von Atari DOS 2.x formartierter Disketten
//----------------------------------------------------------------------------------------------------------------  
// Höhere Funktionen:
// Sind solche, welche komplexere Aufgaben - wie beispielsweise das Lesen oder Schreiben einer Datei - erledigen.
// Diese Funktionen bedienen sich mehreren Kernelfunktionen
//
// - Show
//      ⎜
//      +- shhdr        shhdr (char daten)                              ATR- Fileheader anzeigen
//      ⎜                                                               Rückgabe: 0
//      ⎜
//      +- shdir        shdir   (char daten)                            Directory ausgeben
//      ⎜                                                               Rückgabe: Anzahl der Files auf der Diskette
//      ⎜
//      +- shvtoc       shvtoc  (char daten)                            Zeigt die Vtoc (Volume Table of Contents) der Diskette
//                                                                      Rückgabe: 0
//  - Read               
//      ⎜
//      +- rfile        rfile (char daten,char buffer,char filename)    Liest die Datei "filename" in den durch "buffer" 
//                                                                      spezifizierten Speicherbereich
//  - Write
//      ⎜
//      +- wfile
//
//----------------------------------------------------------------------------------------------------------------
// Kernelfunktionen
// Jede Funktionen dieser Gruppe, stellt für sich den jew. kleinst möglichen Funktionsumfang dar.
//
//  - Sector
//      ⎜
//      +- secread       (char daten, char buffer,sector,bytes) Einen Sektor lesen
//      ⎜
//      +- secwrite      (char daten, char buffer,sector,bytes) Einen Sektor schreiben
//      ⎜
//      +- secnext       (char daten, char sektor)              Liefert die Nummer des Sektors zurück, auf den "sektor zeigt
//      ⎜
//      +- secwnext     
//      ⎜   
//      +- secbytes      (char daten, char sector)              Liefert die Anzahl der Datenbytes des mit "sector" bezeichneten
//      ⎜                                                       Sektors zurück.
//      ⎜                                                                   
//      +- secfile       (char daten, char sector)              Liefert die Nummer der Datei zurück, zu der der Sektor gehört
//
//  - Directory 
//      ⎜
//      +- dirsrc        (char daten, char file)                Prüft ob der Dateiname file im Directory exestiert und liefer im 
//                                                              Erfolgsfalle den Offset auf den Begin des Eintrages in daten zurück
//  
//
//  - File
//      ⎜
//      +-fgetstart     (char daten, char filename)             Liefer den Startsektor des Files zurück 
//      ⎜
//      +-fgetstat      (char daten, char filename)             Liefert den Status des Files zurück
//      ⎜
//      +-fgetscnt      (char daten, char filename)             Liefer die Anzahl der Sektoren die durch das File belegt werden zurück
//      ⎜
//      +-fgetfno       (char daten, char filename)             Liefert den Index des Files entsprechend seines Auftretens im Directory zurück
//
//      filename:   Name des Files im Format "XXXXXXXX.XXX". Wobei X für einen Großbuchstaben im ASCI- Format steht.
//                  Eine Prüfung ob gültige zeichen übergeben wurden, findet derzeit noch nicht statt.
//
//      daten:      Enthält die Rohdaten des ATR- Files (Disk Image).
//
//      sektor:     Nummer des gewünschten Sektors
//
//      Jede der Funktionen liefert eine "0" zurück, wenn das File nicht exestiert.
//
//  -Vtoc
//      ⎜
//      +-vtchecks      (char daten, char sektor) 
//      ⎜
//      ⎜               Prüfft ob der Sektor "sektor" belegt ist oder nicht
//      ⎜               Rückgabe=1 => Sektor frei
//      ⎜               Rückgabe=0 => Sektor belegt
//      ⎜
//      +-vtwrites      (char daten, char sektor)
//      ⎜
//      ⎜               Kennzeichnet den Sektor "sector" als belegt (Bit innerhalb der Vtoc nicht gesetzt)
//      ⎜               Vermindert die Anzahl der freien Sektoren um eins
//      ⎜               Rückgabe=1 => Der Sektor ist bereits belegt
//      ⎜               Rückgabe=0 => Erfolg! Sektor als belegt gekennzeichnet!
//      ⎜
//      +-vtfrees       (char daten, char sektor)
//      ⎜               
//      ⎜               Kennzeichnet den Sektor "sector" als nicht belegt und erhöht die Anzahl der freien
//      ⎜               Sektoren um eins
//      ⎜               Rückgabe=0 => Der Sektor ist bereits mit "frei" gekennzeichnet
//      ⎜               Rückgabe=1 => Erfolg! Sektor ist nun mit "frei" gekennzeichnet
//      ⎜
//      +-vtgetfree     (char daten)
//      ⎜               
//      ⎜               Sucht in der Vtoc nach dem ersten freien Sektor und liefert die
//      ⎜               Nummer des Sektors zurück.
//      ⎜   
//      ⎜               Rückgabewert    >   0   => Nummer des freien Sektors
//      ⎜               Rückgabe        =   0   => Es gibt keine freien Sektoren mehr!
//
//
// 
//----------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include "DOS2XfuncSD.h"

//----------------------------------------------------------------------------------------------------------------    
// ATR File Header anzeigen
//----------------------------------------------------------------------------------------------------------------
   
shhdr (unsigned char *daten)
{   
    printf ("Contents of File Header:\n");
    printf ("------------------------\n\n");
    printf ("Magic 1:\t\t\t%d\n",daten [0]);            // Byte 1+2 Magic
    printf ("Magic 2:\t\t\t%d\n",daten [1]);
    printf ("Para 1:\t\t\t\t%d\n",daten [2]);           // Paragraph's Low + High ?????
    printf ("Para 2:\t\t\t\t%d\n",daten [3]);
    printf ("Bytes/sector Low:\t%d\n",daten [4]);       // # Sektoren low und High Byte
    printf ("Bytes/Sektor High:\t%d\n",daten [5]);
    printf ("Para ext. 1:\t\t%d\n",daten [6]);          // Paragraph's Low + High extended
    printf ("Para ext. 2:\t\t%d\n",daten [7]);
    printf ("\t\t%d\n",daten [8]);                      // 8 bis 16 sind noch unbenutzt.
    printf ("\t\t%d\n",daten [9]);
    printf ("\t\t%d\n",daten [10]);
    printf ("\t\t%d\n",daten [11]);
    printf ("\t\t%d\n",daten [12]);
    printf ("\t\t%d\n",daten [13]);
    printf ("\t\t%d\n",daten [14]);
    printf ("\t\t%d\n",daten [15]); 
    
    return(0);
    
}
    
//----------------------------------------------------------------------------------------------------------------
// Inhaltsverzeichnis ausgeben
//
// Afruf:   directory (daten);
//          daten    ATR- File Rohdaten
//
// Rückgabe:entry= Anzahl der Files auf der Disk
//----------------------------------------------------------------------------------------------------------------
  
shdir (BYTE *daten[])
{
    int             offset=DSEKTOR;             // Dort findet sich das Inhaltsverzeichniss im Buffer
    
    int             entry=0;                    // Zähler. Wieviele Einträge werden ausgegeben    
    unsigned char   fisize,                     // Filesize aus Low+ High von scountl/h
                    ssektor,                    // Startsektor aus Low+ High von stsekl/h
                    stsekl,                     // s.o.
                    stsekh,
                    scountl,
                    scounth;
    int             i;
    
    printf ("\nDisk Directory\n");
    printf ("--------------\n\n");
    printf ("Start\t# Sektors\t\tFilename\t\t\tStatus\n");        // Tabellen Kopf zeigen
    
    while ((daten [offset])!=NULL)              // Erster Buchstabe=0=Verzeichnissschluss
    {
        scountl=daten [offset+1];               // Filegröße lesen
        scounth=daten [offset+2];               
        stsekl=daten [offset+3];                // Startsektor lesen
        stsekh=daten [offset+4];
        
        //
        // Wir komunizieren mit der 8- Bit Welt
        // Deshalb: Aus Low- und High Byte den 16 Bit Integer berechnen als wär's 1980 
        //
        
        ssektor=stsekl+stsekh*256;              // Startsektor und Filegröße aus den High- und Low
        fisize=scountl+scounth*256;             // Bytes berechnen
    
        printf ("%d\t\t\t%d\t\t\t",ssektor,fisize); // Ausgabe
               
            
        //
        // Einen Verzeichnisseintrag ausgeben (16 Bytes)
        //
        
        offset=offset+5;                        // Offset auf das Startbyte des Filenamens zeige lassen

        for (i=offset;i<(offset+11);i++) printf ("%c", daten [i]); 
        printf ("\t");
        
        // File Status ermittel und ausgeben
        // 
        // Das erste Byte im Directory Eintrag beschreibt den File Status.
        // Jedes Bit hat seine Bedeutung.
        // Im Folgenden wird, vorerst, nur das gesammte Byte betrachtet.
        //       
        // Status   Klartext                    Byte
        // ------   --------                    ----
        // NUSD     File has never been used    $00
        // OPEN     File opened for output      $01
        // DOS2     Created by DOS 2.0          $02
        // DEL      File deleted                $80
        // LOCK     Entry locked                $20
        // INUS     File in Use                 $40
        
        offset=offset-5;
        
        switch ((int)daten [offset])                             
        {
            case NUSD:                 
                printf ("\t\tNUSD");
                break;
            case OPEN:
                printf ("\t\tOPEN");
                break;
            case DEL:
                printf ("\t\tDEL");
                break;
            case LOCK:
                printf ("\t\tLOCK");
                break;
            case DOS2:
                printf ("\t\tDOS2");
                break;
            case INUS:
                printf ("\t\tINUS");
                break;
            default:
                printf ("\t\t----");
                break;
        }
        offset=offset+5;
              
        printf ("\n");
        
        offset=offset+11;                       // Offset auf das Startbyte des nächsten Filenamens zeigen lassen
        entry++;                                // Nächster Eintrag 
    }
    return (entry);                             // Das war es. Anzahl der Files zurückgeben.
}

//----------------------------------------------------------------------------------------------------------------
// Vtoc anzeigen 
//
// Vtoc= Volume Table of Contents
// 
// Struktur der Vtoc:
//
// Die Vtoc ist auf Sektor 360 angelegt. Byte 0 bis 9 enthalten div. Informationen zum Disk Format.
// Die Bytes 10 bis 90 enthalten die sog. Sektor Bitmap und repräsentieren den Status eines jeden Sektors
// der Diskette.
//
// Div. Info: Bytes 0 bis 9
// - Byte 0     :   0= Dos 2.0 
// - Byte 1+2   :   Low+ High Anzahl der Sektoren
// - Byte 3+4   :   Low+ HIgh Anzahl der benutzten Sektoren
// - Byte 5     :   Reserviert 
// - Byte 6-9   :   Unbenutzt
// 
// Sektor Bitmap: Bytes 10 bis 100
//  - Jedes Bit der 90 Bytes repräsentiert den Status eines der 720 Sektoren der Diskette (90 Bytes x 8 Bit = 720)
//      Bit=1 Sektor frei
//      Bit=0 Sektor belegt
//  - Das linke Bit des Byte 0 repräsentiert Sektor 0
//  - Das rechte Bit des Byte 0 repräsentiert Sektor 719
//
// Interessant: Das FMS des Dos 2.0 akzeptiert keine "0" als Sektor Angabe. Es akzeptiert aber Sektor 720
// Das FMS wird nie auf den Sektor 720 schreiben! Interessanes Feature... (Inside Atari DOS)
// 
//----------------------------------------------------------------------------------------------------------------

shvtoc (unsigned char *daten)
{
    int                 offset=VTOC;    // Offset im ATR- File
    
    unsigned    char    dosbyte,        // Info zum Dos.
                        scountl,        // Anzahl der Sektoren der Diskette Low- Byte
                        scounth,        // Anzahl der Sektoren der Diskette High- Byte
                        sfreel,         // Anzahl der freien Sektoren Low- Byte
                        sfreeh;         // Anzahl der freien Sektoren High- Byte
    
    int                 i,j,            // Zähler
                        totalsektors,   // 16 Bit für die Anzahl der Sektoren
                        totalused;      // Und nochmal 16 Bit für die Anzahl der benutzten Sektoren
    
    //
    // Vtoc Bytes ermitteln
    //
       
    dosbyte=daten [offset];
    
    scountl=daten [offset+1];           // Anzahl der nutzbaren Sektoren
    scounth=daten [offset+2];
    totalsektors=scountl+256*scounth;
    
    sfreel=daten [offset+3];            // Anzahl der freien Sektoren
    sfreeh=daten [offset+4];
    totalused=sfreel+256*sfreeh;
    
    printf ("Volume Table of Contents (VTOC)\n");
    printf ("-------------------------------\n");
    printf ("Number of Sectors:\t\t\t\t%d\n",totalsektors);
    printf ("Total number of free Sectors:\t%d\n\n",totalused);
    
    //
    // Vtoc- Sektor Bitmap anzeigen
    //
    
    offset=offset+10;
    
    
    printf ("\t\t");
    for (i=1; i<72;i=i+8) printf ("%d\t\t\t",i);
    
    printf ("\n");
    
    for (i=1;i<11;i++)
    {
        printf("%d--->\t",i);
        
        for (j=0;j<9;j++)
        {
            // binary_op((daten[offset++]));
            printf ("\t");
        }
        printf("\n");
    }
                        
    return (0);
}

//-------------------------------------------------------------------------------------------------------------
// rfile
//
// Afruf:   rfile (daten,buffer,filename)
//
//          daten       ATR- File Rohdaten
//          buffer      Speicherort für die gelesenen Daten
//          filename    Name der Datei - im Image - die gelesen werden soll
//
// Rückgabe:Die Anzahl der gelesenen Bytes= Dateigröße. Rückgabe=0 wenn die Datei nicht gefunden wurde.
//----------------------------------------------------------------------------------------------------------------

rfile (BYTE *daten [130000],BYTE *buffer[130000], char *filename [12])
{
    int     ssector,                        // Startsektor
            sector,                         // Sektor der gerade gelesen wird
            sectors,                        // soviele Sektoren hat die Datei
            nexts,                          // Nächster Sektor
            totalbytes,                     // Filegröße
            i;                              // Zähler
    char    bytes;                          // Bytes/ Sektor
        
    printf ("\nReading file:%s",filename);
    
    ssector=fgetstart(daten,filename);      // Startsektor holen und dabei prüfen, ob es die Datei überhaupt gibt
    if (ssector==0) return (0);             // Datei nicht gefunden, mit "0" zurück!
    
    sectors=fgetscnt (daten,filename);      // Datei vorhanden: Anzahl der Sektoren der Datei besorgen
    
    printf ("\nStarting at sector %d and reading %d sectors in total",ssector,sectors);
    
    //
    // Datei lesen
    //
    
    totalbytes=0;                           // Bytes gelesen=0!
    sector=ssector;                         // Aktueller Sektor= Startsektor der Datei
    while (sector!=0)
    {
        bytes=secbyte(daten,sector);        // Anzahl der Bytes holen, welche die Datei belegt
        totalbytes=totalbytes+bytes;        // Aufsummieren
        
        secread (daten,buffer,sector,bytes);    // Sektor lesen
        
        printf ("\nReading sector %d  Databytes %d",sector,bytes);
        
        sector=secnext(daten,sector);       // Nächsten Sektor der Datei auslesen und weiter im Text
            
    }   
    return (totalbytes);                    // Erfolg! Anzah der gelesenen Bytes zurückgeben
}

//----------------------------------------------------------------------------------------------------------------
// secread
//
// Liest einen Sektor
//
// Aufruf:      secread     (daten,buffer,sector,bytes)
//
//              daten       ATR- File Rohdaten
//              buffer      Dahin kommen die gelesenen Daten
//              sector      der Sector soll gelesen werden
//              bytes       Anzahl der zu lesenden Bytes, man besorge sich diese vorzugsweise über die Funktion "secbytes"
//
// Rückgabe:    -0
//
//----------------------------------------------------------------------------------------------------------------

secread (BYTE *daten [], BYTE *buffer[],int sector, int bytes)
{
    int offset;                                                         // Offset auf das erste Datenbyte
    int i,                                                              // Zähler
        j=0;                                                            // Zeiger in buffer
            
    offset=(BYTESEC*sector+DTSTART)-128;                                // Offset auf das erste Datenbyte von "sector" verbiegen
    
    for (i=offset;i<(offset+bytes);i++) buffer[j++]=daten[i];           // Datenbytes in den Buffer schreiben
    
}

//----------------------------------------------------------------------------------------------------------------
// secwrite
//
// Schreibt einen Sektor
//
// Aufruf:      secwrite   (daten,buffer,sector,bytes)
//
//              daten       ATR- File Rohdaten
//              buffer      daher kommen die zu schreibenden Daten
//              boffset     Offsetim Buffer, das heist: ab hier werden die Daten aus dem Buffer gelesen
//              sector      der Sector soll geschrieben werden
//              bytes       Anzahl der zu schreibenden Bytes, man besorge sich diese vorzugsweise über die Funktion "secbytes"
//
// Rückgabe:    -
//
//----------------------------------------------------------------------------------------------------------------

secwrite (unsigned char *daten[], unsigned char *buffer[],int boffset,int sector, int bytes)
{
    int offset;                                                         // Offset auf das erste Datenbyte
    int i;                                                              // Zähler
       
    offset=(BYTESEC*sector+DTSTART)-128;                                // Offset auf das erste Datenbyte von "sector" verbiegen
    
    for (i=offset;i<=(offset+bytes);i++) daten[i]=buffer[boffset++];    // Datenbytes in den Buffer schreiben
    
}

//----------------------------------------------------------------------------------------------------------------
// fgetstart
//
// Liefert den Ersten Sektor den das File belegt zurück
//
// Aufruf:      getfile (daten,fname)
//              daten   ATR- File Rohdaten
//              fname   Dateiname im folgendem Format: "xxxxxxxx.xxx"             
//
// Rückgabe:    0=File not found
//              Alles ungeich der 0 ist der Startsektor des Files
//----------------------------------------------------------------------------------------------------------------

fgetstart (BYTE *daten[],BYTE *fname[])
{
    int             i,j,k;      // Zähler
           
    char name [20],name2[20] ;  //Aufgeräumter Name des Files das wir suchen
    
    unsigned char   scountl,
                    scounth,
                    stsekl,
                    stsekh;
    int             ssektor,
                    offset;         // Im Erfolgsfalle steht hier der Offset auf den Beginn des gef. Directory Eintrages
    
    //
    // Directory durchsuchen
    //
    
    if ((offset=dirssrc(daten,fname))==0) return (0);           // Datei gibt's net, zurück mit 0=Fehler!
    
    //
    // Startsektor aus dem Directory- Eintrag ziehen
    //
    
    offset=offset+3;                // Low- und High Byte holen
    stsekl=daten [offset];
    offset++;
    stsekh=daten [offset];
    ssektor=stsekl+256*stsekh;      // 8-Bit Welt... aus Low- und High unseren 16- Bit Wert für den Startsektor berechnen
        
    return (ssektor);               // Startsektor zurückliefern                                                                
}

//----------------------------------------------------------------------------------------------------------------
// fgetstat
//
// Liefert den Status des Files zurück
//
// Aufruf:      fgetstat    (daten,fname)
//              daten       ATR- File Rohdaten
//              fname       Dateiname im folgendem Format: "xxxxxxxx.xxx"             
//
// Rückgabe:    0=File not found
//              Alles ungeich der 0 ist der Status des Files
//----------------------------------------------------------------------------------------------------------------

fgetstat (BYTE *daten[],BYTE *fname[])
{
    int offset=DSEKTOR,         // Erstes Byte im Directory
    i,j,k;                      // Zähler
    
    char name [20],name2[20] ;  //Aufgeräumter Name des Files das wir suchen
    
    unsigned char   status;     //File Status
        
    //
    // Directory durchsuchen
    //
    
    if ((offset=dirssrc(daten,fname))==0) return (0);           // Datei gibt's net, zurück mit 0=Fehler!
    
    //
    // Status des Files aus dem Directory- Eintrag ziehen
    //
    
    status=daten [offset];      // Status der Datei holen
    
    return (status);            // Status der Datei zurückholen
}

//----------------------------------------------------------------------------------------------------------------
// fgetfcnt
//
// Liefert die Anzahl der durch das File belegten Sektoren zurück
//
// Aufruf:      fgetstat    (daten,fname)
//              daten       ATR- File Rohdaten
//              fname       Dateiname im folgendem Format: "xxxxxxxx.xxx"             
//
// Rückgabe:    0=File not found
//              Alles ungeich der 0 ist der Status des Files
//----------------------------------------------------------------------------------------------------------------

fgetscnt (BYTE *daten[],BYTE *fname[])
{
    int     offset=DSEKTOR,                     // Erstes Byte im Directory
            i,j,k,                              // Zähler
            flag;                               // Flag=1=Datei gefunden
    
    char    name [20],name2[20] ;               // Aufgeräumter Name des Files das wir suchen
    
    char    scountl,                            // Anzhal Sektoren Low und
            scounth;                            // High- Byte
    
    int     sectors;                            // Anzahl der Sektoren 16- Bit
        
    //
    // Directory durchsuchen
    //
    
    if ((offset=dirssrc(daten,fname))==0) return (0);           // Datei gibt's net, zurück mit 0=Fehler!
    
    //
    // Anzahl der Sektoren aus dem gefundenen Eintrag ziehen
    //
    
    offset=offset+1;                // Low- und High Byte holen
    scountl=daten [offset];
    offset++;
    scounth=daten [offset];
    
    sectors=scountl+256*scounth;    // 8-Bit Welt... aus Low- und High unseren 16- Bit Wert für den Startsektor berechnen
 
    return (sectors);
}

//----------------------------------------------------------------------------------------------------------------
// secnext
//
// Liefert die Nummer des Sektors zurück, auf den "sektor" zeigt
//
// Aufruf:          secnext (daten,sektor)
//                  daten   ATR File Rohdaten
//                  sektor  gewünschter Sektor
//
// Rückgabe:        Zeiger auf den nächsten Daten Sektor
// 
// Aufbau eines Daten Sektors:
// - Byte 0-124:    Data Bytes
// - Byte 125:      Die ersten 6 Bits enthalten die Nummer des zugehörigen Files im Verzeichniss.
//                  Eintrag 0 hat die Nummer 0. Eintrag 1 hat die Nummer 1 und so weiter....
//                  Die  reslichen 2 Bits bilden zusammen mit allen 8 Bit von Byte 126 eine 10- Bit große Zahl...
// - Byte 126:      ..welche auf den nächsten Data Sektor des Files zeigt. Gültige Werte reichen von 0 - 719
//                  Eine 0 zeigt an, dass das Ende des Files erreicht wurde.
// - Byte 127:      Sektor Byte Count. Enthält die Anzahl der Bytes im Sektor
//
//----------------------------------------------------------------------------------------------------------------

secnext (BYTE *daten[], int sektor)
{
    int offset=DTSTART,                         // Offset auf das erstes Daten Byte im ATR- File
    i,j,k,l;                                    // Zähler und Zeiger
    int nsektor;                                // Nächster Sektor
      
    i=(128*sektor+DTSTART)-128;                 // i=Zeiger auf das erste Daten Byte des gew. Sektors
    k=i+125;                                    // j+l Zeiger auf den nächsten Sektor
    j=i+126;                                                                       
       
    //
    // File- Nummer und nächster Sektor
    // Sind in Byte 125 und 126 des Sektors enthalten. Format:
    //
    // Byte         /             1                 //               2               /
    //              --------------------------------//--------------------------------
    // Bit          /128/64 /32 /16 /8  /4  /2  /1  //128/64 /32 /16 /8  /4  /2  /1  /
    //              --------------------------------//--------------------------------
    // Bit          /128/64 /32 /16 /8  /4  //512/256/128/64 /32 /16 /8  /4  /2  /1  /
    //              ------------------------------------------------------------------
    //              /    File #             // Nächster Sektor                       / 
    //
    // File # ist eine Zahl aus 6- bit und die Sektor # eine Zahl im 10- Bit Format.
    //
    
    nsektor=0;
    if ((((int)daten [k])& 2)==2) nsektor=nsektor+512;        // Sektor aus Bit 1 und 2 des Byte 1 und weiter aus
    if ((((int)daten [k])& 1)==1) nsektor=nsektor+256;        // Bit 1 bis 8 des Byte 2 auslesen
    if ((((int)daten [j])& 1)==1) nsektor=nsektor+1;
    if ((((int)daten [j])& 2)==2) nsektor=nsektor+2;
    if ((((int)daten [j])& 4)==4) nsektor=nsektor+4;
    if ((((int)daten [j])& 8)==8) nsektor=nsektor+8;
    if ((((int)daten [j])& 16)==16) nsektor=nsektor+16;
    if ((((int)daten [j])& 32)==32) nsektor=nsektor+32;
    if ((((int)daten [j])& 64)==64) nsektor=nsektor+64;
    if ((((int)daten [j])& 128)==128)nsektor=nsektor+128;
    
    return (nsektor);
}

//----------------------------------------------------------------------------------------------------------------
// secbyte
//
// Liefert die Anzhal der belegten Datenbytes des Sektors zurück
//
// Aufruf:          secbyte (daten,sektor)
//                  daten   ATR File Rohdaten
//                  sektor  gewünschter Sektor
//
// Rückgabe:        Anzahl der Datenbytes
//----------------------------------------------------------------------------------------------------------------

secbyte (unsigned char *daten,int sector)
{
    int offset=DTSTART;                         // Offset auf das erstes Daten Byte im ATR- File
    char byte;
    
    offset=offset+(128*sector)-128;             // Offset auf den Beginn des Sektors verbiegen
    offset=offset+127;                          // Offset auf Anzahl Bytes des gewählten Sektors verbiegen

    byte=daten [offset];                        // Anzahl der Datenbytes auslesen

    return (byte);
}

//----------------------------------------------------------------------------------------------------------------
// secfile
//
// Liefert die Nummer der Datei zurück, zu der der Sektor gehört
//
// Aufruf:          secfile (daten,sektor)
//                  daten   ATR File Rohdaten
//                  sektor  gewünschter Sektor
//
// Rückgabe:        Die Filenummer
// 
// Aufbau eines Daten Sektors:
// - Byte 0-124:    Data Bytes
// - Byte 125:      Die ersten 6 Bits enthalten die Nummer des zugehörigen Files im Verzeichniss.
//                  Eintrag 0 hat die Nummer 0. Eintrag 1 hat die Nummer 1 und so weiter....
//                  Die  reslichen 2 Bits bilden zusammen mit allen 8 Bit von Byte 126 eine 10- Bit große Zahl...
// - Byte 126:      ..welche auf den nächsten Data Sektor des Files zeigt. Gültige Werte reichen von 0 - 719
//                  Eine 0 zeigt an, dass das Ende des Files erreicht wurde.
// - Byte 127:      Sektor Byte Count. Enthält die Anzahl der Bytes im Sektor
//
//----------------------------------------------------------------------------------------------------------------

secfile (BYTE *daten[], int sektor)
{
    int offset=DTSTART,                         // Offset auf das erstes Daten Byte im ATR- File
    i,j,k,l;                                    // Zähler und Zeiger
    int fno;                                    // Filenummer
    
    i=(128*sektor+DTSTART)-128;                 // i=Zeiger auf das erste Daten Byte des gew. Sektors
    k=i+125;                                    // j+l Zeiger auf den nächsten Sektor
    j=i+126;                                                                       
    
    //
    // File- Nummer und nächster Sektor
    // Sind in Byte 125 und 126 des Sektors enthalten. Format:
    //
    // Byte         /             1                 //               2               /
    //              --------------------------------//--------------------------------
    // Bit          /128/64 /32 /16 /8  /4  /2  /1  //128/64 /32 /16 /8  /4  /2  /1  /
    //              --------------------------------//--------------------------------
    // Bit          /64 /32 /16 /4 /2  /1 //512/256//128/64 /32 /16 /8  /4  /2  /1  /
    //              ------------------------------------------------------------------
    //              /    File #           // Nächster Sektor                        / 
    //
    // File # ist eine Zahl aus 6- bit und die Sektor # eine Zahl im 10- Bit Format.
    //
    
    fno=0;
    if ((((int)daten [k])& 4)==2) fno=fno+1;   
    if ((((int)daten [k])& 8)==8) fno=fno+2;
    if ((((int)daten [k])& 16)==16) fno=fno+4;
    if ((((int)daten [k])& 32)==32) fno=fno+8;
    if ((((int)daten [k])& 64)==64) fno=fno+16;
           
    return (fno);
}

//----------------------------------------------------------------------------------------------------------------
// fgetfno
//
// Liefert die Filenummer zurück
//
// Aufruf:      fgetfno     (daten,fname)
//              daten       ATR- File Rohdaten
//              fname       Dateiname im folgendem Format: "xxxxxxxx.xxx"             
//
// Rückgabe:    0=File not found
//              Alles ungeich der 0 ist der Filenummer
//----------------------------------------------------------------------------------------------------------------

fgetfno (unsigned char *daten,char *fname)
{
    int offset=DSEKTOR,                     // Erstes Byte im Directory
    i,j,k,                                  // Zähler
    flag;                                   // Flag=1=Datei gefunden
    
    char            name [20],name2[20] ;   // Aufgeräumter Name des Files das wir suchen
    char            filecount;              // Nummer des Files
           
    //
    // Filenamen aufräumen
    // Aus: "TEST.OBJ" machen wir: "TEST     OBJ" damit dieser String dann direkt mit
    // dem ausgelesenen Eintrag aus dem Directory verglichen werden kann
    //
    
    for (i=0;(fname[i])!='.';i++) name [i]=fname[i];    // Übergebenen Namen bis zum Punkt kopieren
    k=i;                                                // Name zwischen letztem Zeichen und Punkt
    for (;i<8;i++) name[i]=' ';                         // mit Leerzeichen füllen
    k++;                                                // Punkt nicht beachten
    for (j=8;j!=12;j++) name [j]=fname[k++];            // Den Teil nach dem Punkt kopieren
    name [j++]='\0';                                    // String abschließen
    
    //
    // Dateinamen im Verzeichniss suchen
    //
    
    filecount=0;
    
    while (daten[offset]!=0)
    {                   
        offset=offset+5;                                // Offset auf Beginn des Dateinamens verbiegen
        
        j=0;                                            // j: Zeiger in name2
        for (i=offset;i<(offset+11);i++)                // i: Zeiger auf den Dateinamen in daten
        {
            name2[j]=daten[i]; 
            j++;
        }
        name2[j++]='\0';                                // String abschließen
        
        if ((strcmp(name,name2)==0))flag=filecount;     // Eintrag gefunden? Ja=> Startsektor für Rückgabe in flag
        offset=offset+11;                               // Offset auf den Beginn des nächsten Eintrags verbiegen
        filecount++;
    }
    return (flag);
}

//----------------------------------------------------------------------------------------------------------------
// vtchecks 
//
// Prüft, ob ein bestimmter Sektor belegt ist, oder nicht.
//
// Aufruf:      vtchecks    (daten,sector)
//              daten       ATR- File Rohdaten
//              sector      Datensektor der Gegenstand der Prüfung ist
//
// Rückgabe:    1=Sektor frei/ 0= Sektor belegt
//
//----------------------------------------------------------------------------------------------------------------

vtchecks (BYTE *daten[],int sector)
{
    int offset=VTOC,                        // Offset im ATR- File
        i,                                  // Zähler
        secbyte,                            // Offset auf das Byte der Vtoc, in dem das gesuchte Bit steckt
        secbit;                             // Das ist der Offset auf das gesuchte Bit innerhalb von "secbyte"
    
    char byte;                              // Hierher kommt das an der Stelle von "secbyte" gelegene Byte hin
        
    offset=offset+10;                       // Offset auf den Start der VTOC Bitmap Table verbiegen
    
    secbyte=sector/8;                       // Index auf Sektorbyte ermitteln
    offset=offset+secbyte;                  // offset dahin verbiegen
    secbit=sector % 8;                      // Rest aus "sector/8" ist unser gesuchtes Bit innerhalb von "secbyte"
    
    if (secbit==0) offset--;

    byte=daten [offset];                    // In diesem Byte steckt unser Sektor
    
    //printf ("\n%d   %d  %d\n",sector,secbyte,secbit);    // Debug Info :-) If needed.....
    //binary_op (byte);
    
    for (i=0;i<secbit;i++) byte <<=1;       // "byte" um secbit bit's nach links
    
    if ((byte & 128)==128) return (1);      // Bit gesetzt => Sektor nicht belegt!
    else return (0);                        // Bit nicht gesetzt => Sektor belegt!
            
}

//----------------------------------------------------------------------------------------------------------------
// vtwrites
//
// Kennzeichnet in der Vtoc einen Sektor als belegt und vermindert die Anzahl der freien Sektoren in der Vtoc 
// um eins.
//
// Für "sector" liegen gültige Werte zwischen 1 und 720. Das wird hier selbstverständlich nicht überprüft.
// Die aufrufende Funktion muss sich selbst im die Übergabe der Richtigen Werte kümmern.
//
// Aufruf:      vtchecks    (daten,sector)
//              daten       ATR- File Rohdaten
//              sector      Datensektor der als belegt gekennzeichnet werden soll
//    
// Rückgabe:    0=          Diskette voll
//              1=          Erfolg
//
//----------------------------------------------------------------------------------------------------------------

vtwrites (char *daten,int sector)
{
    int     offset=VTOC,                        // Offset im ATR- File
            i,j,                                // Zähler
            secbyte,                            // Offset auf das Byte der Vtoc, in dem das gesuchte Bit steckt
            secbit;                             // Das ist der Offset auf das gesuchte Bit innerhalb von "secbyte"

    char    byte;                               // Aus der Vtoc ausgelesenes Byte mit dem Bit, dass zur Kennzeichnung
                                                // gelöscht werden muss.
    
    BYTE    sfreel,                             // Anzahl der freien Sektoren Low- Byte
            sfreeh;                             // Anzahl der freien Sektoren High- Byte
    
    int     sfree;                              // Gesammtzahl der freien Sektoren in der 16- Bit Welt
    
    int     totalused;                          // Gesammtzahl der genutzten Sektoren in der 16- Bit Welt
    
    char   pow [8]={128,64,32,16,8,4,2,1};    // 2^secbit
         
    //
    // zunächst prüfen, ob der Sektor bereits belegt ist
    //
    
    // if ((vtchecks(daten, sector))==0) return (0);   // Sektor bereits belegt, Fehler melden und Heim!
    
    //
    // Wir lesen jenes Byte aus der Vtoc aus, in welchem unser Bit steckt das unseren Sektor repräsentiert,
    // setzen das Bit welches unseren Sektor repräsentiert.
    //
    
    offset=offset+10;                           // Offset auf den Start der VTOC Bitmap Table verbiegen
    
    secbyte=sector/8;                           // Index auf Sektorbyte ermitteln
    offset=offset+secbyte;                      // offset dahin verbiegen
    secbit=sector % 8;                          // Rest aus "sector/8" ist unser gesuchtes Bit innerhalb von "secbyte"
    
    if (secbit==0) offset--;

    byte=daten [offset];                        // In diesem Byte steckt unser Sektor

    // printf ("\n%d   %d  %d\n",sector,secbyte,secbit);    // Debug Info :-) If needed.....
    // binary_op (byte);
    
    j=pow [secbit];                                 // 2^secbit
    
    // printf ("\n");                               // Again, debug Info if needed :-)
    // printf ("%d",j);
    // printf ("\n");
    
    byte=(byte ^ j);                                // Das Bit setzen, welches unseren Sektor repräsentiert
    
    // binary_op (byte);                            // Debug Info.......
    
    daten [offset]=byte;                            // Das geänderte Byte zurück in die Vtoc schreiben
    
    //
    // Anzahl der freien Sektoren um eins verringern
    //
    
    offset=VTOC;
    
    sfreel=daten [offset+3];
    sfreeh=daten [offset+4];
    
    sfree=sfreel+256*sfreeh;                    // Bekannt? :-)
    sfree--;

    
    sfreeh=sfree/256;                           // Low und High Byte ermitteln
    sfreel=sfree-256;                           // und:
    
    daten [offset+3]=sfreel;                    // Neue Anzahl Sektoren zurück in die VTOC schreiben
    daten [offset+4]=sfreeh;
    
    return (1);                                 // Erfolg melden!
    
}

//----------------------------------------------------------------------------------------------------------------
// vtfrees
//
// Kennzeichnet in der Vtoc einen Sektor als frei und erhöht die Anzahl der freien Sektoren in der Vtoc 
// um eins.
//
// Für "sector" leigen gültige Werte zwischen 1 und 720. Das wird hier selbstverständlich nicht überprüft.
// Die aufrufende Funktion muss sich selbst im die Übergabe der Richtigen Werte kümmern.
//
// Aufruf:      vtchecks    (daten,sector)
//              daten       ATR- File Rohdaten
//              sector      Datensektor der als belegt gekennzeichnet werden soll
//    
// Rückgabe:    -
//
//----------------------------------------------------------------------------------------------------------------

vtfrees (unsigned char *daten,int sector)
{
    ADRESS     offset=VTOC,                     // Offset im ATR- File
    i,j,                                        // Zähler
    secbyte,                                    // Offset auf das Byte der Vtoc, in dem das gesuchte Bit steckt
    secbit;                                     // Das ist der Offset auf das gesuchte Bit innerhalb von "secbyte"
    
    BYTE    byte;                               // Aus der Vtoc ausgelesenes Byte mit dem Bit, dass zur Kennzeichnung
                                                // gelöscht werden muss.
    
    BYTE    sfreel,                             // Anzahl der freien Sektoren Low- Byte
            sfreeh;                             // Anzahl der freien Sektoren High- Byte
    
    ADRESS  sfree;                              // Gesammtzahl der freien Sektoren in der 16- Bit Welt
    
    ADRESS  totalused;                          // Gesammtzahl der genutzten Sektoren in der 16- Bit Welt
    
    char   pow [8]={1,128,64,32,16,8,4,2,1};    // 2^secbit
    
    //
    // zunächst prüfen, ob der Sektor bereits frei ist
    //
    
    if ((vtchecks(daten, sector))==1) return (0);   // Sektor bereits frei, Fehler melden und Heim!
    
    //
    // Wir lesen jenes Byte aus der Vtoc aus, in welchem unser Bit steckt das unseren Sektor repräsentiert,
    // setzen das Bit welches unseren Sektor repräsentiert.
    //
    
    offset=offset+10;                           // Offset auf den Start der VTOC Bitmap Table verbiegen
    
    secbyte=sector/8;                           // Index auf Sektorbyte ermitteln
    offset=offset+secbyte;                      // offset dahin verbiegen
    secbit=sector % 8;                          // Rest aus "sector/8" ist unser gesuchtes Bit innerhalb von "secbyte"
    
    if (secbit==0) offset--;
    
    byte=daten [offset];                        // In diesem Byte steckt unser Sektor
    
    printf ("\n%d   %d  %d\n",sector,secbyte,secbit);    // Debug Info :-) If needed.....
    // binary_op (byte);
    
    j=pow [secbit];                             // 2^secbit
    
    // printf ("\n");                           // Again, debug Info if needed :-)
    // printf ("%d",j);
    // printf ("\n");
    
    byte=(byte ^ j);                            // Das Bit setzen, welches unseren Sektor repräsentiert
    
    // binary_op (byte);                        // Debug Info.......
    
    daten [offset]=byte;                        // Das geänderte Byte zurück in die Vtoc schreiben
    
    //
    // Anzahl der freien Sektoren um eins veringern
    //
    
    offset=VTOC;
    
    sfreel=daten [offset+3];
    sfreeh=daten [offset+4];
    
    if (sfreel<256) sfreel=sfreel+1;
    else sfreeh=sfreeh+1;
        
    sfree=sfreel+256*sfreeh;                    // Bekannt? :-)
    
    daten [offset+3]=sfreel;                    // Neue Anzahl Sektoren zurück in die VTOC schreiben
    daten [offset+4]=sfreeh;
    
    return (1);                                 // Erfolg melden!
    
}

//----------------------------------------------------------------------------------------------------------------
// vtgetfree     
//
// Liefert die Nummer des ersten freien Sektors, der in der Vtoc gefunden wurden, als Rückgabewert
//
// Aufruf:          vtgetfree (daten)
//                  daten   ATR File Rohdaten
//                  
// Rückgabe:        Rückgabewert > 0:  Nummer des gefundenen Sektors
//                  Rückgabewert = 0:  Es gibt keine freien Sektoren mehr!
//----------------------------------------------------------------------------------------------------------------
vtgetfree (char *daten)
{
    ADRESS      offset=VTOC,            // Die Vtoc steht hier im ATR- Image
                sector=1;               // Wir beginne bei Sektor 1 mit der Suche
    
    offset=offset+10;                   // offset auf den Beginn der Sektor- Bitmap der Vtoc verbiegen
    
    while (sector <= 719)
        {
            if ((vtchecks(daten,sector))==1) return (sector);
            sector++;
        }
            
    return (0);
}

//----------------------------------------------------------------------------------------------------------------
// dirsrc
//
// Durchsucht das Directory und prüft, ob eine bestimmte Datei vorhanden ist. Im Erfolgsfall wird ein Offset
// zurückgeliefert, der auf den Beginn des gefundenen Eintrages zeigt.
//
// Aufruf:      dirscr (daten,file)
//              daten   ATR File Rohdaten
//              file    Dateinamen
//
// Rückgabe:    =0 File not found
//              >0 Offset auf den Beginn von daten zum Begin des gefundenen Directory Entrages
//
// Ein Directory Eintrag schaut unter DOS 2.X so aus:
//
// Byte         0       1   2   3       4   5               12  13      15  = 16 Bytes / Eintrag
//              FLAG    COUNT   STARTSEC    FILENAME        EXT         
//
//----------------------------------------------------------------------------------------------------------------

dirssrc (BYTE *daten [130000],BYTE *file [12])
{
    int     offset=DSEKTOR,                             // Offset auf den Anfang des ersten Directory Eintrags verbiegen
         
            k,i,j;                                      // Zähler
    
    char    name [12],                                  // Aufgeräumter Dateiname
            name2 [12];                                 // Dateiname aus dem Verzeichniss, zum Vergleich mit dem aufgräumten Dateinamen
    
    
    for (i=0;(file[i]!= '.');i++) name [i]=file[i];     // Übergebenen Namen bis zum Punkt kopieren
    k=i;                                                // Name zwischen letztem Zeichen und Punkt
    for (;i<8;i++) name[i]=' ';                         // mit Leerzeichen füllen
    k++;                                                // Punkt nicht beachten
    for (j=8;j!=12;j++) name [j]=file[k++];             // Den Teil nach dem Punkt kopieren
    name [j++]='\0';                                    // String abschließen

    //
    // Dateinamen im Verzeichniss suchen
    //

    while (daten[offset]!=0)
    {                   
        offset=offset+5;                                // Offset auf Beginn des Dateinamens verbiegen
        
        j=0;                                            // j: Zeiger in name2
        
        for (i=offset;i<(offset+11);i++)                // i: Zeiger auf den Dateinamen in daten
        {
            name2[j]=daten[i]; 
            j++;
        }
        name2[j++]='\0';                                // String abschließen
    
        if ((strcmp(name,name2)==0))                    // Datei gefunden? 
        {
            offset=offset-5;                            // Ja, Offset auf den Beginn des Directory Eintrages verbiegen
            return(offset);                             // Offset den Beginn des Directory Eintrages zurückgeben 
        }
    
        offset=offset+11;                               // Offset auf den Beginn des nächsten Eintrags verbiegen
        k++;
    }
    return (0);                                          // Eintrag nicht gefunden :(
}

//----------------------------------------------------------------------------------------------------------------
// sek_Info >> Not used yet <<
//
// Gibt 
//
// Aufruf:          readsek (daten,sektor)
//                  daten   ATR File Rohdaten
//                  sektor  gewünschter Sektor
//
// Rückgabe:        Zeiger auf den nächsten Daten Sektor
// 
// Aufbau eines Daten Sektors:
// - Byte 0-124:    Data Bytes
// - Byte 125:      Die ersten 6 Bits enthalten die Nummer des zugehörigen Files im Verzeichniss.
//                  Eintrag 0 hat die Nummer 0. Eintrag 1 hat die Nummer 1 und so weiter....
//                  Die  reslichen 2 Bits bilden zusammen mit allen 8 Bit von Byte 126 eine 10- Bit große Zahl...
// - Byte 126:      ..welche auf den nächsten Data Sektor des Files zeigt. Gültige Werte reichen von 0 - 719
//                  Eine 0 zeigt an, dass das Ende des Files erreicht wurde.
// - Byte 127:      Sektor Byte Count. Enthält die Anzahl der Bytes im Sektor
//
//----------------------------------------------------------------------------------------------------------------

readsekt (unsigned char *daten, int sektor)
{
    int offset=DTSTART,                         // Offset auf das erstes Daten Byte im ATR- File
    i,j,k,l;                                    // Zähler und Zeiger
    int nsektor,                                // Nächster Sektor
        fno,                                    // Filenummer
        bytes;                                  // Anzahl der genutzten Bytes des Sektors
    
    unsigned char a,b,c;
        
    i=(128*sektor+DTSTART)-128;                 // i=Zeiger auf das erste Daten Byte des gew. Sektors
    k=i+125;                                    // j+l Zeiger auf den nächsten Sektor
    j=i+126;                                                                       
    l=i+127;                                    // l=Zeiger auf Anzahl Bytes des gewählten Sektors 
               
    //
    // File- Nummer und nächster Sektor
    // Sind in Byte 125 und 126 des Sektors enthalten. Format:
    //
    // Byte         /             1                 //               2               /
    //              --------------------------------//--------------------------------
    // Bit          /128/64 /32 /16 /8  /4  /2  /1  //128/64 /32 /16 /8  /4  /2  /1  /
    //              --------------------------------//--------------------------------
    // Bit          /128/64 /32 /16 /8  /4  //512/256/128/64 /32 /16 /8  /4  /2  /1  /
    //              ------------------------------------------------------------------
    //              /    File #             // Nächster Sektor                       / 
    //
    // File # ist eine Zahl aus 6- bit und die Sektor # eine Zahl im 10- Bit Format.
    //
       
    fno=0;
    if (((daten [k])& 4)==4)fno=fno+1;                  // Filenummer aus Bit 3 bis 8 des Byte 1 auslesen
    if (((daten [k])& 8)==8)fno=fno+2;
    if (((daten [k])& 16)==16)fno=fno+4;
    if (((daten [k])& 32)==32)fno=fno+8;
    if (((daten [k])& 64)==64)fno=fno+16; 
    if (((daten [k])& 128)==128)fno=fno+32;
    
    nsektor=0;
    if (((daten [k])& 2)==2) nsektor=nsektor+512;       // Sektor aus Bit 1 und 2 des Byte 1 und weiter aus
    if (((daten [k])& 1)==1) nsektor=nsektor+256;       // Bit 1 bis 8 des Byte 2 auslesen
    if (((daten [j])& 1)==1) nsektor=nsektor+1;
    if (((daten [j])& 2)==2) nsektor=nsektor+2;
    if (((daten [j])& 4)==4) nsektor=nsektor+4;
    if (((daten [j])& 8)==8) nsektor=nsektor+8;
    if (((daten [j])& 16)==16) nsektor=nsektor+16;
    if (((daten [j])& 32)==32) nsektor=nsektor+32;
    if (((daten [j])& 64)==64) nsektor=nsektor+64;
    if (((daten [j])& 128)==128)nsektor=nsektor+128;
    
    //
    // Anzahl der belgten Bytes des Sektors
    // findet sich in Byte 127 
    //
    
    bytes=daten [l];
    
    printf ("\nBytes: %d",bytes);                       // # Bytes des Sektors
    printf ("\nNext: %d",nsektor);
    printf ("\nFNO: %d",fno);
    return (c);
}

