#ifndef SAMPLEOS_DISK_H
#define SAMPLEOS_DISK_H

typedef unsigned int SAMPLEOS_DISK_TYPE;

// Represents a real physical hard disk
#define SAMPLEOS_DISK_TYPE_REAL 0

struct disk {
  SAMPLEOS_DISK_TYPE type;
  int sector_size;
};

void disk_search_and_init();
struct disk *disk_get(int index);
int disk_read_block(struct disk *idisk, unsigned int lba, unsigned int total,
                    void *buf);

#endif  // SAMPLEOS_DISK_H
