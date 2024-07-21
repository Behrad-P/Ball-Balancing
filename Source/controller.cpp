
#include "controller.hpp"

float pidController :: proportional(const float errVal) 
{
    pRes = errVal * kp;
    return pRes;
}

float pidController :: integral(const float errVal)
{
    iRes += errVal * ki;
  
    if(iRes > maxRes)
        iRes = maxRes;
    else if(iRes < minRes)
        iRes = minRes;
    
    return iRes;
}

float pidController :: derivative(const float errVal)
{
    if(isSpChanged)
        dRes = 0;
    else  
        dRes = (errVal - preErrVal) * kd;

    preErrVal = errVal;

    return dRes;
}

float pidController :: response(const float feedback) 
{   
    float pTerm, iTerm, dTerm, err, res;

    err = error(feedback);

    pTerm = proportional(err);
    iTerm = integral(err);
    dTerm = derivative(err);

    isSpChanged = false;
    
    res = pTerm + iTerm + dTerm;

    if(res > maxRes)
        res = maxRes;

    else if(res < minRes)
        res = minRes;

    return res;
}


