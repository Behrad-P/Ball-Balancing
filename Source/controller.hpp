
/**
 * @author  Behrad Pourhadi
 * @email   behradp32@gmail.com
 * @ide     Visual Studio Code
 * @brief   controller (.hpp) 
*/

#ifndef __CONTROLLER_HPP__
#define __CONTROLLER_HPP__

class pidController
{

private:

    float kp, ki, kd;
    float sp;
    float minRes, maxRes;
    float preErrVal;
    float pRes, iRes, dRes;
    bool isSpChanged = true;

    /**
     * @brief  Calculate error
     * @param  f: Feedback
     * @retval Error value
    */
    inline float error(const float f) const
    {
        return f - sp;
    } 

    /**
     * @brief  Calculate the proportional response
     * @param  errVal: Error value
     * @retval Proportional response
    */
    float proportional(const float errVal); 

    /**
     * @brief  Calculate the integral response
     * @param  errVal: Error value
     * @retval Integral response
    */
    float integral(const float errVal);

    /**
     * @brief  Calculate the derivative response
     * @param  errVal: Error value
     * @retval Derivative response
    */
    float derivative(const float errVal); 

public:

    /**
     * @brief  Initialize PID controller
     * @param  pCoefficient: Proportional coefficient
     * @param  iCoefficient: Integral coefficient
     * @param  dCoefficient: Derivative coefficient
     * @param  setpoint: Desired setpoint
     * @param  minResponse: Minimum value of the controller output (response)
     * @param  maxResponse: Maximum value of the controller output (response)
     * @retval None
    */
    pidController(const float pCoefficient, const float iCoefficient, const float dCoefficient, const float setpoint,
                               const float minResponse, const float maxResponse) : kp(pCoefficient), ki(iCoefficient),
                               kd(dCoefficient), sp(setpoint), minRes(minResponse), maxRes(maxResponse) {}

    /**
     * @brief  Default constructor
     * @param  None
     * @retval None
    */
    pidController() = default;

    /**
     * @brief  Calculate the controller response
     * @param  feedback: System feedback
     * @retval Controller response
    */
    float response(const float feedback); 

    /**
     * @brief  Change the system setpoint
     * @param  setpoint: New setpoint
     * @retval None
    */
    inline void setSetpoint(const float setpoint)
    {
        sp = setpoint;
        isSpChanged = true;
    }

    /**
     * @brief  Change the controller coefficients
     * @param  pCoefficient: Proportional coefficient
     * @param  iCoefficient: Integral coefficient
     * @param  dCoefficient: Derivative coefficient
     * @retval None
    */
    inline void setCoefficients(const float pCoefficient, const float iCoefficient, const float dCoefficient)
    {
        kp = pCoefficient;
        ki = iCoefficient;
        kd = dCoefficient;
    }

    /**
     * @brief  Change the minimum and maximum response values of the controller
     * @param  minResponse: Minimum value of the controller output (response)
     * @param  maxResponse: Maximum value of the controller output (response)
     * @retval None
    */
    inline void setResponseRange(const float minResponse, const float maxResponse)
    {
        minRes = minResponse;
        maxRes = maxResponse;
    }   

    /**
     * @brief  Reset controller response
     * @param  None
     * @retval None
    */
    inline void resetResponse()
    {
        iRes = 0;
        preErrVal = 0;
    }
};

#endif
