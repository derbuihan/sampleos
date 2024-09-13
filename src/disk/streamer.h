#ifndef SAMPLEOS_STREAMER_H
#define SAMPLEOS_STREAMER_H

#include "disk.h"

struct disk_streamer {
  int pos;
  struct disk* disk;
};

struct disk_streamer* diskstreamer_new(int disk_id);
int diskstreamer_seek(struct disk_streamer* stream, int pos);
int diskstreamer_read(struct disk_streamer* stream, void* out, int total);
void diskstreamer_close(struct disk_streamer* stream);

#endif  // SAMPLEOS_STREAMER_H
