#ifndef Altimeter_HPP
#define Altimeter_HPP

#include "cadmium/modeling/devs/atomic.hpp"
#include <iostream>

using namespace cadmium;

struct AltimeterState {
    double sigma;
    int altitude;

    explicit AltimeterState(): sigma(std::numeric_limits<double>::infinity()),altitude(0){
    }
};

std::ostream& operator<<(std::ostream &out, const AltimeterState& state) {
    out  << "{Altitude: " << state.altitude << "}"; 
    return out;
}

class Altimeter : public Atomic<AltimeterState> {
    public:

    Port<int> newalt_out;    //Input to set the increment value
    Port<int> altitude_in;     //altitude in from the iestream
    Port<bool> altreq_in;     //Request to send updated altimeter altitude from transponder

    Altimeter(const std::string id) : Atomic<AltimeterState>(id, AltimeterState()) {
        altreq_in = addInPort<bool>("Altimeter - Request in"); //input port for new altimeter request from transponder
        altitude_in = addInPort<int>("Altimeter - Altitude in"); //input port for new altimeter altitude
        newalt_out = addOutPort<int>("Altimeter - Altitude Out"); //output port for altimeter altitude
    }

    // inernal transition
    void internalTransition(AltimeterState& state) const override {
        state.sigma = std::numeric_limits<double>::infinity();
    }

    // external transition
    void externalTransition(AltimeterState& state, double e) const override {
        if(!altitude_in->empty()){
            state.altitude = altitude_in->getBag().back(); 
            state.sigma = std::numeric_limits<double>::infinity();
          }
        
        if(!altreq_in -> empty()) {
            state.sigma = 0; 
        }
    }
    
    // output function
    void output(const AltimeterState& state) const override {
        newalt_out->addMessage(state.altitude);
    }

    // time_advance function
    [[nodiscard]] double timeAdvance(const AltimeterState& state) const override {     
            return state.sigma;
    }
};

#endif
