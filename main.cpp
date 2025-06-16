#include "process_input.h"
#include <iostream>


using namespace std;


int main() {

    char input[256];

    admins = new Administrator*[2];
    admins[0] = new Administrator("adi", "smith", "admin1", "Pass1");
    admins[1] = new Administrator("adi2", "smith2", "admin2", "Pass2");
    admins_count = 2;


    while (true) {
        cout << "> ";
        cin.getline(input, 256);
        if (strings_equal(input, "quit")) break;
        process_input (input);
    }

    return 0;

}


