#ifndef SPI
#define SPI

class spi {
    private:
        const char *device = "/dev/spidev0.0";
        uint32_t mode = 0;
        uint8_t bits = 8;
        uint32_t speed = 13000000;
        uint16_t delay = 0;
        uint32_t buff_size = 4096;
        uint8_t fd;

    public:
        spi();
        int spi_open();
        int spi_close();
        int transmit(uint8_t *data, uint32_t len);

};

#endif