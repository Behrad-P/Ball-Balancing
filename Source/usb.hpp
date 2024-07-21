
/**
 * @author  Behrad Pourhadi
 * @email   behradp32@gmail.com
 * @ide     Visual Studio Code
 * @brief   usb (.hpp) 
*/

#ifndef __USB_HPP__
#define __USB_HPP__

/**
 * @brief  Initialize USB communication
 * @param  port: Port number of the connected device
 * @param  baudrate: Speed of communication
 * @retval false: If there is a problem with USB initialization
 * @retval true: If there is no problem
*/
bool usbInit(const char* port, const int baudrate);

/**
 * @brief  Send data via USB
 * @param  txBuffer: Data buffer to be sent
 * @param  size: Length of the data in bytes
 * @retval false: If there is a problem with USB transmission
 * @retval true: If there is no problem
*/
bool usbTx(const string& txBuffer, const size_t size);

/**
 * @brief  Receive data via USB
 * @param  rxBuffer: Buffer to store the incoming data
 * @retval false: If there is a problem with receiving data
 * @retval true: If there is no problem
*/
bool usbRx(string& rxBuffer);

#endif