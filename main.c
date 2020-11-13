#include <msp430g2553.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;  // set range
    DCOCTL = CALDCO_1MHZ;   // set step

    P1REN = 0x08;   // enabling pull up/down resistor for P1.3
    P1DIR = 0x01;   // make P1.0 output
    P1OUT = 0x08;   // pull up resistor is set

    P1IES |= BIT3;   // edge select high->low
    P1IFG &= ~BIT3; // interrupt flag cleared
    P1IE |= BIT3;   // enable interrupt for P1.3

   //  _enable_interrupt();    // enable interrupt
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    if(P1IFG & BIT3)    // if P1.3 is pressed
    {
        P1IE &= ~BIT3;  // disable interrupt
        P1IFG &= ~BIT3; // clear flag
        P1OUT ^= 0x01;  // toggle LED

        TACCR0 = 62499; // count up to 32
        TACCTL0 = CCIE; // interrupt enabled
        TACTL = TASSEL_2 + ID_1 + MC_1 + TACLR;
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void CCR0_ISR(void)
{
    TACCTL0 &= ~CCIE;   // interrupt disabled
    P1IE |= BIT3;   // re-enable interrupt for P1.3
}
