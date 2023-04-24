#pragma once


#ifdef _WIN32
  #define LIBREMU8086_EXPORT __declspec(dllexport)
#else
  #define LIBREMU8086_EXPORT
#endif

LIBREMU8086_EXPORT void libremu8086();
