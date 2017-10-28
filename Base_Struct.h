//
//  Base_Struct.h
//  Read ATR
//
//  Created by Berthold Fritz on 18.03.14.

//
// Sonstiges
//

#define     BYTE    unsigned char
#define     ADRESS  int
#define     BYTESEC 128                                 // Bytes/ Sektor einer im Single Desnsity Format formatierten Diskette
#define     VTOCSEC 360                                 // Startsektor der VTOC
#define     DIRSEC  361                                 // Startsektor des Directorys
#define     DTSTART 16                                  // Erstes Datenbyte im ATR- FIle
#define     DSEKTOR (BYTESEC*DIRSEC+DTSTART)-BYTESEC    // Offset im ATR- File auf das erste Byte des Directorys (Sektor 361)
#define     VTOC    (BYTESEC*VTOCSEC+DTSTART)-BYTESEC   // Offset im ATR- File auf das erste Byte der Vtoc (Sektor 360)

// DOS 2.x File Stats

#define     NUSD    0       // File has never been used
#define     OPEN    1       // File has been opened
#define     DOS2    2       // File created by DOS 2.0
#define     LOCK    32      // File write protected
#define     INUS    64      // File is in use
#define     DEL     128     // File has been deleted

// *.ATR Image Format

struct atr_image
{
    unsigned char signature;         // Nick Atari signature=96 
    unsigned char e1;                
    unsigned char p_low;             // Paragraphs low + high
    unsigned char p_high;
    unsigned char sec_low;           // Sektor Größe
    unsigned char sec_high;
    unsigned char p_extented;        // Paragraphs extented
    unsigned char u1;                // Unbenutzt
    unsigned char u2;
    unsigned char u3;
    unsigned char u4;
    unsigned char u5;
    unsigned char u6;
    unsigned char u7;
    unsigned char u8;
    unsigned char u9;
};                                    // 16 Bytes


// Dos 2.x Directory Eintrag

struct atr_dir 
{
    char flag;              // File Status
    char count_low;         // Anzahl Sektoren, die die Datei belegt
    char count_high;
    char ssn_lo;            // Erster Sektor der Datei
    char ssn_high;
    char filename [8];      // Dateiname
    char ext [3];           // Extension
};                          // 16 Bytes

// VTOC Volume Table of Contents

struct vtoc
{
    unsigned char   dos;      // DOS Type. 0 zeigt an, es ist eine DOS 2.0 Disk
    unsigned char   sec_low;  // Anzahl der Sektoren insgesammt
    unsigned char   sec_high;  
    unsigned char   sec_unul; // Anzahl der nicht benutzten Sektoren
    unsigned char   sec_unuh;
    unsigned char   reserved; 
    unsigned char   u1;   
    unsigned char   u2;
    unsigned char   u3;
    unsigned char   u4;       // 10 Bytes
    
    unsigned char   sector_bm [90]; // 90 Bytes
    
    unsigned char   u7[28];
};                          // 128 Bytes
    
    
    
    




