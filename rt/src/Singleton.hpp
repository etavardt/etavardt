#pragma once

class Singleton
{
public:
   template <class T>
   static T& getInstance() 
   {
      return T::getInstance();
   }

private:
   Singleton() { };
};