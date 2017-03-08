#ifndef FREDBUFFEREDVALUE_H
#define FREDBUFFEREDVALUE_H
class BufferedValue{
protected:
    float value = 0;
    int steps = 20;
    float oneMinusStep = 0.95, step = 0.05;
public:
  
    BufferedValue(float val):value(val){
    }
  
    void update(float newValue){
        value = oneMinusStep * value + step * newValue;
    }
    
    void update(float newValue, int timesteps){
        if(timesteps >= steps){
            value = newValue;
            return;
        }
        if(timesteps == 1){
            update(newValue);
            return;
        }
        float step = timesteps * this->step;
        float oms = 1-step;
        value = oms * value + step * newValue;
    }
  
    //more steps => value changes slower
    void setSteps(int steps){
        this->steps = steps;
        step = 1.0/steps;
        oneMinusStep = 1 - step;
    }
    
    float getSteps(){
        return steps;
    }
  
    void reset(float val = -1){
        value = val;
    }
  
    float getValue(){
        return value;
    }
};
#endif