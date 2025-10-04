#include <Arduino.h>
#include <SimpleCLI.h>

#include "../adau1701/adau1701.h"

SimpleCLI cli;

// Basic commands
Command cmd_reset;
Command cmd_help;

// Test
Command cmd_freq;

void cli_init(void);
void cli_parse(String input);

void cli_handle(void);

void cb_error(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}

void cb_help(cmd* c)
{
    Serial.println("--- Commands ---");
    Serial.println(cli.toString());
}

void cb_reset(cmd* c) 
{
    Command cmd(c);

    Serial.println("I will be resetting!");

    delay(1000);

    ESP.restart();
}

void cb_freq(cmd* c)
{
    Command cmd(c);
    if(cmd.getArgument(0).isSet())
    {
        
        int freq = cmd.getArgument(0).getValue().toInt();
        Serial.println("Set freq to " + String(freq) + " Hz");
        adau1701_set_testfreq(freq);
    }

    
}

void cli_init(void)
{
    cli.setOnError(cb_error);

    cmd_reset = cli.addCommand("reset", cb_reset);
    cmd_reset.setDescription("- Reset the ESP32");

    cmd_help = cli.addCommand("help", cb_help);
    cmd_help.setDescription("- Show this help");

    cmd_freq = cli.addSingleArgCmd("freq", cb_freq);
    cmd_freq.setDescription("- Set the frequency");
}

void cli_parse(String input)
{
    cli.parse(input);
}

void cli_handle(void)
{
    if (Serial.available()) 
    {
        static char commandbuffer[100] = "";
        static uint8_t commandbuffer_idx = 0;

        char inp = Serial.read();
        Serial.print('\r');
        //Serial.print((char)inp);
        
        commandbuffer[commandbuffer_idx] = inp;
        commandbuffer_idx ++;
        commandbuffer[commandbuffer_idx] = '\0';

        Serial.print(commandbuffer);

        if(inp == '\n')
        {        
            String commandstr = String(commandbuffer);
            cli_parse(commandstr);
            commandbuffer_idx = 0;
        }
    }
}