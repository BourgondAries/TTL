TTL - Tea Tank Library
======================

A C++ library of tools to make generic functionality easier to achieve.
The library's only dependence is the C++ standard library.
Many parts of the library are stand-alone parts and can function without other library elements.

Installing
----------

You will need to build all sources into a static or dynamic library and link it in your program.
You will need to include <TTL.hpp>, which includes all other sub-parts.
If you have code::blocks, then you can use the included project file in ide/codeblocks/project.cbp

Simplicity
----------

TTL is a very simple to use. Here is an example of a program using Ips: "Iterations Per Second" limiter.

\code
#include <TTL.hpp>
#include <iostream>

int main()
{
	ttl::Ips frames(30.f); // Limit the frames to 30 fps.
	for (int i = 0; i < 60; ++i)
	{
		std::cout << i << std::endl;
		frames.limit(); // Records current time versus previous time
		// Sleeps an amount based on the time the loop took.
		// If the loop takes 1 second to get back to frames.limit(),
		// Then the sleep time will be 0
	}
}
\endcode


Author
------

Kevin Robert Stravers



The name has its origin from the creator's excessive consumption of tea during programming (Tea Tanking).