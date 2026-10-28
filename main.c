#include <msp430.h>
#include <stdlib.h>

#define LED_DIR_REG        P1DIR
#define LED_OUT_REG        P1OUT
#define LED_1_PIN          BIT0
#define LED_2_PIN          BIT1
#define LED_3_PIN          BIT2
#define LED_4_PIN          BIT3
#define ALL_LEDS           (BIT0 | BIT1 | BIT2 | BIT3)

#define BUTTON_DIR_REG        P2DIR
#define BUTTON_REN            P2REN
#define BUTTON_IN             P2IN
#define BUTTON_EN             P2OUT
#define BUTTON_1_PIN          BIT0
#define BUTTON_2_PIN          BIT1
#define BUTTON_3_PIN          BIT2
#define BUTTON_4_PIN          BIT3
#define ALL_BUTTONS           (BIT0 | BIT1 | BIT2 | BIT3)
#define MAX_SEQUENCE_LENGTH   10

typedef enum{
    IDLE, SHOW_SEQUENCE, PLAYER_INPUT, CHECK_ANSWER, WIN, GAME_OVER
} GameState_t;

typedef enum{
    LED1, LED2, LED3, LED4, ALL_OFF, ALL_ON
} LED_t;

LED_t sequence[MAX_SEQUENCE_LENGTH];
LED_t playerInput;
unsigned int sequenceLength = 3;
unsigned int inputIndex = 0;

void init(void);
static void setLED(LED_t LED);
static LED_t getRandomLED(void);
static void generateSequence(void);
static void waitForButtonRelease(void);
static GameState_t runIdleState(void);
static GameState_t runShowSequenceState(void);
static GameState_t runPlayerInputState(void);
static GameState_t runCheckAnswerState(void);
static GameState_t runWinState(void);
static GameState_t runGameOverState(void);

int main(void){
    GameState_t currentState = IDLE;
    GameState_t nextState = currentState;
    init();

    while(1){
        switch(currentState){
            case IDLE:
                nextState = runIdleState();
                break;
            case SHOW_SEQUENCE:
                nextState = runShowSequenceState();
                break;
            case PLAYER_INPUT:
                nextState = runPlayerInputState();
                break;
            case CHECK_ANSWER:
                nextState = runCheckAnswerState();
                break;
            case WIN:
                nextState = runWinState();
                break;
            case GAME_OVER:
                nextState = runGameOverState();
                break;
            default:
                break;
        }

        currentState = nextState;
    }
}

void init(void){
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer

    LED_DIR_REG |= ALL_LEDS; // Initialize all LED pins
    LED_OUT_REG &= ~ALL_LEDS; // All LEDs OFF

    BUTTON_DIR_REG &= ~ALL_BUTTONS; // Initialize all button pins
    BUTTON_REN |= ALL_BUTTONS; // Enable internal pull-up resistors
    BUTTON_EN |= ALL_BUTTONS;

    TACTL = TASSEL_2 | MC_2 | TACLR; // Start Timer_A - SMCLK, Continuous, Clear
}

static void setLED(LED_t LED){
    switch (LED) {
    case LED1:
        LED_OUT_REG |= LED_1_PIN;
        break;
    case LED2:
        LED_OUT_REG |= LED_2_PIN;
        break;
    case LED3:
        LED_OUT_REG |= LED_3_PIN;
        break;
    case LED4:
        LED_OUT_REG |= LED_4_PIN;
        break;
    case ALL_OFF:
        LED_OUT_REG &= ~ALL_LEDS;
        break;
    case ALL_ON:
        LED_OUT_REG |= ALL_LEDS;
        break;
    default:
        LED_OUT_REG &= ~ALL_LEDS;
        break;
    }
}

static LED_t getRandomLED(void){
    return (LED_t)(rand() % 4);
}

static void generateSequence(void){
    srand(TAR);
    sequenceLength = 3;
    inputIndex = 0;
    unsigned int i;
    for(i = 0; i < MAX_SEQUENCE_LENGTH; i++){
        sequence[i] = getRandomLED();
    }
}

static void waitForButtonRelease(void){
    while ((BUTTON_IN & ALL_BUTTONS) != ALL_BUTTONS) {}
    __delay_cycles(20000);
}

static GameState_t runIdleState(void){
    setLED(ALL_OFF);
    if((BUTTON_IN & ALL_BUTTONS) != ALL_BUTTONS){
        generateSequence();
        return SHOW_SEQUENCE;
    } else {
        return IDLE;
    }
}

static GameState_t runShowSequenceState(void){
    unsigned int i;

    for (i = 0; i < sequenceLength; i++){
        setLED(ALL_OFF);
        setLED(sequence[i]);
        __delay_cycles(500000);
        setLED(ALL_OFF);
        __delay_cycles(500000);
    }

    return PLAYER_INPUT;
}

static GameState_t runPlayerInputState(void){
    if((BUTTON_IN & BUTTON_1_PIN) == 0){
            playerInput = LED1;
            setLED(LED1);
            __delay_cycles(400000);
            setLED(ALL_OFF);
            waitForButtonRelease();
            return CHECK_ANSWER;
    }

    if((BUTTON_IN & BUTTON_2_PIN) == 0){
        playerInput = LED2;
        setLED(LED2);
        __delay_cycles(400000);
        setLED(ALL_OFF);
        waitForButtonRelease();
        return CHECK_ANSWER;
    }

    if((BUTTON_IN & BUTTON_3_PIN) == 0){
        playerInput = LED3;
        setLED(LED3);
        __delay_cycles(400000);
        setLED(ALL_OFF);
        waitForButtonRelease();
        return CHECK_ANSWER;
    }

    if((BUTTON_IN & BUTTON_4_PIN) == 0){
        playerInput = LED4;
        setLED(LED4);
        __delay_cycles(400000);
        setLED(ALL_OFF);
        waitForButtonRelease();
        return CHECK_ANSWER;
    }

    return PLAYER_INPUT;
}

static GameState_t runCheckAnswerState(void){
    if(playerInput != sequence[inputIndex]){
        return GAME_OVER;
    }

    inputIndex++;

    if(inputIndex >= sequenceLength){
        inputIndex = 0;

        if(sequenceLength >= MAX_SEQUENCE_LENGTH){
            return WIN;
        }

        sequenceLength++;
        return SHOW_SEQUENCE;
    }

    return PLAYER_INPUT;
}

static GameState_t runWinState(void){
    volatile LED_t i;
    volatile LED_t j;

    while((BUTTON_IN & ALL_BUTTONS) == ALL_BUTTONS){
        for(i = LED1; i < LED3; i++){
            setLED(ALL_OFF);
            if((i % 2) == 0){
                for(j = LED1; j < ALL_OFF; j += 2){
                    setLED(j);
                }
                __delay_cycles(100000);
            } else {
                for(j = LED2; j < ALL_OFF; j += 2){
                   setLED(j);
                }
                __delay_cycles(100000);
            }
        }
    }
    waitForButtonRelease();
    return IDLE;
}

static GameState_t runGameOverState(void){
    setLED(ALL_ON);

    while((BUTTON_IN & ALL_BUTTONS) == ALL_BUTTONS) {}
    waitForButtonRelease();

    return IDLE;
}
