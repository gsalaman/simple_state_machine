#include "queue.h"

typedef enum
{
  STATE_INIT,
  STATE_RED,
  STATE_GREEN,
  STATE_YELLOW,
  NUM_STATES     /* MUST BE LAST!!! */
} state_type;

typedef enum
{
  INPUT_INVALID = -1,
  INPUT_NEXT,
  NUM_INPUTS /* MUST BE LAST */
} input_type;

typedef void (*entry_func_type)(void);

void enter_init( void );
void enter_red( void );
void enter_green( void );
void enter_yellow( void );

entry_func_type entry_table[] =
{
  enter_init,
  enter_red,
  enter_green,
  enter_yellow
};

state_type transition_table[NUM_STATES][NUM_INPUTS] =
{
                   /* INPUT_NEXT */
  /* STATE_INIT */   {STATE_GREEN},
  /* STATE_RED */    {STATE_GREEN},
  /* STATE_GREEN */  {STATE_YELLOW},
  /* STATE_YELLOW */ {STATE_RED}
};

void enter_init( void )
{
  Serial.println("Enter Init");
  
  /* Light all LEDS */ 
}

void enter_red( void )
{
  Serial.println("Enter Red");
  
  /* Turn off Yellow and Green, if on */

  /* Turn on Red */
}

void enter_yellow( void )
{
  Serial.println("Enter Yellow");
  
  /* turn off red and green, if on */

  /* Turn on Yellow */
}

void enter_green( void )
{
  Serial.println("Enter Green");
  
  /* turn off red and yellow, if on */

  /* turn on green */
}


void state_machine_init( void )
{
  enter_init();
}

void state_machine_add_input( input_type input )
{
  if (queue_put(input) == -1)
  {
    Serial.println("ERROR!  QUEUE FULL!!!");
  }
  
}

void state_machine_driver( void )
{
  static state_type current_state = STATE_INIT;
  state_type        next_state;
  input_type        current_input;
  entry_func_type   next_entry_func;
  
  /* Loop over all queued inputs */
  current_input = queue_get();
  while (current_input != INPUT_INVALID)
  {
    /* Look up next state */
    next_state = transition_table[current_state][current_input];
  
    /* debug print */
    Serial.print("Current state: ");
    Serial.print(current_state);
    Serial.print(", Current input: ");
    Serial.print(current_input);
    Serial.print(", Next state: ");
    Serial.println(next_state);

    /* If we're changing state, call the entry function for the next state */
    if (current_state != next_state)
    {
      next_entry_func = entry_table[next_state];

      /* only call it if it's not void */
      if (next_entry_func)
      {
        next_entry_func();
      }
    }

    /* update our state */
    current_state = next_state;
    
    /* ... and get the next input */
    current_input = queue_get();
    
  } /* end of looping over all inputs */
  
}

void setup() 
{
  Serial.begin(9600);
  state_machine_init();
 
}

void loop() 
{

  state_machine_add_input(INPUT_NEXT);

  delay(1000);
  
  // put your main code here, to run repeatedly:
  state_machine_driver();
}
