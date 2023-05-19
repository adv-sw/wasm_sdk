
#ifndef UTIL_H
#define UTIL_H
   
// Basic string processing as wasi sprintf currently broken.
// TODO: Ensure stays within bounds of supplied string.
void Append_Number(char *msg, size_t dest_size, uint32_t value)
{
   bool started = false;
   const uint32_t max_digits = 3;
   char *ptr = msg + strlen(msg);
   
               
   for (int32_t i=max_digits;i>=0;i--)
   {
      size_t column = pow(10, i);
      uint32_t digit = value / column;
       value -= column * digit;
         
      started |= (digit != 0);
         
      if ( (digit != 0) || started)
         *(ptr++) = '0' + digit;
   }
      
   if (!started)
      *(ptr++) = '0';
   
   // Terminate
   *ptr = '\0'; 
}


#endif // UTIL_H
