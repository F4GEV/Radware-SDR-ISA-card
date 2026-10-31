#include "thisCLS.H"
#include <stdio.h>

#define MIN_SPAN 8000
Start_Control::Start_Control(long *Start,long *Stop)

{
  start = Start;
  stop = Stop;
}

char *Start_Control::Get_Name(void)

{
  return "Start";
}

char *Start_Control::Get_Error_Message(void)

{
  return "Start frequency out of range.";
}

long Start_Control::Get_Value(void)

{
  return *start;
}

int Start_Control::Set_Value(long value)

{
  if (value<*stop)
  {
    if (Frequency_Control::Set_Value(value))
    {
      *start = value;
      return 1;
    }
    else
      return 0;
  }
  else
  {
    if (value<=Frequency_Control::Get_Max()-MIN_SPAN)
      if (Frequency_Control::Set_Value(value))
      {
        *start = value;
        *stop = value+MIN_SPAN;
        return 1;
      }
      else
        return 0;
    else
      return 0;
  }
}

Stop_Control::Stop_Control(long *Start,long *Stop)

{
  start = Start;
  stop = Stop;
}

char *Stop_Control::Get_Name(void)

{
  return "Stop";
}

char *Stop_Control::Get_Error_Message(void)

{
  return "Stop frequency out of range.";
}

long Stop_Control::Get_Value(void)

{
  return *stop;
}

int Stop_Control::Set_Value(long value)

{
  if (value>*start)
  {
    if (Frequency_Control::Set_Value(value))
    {
      *stop = value;
      return 1;
    }
    else
      return 0;
  }
  else
  {
    if (value>=Frequency_Control::Get_Min()+MIN_SPAN)
      if (Frequency_Control::Set_Value(value))
      {
        *stop = value;
        *start = value-MIN_SPAN;
        return 1;
      }
      else
        return 0;
    else
      return 0;
  }
}

Center_Control::Center_Control(long *Start,long *Stop)

{
  start = Start;
  stop = Stop;
}

char *Center_Control::Get_Name(void)

{
  return "Center";
}

char *Center_Control::Get_Error_Message(void)

{
  return "Center frequency out of range.";
}

long Center_Control::Get_Value(void)

{
  return (*start+*stop)/2;
}

int Center_Control::Set_Value(long value)

{
  long span;

  if (Frequency_Control::Set_Value(value))
  {
    span = stop-start;
    if (value-span/2<Frequency_Control::Get_Min() ||
	 value+span/2>Frequency_Control::Get_Max())
      return 0;
    *start = value-span/2;
    if (span & 1)
      *stop = value+span/2+1;
    else
      *stop = value+span/2;
    return 1;
  }
  else
    return 0;
}

Span_Control::Span_Control(long *Start,long *Stop)

{
  start = Start;
  stop = Stop;
}

char *Span_Control::Get_Error_Message(void)

{
  return "Span frequency out of range.";
}

char *Span_Control::Get_Name(void)

{
  return "Span";
}

long Span_Control::Get_Value(void)

{
  return *stop-*start;
}

int Span_Control::Set_Value(long value)

{
  long center;

  if (value>=MIN_SPAN && value<=29500000)
  {
    center = (*start+*stop)/2;
    if (center-value/2<Frequency_Control::Get_Min() ||
        center+value/2>Frequency_Control::Get_Max())
      return 0;
    *start = center-value/2;
    if (value & 1)
      *stop = center+value/2+1;
    else
      *stop = center+value/2;
    return 1;
  }
  else
    return 0;
}

long Span_Control::Get_Min(void)

{
 return MIN_SPAN;
}

Marker_Control::Marker_Control(long *Start,long *Stop)

{
  start = Start;
  stop = Stop;
  marker = (*start+*stop)/2;
}

char *Marker_Control::Get_Name(void)

{
  return "Marker";
}

char *Marker_Control::Get_Error_Message(void)

{
  return "Marker out of range.";
}

long Marker_Control::Get_Value(void)

{
  if (marker<*start || marker>*stop)
    marker = (*start+*stop)/2;
  return marker;
}

int Marker_Control::Set_Value(long value)

{
  if (value>=*start && value<=*stop)
  {
    marker = value;
    return 1;
  }
  else
  {
    if (value<*start)
      marker = *start;
    else
      marker = *stop;
    return 0;
  }
}

long Marker_Control::Get_Min(void)

{
  return *start;
}

long Marker_Control::Get_Max(void)

{
  return *stop;
}

Step_Size_Control::Step_Size_Control()

{
  step_size = 8000;
}

char *Step_Size_Control::Get_Name(void)

{
  return "Step";
}

char *Step_Size_Control::Get_Error_Message(void)

{
  return "Step Size out of range.";
}

long Step_Size_Control::Get_Value(void)

{
  return step_size;
}

int Step_Size_Control::Set_Value(long value)

{
  if (value>=Get_Min() && value <= Get_Max())
  {
    step_size = value;
    return 1;
  }
  else
    return 0;
}

long Step_Size_Control::Get_Min(void)

{
  return 1000l;
}

long Step_Size_Control::Get_Max(void)

{
  return 16000l;
}
