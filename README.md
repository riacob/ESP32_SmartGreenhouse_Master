# To Do List
- Implement SDCard XBitmap loader (must use heap allocation to reduce stack size and because xbms aren't needed after they are printed on the display)
- Wire SDCard in parallel on the SPI bus
- Develop nice UIs with status messages etc. (?)
- Buy and test CC1101 radio modules, implement their code
- Implement other widgets / write a widget library with dynamic resource loader (for TFT_eSPI) ?
- Develop the greenhouse slave and connect the two parts of the system thgough beforementioned radio module
- Design PCBs for master and slave
- Implement bluetooth/serial/wifi data handling (bluetooth and serial optional, wifi with mqtt broker or thingspeak)
- Implement password admin login?

# Limitations
- ESP32's SRAM is "only" 320kB, not enough for 315kB needed to buffer the entire display as a sprite. It might be possible to subdivide the display in multiple sprites, but it's been deemed unnecessary for the time being.
