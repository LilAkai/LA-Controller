#pragma once

#ifdef LA_CONTROLLER_EXPORTS
#define LA_CONTROLLER_API __declspec(dllexport)
#else
#define LA_CONTROLLER_API __declspec(dllimport)
#endif