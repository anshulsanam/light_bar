#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spi.h"

#include <iostream>
using namespace std;


spi::spi() {
    // mode |= SPI_LSB_FIRST;
    mode |= SPI_NO_CS;

}

int spi::spi_open() {
    int n;
    FILE *fp;

    
    // Open device file
    if ((fd = open(device, O_RDWR)) < 0) {
        cerr << "Can't open device" << endl;
        return -1;
    }

    // Set SPI mode in this case 0
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0 || ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0) {
        cerr << "Can't get or set SPI mode" << endl;
        close(fd);
        return -1;
    }

    // Set SPI bits per word
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        cerr << "Can't set bits per word" << endl;
        close(fd);
        return -1;
    }

    // Set SPI speed
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        cerr << "Can't set max speed hz" << endl;
        close(fd);
        return -1;
    }    

    return fd;
}

int spi::spi_close() {
    int ret;

    if ((ret = close(fd)) < 0)
        cerr << "Error closing SPI device" << endl;
    return ret;
}

int spi::transmit(uint8_t *data, uint32_t len) {
    int ret;

    uint8_t rx[buff_size];
    
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long) data,
        .rx_buf = (unsigned long) rx,
        .len = len,
        .speed_hz = speed,
        .delay_usecs = delay,
        .bits_per_word = bits,
    };

    if ((ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr)) < 0)
        cerr << "Can't send spi message" << endl;
    
    return ret;
}
