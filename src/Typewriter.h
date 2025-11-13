#pragma once

class Typewriter
{
public:
    Typewriter();
    void reset();
    // Prints one step; returns true when the text is fully printed
    void tick(const char *text, int delay_ms);
    bool textrunning_;

private:
    unsigned long texttime_;
    unsigned long pendingDelay_; // <- added to match typewriter.cpp
    int i_;
};