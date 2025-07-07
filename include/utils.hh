#pragma once

#include <iostream>
#define FAIL_ON(Condition, Message)                                            \
  if (Condition) {                                                             \
    std::cerr << Message << std::endl;                                         \
    exit(-1);                                                                  \
  }

#define GL_ERR                                                                 \
  {                                                                            \
    GLenum err;                                                                \
    while ((err = glGetError()) != GL_NO_ERROR) {                              \
      std::cerr << "OpenGL error: " << err << std::endl;                       \
    }                                                                          \
  }
