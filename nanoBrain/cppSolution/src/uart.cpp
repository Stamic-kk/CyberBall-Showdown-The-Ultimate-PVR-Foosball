#include "uart.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>     
#include <sys/fcntl.h>
#include <termios.h>
#include <string>

Uart::Uart(/* args */)
{
    
    struct termios  config;
    
    id = open("/dev/ttyTHS1", O_RDWR | O_NOCTTY);
    tcgetattr(id, &config);
    tcflush(id, TCIFLUSH);
    tcflush(id, TCIOFLUSH);
    usleep(1000000);
    if (id == -1)
    {
        std::cerr << "Unable to open UART" << std::endl;    
    }

      // 1 sec delay

    config.c_cflag &= ~PARENB;            
    config.c_cflag &= ~CSTOPB;            
    config.c_cflag &= ~CSIZE;	            
    config.c_cflag |=  CS8;               
    config.c_cflag &= ~CRTSCTS;           
    config.c_cflag |=  CREAD | CLOCAL;                  
    config.c_iflag &= ~(IXON | IXOFF | IXANY);          
    config.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  
    config.c_oflag &= ~OPOST;                           

    config.c_lflag = 0;               
    config.c_cc[VMIN]  = 1;      
    config.c_cc[VTIME] = 0;   
    cfsetospeed(&config,BAUDRATE);
    cfsetispeed(&config,BAUDRATE); 
    bool att = tcsetattr(id, TCSANOW, &config);
    if(att) std::cerr << "failed to set attributes" << std::endl;
    tcflush(id, TCIFLUSH);
    tcflush(id, TCIOFLUSH);
    usleep(500000);
}

Uart::~Uart()
{
    close(id);
}

void Uart::send(char *msg, int numBytes)
{
    if(id != -1){
        int count = write(id, msg, numBytes);
        usleep(1000);
        if(count < 0) std::cerr << "Failed to write to the UART" << std::endl;
        else if (count != numBytes) std::cout << "Attempt to write " <<
        numBytes << " bytes but " << count << " written."  << std::endl;
    }

}

char packByte(int rod, int value){
    return (char)((rod << 5) | (value & 0x1F))&0xFF;
}