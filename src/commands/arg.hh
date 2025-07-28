#pragma once

enum ArgType { VOID, STR, OBJECT };

struct Arg {
  const char *fullName;
  const char *desc;
  enum ArgType type = STR;
  const char *defaultValue = nullptr;
};
