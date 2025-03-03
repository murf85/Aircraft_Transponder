#ifndef DIRECTIONAL_ANTENNA_HPP
#define DIRECTIONAL_ANTENNA_HPP

#include "cadmium/modeling/devs/atomic.hpp"
#include <iostream>


using namespace cadmium;

struct DirectionalAntennaState {
    double sigma;
    double outputpower;
 
    explicit DirectionalAntennaState(): sigma(std::numeric_limits<double>::infinity()), outputpower(0) {
    }
};

std::ostream& operator<<(std::ostream &out, const DirectionalAntennaState& state) {
    out  << "{Power Setting: " << state.outputpower << "}"; 
    return out;
}

class DirectionalAntenna : public Atomic<DirectionalAntennaState> {
    public:

    Port<double> interrogation_list_in;
    Port<double> directional_out;
    Port<bool> cue_omni_out;


    //Declare your ports here
    DirectionalAntenna(const std::string id) : Atomic<DirectionalAntennaState>(id, DirectionalAntennaState()) {
        cue_omni_out = addOutPort<bool>("Directional - Cue to omni out");                       //Cue the omni antenna for sidelobe suppression pulse
        directional_out = addOutPort<double>("Directional - Interrogation pulse out");          //Interrogation pulse to the aircraft
        interrogation_list_in = addInPort<double>("Directional - Interrogation list in");       //from IEStream
    }

    // inernal transition
    void internalTransition(DirectionalAntennaState& state) const override {
        //your internal transition function goes here
        state.outputpower = 0;
        state.sigma = std::numeric_limits<double>::infinity();
    }

    // external transition
    void externalTransition(DirectionalAntennaState& state, double e) const override {
        state.outputpower = interrogation_list_in->getBag().back();
        state.sigma = 0;
        //your external transition function hoes here
    }
    
    // output function
    void output(const DirectionalAntennaState& state) const override {
        directional_out->addMessage(state.outputpower);
        //check if pulse is P1
        if(state.outputpower == 0.1){    
            cue_omni_out->addMessage(true);
        }
        else if(state.outputpower == -20.0){
            cue_omni_out->addMessage(true);
        }
    }

    // time_advance function
    [[nodiscard]] double timeAdvance(const DirectionalAntennaState& state) const override {     
            return state.sigma;
    }
};

#endif