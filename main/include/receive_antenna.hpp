#ifndef RECEIVE_ANTENNA_HPP
#define RECEIVE_ANTENNA_HPP

#include "cadmium/modeling/devs/atomic.hpp"
#include <iostream>

using namespace cadmium;

struct ReceiveAntennaState{
    double sigma;
    int reply;

    explicit ReceiveAntennaState(): sigma(std::numeric_limits<double>::infinity()),reply(0){
    }
};

std::ostream& operator<<(std::ostream &out, const ReceiveAntennaState& state) {
    if(state.reply == 0){
        out << "Passive";
    }
    else {
        out  << "{REPLY FROM AIRCRAFT: " << state.reply << "}"; 
    }
    return out;
}

class ReceiveAntenna : public Atomic<ReceiveAntennaState> {
    public:
    
    Port<int> reply_in;
    Port<int> out_to_atc;

    ReceiveAntenna(const std::string id) : Atomic<ReceiveAntennaState>(id, ReceiveAntennaState()) {
        reply_in = addInPort<int>("Receive - Reply from aircraft"); //Set it to input port
        out_to_atc = addOutPort<int>("Receive - Send reply out to ATC"); //Set it to output
    }

    // inernal transition
    void internalTransition(ReceiveAntennaState& state) const override {
        state.reply = 0;
        state.sigma = std::numeric_limits<double>::infinity();
    }

    // external transition
    void externalTransition(ReceiveAntennaState& state, double e) const override {  
        state.reply = reply_in->getBag().back();
        state.sigma = 0.0;
    }
    
    // output function
    void output(const ReceiveAntennaState& state) const override {
        out_to_atc->addMessage(state.reply);
    }

    // time_advance function
    [[nodiscard]] double timeAdvance(const ReceiveAntennaState& state) const override {     
            return state.sigma;
    }
};

#endif