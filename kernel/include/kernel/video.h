#ifndef _KERNEL_VIDEO_H
#define _KERNEL_VIDEO_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <kernel/device.h>

typedef enum video_mode {
  TEXT_SCREEN,
  PIXEL_SCREEN,
} video_mode_t;

struct video {
  size_t         width;
  size_t         height;

  size_t         bpe;
  size_t         pitch;

  size_t         size;
  video_mode_t   mode;

  uint32_t      *buffer;
  struct device *dev;
};

extern struct video *video_inst;

typedef struct vga_320x200x256_s {
  uint8_t  *address;
  uint32_t  *buffer;

  size_t size;
} vga_320x200x256_t;

void init_video    (bool);
void swap_buffers (struct video *video, size_t minx, size_t miny, size_t maxx, size_t maxy);

void screen_loop ();

#endif
