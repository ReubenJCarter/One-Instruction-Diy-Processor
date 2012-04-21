#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
#define ACCUMULATOR 0xFFF7
#define GPO1 0xFFF6
#define GPI1 0xFFF5
#define GPO2 0xFFF4
#define GPI2 0xFFF3
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
		   
		   for(int i = 0; i < fSize; i++)
		   {
		     data[i] = 0;
		   }
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

		 void DumpImage(string fFileName)
		 {
		   FILE* fptr = fopen(fFileName.c_str(), "w");
		   
		   if(fptr != 0)
		   {
		     for(unsigned int i = 0; i < size; i++)
			 {
			   fprintf(fptr, "%08X\n", data[i]);
			 }
		     fclose(fptr);
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
			  default : return 0;
		    }
		  }
		  
		  unsigned int HexStringToInt(string fStr)
		  {
		    unsigned int temp = 0;
			unsigned int factor = 1;
		    for( int i = (fStr.length() - 1); i >= 0; i--)
			{
			  temp += factor * HexCharToInt(fStr[i]);
			  factor *= 16;
			}
			return temp;
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
		   for(int i = 0; i < fSize; i++)
		   {
		     data[i] = 0;
		   }
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
		   
		   string tempStr ="";
		   unsigned int adressPointer = 0;
		   
		   for(unsigned int i = 0; i < romImage.length(); i++)
		   {
		
		     if(romImage[i] > 47 && romImage[i] < 58 || romImage[i] > 64 && romImage[i] < 91)
			 {
			   tempStr.clear();
			   tempStr += romImage[i];
			   i++;
			   
			   if(i < romImage.length())
			   {
			     while((romImage[i] > 47 && romImage[i] < 58 || romImage[i] > 64 && romImage[i] < 91) && i < romImage.length())
			     {
			       tempStr += romImage[i];
			       i++;
			     }
			   }
			   data[adressPointer] = HexStringToInt(tempStr);
			   adressPointer++;
			 }
			 
			 if(adressPointer > size)
			 {
			   break;
		     }
		   }
		 }
		 
		 void DumpImage(string fFileName)
		 {
		   FILE* fptr = fopen(fFileName.c_str(), "w");
		   
		   if(fptr != 0)
		   {
		     for(unsigned int i = 0; i < size; i++)
			 {
			   fprintf(fptr, "%08X\n", data[i]);
			 }
		     fclose(fptr);
		   }
		 }
};


class TTCore
{
  private:
          Rom rom;
		  Ram ram;
		  unsigned int programCounter;
		  unsigned int regA;
		  unsigned int regB;
		  unsigned int accumulator;
  		  unsigned int gpo1;
		  unsigned int gpo2;
		  unsigned int gpi1;
		  unsigned int gpi2;
		  
  public:
	     
		 TTCore()
		 {
		   Reset();
		 }
		 
		 void Reset()
		 {
		   rom.SetSize(ROM_UPPER - ROM_LOWER + 1);
		   ram.SetSize(RAM_UPPER - RAM_LOWER + 1);
		   programCounter = 0;
		   regA = 0;
		   regB = 0;
		   accumulator = 0;
		   gpo1 = 0;
		   gpo2 = 0;
		   gpi1 = 0;
		   gpi2 = 0;
		 }
		 
		 void WriteTo(unsigned int fAdress, unsigned int fData)
		 {
		   if(fAdress >= RAM_LOWER && fAdress <= RAM_UPPER)
		   {
		     ram.WriteTo(fAdress - RAM_LOWER, fData);
		   }
		   
		   switch(fAdress)
		   {
		     case PC: 
			   programCounter = fData;
			 
			 case PC_IF_CARRY:
			   if( (regA + regB) > 0xFFFF)
			     programCounter = fData; 
			   
			 case PC_IF_SIGN: 
			   if(accumulator >= 0x8000)
		         programCounter = fData;
			   
			 case PC_IF_ZERO: 
			   if(accumulator == 0)
			     programCounter = fData;
			   
			 case REG_B: 
			   regB = fData;
			 
			 case REG_A: 
			   regA = fData;
			   
			 case ACCUMULATOR:
			   accumulator = fData;
			 
			 case GPO1: 
			   gpo1 = fData;
			 
			 case GPO2: 
			   gpo2 = fData;
		   }
		 }
		 
		 unsigned int ReadFrom(unsigned int fAdress)
		 {
		   if(fAdress >= ROM_LOWER && fAdress <= ROM_UPPER)
		   {
		     return rom.ReadFrom(fAdress - ROM_LOWER);
		   }
		   
		   if(fAdress >= RAM_LOWER && fAdress <= RAM_UPPER)
		   {
		     return ram.ReadFrom(fAdress - RAM_LOWER);
		   }
		   
		   switch(fAdress)
		   {
			 case ADDER:
			   return regA + regB;
			   
			 case ADDER_INC: 
			   return regA + regB + 1;
			   
			 case GPI1: 
			   return gpi1;
			   
			 case GPI2:
			   return gpi2;
			   
			 case XOR_AB: 
			   return regA ^ regB;
			 
			 case OR_AB: 
			   return regA | regB;
			 
			 case AND_AB: 
			   return regA & regB;
			  
			 case NOT_A: 
			   return ~regA;
			   
			 case ACCUMULATOR:
			   return accumulator;
		   }
		 }
		 
		 void Cycle()
		 {
		   unsigned int adr1 = ReadFrom(programCounter);
		   programCounter++;
		   unsigned int adr2 = ReadFrom(programCounter);
		   programCounter++;
		   unsigned int value = ReadFrom(adr1);
		   WriteTo(adr2, value);
		 }
		 
		 void LoadRomImage(string fFileName)
		 {
		   rom.LoadImage(fFileName);
		 }
		 
		 unsigned int ReadGPO1()
		 {
		   return gpo1;
		 }
		 
		 unsigned int ReadGPO2()
		 {
		   return gpo2;
		 }
		 
		 void WriteGPI1(unsigned int fValue)
		 {
		   gpi1 = fValue;
		 }
		 
		 void WriteGPI2(unsigned int fValue)
		 {
		   gpi2 = fValue;
		 }
		 
		 void DumpRamImage(string fFileName)
		 {
		   ram.DumpImage(fFileName); 
		 }
		 void DumpRomImage(string fFileName)
		 {
		   rom.DumpImage(fFileName); 
		 }
};


int main(int argc, char *argv[])
{
  if(argc == 3)
  {
    TTCore core;
	core.LoadRomImage(argv[1]);
	
	for(int i = 0; i < atoi(argv[2]); i++)
	{
	  core.Cycle();
	}
	
	core.DumpRamImage("RamImage.im");
	core.DumpRomImage("RomImage.im");
	
	printf("Ram and rom images dumped\nGPO1 = %08X\nGPO2 = %08X\n", core.ReadGPO1(), core.ReadGPO2());
  }
  else 
  {
    printf("failed to find rom image file");
  }
  
  return 0;
}