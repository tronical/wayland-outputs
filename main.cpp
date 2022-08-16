#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>

struct DisplaySize {
  int width = 0;
  int height = 0;
};

static void display_handle_geometry(void *data, struct wl_output *wl_output,
                                    int x, int y, int physical_width,
                                    int physical_height, int subpixel,
                                    const char *make, const char *model,
                                    int transform) {}

static void display_handle_mode(void *data, struct wl_output *wl_output,
                                uint32_t flags, int width, int height,
                                int refresh) {
  if (flags & WL_OUTPUT_MODE_CURRENT) {
    DisplaySize *dps = (DisplaySize *)data;
    dps->width = width;
    dps->height = height;
  }
}

static const struct wl_output_listener output_listener = {
    display_handle_geometry, display_handle_mode};

static void handle_global(void *data, struct wl_registry *registry,
                          uint32_t name, const char *interface,
                          uint32_t version) {
  if (strcmp(interface, wl_output_interface.name) == 0) {
    wl_output *output =
        (wl_output *)wl_registry_bind(registry, name, &wl_output_interface, 1);
    wl_output_add_listener(output, &output_listener, data);
  }
}

static void handle_global_remove(void *data, struct wl_registry *registry,
                                 uint32_t name) {
  // Nothing to do
}

static const struct wl_registry_listener registry_listener = {
    .global = handle_global,
    .global_remove = handle_global_remove,
};

int main() {
  struct wl_display *display = wl_display_connect(0);
  if (!display) {
    std::cerr << "Could not connect to wayland display\n";
    return EXIT_FAILURE;
  }
  DisplaySize dps{};
  struct wl_registry *registry = wl_display_get_registry(display);
  wl_registry_add_listener(registry, &registry_listener, &dps);
  wl_display_roundtrip(display);
  wl_display_roundtrip(display);

  std::cout << "width = " << dps.width << " height = " << dps.height
            << std::endl;

  return 0;
}