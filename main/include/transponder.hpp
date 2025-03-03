#ifndef TRANSPONDER_HPP
#define TRANSPONDER_HPP

#include "cadmium/modeling/devs/atomic.hpp"
#include <iostream>

using namespace cadmium;

struct TransponderState{
    double sigma;
    int status;
    double elapsed;
    double rxpower;
    double omnipower;
    int modeA;
    int altitude;
    int outputstatus;

    explicit TransponderState(): sigma(std::numeric_limits<double>::infinity()), status(0), elapsed(0.0), rxpower(0.0), omnipower(0.0), modeA(0), altitude(0), outputstatus(0){
    }
};

std::ostream& operator<<(std::ostream &out, const TransponderState& state) {
    out  << "{Status: " << state.status << ", Elapsed: " << state.elapsed << ", Mode A:" << state.modeA << ", Altitude: " << state.altitude << ", OutputStatus: " << state.outputstatus << "}"; 
    return out;
}

class Transponder : public Atomic<TransponderState> {
    public:
    Port<double> directional_in; //Input from radar
    Port<double> omni_in;       //Input from radar
    Port<bool> altreq_out;      //Get the current altimeter
    Port<int> newalt_in;        //altitude recieved from altimeter 
    Port<int> modeA_in;         //Mode A from pilot (iestream)
    Port<int> reply_out;        //Reply back to radar

    Transponder(const std::string id) : Atomic<TransponderState>(id, TransponderState()) {
        directional_in = addInPort<double>("Transponder - Directional pulse in");           //Input port for interrogation pulse from a radar
        omni_in = addInPort<double>("Tranpsonder - Omni pulse in");                         //Input port for sidelobe suppression pulse
        altreq_out = addOutPort<bool>("Transponder - Altitude request out");                //Output port requesting altimeter altitude
        newalt_in = addInPort<int>("Transponder - New Altitude in");                        //Input port for altimeter altitude
        modeA_in = addInPort<int>("Transponder - ModeA in");                                //Input port for new mode A code
        reply_out = addOutPort<int>("Transponder - Reply to radar out");                    //output port for reply to radar
    }

    // inernal transition
    void internalTransition(TransponderState& state) const override {
     
        if(state.outputstatus == 2){
            //wait for the new altitude from the altimeter
            state.sigma = std::numeric_limits<double>::infinity();
        }
        else {           //we've just sent a reply or not received a second directional pulse, now we resest the system
            state.sigma = std::numeric_limits<double>::infinity();
            state.status = 0;
            state.altitude = 0;
            state.elapsed = 0.0;
            state.rxpower = 0.0;
            state.outputstatus = 0; 
        }
    }   

    // external transition
    void externalTransition(TransponderState& state, double e) const override {
        if(!newalt_in->empty()){
            state.altitude = newalt_in->getBag().back(); 
            state.outputstatus = 3;
            state.sigma = 3.0;
        }

        if(!modeA_in->empty()){
            state.modeA = modeA_in->getBag().back(); 
        }
        if(!omni_in->empty()){
            if(state.status==1){
                if(e == 2.0) {
                    state.omnipower = omni_in->getBag().back();
                    if(state.rxpower > state.omnipower) {
                        state.elapsed += e;
                        state.sigma -= e;
                    }
                    else {                //this is side-lobe so reset the transponder to waiting
                        state.elapsed += e;
                        state.sigma -= e;
                        state.status = 2;
                    }    
                }
            }
        } 
        
        if(!directional_in->empty()){
            if(state.status==1){
                if(e+state.elapsed == 8.0){
                    state.outputstatus = 1;
                    state.elapsed += e;
                    state.sigma = 3.0;
                }
                else if(e+state.elapsed == 21.0){
                    state.outputstatus = 2;
                    state.elapsed += e;
                    state.sigma = 0.0;
                }
                else{
                    state.elapsed += e;
                    state.sigma -= e;
                }
                
            }
            else if(state.status==2){
                //reset the system without setting an output status    
                state.sigma = 0.0;
            }
            else {
                state.rxpower = directional_in->getBag().back();
                state.status = 1;
                state.sigma = 22.0;
            }      
            
        }     
    }
    
    // output function
    void output(const TransponderState& state) const override {
        if(state.outputstatus == 1){
            reply_out->addMessage(state.modeA);
        }    
        else if(state.outputstatus == 2){  
            altreq_out->addMessage(true);
        }
        else if(state.outputstatus == 3){
            reply_out->addMessage(state.altitude);
        }       
    }

    // time_advance function
    [[nodiscard]] double timeAdvance(const TransponderState& state) const override {     
            return state.sigma;
    }
};

#endif
