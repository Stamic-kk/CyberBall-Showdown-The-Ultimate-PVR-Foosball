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
    handle = open("/dev/ttyTHS1", O_RDWR | O_NOCTTY);
    tcgetattr(handle, &config);
    tcflush(handle, TCIFLUSH);
    tcflush(handle, TCIOFLUSH);
    usleep(1000000);
    if (handle == -1)
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
    bool att = tcsetattr(handle, TCSANOW, &config);
    if(att) std::cerr << "failed to set attributes" << std::endl;
    tcflush(handle, TCIFLUSH);
    tcflush(handle, TCIOFLUSH);
    usleep(500000);
}

Uart::~Uart()
{
    close(handle);
}

void Uart::send(char *msg, int numBytes)
{
    if(handle != -1){
        int count = write(handle, msg, numBytes);
        usleep(1000);
        if(count < 0) std::cerr << "Failed to write to the UART" << std::endl;
        else if (count != numBytes) std::cout << "Attempt to write " <<
        numBytes << " bytes but " << count << " written."  << std::endl;
    }

}

char Uart::recevie(){
    char byte = '\0';
    if(handle != -1){
        int count = read(handle, &byte, 1);
        if(count < 0) std::cerr << "Failed to read from the UART" << std::endl;
        else if (count == 0) std::cerr << "No data available" << std::endl;
    }
    return byte;
}


char packByte(int rod, int value){
    return (char)((rod << 5) | (value & 0x1F))&0xFF;
}