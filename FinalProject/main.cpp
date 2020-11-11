#include "simulation.h"

static Simulation sim;

Simulation &getSim() {
    return sim;
}
int main(void){
    sim.run();
    return 0;
}