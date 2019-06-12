#include <msx/msx.hpp>

namespace higan::MSX {

#include "port.cpp"
#include "gamepad/gamepad.cpp"

Controller::Controller() {
  if(!handle()) Thread::create(1, [&] {
    while(true) scheduler.resume(), main();
  });
  cpu.peripherals.append(this);
}

Controller::~Controller() {
  removeWhere(cpu.peripherals) == this;
  Thread::destroy();
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

}
