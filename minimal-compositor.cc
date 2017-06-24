
#include <iostream>
#include <string>

#include <cstdlib>

#include <wayland-server.h>

#include "minimal-wayland-server-protocol.h"

#include "common.hpp"


int main(int argc, char** argv) {
  LOG << "enter compositor." << std::endl;

  try {
    auto display = ptr(wl_display_create(), wl_display_destroy);
    LOG << wl_display_add_socket_auto(display.get()) << std::endl;

    wl_global_create(display.get(),
		     &wl_compositor_interface,
		     3,
		     nullptr,
		     [](wl_client* client,
			void* data,
			uint32_t version,
			uint32_t id)
		     {
		       LOG << "bind: compositor" << std::endl;
		       auto resource = wl_resource_create(client, &wl_compositor_interface, 1, id);
		       struct wl_compositor_interface interface = {
			 .create_surface = [](wl_client* client, wl_resource* resource, uint32_t id) {
			   LOG << "surface creating..." << std::endl;
			 },
			 .create_region = [](wl_client* client, wl_resource* resource, uint32_t id) {
			   LOG << "region creating..." << std::endl;
			 },
		       };
		       wl_resource_set_implementation(resource, &interface, nullptr, nullptr);
		     });

    wl_global_create(display.get(),
		     &minimal_wayland_interface,
		     1,
		     nullptr,
		     [](wl_client* client,
			void* data,
			uint32_t version,
			uint32_t id)
		     {
		       LOG << "bind: minimal wayland" << std::endl;
		       auto resource = wl_resource_create(client, &minimal_wayland_interface, 1, id);

		       struct minimal_wayland_interface interface = {
			 .quit = [](wl_client* client,
				    wl_resource* resource)
			 {
			   LOG << "quit called:" << client << "," << resource << std::endl;
			 },
		       };
		       wl_resource_set_implementation(resource, &interface, nullptr, nullptr);
		     });

    for (int i = 1; i < argc; ++i) {
      system(concat("./", argv[i], " &").c_str());
    }

    auto event_loop = ptr(wl_display_get_event_loop(display.get()), wl_event_loop_destroy);
    wl_display_run(display.get());
  }
  catch (std::exception& ex) {
    LOG << ex.what() << std::endl;
  }

  LOG << "leave compositor." << std::endl;

  return 0;
}
