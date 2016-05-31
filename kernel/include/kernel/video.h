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

  struct device *dev;
};

struct textmode {
  uint16_t *address;

  uint8_t fg_color;
  uint8_t bg_color;

  size_t x;
  size_t y;

  struct video *video;
} textmode_t;

struct vga_320x200x256_s {
  uint32_t  *address;
  uint8_t   *buffer;
} vga_320x200x256_t;

struct video* video;

int init_video    (bool);
void swap_buffers ();

#endif
