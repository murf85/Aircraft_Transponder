#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include "cadmium/modeling/devs/coupled.hpp" //cadmium header
#include "altimeter.hpp"                     //encoding Altimeter file
#include "transponder.hpp"                  //encoding Altimeter file

using namespace cadmium;

struct Aircraft: public Coupled {

  Port<double> ac_omni_in;
  Port<int> ac_altimeter_in;
  Port<int> ac_reply_out;
  Port<int> ac_modeA_in;
  Port<double> ac_directional_in;

  Aircraft(const std::string& id): Coupled(id){
    auto Altimeter_model = addComponent<Altimeter>("Altimeter");
    auto Transponder_model = addComponent<Transponder>("Transponder");

    ac_directional_in = addInPort<double>("Aircraft - Directional pulse in");
    ac_omni_in = addInPort<double>("Aircraft - Omni pulse in");
    ac_reply_out = addOutPort<int>("Aircraft - Transponder reply out");      
    ac_modeA_in = addInPort<int>("Aircraft - Mode A in");
    ac_altimeter_in = addInPort<int>("Aircraft - Altimeter in");
   

    //Internal Couplings
    addCoupling(Transponder_model->altreq_out, Altimeter_model->altreq_in);
    addCoupling(Altimeter_model->newalt_out, Transponder_model->newalt_in);
    
    
    //External Couplings
    addCoupling(Transponder_model->reply_out, ac_reply_out);
    addCoupling(ac_omni_in, Transponder_model->omni_in);
    addCoupling(ac_directional_in, Transponder_model->directional_in);
    addCoupling(ac_modeA_in, Transponder_model->modeA_in);
    addCoupling(ac_altimeter_in, Altimeter_model->altitude_in);
  }

};


#endif