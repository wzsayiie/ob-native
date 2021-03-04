#include "basis.h"
#include "cmeta.h"
#include "net.h"
#include "osbind.h"
#include "osplat.h"
#include "osui.h"

void NLaunchCPP(void);
void NLaunchJS (void);
void NLaunchC  (void);

void NLaunch(void) {
    NLaunchCPP();
    NLaunchJS ();
    NLaunchC  ();
}

void NLaunchC(void) {
}
