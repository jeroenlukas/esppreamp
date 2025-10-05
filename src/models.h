#ifndef MODELS_H
#define MODELS_H

struct MinMax_float_t
{
    float min;
    float max;
} ;

class Model
{
    public:
        Model();

        String name;
        uint8_t index;

        MinMax_float_t gain_db;

        MinMax_float_t low_db;
        MinMax_float_t mid_db;
        MinMax_float_t high_db;
        MinMax_float_t presence_db;

        float mid_frequency;
        float mid_q;

}

Model::Model()
{
    gain_db.max = 5;
    gain_db.min = -6;
    mid_frequency = 400;
}

#endif