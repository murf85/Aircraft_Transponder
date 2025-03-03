#ifndef RADAR_HPP
#define RADAR_HPP

#include "cadmium/modeling/devs/coupled.hpp"      //cadmium header
#include "cadmium/lib/iestream.hpp"               //iestream header
#include "omni_antenna.hpp"                       //omni atomic model file
#include "directional_antenna.hpp"                //directional atomic model file
#include "receive_antenna.hpp"                    //receive atomic model file

using namespace cadmium;

struct Radar: public Coupled {
  
  Port<double> directional_file_in;
  Port<double> radar_omni_out;
  Port<double> radar_directional_out;
  Port<int> radar_reply_in;
  Port<int> radar_reply_out;

  Radar(const std::string& id): Coupled(id){
    auto directional_antenna_model = addComponent<DirectionalAntenna>("Directional Antenna Model");
    auto omni_antenna_model = addComponent<OmniAntenna>("Omni Antenna Model");
    auto receive_antenna_model = addComponent<ReceiveAntenna>("Receive Antenna Model");
    
    directional_file_in = addInPort<double>("Radar Interrogation list in");
    radar_omni_out = addOutPort<double>("Radar = Omni pulse out");
    radar_directional_out = addOutPort<double>("Radar - Directional pulse out");
    radar_reply_in = addInPort<int>("Radar - Transponder reply in");
    radar_reply_out = addOutPort<int>("Radar - Transponder reply to ATC out");

    //Internal Couplings
    addCoupling(directional_antenna_model->cue_omni_out, omni_antenna_model->cue_omni_in);
    
    //External Couplings
    addCoupling(directional_antenna_model->directional_out, radar_directional_out);
    addCoupling(omni_antenna_model->omni_out, radar_omni_out);
    addCoupling(radar_reply_in, receive_antenna_model->reply_in);
    addCoupling(directional_file_in, directional_antenna_model->interrogation_list_in);
    addCoupling(receive_antenna_model->out_to_atc, radar_reply_out);
  }
};

#endif
