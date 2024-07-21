
#include <Windows.h>
#include <string>

HANDLE portHandle;
DCB dcb;

bool usbInit(const char* port, const int baudrate)
{
    portHandle = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if(portHandle == INVALID_HANDLE_VALUE)
        return false;

    if(!(GetCommState(portHandle, &dcb)))
    {
        CloseHandle(portHandle);
        return false;
    }
    
    dcb.BaudRate = baudrate; 
    dcb.ByteSize = 8; 
    dcb.Parity = NOPARITY; 
    dcb.StopBits = ONESTOPBIT; 

    if(!(SetCommState(portHandle, &dcb)))
    {
        CloseHandle(portHandle);
        return false;
    }

    return true;
}

bool usbTx(const std :: string& txBuffer, const size_t size)
{
    if(!(WriteFile(portHandle, txBuffer.c_str(), size, nullptr, nullptr)))
    {
        CloseHandle(portHandle);
        return false;
    }
    
    return true;
}

bool usbRx(std :: string& rxBuffer)
{
    char ch = '\0';

    rxBuffer = "";
    while(ch != '\n')
    {
        if(!(ReadFile(portHandle, &ch, 1, nullptr, nullptr)))
        {
            CloseHandle(portHandle);
            return false;
        }         
        rxBuffer += ch;
    }
    rxBuffer += '\0';

    return true;
}

