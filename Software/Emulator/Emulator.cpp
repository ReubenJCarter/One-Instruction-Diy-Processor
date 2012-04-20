#include <stdio.h>
#include <string>

using namespace std;

//Predefined memory adressed of functional modules

//ALU MODULE
#define PC 0xFFFF
#define PC_IF_CARRY 0xFFFE
#define PC_IF_SIGN 0xFFFD
#define PC_IF_ZERO 0xFFFC
#define ADDER 0xFFFB
#define ADDER_INC 0xFFFA
#define REG_B 0xFFF9
#define REG_A 0xFFF8
#define GPO1 0xFFF7
#define GPI1 0xFFF6
#define GPO2 0xFFF5
#define GPI2 0xFFF4
#define XOR_AB 0xFFEF
#define OR_AB 0xFFEE
#define AND_AB 0xFFED
#define NOT_A 0xFFEC

//RAM MODULE
#define RAM_LOWER 0x0100
#define RAM_UPPER 0xFEFF

//ROM MODULE
#define ROM_LOWER 0x0000
#define ROM_UPPER 0x00FF


class Log
{    
  #define LOGNAME "log.txt"
  
  private:
          static int warningNumber;
          static int errorNumber;
          static int assertionPass;
          static int assertionFail;
           
          static std::string GetDateTimeString()
          {
            time_t rawtime;
            struct tm* timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            std::string str1 = asctime(timeinfo);
            return str1 + "--> ";
          }
          
  public:
         static void Clear()
         {
           FILE * logFile;
           logFile = fopen(LOGNAME, "w");
           fclose(logFile);
         }
         
         static void WriteWarning(std::string fData)
         {
            FILE * logFile;
            logFile = fopen(LOGNAME, "a"); 

            std::string temp = GetDateTimeString() + "WARNING:\n" + fData + "\n\n";
            
            if (logFile!=NULL)
            {
              fprintf(logFile, temp.c_str());
              fclose(logFile);
            }
             
            warningNumber++;
         }
         
         static void WriteError(std::string fData)
         {
            FILE * logFile;
            logFile = fopen(LOGNAME, "a"); 
            
            std::string temp = GetDateTimeString() + "ERROR:\n" + fData + "\n\n";
            
            if (logFile!=NULL)
            {
              fprintf(logFile, temp.c_str());
              fclose(logFile);
            }
            
            errorNumber++;
         }
         
         static void WriteInfo(std::string fData)
         {
            FILE * logFile;
            logFile = fopen(LOGNAME, "a"); 
            
            std::string temp = GetDateTimeString() + "INFO:\n" + fData + "\n\n";
            
            if (logFile!=NULL)
            {
              fprintf(logFile, temp.c_str());
              fclose(logFile);
            }
         }
         
         static void ProgramStart()
         {
            FILE * logFile;
            logFile = fopen(LOGNAME, "a"); 
            
            std::string temp = GetDateTimeString() + "-----Program Start--------------\n\n\n";
            
            if (logFile!=NULL)
            {
              fprintf(logFile, temp.c_str());
              fclose(logFile);
            }
         }
         
         static bool Assert(bool fCheck, std::string fTrue, std::string fFalse)
         {
           FILE * logFile;
           logFile = fopen(LOGNAME, "a"); 
      
           std::string temp = GetDateTimeString();
           
           if(fCheck)
           {
             temp += "ASSERTION PASS:\n" + fTrue;
             assertionPass++;
           }
           else 
           {
             temp += "ASSERTION FAIL:\n" + fFalse;
             assertionFail++;
           }
           
           temp += "\n\n";
           
           if (logFile!=NULL)
           {
             fprintf(logFile, temp.c_str());
             fclose(logFile);  
           }
           
           return fCheck;
         }
         
         static void ProgramEnd()
         {
            FILE * logFile;
            logFile = fopen(LOGNAME, "a"); 
            
            std::string temp = "\n" + GetDateTimeString() + "-----Program End--------------\n";
            
            if (logFile!=NULL)
            {
              fprintf(logFile, temp.c_str());
              fprintf(logFile, "Warnings: %d\n", warningNumber);
              fprintf(logFile, "Errors: %d\n", errorNumber);
              fprintf(logFile, "Assertions Failed: %d\n", assertionFail);
              fprintf(logFile, "Assertions Pass: %d\n", assertionPass);
              fclose(logFile);
            }
         }
         
         static int GetWarningNumber()
         {
           return warningNumber;
         }
         
         static int GetErrorNumber()
         {
           return warningNumber;
         }
};

int Log::warningNumber = 0;
int Log::errorNumber = 0;
int Log::assertionFail = 0;
int Log::assertionPass = 0;

class Alu
{
  private:
		  
  public:
		 
};

class Register
{
  private:
          
  public:
         
};

class Ram
{
  private:
		  unsigned int* data;
		  unsigned int size; 
		  
  public:
		 Ram()
		 {
		   data = 0;
		   size = 0;
		 }
		 
		 Ram(unsigned int fSize)
		 {
		   SetSize(fSize);
		 }
		 
		 void SetSize(unsigned int fSize)
		 {
		   size = fSize;
		   data = new unsigned int[fSize];
		 }
	
		 unsigned int ReadFrom(unsigned int fAdress)
		 {
		   if(fAdress < size)
		   {
		     return data[fAdress];
		   }
		   else
		   {
		     Log::WriteError("Ram adress went out of bounds on read");
		     return 0;
		   }
		 }
		 
		 void WriteTo(unsigned int fAdress, unsigned int fData)
		 {
		   if(fAdress < size)
		   {
		     data[fAdress] = fData;
		   }
		   else
		   {
		     Log::WriteError("Ram adress went out of bounds on write");
		   }
		 }
};

class Rom
{
  private:
		  unsigned int* data;
		  unsigned int size; 
		  
		  string LoadTextFile(string fFileName)
		  {
            FILE* fptr;
            fptr = fopen(fFileName.c_str(), "r");
		    string temp;
		    if(fptr != 0)
		    {
			  char c;
			  while( c != EOF )
			  {
			    c = fgetc(fptr);
			    temp += c;
			  }
			  fclose(fptr);
		    }
		    else 
		    {
			  Log::WriteError("Failed to open rom image file");
		    }
		    return temp;
		  }
		  
		  unsigned int HexCharToInt(char fChar)
		  {
            switch(fChar)
			{
			  case '0': return 0;
			  case '1': return 1;
			  case '2': return 2;
			  case '3': return 3;
			  case '4': return 4;
			  case '5': return 5;
			  case '6': return 6;
			  case '7': return 7;
			  case '8': return 8;
			  case '9': return 9;
			  case 'A': return 10;
			  case 'B': return 11;
			  case 'C': return 12;
			  case 'D': return 13;
			  case 'E': return 14;
			  case 'F': return 15;
		    }
		  }
		  
  public:
		 Rom()
		 {
		   data = 0;
		   size = 0;
		 }
		 
		 Rom(unsigned int fSize)
		 {
		   SetSize(fSize);
		 }
		 
		 void SetSize(unsigned int fSize)
		 {
		   size = fSize;
		   data = new unsigned int[fSize];
		 }
	
		 unsigned int ReadFrom(unsigned int fAdress)
		 {
		   if(fAdress < size)
		   {
		     return data[fAdress];
		   }
		   else
		   {
		     Log::WriteError("Rom adress went out of bounds on read");
		     return 0;
		   }
		 }
		 
		 void LoadImage(string fFileName)
		 {
		   string romImage = LoadTextFile(fFileName);
		   unsigned int adressPointer = 0;
		   bool counting = false;
		   unsigned int factor = 0;
		   unsigned int value;
		   for(int i = 0; i < romImage.length(); i++)
		   {
		     if(romImage[i] > 47 && romImage[i] < 58 || romImage[i] > 64 && romImage[i] < 71)
			 {
			   if(counting = false)
			   {
			     factor = 1;
				 value = factorTen * HexCharToInt(romImage[i]);
				 factor *= 16;
			   }
			   else
			   {
			     value = factorTen * HexCharToInt(romImage[i]);
				 factor *= 16;
			   } 
			   counting = true;
			 }
			 else
			 {
			   if(counting = true)
			   {
			     data[adressPointer] = value;
			     adressPointer++;
			   }
			   counting = false;
			 }
		   }
		 }
};

class GPIO
{
  private:
		  
  public:
         
};

class TTCore
{
  private:
          
  public:
	     
};

int main()
{
  
  return 0;
}