#pragma once

class Forme
{
public:
    Forme();
    ~Forme();

    virtual double perimetre() = 0;
    virtual double surface() = 0;

protected:

};