#pragma once

#define FH_Print(Time, Color, Text) GEngine->AddOnScreenDebugMessage(-1, Time, Color, TEXT(Text))
#define FH_Printf(Time, Color, Text) GEngine->AddOnScreenDebugMessage(-1, Time, Color, Text)