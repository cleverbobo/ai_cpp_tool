#include "test.h"



int test_nms(TimeStamp& timeStamp,const int LoopNum, const std::string FilePath){

    // load data 
    auto InputBoxVec = load_box_data(FilePath);
    BoxVec OutputBoxVec;

    // loop test
    for(int i = 0 ; i < LoopNum; i++ ){

        timeStamp.RecordTime("C++ NMS time");
        NMS(InputBoxVec, OutputBoxVec);
        timeStamp.RecordTime("C++ NMS time");
        OutputBoxVec.clear();

        // std::cout << "boxVec_Cpp.size(): " << boxVec_Cpp.size() <<std::endl;        
    }
    
#ifdef DEBUG
    dump_Data("./OutputBoxVec.txt",OutputBoxVec);
#endif
    return -1;
}