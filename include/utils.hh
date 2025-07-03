#pragma once

#include <iostream>
#define FAIL_ON(Condition, Message)                                            \
  if (Condition) {                                                             \
    std::cerr << Message << std::endl;                                         \
    exit(-1);                                                                  \
  }
