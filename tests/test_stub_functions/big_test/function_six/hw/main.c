/////////////////////////////////////////////////////////////
// Copyright (c) 2003, Impulse Accelerated Technologies, Inc.
// All Rights Reserved.
//
// HelloWorld.c: Main function.
//

#include "co.h"

#include <stdio.h>

extern co_architecture co_initialize(int arg);

int main(int argc, char** argv)
{
    co_architecture arch = co_initialize(NULL);
    co_execute(arch);
    
    return 0;
}
