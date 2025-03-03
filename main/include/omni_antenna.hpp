#ifndef OMNI_ANTENNA_HPP
#define OMNI_ANTENNA_HPP

#include "cadmium/modeling/devs/atomic.hpp"
#include <iostream>

using namespace cadmium;

struct OmniAntennaState {
    double sigma;
    explicit OmniAntennaState(): sigma(std::numeric_limits<double>::infinity()){
    }
};

std::ostream& operator<<(std::ostream &out, const OmniAntennaState& state) {
    if(state.sigma == (std::numeric_limits<double>::infinity())){
        out  << "Passive"; 
    }
    else {
        out  << "Preparing sidelobe suppression pulse"; 
    }    
    return out;
}

class OmniAntenna : public Atomic<OmniAntennaState> {
    
    public:
    Port<bool> cue_omni_in;
    Port<double> omni_out;
   
    OmniAntenna(const std::string id) : Atomic<OmniAntennaState>(id, OmniAntennaState()) {
        
        cue_omni_in = addInPort<bool>("Omni - Cue from directional in");    //Cue from directional antenna to send sidelobe suppression pulse
        omni_out = addOutPort<double>("Omni - Sidelobe suppression pulse out");                          //Sidelobe suppression pulse out to aircraft
    }

    // inernal transition
    void internalTransition(OmniAntennaState& state) const override {
        state.sigma = std::numeric_limits<double>::infinity();
    }

    // external transition
    void externalTransition(OmniAntennaState& state, double e) const override {
        state.sigma = 2.0;
    }
    // output function
    void output(const OmniAntennaState& state) const override {
        omni_out->addMessage(-18.0);
    }

    // time_advance function
    [[nodiscard]] double timeAdvance(const OmniAntennaState& state) const override {     
            return state.sigma;
    }
};

#endif