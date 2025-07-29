#pragma once

enum ArgType { ARG_VOID, ARG_STR, ARG_SCENE_OBJECT, ARG_ORIENTATION };

struct Arg {
  const char *fullName;
  const char *desc;
  enum ArgType type = ARG_STR;
  const char *defaultValue = nullptr;
};
