/*
This file contains a few hotfixes to generate libnesmikprof.so which acts as a drop-in replacement for libphiprof.so
*/

#include <string>
#include <vector>
#include <map>
#include "mpi.h"

#include "nesmik/nesmik.hpp"

using namespace std; // antipattern

namespace phiprof
{

   static thread_local map<int,string> id_to_label;
   bool initialize(){
      nesmik::nesmik_init();
      return true;
   }


   bool start([[maybe_unused]] const string &label){
      nesmik::region_start(label);
      return true;
   }

   bool start([[maybe_unused]] int id){
      if (id_to_label.count(id)){
         const auto& label = id_to_label[id];
         start(label);
      }
      return true;
   }


   bool stop ([[maybe_unused]] const string &label, [[maybe_unused]] double workUnits, [[maybe_unused]] const string &workUnitLabel){
      nesmik::region_stop(label);
      return true;
   }

   bool stop ([[maybe_unused]] int id) {
      if (id_to_label.count(id)){
         const auto& label = id_to_label[id];
         stop(label,0.0,"");
      }
      return true;
   }
   bool stop ([[maybe_unused]] int id, [[maybe_unused]] double workUnits, [[maybe_unused]] const string &workUnitLabel){
      return stop(id);
   }
   

   int getChildId([[maybe_unused]] const std::string &label) {return 0;}
   
   int initializeTimer([[maybe_unused]] const string &label){
      int new_id = id_to_label.size()+1;
      id_to_label[new_id] = label;
      return new_id;
   }
   
   int initializeTimer([[maybe_unused]] const string &label, [[maybe_unused]] const vector<string> &groups) {
      return initializeTimer(label);
   }
 
   int initializeTimer([[maybe_unused]] const string &label, [[maybe_unused]] const string &group1){return initializeTimer(label);}
   int initializeTimer([[maybe_unused]] const string &label, [[maybe_unused]] const string &group1, [[maybe_unused]] const string &group2){return initializeTimer(label);}
   int initializeTimer([[maybe_unused]] const string &label, [[maybe_unused]] const string &group1, [[maybe_unused]] const string &group2, [[maybe_unused]]const string &group3){return initializeTimer(label);}

   bool print([[maybe_unused]] MPI_Comm comm, [[maybe_unused]] std::string fileNamePrefix){
      nesmik::nesmik_finalize();
      return true;
   }
   

}

