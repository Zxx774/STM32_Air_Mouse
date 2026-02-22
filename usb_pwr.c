#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_pwr.h"

__IO uint32_t bDeviceState = UNCONNECTED; /* USB device status */
__IO bool fSuspendEnabled = TRUE;  /* true when suspend is possible */

void Suspend(void) {
    uint16_t wCNTR;
    wCNTR = _GetCNTR();
    wCNTR |= CNTR_FSUSP;
    _SetCNTR(wCNTR);
    wCNTR |= CNTR_LPMODE;
    _SetCNTR(wCNTR);
    bDeviceState = SUSPENDED;
}

void Resume_Init(void) {
    uint16_t wCNTR;
    wCNTR = _GetCNTR();
    wCNTR &= (~CNTR_LPMODE);
    _SetCNTR(wCNTR);
    _SetCNTR(IMR_MSK);
}

void Resume(RESUME_STATE eResumeSetVal) {
    uint16_t wCNTR;
    if (eResumeSetVal != RESUME_ESOF) Resume_Init();
    if (eResumeSetVal == RESUME_START) {
        wCNTR = _GetCNTR();
        wCNTR |= CNTR_RESUME;
        _SetCNTR(wCNTR);
    } else if (eResumeSetVal == RESUME_OFF) {
        wCNTR = _GetCNTR();
        wCNTR &= (~CNTR_RESUME);
        _SetCNTR(wCNTR);
    }
}

RESULT PowerOn(void) {
    uint16_t wRegVal;
    wRegVal = CNTR_FRES;
    _SetCNTR(wRegVal);
    wInterrupt_Mask = 0;
    _SetCNTR(wInterrupt_Mask);
    _SetISTR(0);
    wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;
    _SetCNTR(wInterrupt_Mask);
    return USB_SUCCESS;
}

RESULT PowerOff(void) {
    _SetCNTR(CNTR_FRES);
    _SetISTR(0);
    return USB_SUCCESS;
}
