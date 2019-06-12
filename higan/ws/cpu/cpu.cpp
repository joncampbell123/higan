#include <ws/ws.hpp>

namespace higan::WonderSwan {

CPU cpu;
#include "io.cpp"
#include "interrupt.cpp"
#include "dma.cpp"
#include "serialization.cpp"

auto CPU::main() -> void {
  poll();
//static uint c=0;if(auto d = disassemble()) if(++c<60) print(d, "\n");
  exec();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(ppu);
  synchronize(apu);
  synchronize(cartridge);
}

auto CPU::wait(uint clocks) -> void {
  step(clocks);
}

auto CPU::read(uint20 addr) -> uint8 {
  return bus.read(addr);
}

auto CPU::write(uint20 addr, uint8 data) -> void {
  return bus.write(addr, data);
}

auto CPU::in(uint16 port) -> uint8 {
  return bus.portRead(port);
}

auto CPU::out(uint16 port, uint8 data) -> void {
  return bus.portWrite(port, data);
}

auto CPU::power() -> void {
  V30MZ::power();
  Thread::create(3'072'000, [&] {
    while(true) scheduler.resume(), main();
  });

  bus.map(this, 0x00a0);
  bus.map(this, 0x00b0, 0x00b6);

  if(Model::WonderSwanColor() || Model::SwanCrystal()) {
    bus.map(this, 0x0040, 0x0049);
    bus.map(this, 0x0062);
  }

  r = {};
}

}
