
/**
 * @author  Behrad Pourhadi
 * @email   behradp32@gmail.com
 * @ide     Visual Studio Code
 * @brief   global (.hpp) 
*/

#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include <string>
#include <chrono>
#include <thread>
#include <mutex> 
#include <atomic>

using namespace std;
using namespace cv;

constexpr int calSpCoor(const uint8_t idx);
void delay(int ms);
void updateValues(int, void*);
void highlightButton(const string buttonLabel);
void onMouse(int event, int x, int y, int, void* userdata);

/******************** USER ********************/

// ========== PID ========== //
constexpr float RESPONSE_MAX = {60};
constexpr float RESPONSE_MIN = {-60};
float kp = {3};
float ki = {0.02};
float kd = {40};
float sp;
pidController* ballBalacingX = new pidController(kp, ki, kd, calSpCoor(0), RESPONSE_MIN, RESPONSE_MAX);      // Controller initialization
pidController* ballBalacingY = new pidController(kp, ki, kd, calSpCoor(1), RESPONSE_MIN, RESPONSE_MAX);      // Controller initialization
// ========== END PID ========== //

// ============ USB ============ //
constexpr uint16_t BAUDRATE = {9600};
const char* PORT = "COM9";
// ============ END USB ============ //

// ============ IMAGE PROCESSING ============ //
constexpr float PIXEL_DIVISOR = {20}; 
constexpr int OBJ_DET_AREA = {200};      // In pixels
constexpr int Y_RANGE[2] = {0, 480};      // 480 Pixels
constexpr int X_RANGE[2] = {80, 560};     // 480 Pixels
constexpr uint8_t OBJ_DET_TIMEOUT = {5};      // Camera fps = 30  =>  OBJ_DET_TIMEOUT * 33.3 = timeout
const vector<Rect> RECTS = {Rect(192, 217, 96, 46), Rect(352, 23, 96, 46), Rect(32, 28, 96, 46),
                                         Rect(352, 405, 96, 46), Rect(32, 407, 96, 46)};   // BR, BL
const Mat BUTTONS_IMG = imread("E:/OpenCV C++/projects/Ball_Balacing/gui/Buttons.jpg");
Mat HIGHLIGHTED;
const string BUTTONS_WINDOW_NAME = "Buttons Window";
int hsvLowerColor[3] = {30, 40, 125};
int hsvUpperColor[3] = {60, 255, 255};
// ============ END IMAGE PROCESSING ============ //

// ============ SERVO & ARDUINO ============ //
constexpr int SERVO_DEFAULT_ANGLE = {90}; 
constexpr int ARDUINO_SETUP_DELAY = {3000};      // 3000 ms
constexpr int COMMUNICATION_DELAY = {10};      // 10 ms
// ============ END SERVO & ARDUINO ============ //

/******************** END USER ********************/

constexpr short int RESET = -1;
constexpr bool STOP = 0;
constexpr bool START = 1;
constexpr bool SUCCESSFUL = 0;
constexpr bool FAILED = 1;
const string ERR_MSGS[9] = {

    "Terminal is busy at the usbCommunication thread!",
    "Invalid command!",
    "Can not open video / camera!",
    "Can not read frame!",
    "Unable to open usb port!",
    "Can not transmit data!",
    "Can not receive data!",
    "Terminal is busy at the userInterface thread! enter any character to release the terminal: "
    "Can not open gui window!"
};
const string MSGS[10] = {
    
    "Command list: \"pid, usbstart, usbstop, exit\".",
    "Enter command: ",
    "Enter P coefficient: ",
    "Enter I coefficient: ",
    "Enter D coefficient: ",
    "Done.",
    "Arduino Error: ",
    "Do you want to continue? y / n: ",
    "Image processing is terminated.",
    "USB communication is terminated."
};

struct Button
{
    Rect rect;
    string label;
    int id;
};

enum errMsgsIt
{
    TERMINAL_0 = 0,
    COMMAND = 1,
    CAMERA = 2,
    FRAME = 3,
    USB = 4,
    TRANSMIT = 5,
    RECEIVE = 6,
    TERMINAL_1 = 7,
    GUI = 8
};
enum msgsIt
{
    CMD_LIST = 0,
    GET_CMD = 1,
    GET_P = 2,
    GET_I = 3,
    GET_D = 4,
    ACK = 5,
    ARD_ERR = 6,
    ASK2CONTINUE = 7,
    IMG_PROC_TER = 8,
    USB_COMM_TER = 9
};

mutex mtx;
atomic<int> x(0), y(0);
atomic<short int> ledCondition(RESET);
atomic<bool> usbCondition(STOP);
atomic<bool> isTerminalBusy(false);
atomic<bool> objDet(false);
atomic<bool> newFrame(false);

constexpr int calSpCoor(const uint8_t idx)
{
	constexpr int X_SCALE = ((X_RANGE[1]) - (X_RANGE[0])) / PIXEL_DIVISOR;
	constexpr int Y_SCALE = ((Y_RANGE[1]) - (Y_RANGE[0])) / PIXEL_DIVISOR;

	if(PIXEL_DIVISOR > 0.0)
	{
		if(idx == 0)
			return (int)((1.0/2.0) * X_SCALE);
		else if(idx == 1)
			return (int)((1.0/2.0) * Y_SCALE);
		else if(idx == 2)
			return (int)((3.0/4.0) * X_SCALE);
		else if(idx == 3)
			return (int)((1.0/4.0) * Y_SCALE);
		else if(idx == 4)
			return (int)((1.0/4.0) * X_SCALE);
		else if(idx == 5)
			return (int)((1.0/4.0) * Y_SCALE);
		else if(idx == 6)
			return (int)((3.0/4.0) * X_SCALE);
		else if(idx == 7)
			return (int)((3.0/4.0) * Y_SCALE);
		else if(idx == 8)
			return (int)((1.0/4.0) * X_SCALE);
		else if(idx == 9)
			return (int)((3.0/4.0) * Y_SCALE);
	}
	
	return 0;
}

void delay(int ms)
{
    this_thread :: sleep_for(chrono :: milliseconds(ms));
}

void updateValues(int, void*)
{
    hsvLowerColor[0] = getTrackbarPos("Hue min", "Trackbars");
    hsvLowerColor[1] = getTrackbarPos("Saturation min", "Trackbars");
    hsvLowerColor[2] = getTrackbarPos("Value min", "Trackbars");
    hsvUpperColor[0] = getTrackbarPos("Hue max", "Trackbars");
    hsvUpperColor[1] = getTrackbarPos("Saturation max", "Trackbars");
    hsvUpperColor[2] = getTrackbarPos("Value max", "Trackbars");
}

void highlightButton(const string buttonLabel)
{
    HIGHLIGHTED = BUTTONS_IMG.clone();
    if (buttonLabel == "ct") rectangle(HIGHLIGHTED, RECTS[0], Scalar(0, 0, 255), 2);
    else if (buttonLabel == "tr") rectangle(HIGHLIGHTED, RECTS[1], Scalar(0, 0, 255), 2);
    else if (buttonLabel == "tl") rectangle(HIGHLIGHTED, RECTS[2], Scalar(0, 0, 255), 2);
    else if (buttonLabel == "br") rectangle(HIGHLIGHTED, RECTS[3], Scalar(0, 0, 255), 2);
    else if (buttonLabel == "bl") rectangle(HIGHLIGHTED, RECTS[4], Scalar(0, 0, 255), 2);

    imshow(BUTTONS_WINDOW_NAME, HIGHLIGHTED);
}

void onMouse(int event, int x, int y, int, void* userdata)
{
    enum spIt
    {
        CT_X = 0,
        CT_Y = 1,
        UR_X = 2,
        UR_Y = 3,
        UL_X = 4,
        UL_Y = 5,
        DR_X = 6,
        DR_Y = 7,
        DL_X = 8,
        DL_Y = 9
    };
    constexpr int SETPOINT_LIST[5][2] = {{calSpCoor(CT_X), calSpCoor(CT_Y)}, {calSpCoor(UR_X), calSpCoor(UR_Y)},
									     {calSpCoor(UL_X), calSpCoor(UL_Y)}, {calSpCoor(DR_X), calSpCoor(DR_Y)},
									     {calSpCoor(DL_X), calSpCoor(DL_Y)}};

    if(event != EVENT_LBUTTONDOWN) return;

    vector<Button>* buttons = (vector<Button>*)userdata;

    for(const Button& button : *buttons)
    {
        if(button.rect.contains(Point(x, y)))
        {
            ballBalacingX->setSetpoint(SETPOINT_LIST[button.id][0]); 
            ballBalacingY->setSetpoint(SETPOINT_LIST[button.id][1]); 
            highlightButton(button.label);
            ledCondition = button.id;
        }
    }
}

#endif