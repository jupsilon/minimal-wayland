
#include <iostream>
#include <thread>

#include <cstring>

#include <wayland-client.h>

#include "minimal-wayland-client-protocol.h"

#include "common.hpp"

int main() {
  LOG << "enter shell." << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  LOG << "shell running..." << std::endl;

  try {
    auto display = ptr(wl_display_connect(nullptr), wl_display_disconnect);

    static void* raw[2] = {};
    {
      auto registry = ptr(wl_display_get_registry(display.get()), wl_registry_destroy);
      wl_registry_listener listener = {
	.global = [](void* data,
		     wl_registry* registry,
		     uint32_t id,
		     char const* interface,
		     uint32_t version)
	{
	  LOG << "---" << id << ", " << interface << std::endl;
	  if (!std::strcmp(interface, wl_compositor_interface.name)) {
	    raw[0] = wl_registry_bind(registry, id, &wl_compositor_interface, 3);
	    LOG << "---" << raw[0] << " bound..." << std::endl;
	  }
	  if (!std::strcmp(interface, minimal_wayland_interface.name)) {
	    raw[1] = wl_registry_bind(registry, id, &minimal_wayland_interface, 1);
	    LOG << "---" << raw[1] << " bound..." << std::endl;
	  }
	},
	.global_remove = [](void* data, wl_registry* registry, uint32_t id) {
	},
      };
      wl_registry_add_listener(registry.get(), &listener, nullptr);

      LOG << 1 << std::endl;
      wl_display_roundtrip(display.get());
      LOG << 2 << std::endl;
      wl_display_dispatch(display.get());
      LOG << 3 << std::endl;
    }
    LOG << raw << std::endl;

    
    auto compositor = ptr(reinterpret_cast<minimal_wayland*>(raw[1]), minimal_wayland_destroy);

    wl_display_roundtrip(display.get());
    wl_display_dispatch(display.get());

    LOG << "<<<<<<<<<<" << std::endl;
    minimal_wayland_quit(compositor.get());
    LOG << ">>>>>>>>>>" << std::endl;

    wl_display_roundtrip(display.get());
    wl_display_dispatch(display.get());

    LOG << "shell bye..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
  catch (std::exception& ex) {
    LOG << ex.what() << std::endl;
  }

  LOG << "leave shell." << std::endl;
  return 0;
}
