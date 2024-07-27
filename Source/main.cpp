
/**
 * @author  Behrad Pourhadi
 * @email   behradp32@gmail.com
 * @ide     Visual Studio Code
 * @brief   main (.cpp) 
 **
 * @note    After running the program, the user must enter the 'usbstart' command in the terminal to start the USB communication.
 *          To disable the USB communication, the 'usbstop' command must be entered.
 *          This allows the user to first ensure the correctness of the HSV values before the controller and servo motors come into action.
 ** 
 * @note    If the servo motor rotation is not correct, you can multiply the response by -1, as shown in line 174.
 **
 * @details Valid commands: "pid", "usbstart", "usbstop", "exit"
 *          "exit" => Terminate the program.
 *          "usbstop" => Disable the USB communication.
 *          "usbstart" => Enable the USB communication.
 *          "pid" => Set the controller coefficients.
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector> 
#include "controller.hpp"
#include "global.hpp"
#include "usb.hpp"

/* Image processing & Gui thread */
bool imageProcessing()
{
    constexpr uint8_t H_MAX = 179;
    constexpr uint8_t S_MAX = 255;
    constexpr uint8_t V_MAX = 255;

    /* Define buttons */
    vector<Button> buttons = {
        {RECTS[0], "ct", 0},
        {RECTS[1], "tr", 1},
        {RECTS[2], "tl", 2},
        {RECTS[3], "br", 3},
        {RECTS[4], "bl", 4}
    };
    uint8_t count = 0;

    if(BUTTONS_IMG.empty())
        cerr << endl << ERR_MSGS[GUI];
    else
    {
        HIGHLIGHTED = BUTTONS_IMG.clone();
        namedWindow(BUTTONS_WINDOW_NAME);
        setMouseCallback(BUTTONS_WINDOW_NAME, onMouse, &buttons);
        rectangle(HIGHLIGHTED, RECTS[0], Scalar(0, 0, 255), 2);
        imshow(BUTTONS_WINDOW_NAME, HIGHLIGHTED);
    }

    namedWindow("Trackbars");
    createTrackbar("Hue min", "Trackbars", &hsvLowerColor[0], H_MAX, updateValues);
    createTrackbar("Saturation min", "Trackbars", &hsvLowerColor[1], S_MAX, updateValues);
    createTrackbar("Value min", "Trackbars", &hsvLowerColor[2], V_MAX, updateValues);
    createTrackbar("Hue max", "Trackbars", &hsvUpperColor[0], H_MAX, updateValues);
    createTrackbar("Saturation max", "Trackbars", &hsvUpperColor[1], S_MAX, updateValues);
    createTrackbar("Value max", "Trackbars", &hsvUpperColor[2], V_MAX, updateValues);

    VideoCapture v(0);

    if(v.isOpened() == false)
    {
        cerr << endl << ERR_MSGS[CAMERA];
        cout << endl << MSGS[IMG_PROC_TER];

        return FAILED;
    }

    while(1)
    {
        Mat frame, cropped, blurred;
        Mat hsv, mask, filter;
        vector<vector<Point>> contours;

        newFrame = false;
        v.read(frame);

        if(frame.empty())
        {
            cerr << endl << ERR_MSGS[FRAME];
            cout << endl << MSGS[IMG_PROC_TER];

            return FAILED;
        }
        newFrame = true;

        cropped = frame(Range(Y_RANGE[0], Y_RANGE[1]), Range(X_RANGE[0], X_RANGE[1]));
        GaussianBlur(cropped, blurred, Size(3, 3), 0);
        cvtColor(blurred, hsv, COLOR_BGR2HSV);
        inRange(hsv, Scalar(hsvLowerColor[0], hsvLowerColor[1], hsvLowerColor[2]), Scalar(hsvUpperColor[0], hsvUpperColor[1],
                                                                                          hsvUpperColor[2]), mask);
        bitwise_and(blurred, blurred, filter, mask);
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        circle(cropped, Point((cropped.size().width / 2), (cropped.size().height / 2)), 5, Scalar(255, 0, 0), FILLED);
        
        for (const auto& contour : contours)
        {
            Rect boundingrect = boundingRect(contour);
            if(boundingrect.area() > OBJ_DET_AREA)
            {
                rectangle(filter, boundingrect, Scalar(255, 0, 0), 2); 
                x = boundingrect.x + (boundingrect.width / 2);
                y = boundingrect.y + (boundingrect.height / 2);
                circle(filter, Point(x, y), 5, Scalar(0, 0, 255), FILLED);
                objDet = true;
                count = 0;
            }
        }

        if(count <= OBJ_DET_TIMEOUT)
        {
            if(count++ >= OBJ_DET_TIMEOUT)
            {
                objDet = false;
                ballBalacingX->resetResponse();
                ballBalacingY->resetResponse();
            }
        }
        
        imshow("Filter", filter);
        imshow("Cropped", cropped);

        if(waitKey(10) == 'q')
            break;
    }

    objDet = false;
    cout << endl << MSGS[IMG_PROC_TER];
    cout << endl << MSGS[GET_CMD];

    return SUCCESSFUL;
}

/* Usb communication thread */
bool usbCommunication()
{   
    const string LED_SET_CMD[5] = {"ledoff", "h,4", "h,5", "h,6", "h,7"};      // h => high; 4, ..., 7 => pin 4, ..., 7; in arduino
    const string ARD_ACK_STR = "Ok"; 
    constexpr uint8_t LED_RST_IDX = 0;
    constexpr bool LED_RST = 0;
    constexpr bool LED_SET = 1;

    string usbTxBuff = "", usbRxBuff = "";
    float xPoint, yPoint;
    int hServoPos, vServoPos;
    int xRes, yRes;
    bool step = LED_RST;

    delay(1000);      // Delay for 1 second

    if(!(usbInit(PORT, BAUDRATE)))
    {
        cerr << endl << ERR_MSGS[USB];        
        cout << endl << MSGS[USB_COMM_TER];

        return FAILED;
    }

    delay(ARDUINO_SETUP_DELAY);
    
    while(1)
    {
        while(!(usbCondition == START && objDet && newFrame)) this_thread :: yield();  

        xPoint = static_cast<float>((x / PIXEL_DIVISOR));
        yPoint = static_cast<float>((y / PIXEL_DIVISOR));
        xRes = static_cast<int>(ballBalacingX->response(xPoint));               
        yRes = static_cast<int>(ballBalacingY->response(yPoint));
        hServoPos = (xRes * (-1)) + SERVO_DEFAULT_ANGLE;      // ((-1) * res) => Reverse servo rotation 
        vServoPos = yRes + SERVO_DEFAULT_ANGLE;

        if(ledCondition == RESET)
            usbTxBuff = to_string(hServoPos) + ',' + to_string(vServoPos);
        else
        {
            if(step == LED_RST)
            {
                usbTxBuff = LED_SET_CMD[LED_RST_IDX];
                step = LED_SET;
            }
            else if(step == LED_SET)
            {
                if(ledCondition != LED_RST_IDX)
                    usbTxBuff = LED_SET_CMD[ledCondition]; 

                step = LED_RST;
                ledCondition = RESET; 
            }
        }

        if(!(usbTx(usbTxBuff, usbTxBuff.length())))
        {
            cerr << endl << ERR_MSGS[TRANSMIT];        
            cout << endl << MSGS[USB_COMM_TER];
            cout << endl << MSGS[GET_CMD];

            return FAILED;
        } 

        if(!(usbRx(usbRxBuff))) 
        {
            cerr << endl << ERR_MSGS[RECEIVE];        
            cout << endl << MSGS[USB_COMM_TER];
            cout << endl << MSGS[GET_CMD];

            return FAILED;
        } 

        else if(!(usbRxBuff[0] == ARD_ACK_STR[0] && usbRxBuff[1] == ARD_ACK_STR[1]))
        {
            char cmdChar = '\0';

            cout << endl << MSGS[ARD_ERR] << usbRxBuff << endl;

            if(isTerminalBusy)
                cerr << ERR_MSGS[TERMINAL_1];
            
            lock_guard<mutex> lock(mtx);
            
            ask:
            cout << MSGS[ASK2CONTINUE];
            isTerminalBusy = true;
            cin >> cmdChar;

            if(cmdChar == 'n')
            {
                isTerminalBusy = false;
                break;
            }
            else if(cmdChar != 'y')
                goto ask; 
                
            isTerminalBusy = false;  
        }

        delay(COMMUNICATION_DELAY);
    }

    cout << endl << MSGS[USB_COMM_TER];
    cout << endl << MSGS[GET_CMD];

    return SUCCESSFUL;
}

thread imgProc(imageProcessing), usbComm(usbCommunication);

/* Cli thread */
int main()
{ 
    enum cmdIt
    {
        PID = 0,
        USB_START = 1,
        USB_STOP = 2,
        EXIT = 3,
    };
    const string COMMAND_LIST[4] = {"pid", "usbstart", "usbstop", "exit"};

    string userCmd;
    bool isCmdValid = false;
    
    delay(2000);      // Delay for 2 second

    cout << endl << MSGS[CMD_LIST] << endl;

    while(1)
    {
        lock_guard<mutex> lock(mtx);

        if(isTerminalBusy)
        {
            cerr << ERR_MSGS[TERMINAL_0] << endl;
            cout << "=> " << MSGS[ASK2CONTINUE];
        }
        else
        {    
            cout << MSGS[GET_CMD];
            isTerminalBusy = true;
            getline(cin, userCmd, '\n');
            
            isCmdValid = true;
            if(userCmd == COMMAND_LIST[PID])
            {
                try
                {
                    cout << MSGS[GET_P];
                    getline(cin, userCmd, '\n');
                    kp = stof(userCmd);
                    cout << MSGS[GET_I];
                    getline(cin, userCmd, '\n');
                    ki = stof(userCmd);
                    cout << MSGS[GET_D];
                    getline(cin, userCmd, '\n');
                    kd = stof(userCmd);
                }
                catch(...)
                {
                    isCmdValid = false;
                    goto cmdCheck;
                }

                ballBalacingX->setCoefficients(kp, ki, kd);
                ballBalacingY->setCoefficients(kp, ki, kd);
            }
            else if(userCmd == COMMAND_LIST[USB_START])
                usbCondition = START;
            else if(userCmd == COMMAND_LIST[USB_STOP])
                usbCondition = STOP;
            else if(userCmd == COMMAND_LIST[EXIT])
                break;
            else
                isCmdValid = false;    

            cmdCheck:
            if(isCmdValid)
                cout << MSGS[ACK] << endl;
            else
                cerr << ERR_MSGS[COMMAND] << endl;

            isTerminalBusy = false;
        }  
    }

    delete ballBalacingX;
    delete ballBalacingY;

    return SUCCESSFUL;
}
