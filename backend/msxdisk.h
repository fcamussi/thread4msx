/* Fernando Camussi
   fcamussi@gmail.com */


#ifndef  __MSXDISK_H__
#define  __MSXDISK_H__

#include "types.h"


#define SC_DISK         0xF37D // 0x0005 para MSX-DOS

#define OP_GETLOGVEC    0x18
#define OP_SETDEFDRV    0x0E
#define OP_GETDEFDRV    0x19
#define OP_CREATE       0x16
#define OP_RENAME       0x17
#define OP_DELETE       0x13
#define OP_OPEN         0x0F
#define OP_CLOSE        0x10
#define OP_RNDREAD      0x27
#define OP_RNDWRITE     0x26
#define OP_SEARCH       0x11
#define OP_NSEARCH      0x12
#define OP_SETDMA       0x1A



typedef struct
{
    uint16_t sectorSize; // en bytes
    uint8_t clusterSize; // en sectores
    uint16_t numUnusedSecs;
    uint8_t numFATs;
    uint16_t numDirs;
    uint16_t numSectors;
    uint8_t mediaID;
    uint16_t sizeFAT; // en sectores
    uint16_t numTracks; // por sector
    uint16_t numSides;
    uint16_t numHiddenSecs;
} bsi_t;


typedef struct
{
    char name[11];
    uint8_t attribute;
    uint8_t notUsed[10];
    uint16_t time;
    uint16_t date;
    uint16_t topCluster;
    uint32_t size;
} dir_t;


typedef struct
{
	uint8_t drive;
	char name[11];
    uint16_t currentBlock;
    uint16_t recordSize;
    uint32_t fileSize;
    uint16_t date;
    uint16_t time;
    uint8_t deviceID;
    uint8_t dirLocation;
    uint16_t topCluster;
    uint16_t currentCluster;
    uint16_t relLocation;
    uint8_t currentRecord;
    uint32_t randomRecord;
} fcb_t;


typedef struct
{
    uint8_t dummy; // no se que hay acá
    dir_t dir;
} search_t;



void fcb_set_name(fcb_t *fcb, char *name);
uint8_t get_login_vector() __naked;
void set_default_drive(uint8_t drive) __naked;
uint8_t get_default_drive() __naked;
uint8_t create(fcb_t *fcb);
uint8_t rename(fcb_t *fcb);
uint8_t delete(fcb_t *fcb);
uint8_t open(fcb_t *fcb);
uint8_t close(fcb_t *fcb);
void seek(fcb_t *fcb, uint32_t randomRecord);
uint16_t read(fcb_t *fcb, uint16_t recSize, uint16_t numRecs, char *buffer);
uint8_t write(fcb_t *fcb, uint16_t recSize, uint16_t numRecs, char *buffer);
uint8_t search_first(fcb_t *fcb, search_t *search);
uint8_t search_next() __naked;

static uint8_t disk_op(void *ptr, uint8_t op) __naked;
static uint16_t random_read(void *ptr, uint16_t numRecs) __naked;
static uint8_t random_write(void *ptr, uint16_t numRecs) __naked;



#endif


