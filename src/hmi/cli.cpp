#include <Arduino.h>
#include <SimpleCLI.h>

#include "../adau1701/adau1701.h"
#include "../models.h"
#include "../patches.h"
#include "../filesystem/filesystem.h"

SimpleCLI cli;

// Basic commands
Command cmd_reset;
Command cmd_help;
Command cmd_cat;

// DSP
Command cmd_gain;
Command cmd_low;
Command cmd_high;

// Model
Command cmd_model;
Command cmd_patch;

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

void cb_cat(cmd* c)
{
    Command cmd(c);

    if(cmd.getArgument(0).isSet())
    {
        String filename = cmd.getArgument(0).getValue();

        Serial.println("---" + filename + "---");
        String file_content = filesystem_readfile(filename);
        Serial.print(file_content);
        Serial.println("---END---");
    }
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

void cb_gain(cmd* c)
{
    Command cmd(c);
    if(cmd.getArgument(0).isSet())
    {        
        uint8_t gain = cmd.getArgument(0).getValue().toInt();
        patch_update_gain(gain);
    }
}

void cb_low(cmd* c)
{
    Command cmd(c);
    if(cmd.getArgument(0).isSet())
    {        
        uint8_t low = cmd.getArgument(0).getValue().toInt();
        patch_update_low(low);
    }
}

void cb_high(cmd* c)
{
    Command cmd(c);
    if(cmd.getArgument(0).isSet())
    {        
        uint8_t high = cmd.getArgument(0).getValue().toInt();        
        patch_update_high(high);
    }
}

void cb_model(cmd* c)
{
    Command cmd(c);

    Argument arg_id = cmd.getArgument("id");
    Argument arg_show = cmd.getArgument("show");
    Argument arg_json = cmd.getArgument("json");

    Model_t model;

    if(arg_id.isSet())
    {
        models_find(&model, arg_id.getValue().toInt());        
    }

    if(arg_show.isSet())
    {
        // Only show the model, in json or yaml
        if(arg_json.isSet())
        {
            String json = model_to_json(model);
            Serial.println(json);
        }
        else
        {
            String yaml = model_to_yaml(model);
            Serial.println(yaml);
        }
    }
    else
    {
        // Actually load the model!
        Serial.println("Activating model " + String(model.id) + ": " + model.name);
        model_activate(model);
    }
}


void cb_patch(cmd* c)
{
    Command cmd(c);

    Argument arg_id = cmd.getArgument("id");
    Argument arg_show = cmd.getArgument("show");
    Argument arg_json = cmd.getArgument("json");

    Patch_t patch;

    if(arg_id.isSet())
    {
        patches_find(&patch, arg_id.getValue().toInt());        
    }

    if(arg_show.isSet())
    {
        // Only show the patch, in json or yaml
        if(arg_json.isSet())
        {
            String json = patch_to_json(patch);
            Serial.println(json);
        }
        else
        {
            String yaml = patch_to_yaml(patch);
            Serial.println(yaml);
        }
    }
    else
    {
        // Actually load the patch!
        Serial.println("Activating patch " + String(patch.id) + ": " + patch.name);
        patch_activate(patch);
    }
}


void cli_init(void)
{
    cli.setOnError(cb_error);

    cmd_reset = cli.addCommand("reset", cb_reset);
    cmd_reset.setDescription("- Reset the ESP32");

    cmd_help = cli.addCommand("help", cb_help);
    cmd_help.setDescription("- Show this help");

    cmd_cat = cli.addSingleArgCmd("cat", cb_cat);
    cmd_cat.setDescription("- Show contents of a file");

    cmd_freq = cli.addSingleArgCmd("freq", cb_freq);
    cmd_freq.setDescription("- Set the frequency");

    cmd_gain = cli.addSingleArgCmd("gain", cb_gain);
    cmd_gain.setDescription("- Set the gain");

    cmd_low = cli.addSingleArgCmd("low", cb_low);
    cmd_high = cli.addSingleArgCmd("high", cb_high);

    cmd_model = cli.addCmd("model", cb_model);
    cmd_model.addPositionalArgument("id", "0");
    cmd_model.addFlagArgument("show");
    cmd_model.addFlagArgument("json");

    cmd_patch = cli.addCmd("patch", cb_patch);
    cmd_patch.addPositionalArgument("id", "0");
    cmd_patch.addFlagArgument("show");
    cmd_patch.addFlagArgument("json");
    cmd_patch.addPositionalArgument("high", "50");
    cmd_patch.addPositionalArgument("mid", "50");

    // model 1                  Load model 1
    // model -id 1 -show
    // model 1 -show            Show model 1 yaml
    // model 1 -show -json      Show model 1 in json 


    //cmd_model_search = cli.addSingleArgCmd("model_search", cb_model_search);


    Serial.println("CLI initialized. Type 'help' to view commands!");
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