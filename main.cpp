#include "test.h"

int main(int argc, char *argv[]){
    
    // init timeStamp
    TimeStamp timeStamp;

    // test case
    test_nms(timeStamp,1);

    // show time info
    timeStamp.ShowInfo();
    return 0;

}