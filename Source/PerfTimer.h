#pragma once

#include "Globals.h"
#include "SDL\include\SDL.h"

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	double ReadMs() const;
	Uint64 ReadTicks() const;

private:
	Uint64	started_at;
	static Uint64 frequency;
};

