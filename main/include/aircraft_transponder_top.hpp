#ifndef TRANSPONDER_TOP_HPP
#define TRANSPONDER_TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"    //Coupled model header
#include "cadmium/lib/iestream.hpp"             //IEStream header
#include "radar.hpp"
#include "aircraft.hpp"


using namespace cadmium;


struct Transponder_top : public Coupled {

    Port<int> reply_out;

    Transponder_top(const std::string& id) : Coupled(id) {

        auto radar_model = addComponent<Radar>("Radar model");
        auto aircraft_model = addComponent<Aircraft>("Aircraft model");
       
        reply_out = addOutPort<int>("Top - Transponder reply to ATC");

        auto modeAgen = addComponent<lib::IEStream<int>>("IEStream Mode A generator file", "/home/murf85/cadmium_v2/include/Aircraft_Transponder/input_data/modeA.txt");
        auto altitudegen = addComponent<lib::IEStream<int>>("IEStream Altitude generator file", "/home/murf85/cadmium_v2/include/Aircraft_Transponder/input_data/altitude.txt");
        auto directional_file = addComponent<lib::IEStream<double>>("IEStream Directional Antenna", "/home/murf85/cadmium_v2/include/Aircraft_Transponder/input_data/directional.txt");

        addCoupling(modeAgen->out, aircraft_model->ac_modeA_in);
        addCoupling(altitudegen->out, aircraft_model->ac_altimeter_in);
        addCoupling(directional_file->out, radar_model->directional_file_in);
        
        addCoupling(radar_model->radar_directional_out, aircraft_model->ac_directional_in);
        addCoupling(radar_model->radar_omni_out, aircraft_model->ac_omni_in);
        addCoupling(aircraft_model->ac_reply_out, radar_model->radar_reply_in);

        addCoupling(radar_model->radar_reply_out, reply_out);

    }   
};

#endif