#include "streamer.h"

#include "../config.h"
#include "../memory/heap/kheap.h"

struct disk_streamer* diskstreamer_new(int disk_id) {
  struct disk* disk = disk_get(disk_id);

  if (!disk) {
    return 0;
  }

  struct disk_streamer* stream = kzalloc(sizeof(struct disk_streamer));
  stream->pos = 0;
  stream->disk = disk;
  return stream;
}

int diskstreamer_seek(struct disk_streamer* stream, int pos) {
  stream->pos = pos;
  return 0;
}

int diskstreamer_read(struct disk_streamer* stream, void* out, int total) {
  int sector = stream->pos / SAMPLEOS_SECTOR_SIZE;
  int offset = stream->pos % SAMPLEOS_SECTOR_SIZE;
  char buf[SAMPLEOS_SECTOR_SIZE];

  int res = disk_read_block(stream->disk, sector, 1, buf);
  if (res < 0) goto out;

  int total_to_read =
      total > SAMPLEOS_SECTOR_SIZE ? SAMPLEOS_SECTOR_SIZE : total;

  for (int i = 0; i < total_to_read; i++) {
    *(char*)out++ = buf[offset + i];
  }

  stream->pos += total_to_read;
  if (total > SAMPLEOS_SECTOR_SIZE) {
    res = diskstreamer_read(stream, out, total - SAMPLEOS_SECTOR_SIZE);
  }

out:
  return res;
}

void diskstreamer_close(struct disk_streamer* stream) { kfree(stream); }