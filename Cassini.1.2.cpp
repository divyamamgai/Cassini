/*
	Author : Divya Mamgai
	License : The MIT License (MIT)
	Copyright (c) 2016 Divya Mamgai
	Version : Cassini.1.2
*/
#define _WIN32_WINNT 0x0500
#include<iostream>
#include<iomanip>
#include<fstream>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<windows.h>
#include<process.h>
#include<winuser.h>
#include<string>
#include<sstream>
#include<dirent.h>
#include<wininet.h>

#define IntegerType 1
#define DoubleType 2
#define StringType 3

#define MenuID_Main 1001
#define MenuID_Project 1011
#define MenuID_ProjectAddFiles 1012
#define MenuID_ProjectAddFilesManualConfirm 1013
#define MenuID_ProjectAddFilesDragNDrop 1014
#define MenuID_ProjectAddException 1015
#define MenuID_MaintainProject 1021
#define MenuID_MaintainProjectNoExist 10219
#define MenuID_PerformCompression 1022
#define MenuID_ManageExceptions 1023
#define MenuID_ManageFileByName 10231
#define MenuID_ManageFileByNameSelected 102311
#define MenuID_ManageFileByList 10232
#define MenuID_ManageProperties 1024
#define MenuID_DeleteProject 1025
#define MenuID_DeleteProjectDeleteFolder 10251
#define MenuID_DeleteProjectDeleteDatabase 10252
#define MenuID_DeleteProjectDone 10254
#define MenuID_CompressFolder 1031
#define MenuID_Settings 1041

using namespace std;

const char* ErrorLogFilePath = "resources\\error.txt";
const char* SettingsFilePath = "resources\\settings.bin";
const char* SettingsTempFilePath = "resources\\temp_settings.bin";
const char* ProjectsDataBaseFilePath = "resources\\projects.bin";
const char* ProjectsTempDataBaseFilePath = "resources\\temp_projects.bin";
const int NoOfSettings = 6;
const int NoOfVariables = 5;
const float FPS = 0.1;
const int TitleCharLen = 30;
const int FileNameCharLen = 255;
byte LinuxEOL = (byte)0x0A;
const double Version = 1.2;
const char* VersionString = "1.2";
string Title = "Cassini";
const int DefaultWidth = 700;
const int DefaultHeight = 380;
const int ButtonsColumnLimit = 5;
const int StringCharLen = 512;
const int MaxNoOfFiles = 512;

struct InputStruct {
    string String;
    int X;
    int Y;
    int Width;
    int Type;
    int Length;
    int NullValue;
    int SpaceAllowed;
    int SpecialChars;
    int FileAddressCharsOnly;
    string DefaultValue;
};

struct SliderInputStruct {
    int StartValue;
    int CommonDifference;
    int NoOfValues;
    int TextRepresentationFlag;
    int InputFieldWidth;
    string String;
    int InputSpace;
};

struct ScrollInputStruct
{
    short int Status;
    char Text[80];
    int AssociatedID;
    char AdditionalText[80];
};

struct ProjectStruct
{
    int CompressionLevel;
    int PNGCompression;
    int PNGCompressionLevel;
    int ZipAlign;
    int SignApk;
    int NoOfFiles;
    char Title[TitleCharLen];
};

struct ProjectFilePropertiesStruct
{
    int CompressionLevel;
    int PNGCompression;
    int PNGCompressionLevel;
    int ZipAlign;
    int SignApk;
    char FileName[FileNameCharLen];
    int ExceptionStatus;
};

struct button {
    short int status;
    int x;
    int y;
    char text[FileNameCharLen];
    void (*FunctionToExecute)();
    int ButtonReturnValue;
};

struct list {
    short int status;
    int x;
    int y;
    char text[FileNameCharLen];
    int ListReturnValue;
    int HighlightedFlag;
};

struct SettingsStruct {
    int SettingsID;
    int X;
    int Y;
    string Title;
};

int DefaultButton[2]= {0,0};
int PreviousMenuID = MenuID_Main;
int StringOutNoOfLines = 0;
int StringOutEndX = 0;
int StringOutEndY = 0;
int KeypressSoundID = 1;
int MenuGoBackFlag = 1;
int CurrentMenuID = 0;
int MaintainProjectSelectType = 0;
int CurrentProjectSelected = 0;
int PNGFileCount = 0;
int TempPNGFileCount = 0;
char CurrentProject[TitleCharLen];
char DataVariableCHAR[NoOfVariables];
int DataVariableINT[NoOfVariables];
double DataVariableDOUBLE[NoOfVariables];
string DataVariableSTRING[NoOfVariables];
string DummyTextRepresentationArray[100];
ProjectStruct *ProjectArray;
ProjectFilePropertiesStruct *ProjectFileArray,*FolderFileArray;
list *ProjectFileList;
list *ProjectList;
int DeleteProjectType = 0;

void Menu(int);
void Back();
void CleanUp();

char Strings[][StringCharLen]= {
    "Project - ``", // 0
    "How do you want to add files to your project?", // 1
    "Have you copied your files to your project folder?", // 2
    "Please Drag And Drop your folder containing APK files in this window or just start typing its full address.", // 3
    "Press Enter To Submit. [Pressing Enter while the address is empty will take you back.]", // 4
    "No. of APK files found - ``", // 5
    "Copying APK files... ``", // 6
    "Press any key to ``.", // 7
    "No APK files were found in the given folder path. Please check if there are any files in the folder path you provided.", // 8
    "No APK files were found in the project folder. Please check if youhave copied the files to the project folder or not.", // 9
    "The project with same name has been already created thus please try a different name for the project.", // 10
    "The project name you entered doesn't exist. To maintain a project you first need to create the project.", // 11
    "The APK file - \"``\" is not found in the project folder. Please make sure that the file you mentioned is available for access or exist.", // 12
    " - ``", // 13
    "Do you really want to ``?", // 14
    "What files do you want to alter?", // 15
    "Choose the method to select the Project you want to maintain.", // 16
    "Select the Project you want to maintain.", // 17
    "Project - `` has been successfully added to the database.", // 18
    "Select the Project file you want to manage.", // 19
    "Note: Project name should be unique and it is not case-sensitive, i.e., Rom is equivalent to ROM,rOM,roM or rom.", // 20
    "There are `` files in your Project. Do you want to start the compression process?", // 21
    "Do you really want to delete this project?", // 22
    "Do you also want to delete the project folder, containing original APK files?", // 23
    "Do you also want to delete this project's database file, containing information about all the APK files? [Note: In the future version(s) you will be able to back-up your database to re-use them, currently it is advised that you perform this deletion process.]", // 24
    "Project deletion process has been completed, press any key to continue.", // 25
    "There is no project yet created to be maintained. Please first create a project in order to maintain it.", // 26
    "Drag and Drop the folder which contains the APK files you want to compress.", // 27
    "Performing the compression of the given folder.", // 28
    "The compressed APK files are in this path - ``", // 29
    "Done, Press any key to ``" // 30
};

char HeaderString[][79]= {
    "",
    "",
    "   ###########%                                         Divya Mamgai",
    "   ###########%",
    "   ###%                                                 An App Compressor!",
    "   ###%      ",
    "   ###%         ########% ########% ########% ########% ##%   #% ########% ",
    "   ###%         ##%   ##% ##%       ##%          ##%    ###%  #%    ##%    ",
    "   ###%         ##%   ##% ########% ########%    ##%    #%##% #%    ##%    ",
    "   ###%         ########%       ##%       ##%    ##%    #% ##%#%    ##%    ",
    "   ###########% ##%   ##%       ##%       ##%    ##%    #%  ###%    ##%    ",
    "   ###########% ##%   ##% ########% ########% ########% #%   ##% ########% ",
};

void InitializeVariables() {
    for(int i=0; i<100; i++) DummyTextRepresentationArray[i] = "";
    for(int i=0; i<NoOfVariables; i++) {
        DataVariableCHAR[i] = ' ';
        DataVariableINT[i] = 0;
        DataVariableDOUBLE[i] = 0.0;
        DataVariableSTRING[i] = "";
    }
    PNGFileCount = 0;
    TempPNGFileCount = 0;
}

// Setting Variables And Fetching Functions

int SettingINTValue[NoOfSettings];
double SettingDOUBLEValue[NoOfSettings];
string SettingSTRINGValue[NoOfSettings];

int GetINTSetting(int SettingNo) {
    return SettingINTValue[SettingNo-1];
}

double GetDOUBLESetting(int SettingNo) {
    return SettingDOUBLEValue[SettingNo-1];
}

string GetSTRINGSetting(int SettingNo) {
    return SettingSTRINGValue[SettingNo-1];
}

class Functions {
public:
    void SetTitle(string AdditionalInfo,int Type = 1) {
        string ConsoleTitle = "";
        switch(Type) {
        case 1:
            ConsoleTitle = Title;
            ConsoleTitle+=".";
            ostringstream TitleVersionStream;
            TitleVersionStream<<Version;
            ConsoleTitle+=TitleVersionStream.str();
            ConsoleTitle+=" - ";
            ConsoleTitle+=AdditionalInfo;
            break;
        }
        SetConsoleTitle(ConsoleTitle.c_str());
    }
    void SetConsoleDimensions(int Width,int Height)
    {
        HWND Console = GetConsoleWindow();
        const HWND Desktop = GetDesktopWindow();
        RECT ConsoleRect,DesktopRect;
        GetWindowRect(Console, &ConsoleRect);
        GetWindowRect(Desktop, &DesktopRect);
        int ConsoleXCord,ConsoleYCord;
        int Screen_Width = DesktopRect.right,Screen_Height = DesktopRect.bottom;
        switch(GetINTSetting(6))
        {
        case 1:
            ConsoleXCord = (Screen_Width-Width)/2;
            ConsoleYCord = (Screen_Height-Height)/2;
            break;
        case 2:
            ConsoleXCord = 25;
            ConsoleYCord = 25;
            break;
        case 3:
            ConsoleXCord = Screen_Width-Width;
            ConsoleYCord = 25;
            break;
        case 4:
            ConsoleXCord = 25;
            ConsoleYCord = Screen_Height-Height-75;
            break;
        case 5:
            ConsoleXCord = Screen_Width-Width;
            ConsoleYCord = Screen_Height-Height-75;
            break;
        case 6:
            ConsoleXCord = ConsoleRect.left;
            ConsoleYCord = ConsoleRect.top;
            break;
        }
        MoveWindow(Console, ConsoleXCord, ConsoleYCord, Width, Height, TRUE);
    }
    void PauseConsole(float PauseTime)
    {
        Sleep((DWORD)PauseTime);
    }
    void ClearConsole(float PauseTime) {
        PauseConsole(PauseTime);
        system("cls");
        Header();
        SetConsoleDimensions(DefaultWidth,DefaultHeight-10);
        SetConsoleDimensions(DefaultWidth,DefaultHeight);
    }
    int StringLength(char* str) {
        int Length=0;
        for(int i=0; str[i]!='\0'; i++) Length++;
        return Length;
    }
    int StringLength(string str) {
        int Length=0;
        for(int i=0; str[i]!='\0'; i++) Length++;
        return Length;
    }
    WORD GetConsoleTextAttribute(HANDLE hCon) {
        CONSOLE_SCREEN_BUFFER_INFO con_info;
        GetConsoleScreenBufferInfo(hCon, &con_info);
        return con_info.wAttributes;
    }
    void ColorOut(char Char,int ColorType) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        const int saved_colors = GetConsoleTextAttribute(hConsole);
        switch(ColorType)
        {
        case 1:
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
            break;
        case 2:
            SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN );
            break;
        case 3:
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY );
            break;
        case 4:
            SetConsoleTextAttribute(hConsole, 0);
            break;
        }
        cout<<Char;
        SetConsoleTextAttribute(hConsole, saved_colors);
    }
    void ColorOut(string String,int ColorType,int Width = 0) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        const int saved_colors = GetConsoleTextAttribute(hConsole);
        switch(ColorType)
        {
        case 1:
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
            break;
        case 2:
            SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN );
            break;
        case 3:
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY );
            break;
        case 4:
            SetConsoleTextAttribute(hConsole, 0);
            break;
        }
        if(Width==0) {
            cout<<String;
        } else {
            if(Width<StringLength(String)) {
                for(int i=0; i<=(Width-3); i++) {
                    cout<<String[i];
                }
                cout<<"...";
            } else cout<<String;
        }
        SetConsoleTextAttribute(hConsole, saved_colors);
    }
    void ColorOut(char* String,int ColorType,int Width = 0) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        const int saved_colors = GetConsoleTextAttribute(hConsole);
        switch(ColorType)
        {
        case 1:
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
            break;
        case 2:
            SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN );
            break;
        case 3:
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY );
            break;
        case 4:
            SetConsoleTextAttribute(hConsole, 0);
            break;
        }
        if(Width==0) {
            cout<<String;
        } else {
            if(Width<StringLength(String)) {
                for(int i=0; i<=(Width-3); i++) {
                    cout<<String[i];
                }
                cout<<"...";
            } else cout<<String;
        }
        SetConsoleTextAttribute(hConsole, saved_colors);
    }
    void SetCord(short x,short y)
    {
        COORD coord = {x,y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
    }
    void CursorToggle(int Toggle)
    {
        HANDLE hOut;
        CONSOLE_CURSOR_INFO ConCurInf;
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        ConCurInf.dwSize = 10;
        switch(Toggle)
        {
        case 1:
            ConCurInf.bVisible = TRUE;
            break;
        case 0:
            ConCurInf.bVisible = FALSE;
            break;
        }
        SetConsoleCursorInfo(hOut, &ConCurInf);
    }
    string DecToBinary(int Number) {
        int Temp,_Temp,Count;
        char *String;
        Count = 0;
        String = (char*)malloc(32+1);
        if ( String == NULL )
            exit(EXIT_FAILURE);
        for (Temp=31; Temp>=0; Temp--) {
            _Temp=Number>>Temp;
            if ((_Temp)&(1)) *(String+Count)=1+'0';
            else *(String+Count)=0+'0';

            Count++;
        }
        *(String+Count)='\0';
        string Return=String;
        return  Return;
    }
    string BinToDecimal(int n) {
        double output = 0;
        for(int i=0; n>0; i++) {
            if(n%10 == 1) {
                output+=pow(2,i);
            }
            n/=10;
        }
        stringstream DecimalStringStream;
        DecimalStringStream<<output;
        string DecimalOutputString = DecimalStringStream.str();
        return DecimalOutputString;
    }
    string EncodeString(string String) {
        string EncodedString = "|";
        for(int i=0; i<StringLength(String); i++) {
            EncodedString+=DecToBinary(int(String[i]));
            if(i==(StringLength(String)-1)) EncodedString+="|";
            else EncodedString+=".";
        }
        return EncodedString;
    }
    string DecodeString(string String) {
        string DecodedString = "",DecodedStringLine,DecodedStringChar;
        stringstream DecodedStringStreamLine;
        DecodedStringStreamLine.str(String);
        while(getline(DecodedStringStreamLine,DecodedStringLine,'|')) {
            stringstream DecodedStringStreamChar;
            DecodedStringStreamChar.str(DecodedStringLine);
            while(getline(DecodedStringStreamChar,DecodedStringChar,'.')) {
                DecodedString+=char(atoi((BinToDecimal(atoi(DecodedStringChar.c_str()))).c_str()));
            }
        }
        return DecodedString;
    }
    int CreateButtons(button Button[][ButtonsColumnLimit],int NoRows,int DefaultButton[2],int FuncType,int ButtonType = 1)
    {

        int Count = 0;
        int NoCols = 0;
        for(int i=0; i<NoRows; i++)
        {
            int TempNoCols = 0;
            for(int j=0; j<ButtonsColumnLimit; j++)
            {
                if(Button[i][j].status==1) TempNoCols++;
            }
            if(TempNoCols>=NoCols) NoCols = TempNoCols;
        }
        int CurrentButton[2] = {DefaultButton[0],DefaultButton[1]};
        int PreviousButton[2] = {DefaultButton[0],DefaultButton[1]};
        int ButtonReturnValue;

        DWORD InputMode;
        INPUT_RECORD InputEvent;
        BOOL Flag = TRUE;
        HANDLE HSTDINHANDLE = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(HSTDINHANDLE,&InputMode);
        SetConsoleMode(HSTDINHANDLE,0);
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

        while(Flag)
        {
            if((WaitForSingleObject(HSTDINHANDLE,0)==WAIT_OBJECT_0)&&(Count!=0))
            {
                DWORD ConsoleInputCount;
                ReadConsoleInput(HSTDINHANDLE,&InputEvent,1,&ConsoleInputCount);
                if((InputEvent.EventType==KEY_EVENT)&&(InputEvent.Event.KeyEvent.bKeyDown))
                {
                    switch(InputEvent.Event.KeyEvent.wVirtualKeyCode)
                    {
                    case VK_RIGHT:
                        if(CurrentButton[1]<(NoCols-1))
                        {
                            if(Button[CurrentButton[0]][(CurrentButton[1]+1)].status) CurrentButton[1]++;
                        }
                        else if(CurrentButton[1]==(NoCols-1))
                        {
                            if(Button[CurrentButton[0]][0].status) CurrentButton[1] = 0;
                        }
                        break;
                    case VK_LEFT:
                        if(CurrentButton[1]<=(NoCols-1)&&CurrentButton[1]!=0)
                        {
                            if(Button[CurrentButton[0]][(CurrentButton[1]-1)].status) CurrentButton[1]--;
                        }
                        else if(CurrentButton[1]==0)
                        {
                            if(Button[CurrentButton[0]][(NoCols-1)].status) CurrentButton[1] = NoCols-1;
                        }
                        break;
                    case VK_UP:
                        if(CurrentButton[0]<=(NoRows-1)&&CurrentButton[0]>0)
                        {
                            if(Button[(CurrentButton[0]-1)][CurrentButton[1]].status) CurrentButton[0]--;
                        }
                        else if(CurrentButton[0]==0)
                        {
                            if(Button[(NoRows-1)][CurrentButton[1]].status) CurrentButton[0] = NoRows-1;
                        }
                        break;
                    case VK_DOWN:
                        if(CurrentButton[0]<(NoRows-1))
                        {
                            if(Button[(CurrentButton[0]+1)][CurrentButton[1]].status) CurrentButton[0]++;
                        }
                        else if(CurrentButton[0]==(NoRows-1))
                        {
                            if(Button[0][CurrentButton[1]].status) CurrentButton[0] = 0;
                        }
                        break;
                    case VK_RETURN:
                        switch(FuncType)
                        {
                        case 1:
                            Flag = FALSE;
                            KeypressSound();
                            SetConsoleMode(HSTDINHANDLE,InputMode);
                            Button[CurrentButton[0]][CurrentButton[1]].FunctionToExecute();
                            break;
                        case 2:
                            Flag = FALSE;
                            KeypressSound();
                            SetConsoleMode(HSTDINHANDLE,InputMode);
                            ButtonReturnValue = Button[CurrentButton[0]][CurrentButton[1]].ButtonReturnValue;
                            delete[] Button;
                            return ButtonReturnValue;
                            break;
                        }
                        break;
                    case VK_ESCAPE:
                        delete[] Button;
                        Back();
                        break;
                    }
                }

            }
            if((Count==0)&&(Flag))
            {
                for(int i=0; i<NoRows; i++)
                {
                    for(int j=0; j<NoCols; j++)
                    {
                        if(Button[i][j].status)
                        {
                            SetCord(Button[i][j].x,Button[i][j].y);
                            if(i==CurrentButton[0]&&j==CurrentButton[1]) {
                                if(ButtonType==2) cout<<" ";
                                ColorOut(Button[i][j].text,1);
                                switch(ButtonType) {
                                case 2:
                                    SetCord(Button[i][j].x-3,Button[i][j].y);
                                    ColorOut(244,3);
                                    ColorOut(245,3);
                                    break;
                                }
                            }
                            else {
                                ColorOut(Button[i][j].text,3);
                                switch(ButtonType) {
                                case 2:
                                    cout<<" ";
                                    SetCord(Button[i][j].x-3,Button[i][j].y);
                                    ColorOut(179,3);
                                    ColorOut(179,3);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else if((((PreviousButton[0]!=CurrentButton[0])||(PreviousButton[1]!=CurrentButton[1]))&&(Flag)))
            {
                SetCord(Button[CurrentButton[0]][CurrentButton[1]].x,Button[CurrentButton[0]][CurrentButton[1]].y);
                if(ButtonType==2) cout<<" ";
                ColorOut(Button[CurrentButton[0]][CurrentButton[1]].text,1);
                switch(ButtonType) {
                case 2:
                    SetCord(Button[CurrentButton[0]][CurrentButton[1]].x-3,Button[CurrentButton[0]][CurrentButton[1]].y);
                    ColorOut(244,3);
                    ColorOut(245,3);
                    break;
                }
                SetCord(Button[PreviousButton[0]][PreviousButton[1]].x,Button[PreviousButton[0]][PreviousButton[1]].y);
                ColorOut(Button[PreviousButton[0]][PreviousButton[1]].text,3);
                cout<<" ";
                switch(ButtonType) {
                case 2:
                    SetCord(Button[PreviousButton[0]][PreviousButton[1]].x-3,Button[PreviousButton[0]][PreviousButton[1]].y);
                    ColorOut(179,3);
                    ColorOut(179,3);
                    break;
                }
                PreviousButton[0] = CurrentButton[0];
                PreviousButton[1] = CurrentButton[1];
            }
            Count++;
        }
        return 0;
    }
    int CreateLists(list List[],int NoRows,int DefaultList,int Width,int ListType = 1)
    {

        int Count = 0;
        int CurrentList = DefaultList;
        int PreviousList = DefaultList;
        int ListReturnValue;

        DWORD InputMode;
        INPUT_RECORD InputEvent;
        BOOL Flag = TRUE;
        HANDLE HSTDINHANDLE = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(HSTDINHANDLE,&InputMode);
        SetConsoleMode(HSTDINHANDLE,0);
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

        while(Flag)
        {
            if((WaitForSingleObject(HSTDINHANDLE,0)==WAIT_OBJECT_0)&&(Count!=0))
            {
                DWORD ConsoleInputCount;
                ReadConsoleInput(HSTDINHANDLE,&InputEvent,1,&ConsoleInputCount);
                if((InputEvent.EventType==KEY_EVENT)&&(InputEvent.Event.KeyEvent.bKeyDown))
                {
                    switch(InputEvent.Event.KeyEvent.wVirtualKeyCode)
                    {
                    case VK_UP:
                        if(CurrentList<=(NoRows-1)&&CurrentList>0)
                        {
                            if(List[(CurrentList-1)].status) CurrentList--;
                        }
                        else if(CurrentList==0)
                        {
                            if(List[(NoRows-1)].status) CurrentList = NoRows-1;
                        }
                        break;
                    case VK_DOWN:
                        if(CurrentList<(NoRows-1))
                        {
                            if(List[(CurrentList+1)].status) CurrentList++;
                        }
                        else if(CurrentList==(NoRows-1))
                        {
                            if(List[0].status) CurrentList = 0;
                        }
                        break;
                    case VK_RETURN:
                        Flag = FALSE;
                        KeypressSound();
                        SetConsoleMode(HSTDINHANDLE,InputMode);
                        ListReturnValue = List[CurrentList].ListReturnValue;
                        delete[] List;
                        return ListReturnValue;
                        break;
                    case VK_ESCAPE:
                        delete[] List;
                        Back();
                        break;
                    }
                }

            }
            if((Count==0)&&(Flag))
            {
                for(int i=0; i<NoRows; i++) {
                    if(List[i].status)
                    {
                        SetCord(List[i].x,List[i].y);
                        if(i==CurrentList) {
                            if(ListType==2) cout<<" ";
                            ColorOut(List[i].text,1,Width);
                            switch(ListType) {
                            case 2:
                                SetCord(List[i].x-3,List[i].y);
                                ColorOut(244,3);
                                ColorOut(245,3);
                                break;
                            }
                        }
                        else {
                            if(List[i].HighlightedFlag==1) ColorOut(List[i].text,0,Width);
                            else ColorOut(List[i].text,3,Width);
                            switch(ListType) {
                            case 2:
                                cout<<" ";
                                SetCord(List[i].x-3,List[i].y);
                                ColorOut(179,3);
                                ColorOut(179,3);
                                break;
                            }
                        }
                    }
                }
            }
            else if((((PreviousList!=CurrentList))&&(Flag)))
            {
                SetCord(List[CurrentList].x,List[CurrentList].y);
                if(ListType==2) cout<<" ";
                ColorOut(List[CurrentList].text,1,Width);
                switch(ListType) {
                case 2:
                    SetCord(List[CurrentList].x-3,List[CurrentList].y);
                    ColorOut(244,3);
                    ColorOut(245,3);
                    break;
                }
                SetCord(List[PreviousList].x,List[PreviousList].y);
                if(List[PreviousList].HighlightedFlag==1) ColorOut(List[PreviousList].text,0,Width);
                else ColorOut(List[PreviousList].text,3,Width);
                cout<<" ";
                switch(ListType) {
                case 2:
                    SetCord(List[PreviousList].x-3,List[PreviousList].y);
                    ColorOut(179,3);
                    ColorOut(179,3);
                    break;
                }
                PreviousList = CurrentList;
            }
            Count++;
        }
        return 0;
    }
    void ClearFrame(short StartX,short StartY,int Width,int Height)
    {
        for(int i=0; i<=Height; ++i)
        {
            for(int j=0; j<=Width; ++j)
            {
                SetCord(StartX+j,StartY+i);
                cout<<" ";
            }
        }
        SetCord(StartX,StartY);
    }
    void Header() {
        for(int i=0; i<(int)(sizeof(HeaderString)/79); i++) {
            for(int j=0; j<79; j++) {
                char Character = (char)(HeaderString[i][j]);
                if(Character=='%') {
                    ColorOut('#',3);
                } else if(Character=='$') {
                    ColorOut('#',0);
                } else {
                    ColorOut(Character,1);
                }
            }
            cout<<"\n";
        }
        SetCord(71,2);
        ColorOut(VersionString,1);
    }
    int DecimalNumberCheck(double Number){
        stringstream StringStream;
        StringStream<<Number;
        if(StringStream.str().find(".")!=string::npos){
            return 1;
        }else return 0;
    }
    int StringDigitValidation(string TempString)
    {
        int DecimalPointCount = 0;
        for(int i=0; i<StringLength(TempString); i++)
        {
            if(TempString[i]=='.') DecimalPointCount++;
            if(DecimalPointCount>1) return 0;
            else
            {
                if(!isdigit(TempString[i])&&TempString[i]!='.') return 0;
            }
        }
        return 1;
    }
    void StringInterpret(string& String,int InterpretType = 1,string StringReplaceDefault = "")
    {
        size_t StartPosition = 0;
        while((StartPosition = String.find('`',StartPosition))!=string::npos)
        {
            int InterpretDelimiterFlag = 0;
            int TempPosition = StartPosition+1;
            while(TempPosition!=string::npos)
            {
                if(!(StringDigitValidation(String.substr(TempPosition,1))))
                {
                    if(String[TempPosition]=='`')
                    {
                        InterpretDelimiterFlag = 1;
                        break;
                    }
                }
                TempPosition++;
            }
            if(InterpretDelimiterFlag)
            {
                if((TempPosition==(StartPosition+1))&&(InterpretType==2))
                {
                    String.replace(StartPosition,2,StringReplaceDefault);
                    StartPosition+=StringReplaceDefault.length();
                }
                else if(InterpretType==1)
                {
                    string TempString = String.substr(StartPosition+1,TempPosition-StartPosition);
                    TempString = (char)(atoi(TempString.c_str()));
                    String.replace(StartPosition,(TempPosition-StartPosition+1),TempString);
                    StartPosition+=TempString.length();
                }
            }
            else StartPosition++;
        }
    }
    int FindSpacePos(string String,int StartPos,int Type) {
        int StringLengthInt = StringLength(String);
        if(StartPos>StringLengthInt) return -1;
        string TempStringFirstHalf = String.substr(0,StartPos);
        string TempStringSecondHalf = String.substr(StartPos,StringLengthInt-StartPos);
        int PosOfSpaceFirstHalf = 0;
        int PosOfSpaceSecondHalf = 0;
        int Return = -1;
        switch(Type) {
        case 1:
            for(int i=StringLength(TempStringFirstHalf)-1; i>=0; i--) {
                PosOfSpaceFirstHalf++;
                if(TempStringFirstHalf[i]==' ') {
                    Return = PosOfSpaceFirstHalf;
                    break;
                }
            }
            break;
        case 2:
            for(int i=0; i<=StringLength(TempStringSecondHalf); i++) {
                PosOfSpaceSecondHalf++;
                if(TempStringSecondHalf[i]==' ') {
                    Return = PosOfSpaceSecondHalf;
                    break;
                }
            }
            break;
        }
        return Return;
    }
    void StringOut(int StringID,int StartX,int StartY,int ParagraphWidth,int ColorType,string InterpretString)
    {
        string TempString = Strings[StringID];
        if(StringLength(InterpretString)!=0) StringInterpret(TempString,2,InterpretString);
        else StringInterpret(TempString,1,InterpretString);
        int StringLengthInt = StringLength(TempString);
        int NumberOfCharacters = 0;
        int CurrentChar = 0;
        int BreakOutType = 0;
        int CurrentRow = 0;
        while(NumberOfCharacters<StringLengthInt)
        {
            SetCord(StartX,CurrentRow+StartY);
            CurrentChar = 0;
            while((CurrentChar<=ParagraphWidth)&&(NumberOfCharacters<StringLengthInt)) {
                if((CurrentChar==ParagraphWidth)&&(TempString[CurrentChar]!=' ')) {
                    int Before = FindSpacePos(TempString,NumberOfCharacters,1);
                    int After = FindSpacePos(TempString,NumberOfCharacters,2);
                    if((Before!=1)&&(After!=1)) {
                        ClearFrame(StartX+CurrentChar-Before,CurrentRow+StartY,Before,0);
                        NumberOfCharacters = NumberOfCharacters - Before + 1;
                        CurrentChar = 0;
                    } else if(After==1) {
                        NumberOfCharacters++;
                    }
                    break;
                } else {
                    ColorOut(TempString[NumberOfCharacters],ColorType);
                }
                CurrentChar++;
                NumberOfCharacters++;
            }
            CurrentRow++;
        }
        StringOutNoOfLines = CurrentRow;
        StringOutEndX = CurrentChar+StartX;
        StringOutEndY = StartY+CurrentRow-1;
    }
    void StringOut(string String,int StartX,int StartY,int ParagraphWidth,int ColorType,string InterpretString)
    {
        string TempString = String;
        if(StringLength(InterpretString)) StringInterpret(TempString,2,InterpretString);
        else StringInterpret(TempString,1,InterpretString);
        int StringLengthInt = StringLength(TempString);
        int NumberOfCharacters = 0;
        int CurrentChar = 0;
        int BreakOutType = 0;
        int CurrentRow = 0;
        while(NumberOfCharacters<StringLengthInt)
        {
            SetCord(StartX,CurrentRow+StartY);
            CurrentChar = 0;
            while((CurrentChar<=ParagraphWidth)&&(NumberOfCharacters<StringLengthInt)) {
                if((CurrentChar==ParagraphWidth)&&(TempString[CurrentChar]!=' ')) {
                    int Before = FindSpacePos(TempString,NumberOfCharacters,1);
                    int After = FindSpacePos(TempString,NumberOfCharacters,2);
                    if((Before!=1)&&(After!=1)) {
                        ClearFrame(StartX+CurrentChar-Before,CurrentRow+StartY,Before,0);
                        NumberOfCharacters = NumberOfCharacters - Before + 1;
                        CurrentChar = 0;
                    } else if(After==1) {
                        NumberOfCharacters++;
                    }
                    break;
                } else {
                    ColorOut(TempString[NumberOfCharacters],ColorType);
                }
                CurrentChar++;
                NumberOfCharacters++;
            }
            CurrentRow++;
        }
        StringOutNoOfLines = CurrentRow;
        StringOutEndX = CurrentChar+StartX;
        StringOutEndY = StartY+CurrentRow-1;
    }
    void KeypressSound()
    {
        if(KeypressSoundID!=0)
        {
            // Beep(KeypressSoundID*100,200);
        }
    }
    int ScrollInput(short Type,short x,short y,ScrollInputStruct ScrollInput[],int NumberOfObjects,int StartID)
    {
        CursorToggle(0);
        if(ScrollInput[StartID-1].Status==0) StartID = 1;
        int DefaultButtonID[2] = {0,0};
        int CurrentID = StartID;
        int CoordinatesOfPreviousKey[2];
        int CoordinatesOfNextKey[2];
        int Count = 0;
        int PreviousID = StartID;
        string ScrollInputTextString,ScrollInputAdditionalTextString;
        DWORD ConsoleMode;
        INPUT_RECORD InputEvent;
        BOOL Flag = TRUE;
        HANDLE StandardInputHandle = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(StandardInputHandle,&ConsoleMode);
        SetConsoleMode(StandardInputHandle,0);
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));                 // To flush the non-required previous data.
        CoordinatesOfPreviousKey[0] = x;
        CoordinatesOfPreviousKey[1] = y;
        CoordinatesOfNextKey[0] = x + 2 + StringLength(ScrollInput[CurrentID-1].Text);
        CoordinatesOfNextKey[1] = y;
        ClearFrame(x,y,(CoordinatesOfNextKey[0]-CoordinatesOfPreviousKey[0]+1),0);
        while(Flag)
        {
            if((WaitForSingleObject(StandardInputHandle,0)==WAIT_OBJECT_0)&&(Count!=0))
            {
                DWORD ConsoleInputCount;
                ReadConsoleInput(StandardInputHandle,&InputEvent,1,&ConsoleInputCount);
                if((InputEvent.EventType==KEY_EVENT)&&(InputEvent.Event.KeyEvent.bKeyDown))
                {
                    switch(InputEvent.Event.KeyEvent.wVirtualKeyCode)
                    {
                    case VK_RIGHT:
                        SetCord(CoordinatesOfNextKey[0]+1,CoordinatesOfNextKey[1]);
                        ColorOut(char(16),1);
                        CurrentID++;
                        if(CurrentID>NumberOfObjects) CurrentID = 1;
                        break;
                    case VK_LEFT:
                        SetCord(CoordinatesOfPreviousKey[0],CoordinatesOfPreviousKey[1]);
                        ColorOut(char(17),1);
                        CurrentID--;
                        if(CurrentID<=0) CurrentID = NumberOfObjects;
                        break;
                    case VK_RETURN:
                        ScrollInputTextString = ScrollInput[CurrentID-1].Text;
                        ScrollInputAdditionalTextString = ScrollInput[CurrentID-1].AdditionalText;
                        StringInterpret(ScrollInputTextString,1);
                        StringInterpret(ScrollInputAdditionalTextString,1);
                        SetCord(CoordinatesOfPreviousKey[0]+2,CoordinatesOfPreviousKey[1]);
                        ColorOut(ScrollInputTextString,1);
                        KeypressSound();
                        PauseConsole(FPS*1000);
                        ClearFrame(x,y,(CoordinatesOfNextKey[0]-CoordinatesOfPreviousKey[0]+StringLength(ScrollInputAdditionalTextString)+2),0);
                        SetCord(x,y);
                        cout<<ScrollInputTextString<<" "<<ScrollInputAdditionalTextString;
                        SetConsoleMode(StandardInputHandle,ConsoleMode);
                        return ScrollInput[CurrentID-1].AssociatedID;
                        break;
                    case VK_ESCAPE:
                        Back();
                        break;
                    }
                }
            }
            if((PreviousID!=CurrentID)||Count==0)
            {
                PauseConsole(FPS*1000);
                ScrollInputTextString = ScrollInput[CurrentID-1].Text;
                ScrollInputAdditionalTextString = ScrollInput[CurrentID-1].AdditionalText;
                StringInterpret(ScrollInputTextString,1);
                StringInterpret(ScrollInputAdditionalTextString,1);
                ClearFrame(x,y,(CoordinatesOfNextKey[0]-CoordinatesOfPreviousKey[0]+StringLength(ScrollInput[PreviousID-1].AdditionalText)+2),0);
                ClearFrame(x,y,(CoordinatesOfNextKey[0]-CoordinatesOfPreviousKey[0]+StringLength(ScrollInputAdditionalTextString)+2),0);
                CoordinatesOfPreviousKey[0] = x;
                CoordinatesOfPreviousKey[1] = y;
                CoordinatesOfNextKey[0] = x + 2 + StringLength(ScrollInputTextString);
                CoordinatesOfNextKey[1] = y;
                SetCord(CoordinatesOfPreviousKey[0],CoordinatesOfPreviousKey[1]);
                cout<<char(17);
                SetCord(CoordinatesOfPreviousKey[0]+2,CoordinatesOfPreviousKey[1]);
                cout<<ScrollInputTextString;
                SetCord(CoordinatesOfNextKey[0],CoordinatesOfNextKey[1]);
                cout<<" "<<char(16);
                if(Type==2) {
                    cout<<" "<<ScrollInputAdditionalTextString;
                }
                PreviousID = CurrentID;
            }
            Count++;
        }
        return 0;
    }
    string InputData(InputStruct Input) {
        cin.clear();
        string InputDefaultValue = Input.DefaultValue;
        string InputGetString = InputDefaultValue;
        string InputString = Input.String;
        int CharCount = StringLength(InputGetString);
        int DecimalCount = 0;
        int DecimalPlace = 0;
        int InputXCord = Input.X;
        int InputYCord = Input.Y;
        int InitialXCordOfInput = InputXCord;
        int InitialYCordOfInput = InputYCord;
        int InputWidth = Input.Width;
        int InputType = Input.Type;
        int InputLength = Input.Length;
        int InputNullValue = Input.NullValue;
        int InputSpaceAllowed = Input.SpaceAllowed;
        int InputSpecialChars = Input.SpecialChars;
        int FileAddressCharsOnly = Input.FileAddressCharsOnly;
        for(int i=0; i<CharCount; i++) {
            if(InputGetString[i]=='.')DecimalCount++;
        }
        if((DecimalCount>1)&&(InputType==DoubleType)) return "Error, Occurred!";
        else {
            for(int i=0; i<CharCount; i++) {
                if(InputGetString[i]=='.')DecimalPlace=i;
            }
        }
        int k = 0,j = 0;
        for(int i=0; i<=StringLength(InputString); i++) {
            if(k==InputWidth) {
                k = 0;
                InitialYCordOfInput++;
                j++;
            }
            if(i==StringLength(InputString)) InitialXCordOfInput+=k-j;
            k++;
        }
        k = 0;
        j = 0;
        if((InitialXCordOfInput>(InputWidth+InputXCord))||(CharCount>InputLength)) return "Error, Occurred!";
        StringOut(InputString,InputXCord,InputYCord,InputWidth,0,"");
        StringOut(InputGetString,StringOutEndX,StringOutEndY,InputLength,0,"");
        int Count = 0;
InputModeLabel:
        DWORD ConsoleMode;
        INPUT_RECORD InputEvent;
        BOOL Flag = TRUE;
        HANDLE StandardInputHandle = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(StandardInputHandle,&ConsoleMode);
        SetConsoleMode(StandardInputHandle,0);
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));                 // To flush the non-required previous data.
        CursorToggle(1);
        while(Flag)
        {
            if((WaitForSingleObject(StandardInputHandle,0)==WAIT_OBJECT_0)&&(Count!=0))
            {
                DWORD ConsoleInputCount;
                ReadConsoleInput(StandardInputHandle,&InputEvent,1,&ConsoleInputCount);
                if((InputEvent.EventType==KEY_EVENT)&&(InputEvent.Event.KeyEvent.bKeyDown))
                {
                    if(CharCount>=InputLength) {
                        switch(InputEvent.Event.KeyEvent.wVirtualKeyCode) {
                        case VK_BACK:
                            CharCount--;
                            if(DecimalPlace==CharCount) DecimalCount = 0;
                            if(CharCount>=0) {
                                SetCord(InitialXCordOfInput+CharCount,InitialYCordOfInput);
                                cout<<" ";
                                InputGetString.erase(InputGetString.end()-1);
                            } else {
                                KeypressSound();
                                CharCount++;
                            }
                            SetCord(InitialXCordOfInput+CharCount,InitialYCordOfInput);
                            break;
                        case VK_DELETE:
                            ClearFrame(InitialXCordOfInput,InitialYCordOfInput,InputLength,0);
                            InputGetString.erase(InputGetString.begin(),InputGetString.end());
                            CharCount = 0;
                            break;
                        case VK_RETURN:
                            if((CharCount==0)&&(!InputNullValue)) {
                                KeypressSound();
                                break;
                            }
                            Flag = FALSE;
                            CursorToggle(0);
                            SetConsoleMode(StandardInputHandle,ConsoleMode);
                            return InputGetString;
                            break;
                        case VK_ESCAPE:
                            Back();
                            break;
                        default:
                            KeypressSound();
                            break;
                        }
                        goto InputModeLabel;
                    }
                    switch(InputType) {
                    case IntegerType:
                    case DoubleType:
                    case StringType:
                        switch(InputEvent.Event.KeyEvent.wVirtualKeyCode) {
                        case VK_BACK:
                            CharCount--;
                            if(DecimalPlace==CharCount) DecimalCount = 0;
                            if(CharCount>=0) {
                                SetCord(InitialXCordOfInput+CharCount,InitialYCordOfInput);
                                cout<<" ";
                                InputGetString.erase(InputGetString.end()-1);
                            } else {
                                KeypressSound();
                                CharCount++;
                            }
                            SetCord(InitialXCordOfInput+CharCount,InitialYCordOfInput);
                            break;
                        case VK_DELETE:
                            ClearFrame(InitialXCordOfInput,InitialYCordOfInput,InputLength,0);
                            InputGetString.erase(InputGetString.begin(),InputGetString.end());
                            CharCount = 0;
                            break;
                        case VK_RETURN:
                            if((CharCount==0)&&(!InputNullValue)) {
                                KeypressSound();
                                break;
                            }
                            Flag = FALSE;
                            CursorToggle(0);
                            SetConsoleMode(StandardInputHandle,ConsoleMode);
                            return InputGetString;
                            break;
                        case VK_ESCAPE:
                            Back();
                            break;
                        case 0x6E:
                        case 0xBE:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                if(DecimalCount||CharCount==InputLength-1) {
                                    KeypressSound();
                                    break;
                                }
                                DecimalCount++;
                                cout<<".";
                                InputGetString+=".";
                                CharCount++;
                                DecimalPlace = CharCount;
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    cout<<".";
                                    InputGetString+=".";
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_OEM_1:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                        if(!FileAddressCharsOnly){
                                            cout<<":";
                                            InputGetString+=":";
                                            CharCount++;
                                        }
                                    } else {
                                        cout<<";";
                                        InputGetString+=";";
                                        CharCount++;
                                    }
                                }
                                break;
                            }
                            break;
                        case VK_OEM_2:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if((InputSpecialChars)&&(!FileAddressCharsOnly)) {
                                    if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                        cout<<"?";
                                        InputGetString+="?";
                                    } else {
                                        cout<<"/";
                                        InputGetString+="/";
                                    }
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_MULTIPLY:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if((InputSpecialChars)&&(!FileAddressCharsOnly)) {
                                    cout<<"*";
                                    InputGetString+="*";
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_ADD:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    cout<<"+";
                                    InputGetString+="+";
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_OEM_PLUS:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                        cout<<"+";
                                        InputGetString+="+";
                                    } else {
                                        cout<<"=";
                                        InputGetString+="=";
                                    }
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_OEM_MINUS:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                        cout<<"_";
                                        InputGetString+="_";
                                    } else {
                                        cout<<"-";
                                        InputGetString+="-";
                                    }
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_SUBTRACT:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    cout<<"-";
                                    InputGetString+="-";
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_DIVIDE:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if((InputSpecialChars)&(!FileAddressCharsOnly)) {
                                    cout<<"/";
                                    InputGetString+="/";
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_OEM_3:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                        cout<<"~";
                                        InputGetString+="~";
                                    } else {
                                        cout<<"`";
                                        InputGetString+="`";
                                    }
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_OEM_4:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                        cout<<"{";
                                        InputGetString+="{";
                                    } else {
                                        cout<<"[";
                                        InputGetString+="[";
                                    }
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_OEM_5:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if((InputSpecialChars)&&(!FileAddressCharsOnly)) {
                                    if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                        cout<<"|";
                                        InputGetString+="|";
                                    } else {
                                        cout<<"\\";
                                        InputGetString+="\\";
                                    }
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_OEM_6:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                        cout<<"}";
                                        InputGetString+="}";
                                    } else {
                                        cout<<"]";
                                        InputGetString+="]";
                                    }
                                    CharCount++;
                                }
                                break;
                            }
                            break;
                        case VK_OEM_7:
                            switch(InputType) {
                            case IntegerType:
                                KeypressSound();
                                break;
                            case DoubleType:
                                KeypressSound();
                                break;
                            case StringType:
                                if(InputSpecialChars) {
                                    if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                        if(!FileAddressCharsOnly){
                                            cout<<"\"";
                                            InputGetString+="\"";
                                            CharCount++;
                                        }
                                    } else {
                                        cout<<"'";
                                        InputGetString+="'";
                                        CharCount++;
                                    }
                                }
                                break;
                            }
                            break;
                        case VK_SPACE:
                            if(InputSpaceAllowed) {
                                cout<<" ";
                                InputGetString+=" ";
                                CharCount++;
                            } else KeypressSound();
                            break;
                        case VK_NUMPAD0:
                        case 0x30:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                cout<<")";
                                InputGetString+=")";
                            } else {
                                cout<<"0";
                                InputGetString+="0";
                            }
                            CharCount++;
                            break;
                        case VK_NUMPAD1:
                        case 0x31:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                cout<<"!";
                                InputGetString+="!";
                            } else {
                                cout<<"1";
                                InputGetString+="1";
                            }
                            CharCount++;
                            break;
                        case VK_NUMPAD2:
                        case 0x32:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                cout<<"@";
                                InputGetString+="@";
                            } else {
                                cout<<"2";
                                InputGetString+="2";
                            }
                            CharCount++;
                            break;
                        case VK_NUMPAD3:
                        case 0x33:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                cout<<"#";
                                InputGetString+="#";
                            } else {
                                cout<<"3";
                                InputGetString+="3";
                            }
                            CharCount++;
                            break;
                        case VK_NUMPAD4:
                        case 0x34:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                cout<<"$";
                                InputGetString+="$";
                            } else {
                                cout<<"4";
                                InputGetString+="4";
                            }
                            CharCount++;
                            break;
                        case VK_NUMPAD5:
                        case 0x35:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                cout<<"%";
                                InputGetString+="%";
                            } else {
                                cout<<"5";
                                InputGetString+="5";
                            }
                            CharCount++;
                            break;
                        case VK_NUMPAD6:
                        case 0x36:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                cout<<"^";
                                InputGetString+="^";
                            } else {
                                cout<<"6";
                                InputGetString+="6";
                            }
                            CharCount++;
                            break;
                        case VK_NUMPAD7:
                        case 0x37:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                cout<<"&";
                                InputGetString+="&";
                            } else {
                                cout<<"7";
                                InputGetString+="7";
                            }
                            CharCount++;
                            break;
                        case VK_NUMPAD8:
                        case 0x38:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                if(!FileAddressCharsOnly){
                                    cout<<"*";
                                    InputGetString+="*";
                                    CharCount++;
                                }
                            } else {
                                cout<<"8";
                                InputGetString+="8";
                                CharCount++;
                            }
                            break;
                        case VK_NUMPAD9:
                        case 0x39:
                            if(((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT))&&(InputSpecialChars)) {
                                cout<<"(";
                                InputGetString+="(";
                            } else {
                                cout<<"9";
                                InputGetString+="9";
                            }
                            CharCount++;
                            break;
                        case 'A':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"A";
                                InputGetString+="A";
                            } else {
                                cout<<"a";
                                InputGetString+="a";
                            }
                            CharCount++;
                            break;
                        case 'B':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"B";
                                InputGetString+="B";
                            } else {
                                cout<<"b";
                                InputGetString+="b";
                            }
                            CharCount++;
                            break;
                        case 'C':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"C";
                                InputGetString+="C";
                            } else {
                                cout<<"c";
                                InputGetString+="c";
                            }
                            CharCount++;
                            break;
                        case 'D':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"D";
                                InputGetString+="D";
                            } else {
                                cout<<"d";
                                InputGetString+="d";
                            }
                            CharCount++;
                            break;
                        case 'E':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"E";
                                InputGetString+="E";
                            } else {
                                cout<<"e";
                                InputGetString+="e";
                            }
                            CharCount++;
                            break;
                        case 'F':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"F";
                                InputGetString+="F";
                            } else {
                                cout<<"f";
                                InputGetString+="f";
                            }
                            CharCount++;
                            break;
                        case 'G':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"G";
                                InputGetString+="G";
                            } else {
                                cout<<"g";
                                InputGetString+="g";
                            }
                            CharCount++;
                            break;
                        case 'H':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"H";
                                InputGetString+="H";
                            } else {
                                cout<<"h";
                                InputGetString+="h";
                            }
                            CharCount++;
                            break;
                        case 'I':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"I";
                                InputGetString+="I";
                            } else {
                                cout<<"i";
                                InputGetString+="i";
                            }
                            CharCount++;
                            break;
                        case 'J':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"J";
                                InputGetString+="J";
                            } else {
                                cout<<"j";
                                InputGetString+="j";
                            }
                            CharCount++;
                            break;
                        case 'K':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"K";
                                InputGetString+="K";
                            } else {
                                cout<<"k";
                                InputGetString+="k";
                            }
                            CharCount++;
                            break;
                        case 'L':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"L";
                                InputGetString+="L";
                            } else {
                                cout<<"l";
                                InputGetString+="l";
                            }
                            CharCount++;
                            break;
                        case 'M':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"M";
                                InputGetString+="M";
                            } else {
                                cout<<"m";
                                InputGetString+="m";
                            }
                            CharCount++;
                            break;
                        case 'N':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"N";
                                InputGetString+="N";
                            } else {
                                cout<<"n";
                                InputGetString+="n";
                            }
                            CharCount++;
                            break;
                        case 'O':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"O";
                                InputGetString+="O";
                            } else {
                                cout<<"o";
                                InputGetString+="o";
                            }
                            CharCount++;
                            break;
                        case 'P':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"P";
                                InputGetString+="P";
                            } else {
                                cout<<"p";
                                InputGetString+="p";
                            }
                            CharCount++;
                            break;
                        case 'Q':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"Q";
                                InputGetString+="Q";
                            } else {
                                cout<<"q";
                                InputGetString+="q";
                            }
                            CharCount++;
                            break;
                        case 'R':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"R";
                                InputGetString+="R";
                            } else {
                                cout<<"r";
                                InputGetString+="r";
                            }
                            CharCount++;
                            break;
                        case 'S':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"S";
                                InputGetString+="S";
                            } else {
                                cout<<"s";
                                InputGetString+="s";
                            }
                            CharCount++;
                            break;
                        case 'T':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"T";
                                InputGetString+="T";
                            } else {
                                cout<<"t";
                                InputGetString+="t";
                            }
                            CharCount++;
                            break;
                        case 'U':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"U";
                                InputGetString+="U";
                            } else {
                                cout<<"u";
                                InputGetString+="u";
                            }
                            CharCount++;
                            break;
                        case 'V':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"V";
                                InputGetString+="V";
                            } else {
                                cout<<"v";
                                InputGetString+="v";
                            }
                            CharCount++;
                            break;
                        case 'W':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"W";
                                InputGetString+="W";
                            } else {
                                cout<<"w";
                                InputGetString+="w";
                            }
                            CharCount++;
                            break;
                        case 'X':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"X";
                                InputGetString+="X";
                            } else {
                                cout<<"x";
                                InputGetString+="x";
                            }
                            CharCount++;
                            break;
                        case 'Y':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"Y";
                                InputGetString+="Y";
                            } else {
                                cout<<"y";
                                InputGetString+="y";
                            }
                            CharCount++;
                            break;
                        case 'Z':
                            if((InputType==IntegerType)||(InputType==DoubleType)) {
                                KeypressSound();
                                break;
                            }
                            if((GetKeyState(VK_CAPITAL) & 0x0001)!=0 || GetAsyncKeyState(VK_SHIFT)) {
                                cout<<"Z";
                                InputGetString+="Z";
                            } else {
                                cout<<"z";
                                InputGetString+="z";
                            }
                            CharCount++;
                            break;
                        default:
                            KeypressSound();
                            break;
                        }
                        break;
                    }
                }
            }
            Count++;
        }
        Flag = FALSE;
    }
    int NoOfDigits(int Number) {
        if(Number<0) return 1;
        int Digits = 0;
        while(Number) {
            Number/=10;
            Digits++;
        }
        return Digits;
    }
    int NoOfDigits(float Number) {
        int Digits = 0;
        stringstream StringStream;
        StringStream<<Number;
        Digits = StringLength(StringStream.str());
        return Digits;
    }
    int SliderInput(SliderInputStruct Input,int X,int Y,int DefaultValue) {
        cin.clear();
        int CommonDifference = Input.CommonDifference;
        int NoOfValues = Input.NoOfValues;
        int StartValue = Input.StartValue;
        string String = Input.String;
        stringstream TempStringStream;
        int TextRepresentationFlag = Input.TextRepresentationFlag;
        int InputStringLength = StringLength(String);
        int InputFieldWidth = Input.InputFieldWidth;
        int LargestValue = StartValue+(NoOfValues-1)*CommonDifference;
        int ValueViewWidth = NoOfDigits(LargestValue);
        int InputSliderWidth = InputFieldWidth-(7+ValueViewWidth);
        if(NoOfValues>InputSliderWidth) return -1;
        int InputSpace = Input.InputSpace;
        int Count = 0;
InputModeLabel:
        DWORD ConsoleMode;
        INPUT_RECORD InputEvent;
        BOOL Flag = TRUE;
        HANDLE StandardInputHandle = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(StandardInputHandle,&ConsoleMode);
        SetConsoleMode(StandardInputHandle,0);
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));                 // To flush the non-required previous data.
        CursorToggle(0);
        StringOut(String,X,Y,70,0,"");
        int SliderInputStartX = X+InputStringLength;
        for(int i=1; i<=InputFieldWidth; i++) {
            if(i==1) {
                SetCord(SliderInputStartX+(i-1),Y-1);
                ColorOut(218,3);
                SetCord(SliderInputStartX+(i-1),Y);
                ColorOut(179,3);
                SetCord(SliderInputStartX+(i-1),Y+1);
                ColorOut(192,3);
            } else if(i==(InputSliderWidth+4)) {
                SetCord(SliderInputStartX+(i-1),Y-1);
                ColorOut(194,3);
                SetCord(SliderInputStartX+(i-1),Y);
                ColorOut(179,3);
                SetCord(SliderInputStartX+(i-1),Y+1);
                ColorOut(193,3);
            } else if(i==InputFieldWidth) {
                SetCord(SliderInputStartX+(i-1),Y-1);
                ColorOut(191,3);
                SetCord(SliderInputStartX+(i-1),Y);
                ColorOut(179,3);
                SetCord(SliderInputStartX+(i-1),Y+1);
                ColorOut(217,3);
            } else {
                SetCord(SliderInputStartX+(i-1),Y-1);
                ColorOut(196,3);
                SetCord(SliderInputStartX+(i-1),Y+1);
                ColorOut(196,3);
            }
        }
        int CurrentSliderID = (DefaultValue/CommonDifference)+1;
        int PreviousSliderID = (DefaultValue/CommonDifference)+2;
        while(Flag)
        {
            if((WaitForSingleObject(StandardInputHandle,0)==WAIT_OBJECT_0)&&(Count!=0))
            {
                DWORD ConsoleInputCount;
                ReadConsoleInput(StandardInputHandle,&InputEvent,1,&ConsoleInputCount);
                if((InputEvent.EventType==KEY_EVENT)&&(InputEvent.Event.KeyEvent.bKeyDown))
                {
                    switch(InputEvent.Event.KeyEvent.wVirtualKeyCode) {
                    case VK_LEFT:
                        CurrentSliderID--;
                        if(CurrentSliderID==0) CurrentSliderID = 1;
                        break;
                    case VK_RIGHT:
                        if(CurrentSliderID==NoOfValues) CurrentSliderID = NoOfValues;
                        else CurrentSliderID++;
                        break;
                    case VK_RETURN:
                        TempStringStream<<(StartValue+(CurrentSliderID-1)*CommonDifference);
                        KeypressSound();
                        SetCord(SliderInputStartX+2+(CurrentSliderID-1)*InputSpace,Y);
                        ColorOut(219,1);
                        SetCord(SliderInputStartX+InputFieldWidth-(ValueViewWidth+2),Y);
                        ColorOut(TempStringStream.str(),1);
                        PauseConsole(FPS*1000);
                        SetCord(SliderInputStartX+2+(CurrentSliderID-1)*InputSpace,Y);
                        ColorOut(219,0);
                        SetCord(SliderInputStartX+InputFieldWidth-(ValueViewWidth+2),Y);
                        ColorOut(TempStringStream.str(),0);
                        SetConsoleMode(StandardInputHandle,ConsoleMode);
                        return (StartValue+(CurrentSliderID-1)*CommonDifference);
                        break;
                    case VK_ESCAPE:
                        Back();
                        break;
                    }
                }
            }
            if((Count==0)&&(Flag)) {
                for(int i=0; i<(InputSpace*(NoOfValues-1)); i++) {
                    SetCord(SliderInputStartX+i+2,Y);
                    ColorOut(196,3);
                }
                for(int i=0; i<NoOfValues; i++) {
                    SetCord(SliderInputStartX+i*InputSpace+2,Y);
                    ColorOut(196,3);
                }
                SetCord(SliderInputStartX+InputFieldWidth-(ValueViewWidth+2),Y);
                cout<<(StartValue+(CurrentSliderID-1)*CommonDifference);
            } else {
                if(PreviousSliderID!=CurrentSliderID) {
                    SetCord(SliderInputStartX+2+(CurrentSliderID-1)*InputSpace,Y);
                    ColorOut(219,0);
                    ClearFrame(SliderInputStartX+InputFieldWidth-(ValueViewWidth+2),Y,ValueViewWidth,0);
                    SetCord(SliderInputStartX+InputFieldWidth-(ValueViewWidth+2),Y);
                    cout<<(StartValue+(CurrentSliderID-1)*CommonDifference);
                    SetCord(SliderInputStartX+2+(PreviousSliderID-1)*InputSpace,Y);
                    ColorOut(196,3);
                    SetCord(SliderInputStartX+InputFieldWidth-(ValueViewWidth+4),Y);
                    ColorOut(179,3);
                    PreviousSliderID = CurrentSliderID;
                }
            }
            Count++;
        }
        Flag = FALSE;
    }
    void HeaderDesign(int Type,int X,int Y,int Width,int Color = 3) {
        switch(Type) {
        case 1:
            SetCord(X,Y);
            ColorOut(244,Color);
            ColorOut(245,Color);
            SetCord(X+1,Y-1);
            ColorOut(244,Color);
            for(int i=0; i<(Width-1); i++) {
                SetCord(X+2+i,Y-1);
                ColorOut(196,Color);
            }
            ColorOut(191,Color);
            SetCord(X+Width+1,Y);
            ColorOut(179,Color);
            for(int i=0; i<(Width); i++) {
                SetCord(X+1+i,Y+1);
                ColorOut(196,Color);
            }
            ColorOut(217,Color);
            break;
        case 2:
            SetCord(X,Y);
            ColorOut(244,Color);
            ColorOut(245,Color);
            SetCord(X+1,Y-1);
            ColorOut(244,Color);
            for(int i=0; i<(Width-1); i++) {
                SetCord(X+2+i,Y-1);
                ColorOut(196,Color);
            }
            ColorOut(191,Color);
            break;
        }
    }
    void InputFrame(int Y,int Height) {
        for(int i=Y; i<=(Y+Height); i++) {
            for(int j=0; j<80; j++) {
                SetCord(j,i);
                if((i==Y)||(i==(Y+Height))) {
                    cout<<"-";
                } else {
                    cout<<" ";
                }
            }
        }
    }
    void DrawVerticalLine(int X,int Y,int Height,int Color){
        for(int i=0;i<Height;i++){
            SetCord(X,Y+i);
            ColorOut(179,Color);
        }
    }
    void PercentageBar(int TotalItems,int CurrentItem,int X,int Y,int Width) {
        string Title = "Processing - ";
    	int LastX = 0;
        float Percentage = ((float)(static_cast<float>(CurrentItem)/static_cast<float>(TotalItems)))*100.0;
        int NoOfPercentageDigits = NoOfDigits(Percentage);
        int INTPercentage = int(Percentage);
        int NoOfINTPercentageDigits = NoOfDigits(INTPercentage);
        stringstream TitleProcess;
        TitleProcess<<floor(Percentage);
        Title+=TitleProcess.str();
        Title+="%";
        for(int i=Y; i<=(Y+2); i++) {
            for(int j=X; j<=(Width+X); j++) {
                SetCord(j,i);
                if(i==Y) {
                    if(j==X) ColorOut(char(218),3);
                    else if(j==(Width+X)) ColorOut(char(191),3);
                    else ColorOut(char(196),3);
                } else if(i==Y+2) {
                    if(j==X) ColorOut(char(192),3);
                    else if(j==(Width+X)) ColorOut(char(217),3);
                    else ColorOut(char(196),3);
                } else {
                    if((j==X)||(j==(Width+X))) ColorOut(char(186),3);
                }
            }
        }
        for(int i=1; i<=(((float)(static_cast<float>(CurrentItem)/static_cast<float>(TotalItems)))*(Width-1)); i++) {
            if(!((i>((Width/2)-5))&&(i<((Width/2)+3)))){
                SetCord(X+i,Y+1);
                ColorOut(char(219),3);
                LastX = i;
            }
        }
        SetCord(X+(Width/2)-5,Y+1);
        cout<<" "<<setprecision(3)<<Percentage;
        if((!DecimalNumberCheck(Percentage))&&(Percentage!=100)) cout<<".0";
        else cout<<" ";
        SetCord(X+(Width/2),Y+1);
        cout<<" % ";
        SetTitle(Title.c_str());
    }
    string CMD(const char* Command) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        const int saved_colors = GetConsoleTextAttribute(hConsole);
        SetConsoleTextAttribute(hConsole,0);
        FILE* Pipe = _popen(Command, "r");
        if (!Pipe) {
            SetConsoleTextAttribute(hConsole,saved_colors);
            return "ERROR";
        }
        char Buffer[128];
        string Output = "";
        while(!feof(Pipe)) {
            if(fgets(Buffer, 128, Pipe) != NULL)
                Output += Buffer;
        }
        _pclose(Pipe);
        SetConsoleTextAttribute(hConsole,saved_colors);
        return Output;
    }
    void _CMD(const char* Command) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        const int saved_colors = GetConsoleTextAttribute(hConsole);
        SetConsoleTextAttribute(hConsole,0);
        string CommandString = "";
        CommandString+=Command;
        CommandString+=" > nul";
        FILE* Pipe = _popen(CommandString.c_str(), "r");
        if (!Pipe) cout<<"ERROR";
        _pclose(Pipe);
        SetConsoleTextAttribute(hConsole,saved_colors);
    }
    int NoOfFiles(string Path,string FileType) {
        int Count = 0;
        DIR *Directory;
        struct dirent *DirectoryStructure;
        if ((Directory=opendir(Path.c_str()))!=NULL) {
            while((DirectoryStructure=readdir(Directory))!=NULL) {
                char* FileName = DirectoryStructure->d_name;
                if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                    if(StringLength(FileType)!=0) {
                        string FileTypeGet = FileName;
                        FileTypeGet = FileTypeGet.substr(StringLength(FileTypeGet)-StringLength(FileType));
                        if(!(strcmp(FileTypeGet.c_str(),FileType.c_str()))) Count++;
                    } else Count++;
                }
            }
            closedir(Directory);
        }
        return Count;
    }
    int CopyFiles(string From,string To,string FileType,int X,int Y,int Width) {
        int NoOfFilesFound = NoOfFiles(From,".apk");
        int Count = 0;
        DIR *Directory;
        struct dirent *DirectoryStructure;
        if ((Directory=opendir(From.c_str()))!=NULL) {
            while((DirectoryStructure=readdir(Directory))!=NULL) {
                char* FileName = DirectoryStructure->d_name;
                if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                    if(StringLength(FileType)!=0) {
                        string FileTypeGet = FileName;
                        FileTypeGet = FileTypeGet.substr(StringLength(FileTypeGet)-StringLength(FileType));
                        if(!(strcmp(FileTypeGet.c_str(),FileType.c_str()))) {
                            string CopyFileCMD = "copy \"";
                            CopyFileCMD+=From;
                            CopyFileCMD+="\\";
                            CopyFileCMD+=FileName;
                            CopyFileCMD+="\" \"";
                            CopyFileCMD+=To;
                            CopyFileCMD+="\"";
                            CMD(CopyFileCMD.c_str());
                            Count++;
                            PercentageBar(NoOfFilesFound,Count,X,Y,Width);
                        }
                    }
                }
            }
            closedir(Directory);
        }
        if(Count==NoOfFilesFound) return 1;
        else return 0;
    }
    int FileSize(const char Path[],int Type = 1) {
        int Size = 0;
        fstream File;
        switch(Type) {
        case 1:
            File.open(Path,ios::in);
            break;
        case 2:
            File.open(Path,ios::in|ios::binary);
            break;
        }
        File.seekg(0,ios::end);
        Size = File.tellg();
        File.close();
        return Size;
    }
    int DeleteFiles(string From,string FileType,int X,int Y,int Width){
        int NoOfFilesFound = NoOfFiles(From,".apk");
        int Count = 0;
        DIR *Directory;
        struct dirent *DirectoryStructure;
        if ((Directory=opendir(From.c_str()))!=NULL) {
            while((DirectoryStructure=readdir(Directory))!=NULL) {
                char* FileName = DirectoryStructure->d_name;
                if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                    if(StringLength(FileType)!=0) {
                        string FileTypeGet = FileName;
                        FileTypeGet = FileTypeGet.substr(StringLength(FileTypeGet)-StringLength(FileType));
                        if(!(strcmp(FileTypeGet.c_str(),FileType.c_str()))) {
                            string FilePath = From;
                            FilePath+="\\";
                            FilePath+=FileName;
                            if(!remove(FilePath.c_str())) Count++;
                            PercentageBar(NoOfFilesFound,Count,X,Y,Width);
                        }
                    }
                }
            }
            closedir(Directory);
        }
        if(Count==NoOfFilesFound) return 1;
        else return 0;
    }
    long double FolderSize(string DirectoryPath,string FileType){
        long double Size = 0.0;
        int Count = 0;
        DIR *Directory;
        struct dirent *DirectoryStructure;
        if ((Directory=opendir(DirectoryPath.c_str()))!=NULL) {
            while((DirectoryStructure=readdir(Directory))!=NULL) {
                char* FileName = DirectoryStructure->d_name;
                if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                    if(StringLength(FileType)!=0) {
                        string FileTypeGet = FileName;
                        FileTypeGet = FileTypeGet.substr(StringLength(FileTypeGet)-StringLength(FileType));
                        if(!(strcmp(FileTypeGet.c_str(),FileType.c_str()))) {
                            string FilePath = DirectoryPath;
                            FilePath+="\\";
                            FilePath+=FileName;
                            Size+=FileSize(FilePath.c_str())/1024;
                        }
                    }
                }
            }
            closedir(Directory);
        }
        return Size;
    }
    int GetCurrentDate(int DateType)
    {
        time_t CurrentTime = time(0);
        struct tm CurrentTimeStruct;
        char CurrentTimeCharArray[80];
        CurrentTimeStruct = *localtime(&CurrentTime);
        switch(DateType)
        {
        case 1:
            strftime(CurrentTimeCharArray,sizeof(CurrentTimeCharArray),"%y",&CurrentTimeStruct);
            return atoi(CurrentTimeCharArray);
            break;
        case 2:
            strftime(CurrentTimeCharArray,sizeof(CurrentTimeCharArray),"%m",&CurrentTimeStruct);
            return atoi(CurrentTimeCharArray);
            break;
        case 3:
            strftime(CurrentTimeCharArray,sizeof(CurrentTimeCharArray),"%d",&CurrentTimeStruct);
            return atoi(CurrentTimeCharArray);
            break;
        case 4:
            strftime(CurrentTimeCharArray,sizeof(CurrentTimeCharArray),"%H",&CurrentTimeStruct);
            return atoi(CurrentTimeCharArray);
            break;
        case 5:
            strftime(CurrentTimeCharArray,sizeof(CurrentTimeCharArray),"%M",&CurrentTimeStruct);
            return atoi(CurrentTimeCharArray);
            break;
        case 6:
            strftime(CurrentTimeCharArray,sizeof(CurrentTimeCharArray),"%S",&CurrentTimeStruct);
            return atoi(CurrentTimeCharArray);
            break;
        }
    }
} Function;

class SettingsClass {
public:
    void ResetSettings() {
        ofstream SettingsFile(SettingsFilePath);
        SettingsFile<<"CompressionLevel#9|PNGCompression#1|PNGCompressionLevel#90|ZipAlignStatus#1|SignApkStatus#1|WindowsPosition#1|";
        SettingsFile.close();
    }
    void SettingInitialize() {
        string SettingsString;
        ifstream SettingsFile(SettingsFilePath,ios::binary);
        if(SettingsFile.is_open())
        {
            int SettingType = 1;
            while(!(SettingsFile.eof()))
            {
                getline(SettingsFile,SettingsString);
                if((Function.StringLength(SettingsString)==0)&&(SettingType==1))
                {
                    ResetSettings();
                }
                else
                {
                    string IndividualSetting;
                    stringstream SettingsStream;
                    SettingsStream.str(SettingsString);
                    int SettingID = 0;
                    while(getline(SettingsStream,IndividualSetting,'|'))
                    {
                        string SettingsValue;
                        stringstream SettingsValueStream(IndividualSetting);
                        int SettingValueID = 0;
                        while(getline(SettingsValueStream,SettingsValue,'#'))
                        {
                            if(SettingValueID==1) {
                                switch(SettingType) {
                                case 1:
                                    SettingINTValue[SettingID] = atoi(SettingsValue.c_str());
                                    break;
                                case 2:
                                    SettingDOUBLEValue[SettingID] = atof(SettingsValue.c_str());
                                    break;
                                case 3:
                                    SettingSTRINGValue[SettingID] = SettingsValue.c_str();
                                    break;
                                }
                            }
                            SettingValueID++;
                        }
                        SettingID++;
                    }
                }
                SettingType++;
            }
        }
        else
        {
            ResetSettings();
        }
        SettingsFile.close();
    }
    int UpdateSetting(int UpdateSettingID,int UpdateSettingType){
        string SettingsString;
        ifstream SettingsFile(SettingsFilePath,ios::binary);
        fstream SettingsTempFile(SettingsTempFilePath,ios::out|ios::binary);
        if((SettingsFile.is_open())&&(SettingsTempFile.is_open()))
        {
            int SettingType = 1;
            while(!(SettingsFile.eof()))
            {
                getline(SettingsFile,SettingsString);
                if(Function.StringLength(SettingsString)==0)
                {
                    ResetSettings();
                }
                else
                {
                    string IndividualSetting;
                    stringstream SettingsStream;
                    SettingsStream.str(SettingsString);
                    int SettingID = 0;
                    while(getline(SettingsStream,IndividualSetting,'|'))
                    {
                        string SettingsValue;
                        stringstream SettingsValueStream(IndividualSetting);
                        int SettingValueID = 0;
                        while(getline(SettingsValueStream,SettingsValue,'#'))
                        {
                            if(SettingValueID==0){
                                SettingsTempFile<<SettingsValue.c_str()<<"#";
                            }else if(SettingValueID==1) {
                                if((UpdateSettingType==SettingType)&&(SettingID==(UpdateSettingID-1))){
                                    switch(SettingType) {
                                    case 1:
                                        SettingsTempFile<<DataVariableINT[0]<<"|";
                                        break;
                                    case 2:
                                        SettingsTempFile<<DataVariableDOUBLE[0]<<"|";
                                        break;
                                    case 3:
                                        SettingsTempFile<<DataVariableSTRING[0]<<"|";
                                        break;
                                    }
                                }else{
                                    SettingsTempFile<<SettingsValue.c_str()<<"|";
                                }
                            }
                            SettingValueID++;
                        }
                        SettingID++;
                    }
                }
                SettingsTempFile<<"\n";
                SettingType++;
            }
        }
        else
        {
            ResetSettings();
        }
        SettingsFile.close();
        SettingsTempFile.close();
        if(!remove(SettingsFilePath)) {
            if(!rename(SettingsTempFilePath,SettingsFilePath)) {
                SettingInitialize();
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }
    SettingsClass() {
        SettingInitialize();
    }
}SettingsObject;

// Prototype
void DummyFunction();
void MenuDesign(int);
void Menu(int);
void Exit();
void Project();
void MaintainProject();
void SettingDisplay(SettingsStruct SettingElement,int Color);
void SettingsChanger(SettingsStruct SettingElement);
void SettingsCall();
void Settings(int StartID);
void CompressFolder();
int CheckProjectExist();
ProjectStruct GetCurrentProject();
int GetCompressionLevelID(int CompressionLevel);
ProjectFilePropertiesStruct GetCurrentFileProperties();
int SearchFile(const char* FileName);
void UpdateProject();
void UpdateProjectDatabase(int Type);
void UpdateProjectDatabaseByFile();
void AddProjectToDataBase();
void AddFilesManual();
void AddFilesManualYes();
void AddFiles();
void CreateProjectFileDatabase();
void UpdateProjectDatabase();
void BuildProject();
void DeleteProject();
void DeleteProjectYes();
void DeleteProjectNo();
void GenerateProjectArray();
void GenerateProjectFileArray();
void PerformCompression();
void PerformCompressionConfirmYes();
void PerformCompressFolder();
void InitializePNGFileCount(const char* DirectoryPath);
void CompressPNG(const char* DirectoryPath,string Quality);
void CompressFile(ProjectFilePropertiesStruct File,int CurrentCompressionID,int NoOfFiles,int Type = 1);
void MaintainProjectButtonList();
void ManageExceptions();
void ManageProperties();
void MaintainProjectBack();
void ManageFileByName();
void ManageFileByList();
void UpdateProjectPropertiesConfirmYes();
void UpdateProjectPropertiesConfirmNo();
void UpdateProjectFilesPropertiesConfirmYes();
void UpdateProjectFilesPropertiesConfirmNo();
int NoOfProjects();
void GenerateProjectList();
void GenerateProjectFileList();
void MaintainProjectSelectType1();
void MaintainProjectSelectType2();

void DummyFunction() {
    // This function doesn't do anything.
}

class ErrorReport {
public:
    fstream ErrorFile;
    void PostError(string Title,string Data,int ErrorID) {
        ErrorFile.open(ErrorLogFilePath,ios::out);
        ErrorFile.close();
    }
} Error;

void MenuDesign(int Height = 15) {
    for(int i=0; i<=Height; i++) {
        Function.SetCord(5,12+i);
        Function.ColorOut(179,3);
    }
    for(int i=0; i<=Height; i++) {
        Function.SetCord(6,12+i);
        Function.ColorOut(179,3);
    }
    Function.SetCord(0,0);
}

// Important Variables.
        
SettingsStruct SettingsArray[NoOfSettings] = {
    {1,6,15,"Compression Level: "},
    {2,6,18,"PNG Compression: "},
    {3,6,21,"PNG Comp. Quality: "},
    {4,6,24,"Zip Align Status: "},
    {5,6,27,"Sign Apk Status: "},
    {6,6,30,"Window Position: "}
};

button MainMenuButtons[][ButtonsColumnLimit]= {
    {1,8,16,"Create a Project",Project},
    {1,8,18,"Maintain a Project",MaintainProject},
    {1,8,20,"Compress APK files in a folder",CompressFolder},
    {1,8,22,"Settings",SettingsCall},
    {1,8,24,"Exit",Exit}
};
InputStruct ProjectName= {"Enter the Name of the Project: ",9,15,50,StringType,TitleCharLen,0,1,0,1,""};
InputStruct FileName= {"Enter the Name of the File: ",9,18,50,StringType,TitleCharLen,0,1,1,1,""};
ScrollInputStruct ProjectApkCompression[6]= {
    {1,"Store Mode",0,"[Don't compress at all.]"},
    {1,"Fastest Mode",1,"[Low compression.]"},
    {1,"Fast Mode",3,"[Fast compression mode.]"},
    {1,"Normal Mode",5,"[Normal compression mode.]"},
    {1,"Maximum Mode",7,"[Maximum compression mode.]"},
    {1,"Ultra Mode",9,"[Ultra compression mode.]"}
};
ScrollInputStruct WindowPosition[6]= {
    {1,"Center",1,""},
    {1,"Top Left",2,""},
    {1,"Top Right",3,""},
    {1,"Bottom Left",4,""},
    {1,"Bottom Right",5,""},
    {1,"Manual",6,""}
};
ScrollInputStruct ProjectPNGCompressionConfirm[2]= {
    {1,"Off",0,"[PNG files will not be compressed"},
    {1,"On",1,"[PNG files will be compressed]"}
};
SliderInputStruct ProjectPNGCompression= {0,5,21,0,51,"PNG Comp. Quality: ",2};
ScrollInputStruct ProjectZipAlign[2]= {
    {1,"No",0,"[APK will not be optimized]"},
    {1,"Yes",1,"[APK will be optimized]"}
};
ScrollInputStruct ProjectSign[2]= {
    {1,"No",0,"[APK will not be signed]"},
    {1,"Yes",1,"[APK will be signed]"}
};
button ProjectAddFilesButton[][ButtonsColumnLimit]= {
    {1,8,20,"Manually copy files to the Project folder.",AddFilesManual},
    {1,8,22,"Drag'n drop folder containing files.",AddFiles},
    {1,8,24,"Back",Back}
};
button ProjectAddFilesManualConfirmButton[][ButtonsColumnLimit]= {
    {1,10,20,"Yes, I have copied files to the folder and want to proceed.",AddFilesManualYes},
    {1,10,22,"No, I want to try Drag'n Drop.",Back}
};
button MaintainProjectSelectTypeButton[][ButtonsColumnLimit]= {
    {1,8,18,"Select Project by Name",MaintainProjectSelectType1},
    {1,8,20,"Select Project by List",MaintainProjectSelectType2},
    {1,8,22,"Back",Back}
};
button MaintainProjectButtons[][ButtonsColumnLimit]= {
    {1,8,18,"Perform Compression",PerformCompression},
    {1,8,20,"Manage Exceptions",ManageExceptions},
    {1,8,22,"Manage Properties",ManageProperties},
    {1,8,24,"Delete Project",DeleteProject},
    {1,8,26,"Back",MaintainProjectBack}
};
button ManageExceptionsButtons[][ButtonsColumnLimit]= {
    {1,8,18,"Manage by Name",ManageFileByName},
    {1,8,20,"Manage by List",ManageFileByList},
    {1,8,22,"Back",Back}
};
button UpdateProjectPropertiesConfirm[][ButtonsColumnLimit]= {
    {1,8,31,"Yes, I confirm the changes.",UpdateProjectPropertiesConfirmYes},
    {1,8,33,"No, I don't want to alter the properties.",UpdateProjectPropertiesConfirmNo}
};
button UpdateProjectFilesPropertiesConfirm[][ButtonsColumnLimit]= {
    {1,8,31,"Do not alter the properties of exceptions.",UpdateProjectFilesPropertiesConfirmYes},
    {1,8,33,"Alter properties of all the files.",UpdateProjectFilesPropertiesConfirmNo}
};
button PerformCompressionConfirm[][ButtonsColumnLimit]= {
    {1,8,21,"Yes, Start the process now.",PerformCompressionConfirmYes},
    {1,8,23,"No, I changed my mind.",Back}
};
button DeleteProjectConfirm[][ButtonsColumnLimit]= {
    {1,8,24,"Yes, Perform deletion process.",DeleteProjectYes},
    {1,8,26,"No, I changed my mind.",DeleteProjectNo}
};

void Menu(int MenuID) {
    CurrentMenuID = MenuID;
    Function.CursorToggle(0);
    switch(MenuID) {
    case MenuID_Main:
        Function.SetTitle("Main Menu");
        Function.ClearConsole(0);
        MenuDesign();
        Function.CreateButtons(MainMenuButtons,5,DefaultButton,1,2);
        break;
    case MenuID_Project:
        Function.SetTitle("Create Project");
        PreviousMenuID = MenuID_Main;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        DataVariableSTRING[0] = Function.InputData(ProjectName);
        if(CheckProjectExist()) {
            Function.HeaderDesign(2,6,18,70);
            Function.StringOut(10,9,18,65,0,"");
            Function.StringOut(20,9,21,65,1,"");
            Function.StringOut(7,9,24,65,0,"retry");
            getch();
            Menu(MenuID_Project);
        } else {
            Function.HeaderDesign(1,6,17,70);
            Function.StringOut("Compression Level:",9,17,50,0,"");
            DataVariableINT[0] = Function.ScrollInput(2,28,17,ProjectApkCompression,6,1);
            Function.HeaderDesign(1,6,19,70);
            Function.StringOut("PNG Compression:",9,19,50,0,"");
            DataVariableINT[1] = Function.ScrollInput(2,26,19,ProjectPNGCompressionConfirm,2,2);
            Function.HeaderDesign(1,6,21,70);
            DataVariableINT[2] = Function.SliderInput(ProjectPNGCompression,9,21,0);
            Function.HeaderDesign(1,6,23,70);
            Function.StringOut("Zip Alignment: ",9,23,50,0,"");
            DataVariableINT[3] = Function.ScrollInput(2,24,23,ProjectZipAlign,2,1);
            Function.HeaderDesign(1,6,25,70);
            Function.StringOut("Sign APK: ",9,25,50,0,"");
            DataVariableINT[4] = Function.ScrollInput(2,19,25,ProjectSign,2,1);
            BuildProject();
        }
        break;
    case MenuID_ProjectAddFiles:
        Function.SetTitle("Create Project - Add Files");
        PreviousMenuID = MenuID_Project;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(1,9,18,65,0,"");
        Function.CreateButtons(ProjectAddFilesButton,3,DefaultButton,1,2);
        break;
    case MenuID_ProjectAddFilesManualConfirm:
        Function.SetTitle("Create Project - Add Files Manually");
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(2,9,18,65,0,"");
        Function.CreateButtons(ProjectAddFilesManualConfirmButton,2,DefaultButton,1,1);
        break;
    case MenuID_ProjectAddFilesDragNDrop:
        Function.SetTitle("Create Project - Add Files By Drag And Drop");
        PreviousMenuID = MenuID_ProjectAddFiles;
        Function.ClearConsole(0);
        Function.SetConsoleDimensions(DefaultWidth,DefaultHeight+20);
        MenuDesign(16);
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(3,9,18,65,0,"");
        Function.InputFrame(20,5);
        Function.StringOut(4,9,26,65,0,"");
        Function.SetCord(1,21);
        getline(cin,DataVariableSTRING[1]);
        break;
    case MenuID_MaintainProject:
        Function.SetTitle("Maintain Project");
        PreviousMenuID = MenuID_Main;
        Function.ClearConsole(0);
        MenuDesign();
        if(!CurrentProjectSelected) {
            if(!MaintainProjectSelectType) {
                Function.HeaderDesign(1,6,15,70);
                Function.StringOut(16,9,15,65,1,"");
                Function.CreateButtons(MaintainProjectSelectTypeButton,3,DefaultButton,1,2);
                Menu(MenuID_MaintainProject);
            } else {
                switch(MaintainProjectSelectType) {
                case 1:
                    PreviousMenuID = MenuID_MaintainProject;
                    Function.HeaderDesign(1,6,15,70);
                    DataVariableSTRING[0] = Function.InputData(ProjectName);
                    if(CheckProjectExist()) {
                        for(int i=0; i<=Function.StringLength(DataVariableSTRING[0]); i++) {
                            CurrentProject[i]=DataVariableSTRING[0][i];
                        }
                        CurrentProjectSelected = 1;
                        GenerateProjectFileArray();
                        Menu(MenuID_MaintainProject);
                    } else {
                        Function.HeaderDesign(2,6,17,70);
                        Function.StringOut(11,9,17,65,0,"");
                        Function.StringOut(7,9,20,65,0,"retry");
                        getch();
                        Menu(MenuID_MaintainProject);
                    }
                    break;
                case 2:
                    PreviousMenuID = MenuID_MaintainProject;
                    Function.HeaderDesign(1,6,15,70);
                    Function.StringOut(17,9,15,65,1,"");
                    GenerateProjectList();
                    break;
                }
            }
        } else {
            Function.HeaderDesign(1,6,15,70);
            Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
            Function.CreateButtons(MaintainProjectButtons,5,DefaultButton,1,2);
        }
        break;
    case MenuID_MaintainProjectNoExist:
        Function.SetTitle("Maintain Project - Error!");
        PreviousMenuID = MenuID_MaintainProject;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(2,6,15,70);
        Function.StringOut(26,9,15,65,0,"");
        Function.StringOut(7,9,18,65,0,"go back");
        getch();
        Menu(MenuID_Main);
        break;
    case MenuID_ManageExceptions:
        Function.SetTitle("Maintain Project - Manage Exceptions");
        PreviousMenuID = MenuID_MaintainProject;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.CreateButtons(ManageExceptionsButtons,3,DefaultButton,1,2);
        break;
    case MenuID_ManageProperties:
        Function.SetTitle("Maintain Project - Manage Properties");
        PreviousMenuID = MenuID_MaintainProject;
        Function.ClearConsole(0);
        Function.SetConsoleDimensions(DefaultWidth,DefaultHeight+90);
        MenuDesign(24);
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(1,6,17,70);
        Function.StringOut("Compression Level:",9,17,50,0,"");
        DataVariableINT[0] = Function.ScrollInput(2,28,17,ProjectApkCompression,6,GetCompressionLevelID(GetCurrentProject().CompressionLevel));
        Function.HeaderDesign(1,6,19,70);
        Function.StringOut("PNG Compression:",9,19,50,0,"");
        DataVariableINT[1] = Function.ScrollInput(2,26,19,ProjectPNGCompressionConfirm,2,GetCurrentProject().PNGCompression+1);
        Function.HeaderDesign(1,6,21,70);
        DataVariableINT[2] = Function.SliderInput(ProjectPNGCompression,9,21,GetCurrentProject().PNGCompressionLevel);
        Function.HeaderDesign(1,6,23,70);
        Function.StringOut("Zip Alignment: ",9,23,50,0,"");
        DataVariableINT[3] = Function.ScrollInput(2,24,23,ProjectZipAlign,2,(GetCurrentProject().ZipAlign+1));
        Function.HeaderDesign(1,6,25,70);
        Function.StringOut("Sign APK: ",9,25,50,0,"");
        DataVariableINT[4] = Function.ScrollInput(2,19,25,ProjectSign,2,(GetCurrentProject().SignApk+1));
        DataVariableINT[5] = GetCurrentProject().NoOfFiles;
        Function.HeaderDesign(1,6,28,70);
        Function.StringOut(14,9,28,50,1,"confirm the changes");
        Function.CreateButtons(UpdateProjectPropertiesConfirm,2,DefaultButton,1,2);
        Menu(MenuID_MaintainProject);
        break;
    case MenuID_ManageFileByName:
        Function.SetTitle("Maintain Project - Manage Exceptions");
        PreviousMenuID = MenuID_ManageExceptions;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(1,6,18,70);
        DataVariableSTRING[1] = Function.InputData(FileName);
        if(SearchFile(DataVariableSTRING[1].c_str())) {
            Menu(MenuID_ManageFileByNameSelected);
        } else {
            Function.StringOut(12,9,21,65,0,DataVariableSTRING[1]);
            Function.StringOut(7,9,25,65,0,"go back");
            getch();
            Menu(MenuID_ManageExceptions);
        }
        break;
    case MenuID_ManageFileByNameSelected:
        Function.SetTitle("Maintain Project - Manage Exceptions");
        Function.ClearConsole(0);
        Function.SetConsoleDimensions(DefaultWidth,DefaultHeight+40);
        MenuDesign(19);
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.StringOut(13,19+Function.StringLength(DataVariableSTRING[0]),15,(55-Function.StringLength(DataVariableSTRING[0])),1,DataVariableSTRING[1]);
        Function.HeaderDesign(1,6,18,70);
        Function.StringOut("Compression Level:",9,18,50,0,"");
        DataVariableINT[0] = Function.ScrollInput(2,28,18,ProjectApkCompression,6,GetCompressionLevelID(GetCurrentFileProperties().CompressionLevel));
        Function.HeaderDesign(1,6,20,70);
        Function.StringOut("PNG Compression:",9,20,50,0,"");
        DataVariableINT[1] = Function.ScrollInput(2,26,20,ProjectPNGCompressionConfirm,2,GetCurrentProject().PNGCompression+1);
        Function.HeaderDesign(1,6,22,70);
        DataVariableINT[2] = Function.SliderInput(ProjectPNGCompression,9,22,GetCurrentFileProperties().PNGCompressionLevel);
        Function.HeaderDesign(1,6,24,70);
        Function.StringOut("Zip Alignment: ",9,24,50,0,"");
        DataVariableINT[3] = Function.ScrollInput(2,24,24,ProjectZipAlign,2,(GetCurrentFileProperties().ZipAlign+1));
        Function.HeaderDesign(1,6,26,70);
        Function.StringOut("Sign APK: ",9,26,50,0,"");
        DataVariableINT[4] = Function.ScrollInput(2,19,26,ProjectSign,2,(GetCurrentFileProperties().SignApk+1));
        UpdateProjectDatabaseByFile();
        Menu(MenuID_ManageExceptions);
        break;
    case MenuID_ManageFileByList:
        Function.SetTitle("Maintain Project - Manage Exceptions");
        PreviousMenuID = MenuID_ManageExceptions;
        Function.ClearConsole(0);
        Function.SetConsoleDimensions(DefaultWidth,DefaultHeight+(GetCurrentProject().NoOfFiles-4)*24);
        MenuDesign(10+GetCurrentProject().NoOfFiles*2);
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(1,6,17,70);
        Function.StringOut(19,9,17,65,0,"");
        GenerateProjectFileList();
        break;
    case MenuID_DeleteProject:
        Function.SetTitle("Maintain Project - Delete Project");
        PreviousMenuID = MenuID_MaintainProject;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(22,9,18,65,0,"");
        DeleteProjectType = 1;
        Function.CreateButtons(DeleteProjectConfirm,2,DefaultButton,1,2);
        break;
    case MenuID_DeleteProjectDeleteFolder:
        Function.SetTitle("Maintain Project - Delete Project Folder");
        PreviousMenuID = MenuID_MaintainProject;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(23,9,18,65,0,"");
        DeleteProjectType = 2;
        Function.CreateButtons(DeleteProjectConfirm,2,DefaultButton,1,2);
        break;
    case MenuID_DeleteProjectDeleteDatabase:
        Function.SetTitle("Maintain Project - Delete Project Database");
        PreviousMenuID = MenuID_MaintainProject;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(24,9,18,65,0,"");
        DeleteProjectType = 3;
        Function.CreateButtons(DeleteProjectConfirm,2,DefaultButton,1,2);
        break;
    case MenuID_DeleteProjectDone:
        Function.SetTitle("Maintain Project - Delete Project");
        PreviousMenuID = MenuID_MaintainProject;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(25,9,18,65,0,"");
        DeleteProjectType = 0;
        getch();
        GenerateProjectArray();
        MaintainProjectBack();
        break;
    case MenuID_CompressFolder:
        Function.SetTitle("Compress Folder");
        PreviousMenuID = MenuID_Main;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(2,6,15,70);
        Function.StringOut(27,9,15,65,0,"");
        Function.InputFrame(18,5);
        Function.StringOut(4,9,25,65,0,"");
        Function.SetCord(1,19);
        getline(cin,DataVariableSTRING[0]);
        PerformCompressFolder();
        break;
    case MenuID_PerformCompression:
        Function.SetTitle("Maintain Project - Perform Compression");
        PreviousMenuID = MenuID_MaintainProject;
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        stringstream NoOfFilesFoundStringStream;
        NoOfFilesFoundStringStream<<GetCurrentProject().NoOfFiles;
        string NoOfFilesFoundString = NoOfFilesFoundStringStream.str();
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(21,9,18,65,0,NoOfFilesFoundString);
        Function.CreateButtons(PerformCompressionConfirm,2,DefaultButton,1,2);
        break;
    }
}

void Exit() {
    CleanUp();
    exit(0);
}

void Back() {
    if(MenuGoBackFlag) {
        if(CurrentMenuID!=MenuID_MaintainProject) {
            if(CurrentMenuID==MenuID_Main) {
                Function.KeypressSound();
                Exit();
            } else {
                Function.KeypressSound();
                Menu(PreviousMenuID);
            }
        } else MaintainProjectBack();
    }
}

void CleanUp() {
    int _NoOfProjects = NoOfProjects();
    int Count = 0;
    int Flag = 0;
    DIR *Directory;
    struct dirent *DirectoryStructure;
    if ((Directory=opendir("projects\\"))!=NULL) {
        while((DirectoryStructure=readdir(Directory))!=NULL) {
            char* FileName = DirectoryStructure->d_name;
            if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                string FileTypeGet = FileName;
                for(int i=0; i<_NoOfProjects; i++) {
                    if(!strcmp(FileName,ProjectArray[i].Title)) {
                        Flag = 1;
                        break;
                    }
                }
                if(Flag==0) {
                    string FilePath = "projects\\";
                    FilePath+=FileName;
                    RemoveDirectory(FilePath.c_str());
                } else Flag = 0;
            }
        }
        closedir(Directory);
    }
    Count = 0;
    Flag = 0;
    if ((Directory=opendir("data\\"))!=NULL) {
        while((DirectoryStructure=readdir(Directory))!=NULL) {
            char* FileName = DirectoryStructure->d_name;
            if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                string _FileName = FileName;
                for(int i=0; i<_NoOfProjects; i++) {
                    if(!strcmp((_FileName.substr(0,Function.StringLength(_FileName)-4)).c_str(),ProjectArray[i].Title)) {
                        Flag = 1;
                        break;
                    }
                }
                if(Flag==0) {
                    string FilePath = "data\\";
                    FilePath+=FileName;
                    remove(FilePath.c_str());
                } else Flag = 0;
            }
        }
        closedir(Directory);
    }
}

void Project() {
    PreviousMenuID = MenuID_Main;
    Menu(MenuID_Project);
}

void MaintainProject() {
    if(NoOfProjects()!=0) Menu(MenuID_MaintainProject);
    else Menu(MenuID_MaintainProjectNoExist);
}

void CompressFolder() {
    Menu(MenuID_CompressFolder);
}
/*
struct SettingsStruct {
    int SettingsID;
    int X;
    int Y;
    char Title[TitleCharLen];
    void (*SettingsFunction)();
}
*/

void SettingDisplay(SettingsStruct SettingElement,int Color){
    Function.SetCord(SettingElement.X+Function.StringLength(SettingElement.Title)+3,SettingElement.Y);
    stringstream StringStream;
    switch(SettingElement.SettingsID){
        case 1:
            switch(GetINTSetting(SettingElement.SettingsID)){
                case 0:
                    Function.ColorOut("Store Mode [Don't compress at all.]",Color);
                    break;
                case 1:
                    Function.ColorOut("Fastest Mode [Low compression.]",Color);
                    break;
                case 3:
                    Function.ColorOut("Fast Mode [Fast compression mode.]",Color);
                    break;
                case 5:
                    Function.ColorOut("Normal Mode [Normal compression mode.]",Color);
                    break;
                case 7:
                    Function.ColorOut("Maximum Mode [Maximum compression mode.]",Color);
                    break;
                case 9:
                    Function.ColorOut("Ultra Mode [Ultra compression mode.]",Color);
                    break;
            }
            break;
        case 2:
            switch(GetINTSetting(SettingElement.SettingsID)){
                case 0:
                    Function.ColorOut("Off [PNG files will not be compressed]",Color);
                    break;
                case 1:
                    Function.ColorOut("On [PNG files will be compressed]",Color);
                    break;
            }
            break;
        case 3:
            StringStream<<GetINTSetting(SettingElement.SettingsID);
            Function.ColorOut(StringStream.str(),Color);
            Function.ColorOut(" %",Color);
            StringStream.str(string());
            StringStream.clear();
            break;
        case 4:
            switch(GetINTSetting(SettingElement.SettingsID)){
                case 0:
                    Function.ColorOut("No [APK will not be optimized]",Color);
                    break;
                case 1:
                    Function.ColorOut("Yes [APK will be optimized]",Color);
                    break;
            }
            break;
        case 5:
            switch(GetINTSetting(SettingElement.SettingsID)){
                case 0:
                    Function.ColorOut("No [APK will not be signed]",Color);
                    break;
                case 1:
                    Function.ColorOut("Yes [APK will be signed]",Color);
                    break;
            }
            break;
        case 6:
            switch(GetINTSetting(SettingElement.SettingsID)){
                case 1:
                    Function.ColorOut("Center",Color);
                    break;
                case 2:
                    Function.ColorOut("Top Left",Color);
                    break;
                case 3:
                    Function.ColorOut("Top Right",Color);
                    break;
                case 4:
                    Function.ColorOut("Bottom Left",Color);
                    break;
                case 5:
                    Function.ColorOut("Bottom Right",Color);
                    break;
                case 6:
                    Function.ColorOut("Manual",Color);
                    break;
            }
            break;
    }
}

void SettingsChanger(SettingsStruct SettingElement){
    switch(SettingElement.SettingsID){
        case 1:
            DataVariableINT[0] = Function.ScrollInput(2,SettingElement.X+Function.StringLength(SettingElement.Title)+3,SettingElement.Y,ProjectApkCompression,6,GetCompressionLevelID(GetINTSetting(SettingElement.SettingsID)));
            break;
        case 2:
            DataVariableINT[0] = Function.ScrollInput(2,SettingElement.X+Function.StringLength(SettingElement.Title)+3,SettingElement.Y,ProjectPNGCompressionConfirm,2,GetINTSetting(SettingElement.SettingsID)+1);
            break;
        case 3:
            Function.ClearFrame(SettingElement.X+Function.StringLength(SettingElement.Title)+3,SettingElement.Y,4,0);
            DataVariableINT[0] = Function.SliderInput(ProjectPNGCompression,SettingElement.X+3,SettingElement.Y,GetINTSetting(SettingElement.SettingsID));
            break;
        case 4:
            DataVariableINT[0] = Function.ScrollInput(2,SettingElement.X+Function.StringLength(SettingElement.Title)+3,SettingElement.Y,ProjectZipAlign,2,GetINTSetting(SettingElement.SettingsID)+1);
            break;
        case 5:
            DataVariableINT[0] = Function.ScrollInput(2,SettingElement.X+Function.StringLength(SettingElement.Title)+3,SettingElement.Y,ProjectSign,2,GetINTSetting(SettingElement.SettingsID)+1);
            break;
        case 6:
            DataVariableINT[0] = Function.ScrollInput(2,SettingElement.X+Function.StringLength(SettingElement.Title)+3,SettingElement.Y,WindowPosition,6,GetINTSetting(SettingElement.SettingsID));
            break;
    }
    if(!SettingsObject.UpdateSetting(SettingElement.SettingsID,1)) cout<<"Error!";
}

void SettingsCall(){
    Settings(0);
}

void Settings(int StartID) {
    CurrentMenuID = MenuID_Settings;
    Function.SetTitle("Settings");
    PreviousMenuID = MenuID_Main;
    Function.ClearConsole(0);
    Function.SetConsoleDimensions(DefaultWidth,DefaultHeight+60);
    MenuDesign(20);

    int Count = 0;
    int CurrentSetting = StartID;
    int PreviousSetting = StartID;

    DWORD InputMode;
    INPUT_RECORD InputEvent;
    BOOL Flag = TRUE;
    HANDLE HSTDINHANDLE = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(HSTDINHANDLE,&InputMode);
    SetConsoleMode(HSTDINHANDLE,0);
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    while(Flag)
    {
        if((WaitForSingleObject(HSTDINHANDLE,0)==WAIT_OBJECT_0)&&(Count!=0))
        {
            DWORD ConsoleInputCount;
            ReadConsoleInput(HSTDINHANDLE,&InputEvent,1,&ConsoleInputCount);
            if((InputEvent.EventType==KEY_EVENT)&&(InputEvent.Event.KeyEvent.bKeyDown))
            {
                switch(InputEvent.Event.KeyEvent.wVirtualKeyCode)
                {
                case VK_UP:
                    if(CurrentSetting<=(NoOfSettings-1)&&CurrentSetting>0)
                    {
                        CurrentSetting--;
                    }
                    else if(CurrentSetting==0)
                    {
                        CurrentSetting = NoOfSettings-1;
                    }
                    break;
                case VK_DOWN:
                    if(CurrentSetting<(NoOfSettings-1))
                    {
                        CurrentSetting++;
                    }
                    else if(CurrentSetting==(NoOfSettings-1))
                    {
                        CurrentSetting = 0;
                    }
                    break;
                case VK_RETURN:
                    Flag = FALSE;
                    Function.KeypressSound();
                    SetConsoleMode(HSTDINHANDLE,InputMode);
                    SettingsChanger(SettingsArray[CurrentSetting]);
                    Settings(CurrentSetting);
                    break;
                case VK_ESCAPE:
                    Back();
                    break;
                }
            }

        }
        if((Count==0)&&(Flag))
        {
            for(int i=0;i<NoOfSettings;i++){
                if(i==CurrentSetting){
                    Function.HeaderDesign(1,SettingsArray[i].X,SettingsArray[i].Y,70,0);
                    Function.StringOut(SettingsArray[i].Title,SettingsArray[i].X+3,SettingsArray[i].Y,50,1,"");
                    SettingDisplay(SettingsArray[i],1);
                    Function.DrawVerticalLine(SettingsArray[i].X,SettingsArray[i].Y+1,2,0);
                }else{
                    Function.HeaderDesign(1,SettingsArray[i].X,SettingsArray[i].Y,70,3);
                    Function.StringOut(SettingsArray[i].Title,SettingsArray[i].X+3,SettingsArray[i].Y,50,0,"");
                    SettingDisplay(SettingsArray[i],0);
                    Function.DrawVerticalLine(SettingsArray[i].X,SettingsArray[i].Y+1,2,3);
                }
            }
        }
        else if(((PreviousSetting!=CurrentSetting)&&(Flag)))
        {
            Function.HeaderDesign(1,SettingsArray[CurrentSetting].X,SettingsArray[CurrentSetting].Y,70,0);
            Function.StringOut(SettingsArray[CurrentSetting].Title,SettingsArray[CurrentSetting].X+3,SettingsArray[CurrentSetting].Y,50,1,"");
            SettingDisplay(SettingsArray[CurrentSetting],1);
            Function.DrawVerticalLine(SettingsArray[CurrentSetting].X,SettingsArray[CurrentSetting].Y+1,2,0);
            Function.HeaderDesign(1,SettingsArray[PreviousSetting].X,SettingsArray[PreviousSetting].Y,70,3);
            Function.StringOut(SettingsArray[PreviousSetting].Title,SettingsArray[PreviousSetting].X+3,SettingsArray[PreviousSetting].Y,50,0,"");
            SettingDisplay(SettingsArray[PreviousSetting],0);
            Function.DrawVerticalLine(SettingsArray[PreviousSetting].X,SettingsArray[PreviousSetting].Y+1,2,3);
            PreviousSetting = CurrentSetting;
        }
        Count++;
    }
                // Function.DrawVerticalLine(6,16,12,1);
}

int CheckProjectExist() {
    int Flag = 0;
    fstream ProjectDataBaseFile(ProjectsDataBaseFilePath,ios::in|ios::binary);
    ProjectStruct Temp;
    if(ProjectDataBaseFile.is_open()) {
        while(ProjectDataBaseFile.read((char*)&Temp,sizeof(ProjectStruct))) {
            if(!strcmpi(Temp.Title,DataVariableSTRING[0].c_str())) {
                DataVariableSTRING[0] = Temp.Title;
                Flag = 1;
            }
        }
    } else {
        cout<<"Error!";
    }
    ProjectDataBaseFile.close();
    return Flag;
}

ProjectStruct GetCurrentProject() {
    fstream ProjectDataBaseFile(ProjectsDataBaseFilePath,ios::in|ios::binary);
    ProjectStruct Temp;
    if(ProjectDataBaseFile.is_open()) {
        while(ProjectDataBaseFile.read((char*)&Temp,sizeof(ProjectStruct))) {
            if(!strcmp(Temp.Title,CurrentProject)) break;
        }
    } else {
        cout<<"Error!";
    }
    ProjectDataBaseFile.close();
    return Temp;
}

int GetCompressionLevelID(int CompressionLevel) {
    switch(CompressionLevel) {
    case 0:
        return 1;
        break;
    case 1:
        return 2;
        break;
    case 3:
        return 3;
        break;
    case 5:
        return 4;
        break;
    case 7:
        return 5;
        break;
    case 9:
        return 6;
        break;
    }
}

ProjectFilePropertiesStruct GetCurrentFileProperties() {
    string ProjectDataBasePath = "data\\";
    ProjectDataBasePath+=DataVariableSTRING[0];
    ProjectDataBasePath+=".bin";
    if(DataVariableSTRING[1].find(".apk")==DataVariableSTRING[1].npos) DataVariableSTRING[1]+=".apk";
    fstream ProjectDataBase(ProjectDataBasePath.c_str(),ios::in|ios::binary);
    ProjectFilePropertiesStruct Temp;
    if(ProjectDataBase.is_open()) {
        while(!ProjectDataBase.eof()) {
            ProjectDataBase.read((char*)&Temp,sizeof(ProjectFilePropertiesStruct));
            if(!strcmp(Temp.FileName,DataVariableSTRING[1].c_str())) break;
        }
    } else {
        cout<<"Error!";
    }
    ProjectDataBase.close();
    return Temp;
}

int SearchFile(const char* FileName) {
    int Flag = 0;
    string FilePath = "projects\\";
    FilePath+=DataVariableSTRING[0];
    FilePath+="\\";
    if(DataVariableSTRING[1].find(".apk")==DataVariableSTRING[1].npos) DataVariableSTRING[1]+=".apk";
    FilePath+=DataVariableSTRING[1];
    ifstream File(FilePath.c_str());
    if(File.is_open()) {
        Flag = 1;
    }
    File.close();
    return Flag;
}

void AddProjectToDataBase() {
    fstream ProjectDataBaseFile(ProjectsDataBaseFilePath,ios::out|ios::binary|ios::app);
    ProjectStruct Temp;
    for(int i=0; i<=Function.StringLength(DataVariableSTRING[0]); i++) {
        Temp.Title[i]=DataVariableSTRING[0][i];
    }
    Temp.CompressionLevel = DataVariableINT[0];
    Temp.PNGCompression = DataVariableINT[1];
    Temp.PNGCompressionLevel = DataVariableINT[2];
    Temp.ZipAlign = DataVariableINT[3];
    Temp.SignApk = DataVariableINT[4];
    Temp.NoOfFiles = DataVariableINT[5];
    if(ProjectDataBaseFile.is_open()) {
        ProjectDataBaseFile.write((char*)&Temp,sizeof(ProjectStruct));
    } else {
        cout<<"Error!";
    }
    ProjectDataBaseFile.close();
}

void CreateProjectFileDatabase() {
    string ProjectFileName = "data\\";
    ProjectFileName+=DataVariableSTRING[0];
    ProjectFileName+=".bin";
    fstream ProjectFile;
    ProjectFile.open(ProjectFileName.c_str(),ios::out|ios::binary);
    if(ProjectFile.is_open()) {
        string ProjectFilesPath = "projects\\";
        ProjectFilesPath+=DataVariableSTRING[0];
        string FileType = ".apk";
        int NoOfFilesFound = Function.NoOfFiles(ProjectFilesPath,".apk");
        ProjectFilePropertiesStruct Temp;
        int Count = 0;
        DIR *Directory;
        struct dirent *DirectoryStructure;
        if ((Directory=opendir(ProjectFilesPath.c_str()))!=NULL) {
            while((DirectoryStructure=readdir(Directory))!=NULL) {
                char* FileName = DirectoryStructure->d_name;
                if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                    if(Function.StringLength(FileType)!=0) {
                        string FileTypeGet = FileName;
                        FileTypeGet = FileTypeGet.substr(Function.StringLength(FileTypeGet)-Function.StringLength(FileType));
                        if(!(strcmp(FileTypeGet.c_str(),FileType.c_str()))) {
                            Temp.CompressionLevel = DataVariableINT[0];
                            Temp.PNGCompression = DataVariableINT[1];
                            Temp.PNGCompressionLevel = DataVariableINT[2];
                            Temp.ZipAlign = DataVariableINT[3];
                            Temp.SignApk = DataVariableINT[4];
                            Temp.ExceptionStatus = 0;
                            strcpy(Temp.FileName,FileName);
                            ProjectFile.write((char*)&Temp,sizeof(ProjectFilePropertiesStruct));
                            Count++;
                        }
                    }
                }
            }
            closedir(Directory);
        }
    } else cout<<"Error!";
    ProjectFile.close();
}

void UpdateProject() {
    fstream ProjectDataBaseFileTemp(ProjectsTempDataBaseFilePath,ios::out|ios::binary);
    fstream ProjectDataBaseFile(ProjectsDataBaseFilePath,ios::in|ios::binary);
    ProjectStruct Temp;
    ProjectStruct Project;
    for(int i=0; i<=Function.StringLength(DataVariableSTRING[0]); i++) Project.Title[i]=DataVariableSTRING[0][i];
    Project.CompressionLevel = DataVariableINT[0];
    Project.PNGCompression = DataVariableINT[1];
    Project.PNGCompressionLevel = DataVariableINT[2];
    Project.ZipAlign = DataVariableINT[3];
    Project.SignApk = DataVariableINT[4];
    Project.NoOfFiles = DataVariableINT[5];
    if(ProjectDataBaseFile.is_open()) {
        while(ProjectDataBaseFile.read((char*)&Temp,sizeof(ProjectStruct))) {
            if(!strcmpi(Temp.Title,Project.Title)) {
                ProjectDataBaseFileTemp.write((char*)&Project,sizeof(ProjectStruct));
            } else {
                ProjectDataBaseFileTemp.write((char*)&Temp,sizeof(ProjectStruct));
            }
        }
    } else {
        cout<<"Error!";
    }
    ProjectDataBaseFile.close();
    ProjectDataBaseFileTemp.close();
    Function.ClearFrame(9,28,50,0);
    Function.ClearFrame(8,31,50,3);
    if(!remove(ProjectsDataBaseFilePath)) {
        if(!rename(ProjectsTempDataBaseFilePath,ProjectsDataBaseFilePath)) {
            GenerateProjectArray();
            GenerateProjectFileArray();
            Function.SetCord(9,28);
            cout<<"Project Updated, Press any key to continue.";
            getch();
        } else {
            cout<<"Error!";
        }
    } else {
        cout<<"Error!";
    }
}

void UpdateProjectDatabase(int Type) {
    string ProjectDatabasePath = "data\\";
    ProjectDatabasePath+=DataVariableSTRING[0];
    ProjectDatabasePath+=".bin";
    string ProjectTempDatabasePath = "data\\temp_";
    ProjectTempDatabasePath+=DataVariableSTRING[0];
    ProjectTempDatabasePath+=".bin";
    fstream ProjectDatabaseIn(ProjectDatabasePath.c_str(),ios::in|ios::binary);
    fstream ProjectDatabaseOut(ProjectTempDatabasePath.c_str(),ios::out|ios::binary);
    ProjectFilePropertiesStruct Temp;
    ProjectFilePropertiesStruct FileProperty;
    FileProperty.CompressionLevel = DataVariableINT[0];
    FileProperty.PNGCompression = DataVariableINT[1];
    FileProperty.PNGCompressionLevel = DataVariableINT[2];
    FileProperty.ZipAlign = DataVariableINT[3];
    FileProperty.SignApk = DataVariableINT[4];
    FileProperty.ExceptionStatus = 0;
    if(ProjectDatabaseIn.is_open()) {
        while(ProjectDatabaseIn.read((char*)&Temp,sizeof(ProjectFilePropertiesStruct))) {
            switch(Type) {
            case 1:
                if(Temp.ExceptionStatus!=1) {
                    strcpy(FileProperty.FileName,Temp.FileName);
                    ProjectDatabaseOut.write((char*)&FileProperty,sizeof(ProjectFilePropertiesStruct));
                } else {
                    ProjectDatabaseOut.write((char*)&Temp,sizeof(ProjectFilePropertiesStruct));
                }
                break;
            case 2:
                strcpy(FileProperty.FileName,Temp.FileName);
                ProjectDatabaseOut.write((char*)&FileProperty,sizeof(ProjectFilePropertiesStruct));
                break;
            }
        }
    }
    ProjectDatabaseIn.close();
    ProjectDatabaseOut.close();
    if(!remove(ProjectDatabasePath.c_str())) {
        if(rename(ProjectTempDatabasePath.c_str(),ProjectDatabasePath.c_str())) {
            GenerateProjectFileArray();
            cout<<"Error!";
        }
    } else {
        cout<<"Error!";
    }
}

void UpdateProjectDatabaseByFile() {
    string ProjectDatabasePath = "data\\";
    ProjectDatabasePath+=DataVariableSTRING[0];
    ProjectDatabasePath+=".bin";
    string ProjectTempDatabasePath = "data\\temp_";
    ProjectTempDatabasePath+=DataVariableSTRING[0];
    ProjectTempDatabasePath+=".bin";
    fstream ProjectDatabaseIn(ProjectDatabasePath.c_str(),ios::in|ios::binary);
    fstream ProjectDatabaseOut(ProjectTempDatabasePath.c_str(),ios::out|ios::binary);
    ProjectFilePropertiesStruct Temp;
    ProjectFilePropertiesStruct FileProperty;
    if(DataVariableSTRING[1].find(".apk")==DataVariableSTRING[1].npos) DataVariableSTRING[1]+=".apk";
    for(int i=0; i<=Function.StringLength(DataVariableSTRING[1]); i++) FileProperty.FileName[i] = DataVariableSTRING[1][i];
    FileProperty.CompressionLevel = DataVariableINT[0];
    FileProperty.PNGCompression = DataVariableINT[1];
    FileProperty.PNGCompressionLevel = DataVariableINT[2];
    FileProperty.ZipAlign = DataVariableINT[3];
    FileProperty.SignApk = DataVariableINT[4];
    FileProperty.ExceptionStatus = 1;
    if(ProjectDatabaseIn.is_open()) {
        while(ProjectDatabaseIn.read((char*)&Temp,sizeof(ProjectFilePropertiesStruct))) {
            if(!strcmp(FileProperty.FileName,Temp.FileName)) ProjectDatabaseOut.write((char*)&FileProperty,sizeof(ProjectFilePropertiesStruct));
            else ProjectDatabaseOut.write((char*)&Temp,sizeof(ProjectFilePropertiesStruct));
        }
    }
    ProjectDatabaseIn.close();
    ProjectDatabaseOut.close();
    if(!remove(ProjectDatabasePath.c_str())) {
        if(!rename(ProjectTempDatabasePath.c_str(),ProjectDatabasePath.c_str())) {
            GenerateProjectFileArray();
            Function.SetCord(9,29);
            cout<<"Exception Updated, Press any key to continue.";
            getch();
        } else {
            cout<<"Error!";
        }
    } else {
        cout<<"Error!";
    }
}

void AddFiles() {
    Menu(MenuID_ProjectAddFilesDragNDrop);
    Function.ClearConsole(0);
    if(Function.StringLength(DataVariableSTRING[1])==0) Menu(PreviousMenuID);
    string FilesPathReal = DataVariableSTRING[1];
    if((DataVariableSTRING[1][0]=='"')&&(DataVariableSTRING[1][Function.StringLength(DataVariableSTRING[1])-1]=='"')) {
        FilesPathReal = FilesPathReal.substr(1,Function.StringLength(FilesPathReal)-1);
        FilesPathReal = FilesPathReal.substr(0,Function.StringLength(FilesPathReal)-1);
    }
    int NoOfFilesFound = Function.NoOfFiles(FilesPathReal,".apk");
    if(NoOfFilesFound!=0) {
        stringstream NoOfFilesFoundStringStream;
        NoOfFilesFoundStringStream<<NoOfFilesFound;
        string NoOfFilesFoundString = NoOfFilesFoundStringStream.str();
        string CopyToFilePath = "projects\\";
        CopyToFilePath+=DataVariableSTRING[0];
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(5,9,18,65,0,NoOfFilesFoundString);
        Function.HeaderDesign(2,6,20,70);
        Function.StringOut(6,9,20,65,0,"");
        if(Function.CopyFiles(FilesPathReal,CopyToFilePath,".apk",9,22,68)) {
            Function.StringOut(6,9,20,65,0,"Done!");
            DataVariableINT[5] = NoOfFilesFound;
            AddProjectToDataBase();
            CreateProjectFileDatabase();
            for(int i=0; i<=Function.StringLength(DataVariableSTRING[0]); i++) {
                CurrentProject[i]=DataVariableSTRING[0][i];
            }
            CurrentProjectSelected = 1;
            GenerateProjectArray();
            GenerateProjectFileArray();
            Function.StringOut(7,9,26,65,0,"continue");
            getch();
            Menu(MenuID_MaintainProject);
        } else {

        }
    } else {
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(8,9,18,65,0,"");
        Function.StringOut(7,9,21,65,0,"go back");
        getch();
        Menu(PreviousMenuID);
    }
}

void AddFilesManual() {
    PreviousMenuID = MenuID_ProjectAddFiles;
    string CMDOpenProjectFolder = "start projects\\";
    CMDOpenProjectFolder+=DataVariableSTRING[0];
    system(CMDOpenProjectFolder.c_str());
    Menu(MenuID_ProjectAddFilesManualConfirm);
}

void AddFilesManualYes() {
    Function.ClearConsole(0);
    string ProjectFolderPath = "projects\\";
    ProjectFolderPath+=DataVariableSTRING[0];
    int NoOfFilesFound = Function.NoOfFiles(ProjectFolderPath,".apk");
    if(NoOfFilesFound!=0) {
        stringstream NoOfFilesFoundStringStream;
        NoOfFilesFoundStringStream<<NoOfFilesFound;
        string NoOfFilesFoundString = NoOfFilesFoundStringStream.str();
        DataVariableINT[5] = NoOfFilesFound;
        AddProjectToDataBase();
        CreateProjectFileDatabase();
        for(int i=0; i<=Function.StringLength(DataVariableSTRING[0]); i++) {
            CurrentProject[i]=DataVariableSTRING[0][i];
        }
        CurrentProjectSelected = 1;
        GenerateProjectArray();
        GenerateProjectFileArray();
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(5,9,18,65,0,NoOfFilesFoundString);
        Function.StringOut(18,9,20,65,0,DataVariableSTRING[0]);
        Function.StringOut(7,9,24,65,0,"continue");
        getch();
        Menu(MenuID_MaintainProject);
    } else {
        Function.ClearConsole(0);
        MenuDesign();
        Function.HeaderDesign(1,6,15,70);
        Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
        Function.HeaderDesign(2,6,18,70);
        Function.StringOut(9,9,18,65,0,"");
        Function.StringOut(7,9,21,65,0,"go back");
        getch();
        Menu(PreviousMenuID);
    }
}

int NoOfProjects() {
    int Count = 0;
    fstream ProjectDataBaseFile(ProjectsDataBaseFilePath,ios::in|ios::binary);
    ProjectStruct Temp;
    if(ProjectDataBaseFile.is_open()) {
        while((!ProjectDataBaseFile.eof())&&(ProjectDataBaseFile.read((char*)&Temp,sizeof(ProjectStruct)))) {
            Count++;
        }
    } else cout<<"Error!";
    ProjectDataBaseFile.close();
    return Count;
}

void GenerateProjectArray() {
    int Count = 0;
    int _NoOfProjects = NoOfProjects();
    if(_NoOfProjects!=0){
        ProjectArray = new ProjectStruct[_NoOfProjects];
        fstream ProjectDataBaseFile(ProjectsDataBaseFilePath,ios::in|ios::binary);
        ProjectStruct Temp;
        if(ProjectDataBaseFile.is_open()) {
            while((!ProjectDataBaseFile.eof())&&(Count<_NoOfProjects)) {
                ProjectDataBaseFile.read((char*)&Temp,sizeof(ProjectStruct));
                ProjectArray[Count].CompressionLevel = Temp.CompressionLevel;
                ProjectArray[Count].NoOfFiles = Temp.NoOfFiles;
                ProjectArray[Count].PNGCompression = Temp.PNGCompression;
                ProjectArray[Count].PNGCompressionLevel = Temp.PNGCompressionLevel;
                ProjectArray[Count].SignApk = Temp.SignApk;
                ProjectArray[Count].ZipAlign = Temp.ZipAlign;
                strcpy(ProjectArray[Count].Title,Temp.Title);
                Count++;
            }
        } else cout<<"Error!";
        ProjectDataBaseFile.close();
    }
}

void GenerateProjectFileArray() {
    int Count = 0;
    ProjectFileArray = new ProjectFilePropertiesStruct[GetCurrentProject().NoOfFiles];
    string ProjectDataBasePath = "data\\";
    ProjectDataBasePath+=DataVariableSTRING[0];
    ProjectDataBasePath+=".bin";
    fstream ProjectDataBase(ProjectDataBasePath.c_str(),ios::in|ios::binary);
    ProjectFilePropertiesStruct Temp;
    if(ProjectDataBase.is_open()) {
        while((!ProjectDataBase.eof())&&(Count<GetCurrentProject().NoOfFiles)) {
            ProjectDataBase.read((char*)&Temp,sizeof(ProjectFilePropertiesStruct));
            ProjectFileArray[Count].CompressionLevel=Temp.CompressionLevel;
            ProjectFileArray[Count].PNGCompression=Temp.PNGCompression;
            ProjectFileArray[Count].PNGCompressionLevel=Temp.PNGCompressionLevel;
            ProjectFileArray[Count].ZipAlign=Temp.ZipAlign;
            ProjectFileArray[Count].SignApk=Temp.SignApk;
            strcpy(ProjectFileArray[Count].FileName,Temp.FileName);
            ProjectFileArray[Count].ExceptionStatus=Temp.ExceptionStatus;
            Count++;
        }
    } else {
        cout<<"Error!";
    }
    ProjectDataBase.close();
}

void GenerateFolderFileArray(string FolderPath,string FileType){
    int Count = 0;
    int Flag = 0;
    DIR *Directory;
    struct dirent *DirectoryStructure;
    if ((Directory=opendir(FolderPath.c_str()))!=NULL) {
        while((DirectoryStructure=readdir(Directory))!=NULL) {
            char* FileName = DirectoryStructure->d_name;
            if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                string FileTypeGet = FileName;
                FileTypeGet = FileTypeGet.substr(Function.StringLength(FileTypeGet)-Function.StringLength(FileType));
                if(!(strcmp(FileTypeGet.c_str(),FileType.c_str()))) {
                    strcpy(FolderFileArray[Count].FileName,FileName);
                    FolderFileArray[Count].CompressionLevel=GetINTSetting(1);
                    FolderFileArray[Count].PNGCompression=GetINTSetting(2);
                    FolderFileArray[Count].PNGCompressionLevel=GetINTSetting(3);
                    FolderFileArray[Count].ZipAlign=GetINTSetting(4);
                    FolderFileArray[Count].SignApk=GetINTSetting(5);
                    FolderFileArray[Count].ExceptionStatus=0;
                    Count++;
                }
            }
        }
        closedir(Directory);
    }
}

void BuildProject() {
    PreviousMenuID = MenuID_Project;
    string ProjectFileName = "data\\";
    ProjectFileName+=DataVariableSTRING[0];
    ProjectFileName+=".bin";
    fstream ProjectFile;
    ProjectFile.open(ProjectFileName.c_str(),ios::out);
    ProjectFile.close();
    string CMDForProjectFolder = "mkdir projects\\";
    CMDForProjectFolder+=DataVariableSTRING[0];
    Function._CMD(CMDForProjectFolder.c_str());
    Menu(MenuID_ProjectAddFiles);
}

void DeleteProject(){
    Menu(MenuID_DeleteProject);
}

void DeleteProjectYes(){
    string ProjectName = DataVariableSTRING[0];
    if(DeleteProjectType==1){
        fstream ProjectDatabase(ProjectsDataBaseFilePath,ios::in|ios::binary);
        fstream ProjectTempDatabase(ProjectsTempDataBaseFilePath,ios::out|ios::binary);
        ProjectStruct Temp;
        if(ProjectDatabase.is_open()){
            while(!ProjectDatabase.eof()){
                ProjectDatabase.read((char*)&Temp,sizeof(ProjectStruct));
                if(strcmp(Temp.Title,ProjectName.c_str())!=0){
                    ProjectTempDatabase.write((char*)&Temp,sizeof(ProjectStruct));
                }
            }
        }
        ProjectDatabase.close();
        ProjectTempDatabase.close();
        if(!remove(ProjectsDataBaseFilePath)) {
            if(!rename(ProjectsTempDataBaseFilePath,ProjectsDataBaseFilePath)) {
                Menu(MenuID_DeleteProjectDeleteFolder);
            } else {
                cout<<"Error!";
            }
        } else {
            cout<<"Error!";
        }
    }else if(DeleteProjectType==2){
        string Command = "rmdir \"projects\\";
        Command+=ProjectName;
        Command+="\" /s /q";
        Function.CMD(Command.c_str());
        Menu(MenuID_DeleteProjectDeleteDatabase); 
    }else if(DeleteProjectType==3){
        string Command = "del \"data\\";
        Command+=ProjectName;
        Command+=".bin\" /q";
        Function.CMD(Command.c_str());
        Menu(MenuID_DeleteProjectDone);
    }else cout<<"Error!";
}

void DeleteProjectNo(){
    if(DeleteProjectType==1){
        Back();
    }else if(DeleteProjectType==2){
        Menu(MenuID_DeleteProjectDeleteDatabase);
    }else if(DeleteProjectType==3){
        Menu(MenuID_DeleteProjectDone);
    }else cout<<"Error!";
}

void PerformCompression() {
    Menu(MenuID_PerformCompression);
}

void PerformCompressionConfirmYes() {
    int CurrentCompressionID = 0;
    int NoOfFiles = GetCurrentProject().NoOfFiles;
    Function.ClearConsole(0);
    MenuDesign();
    Function.HeaderDesign(1,6,15,70);
    Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
    for(; CurrentCompressionID<NoOfFiles; CurrentCompressionID++) {
        Function.HeaderDesign(1,6,18,70);
        Function.SetCord(9,18);
        Function.ColorOut("Current File : ",0);
        Function.ColorOut(ProjectFileArray[CurrentCompressionID].FileName,1,50);
        Function.HeaderDesign(1,6,20,70);
        Function.SetCord(9,20);
        Function.ColorOut("No Of Files Done : ",0);
        cout<<CurrentCompressionID<<" out of "<<NoOfFiles;
        Function.HeaderDesign(1,6,22,70);
        Function.PercentageBar(NoOfFiles*6,CurrentCompressionID*6,8,24,69);
        CompressFile(ProjectFileArray[CurrentCompressionID],CurrentCompressionID,NoOfFiles,1);
        Function.ClearFrame(24,18,Function.StringLength(ProjectFileArray[CurrentCompressionID].FileName),0);
    }
    Function.SetCord(9,18);
    Function.ColorOut("Current File : ",0);
    Function.ColorOut(ProjectFileArray[CurrentCompressionID-1].FileName,1,50);
    Function.HeaderDesign(1,6,20,70);
    Function.SetCord(9,20);
    Function.ColorOut("No Of Files Done : ",0);
    cout<<CurrentCompressionID<<" out of "<<NoOfFiles;
    Function.HeaderDesign(1,6,22,70);
    Function.PercentageBar(NoOfFiles*6,NoOfFiles*6,8,24,69);
    string ProjectBuildFolder = "builds\\";
    ProjectBuildFolder+=DataVariableSTRING[0];
    string Command = "mkdir ";
    Command+=ProjectBuildFolder;
    Function.ClearFrame(9,22,25,0);
    Function.SetCord(9,22);
    Function.ColorOut("Copying Files...",0);
    Function.CMD(Command.c_str());
	Function.ClearFrame(8,24,69,2);
    if(Function.CopyFiles("temp",ProjectBuildFolder.c_str(),".apk",8,24,69)){
	    Function.ClearFrame(8,24,69,2);
	    if(Function.DeleteFiles("temp",".apk",8,24,69)){  
            Function.ClearConsole(0);
            MenuDesign();
            Function.HeaderDesign(1,6,15,70);
            Function.StringOut(0,9,15,65,1,DataVariableSTRING[0]);
            Function.HeaderDesign(1,6,18,70);
            Function.SetCord(9,18);
            Function.ColorOut("Compression Ratio : ",0);
            string ProjectFolderPath = "projects\\";
            ProjectFolderPath+=DataVariableSTRING[0];
            double CompressionRatio = (Function.FolderSize(ProjectBuildFolder.c_str(),".apk")/Function.FolderSize(ProjectFolderPath.c_str(),".apk"))*100;
            cout<<CompressionRatio<<" %";
            Function.HeaderDesign(1,6,20,70);
            Function.StringOut(30,9,20,65,0,"go back.");
            getch();
    	    Menu(PreviousMenuID);
        }else cout<<"Error!";
	}else cout<<"Error!";
    getch();
}

void PerformCompressFolder(){
    if(Function.StringLength(DataVariableSTRING[0])==0) Menu(MenuID_Main);
    string Folder = DataVariableSTRING[0];
    if((DataVariableSTRING[0][0]=='"')&&(DataVariableSTRING[0][Function.StringLength(DataVariableSTRING[0])-1]=='"')) {
        Folder = Folder.substr(1,Function.StringLength(Folder)-1);
        Folder = Folder.substr(0,Function.StringLength(Folder)-1);
    }
    if(Function.StringLength(Folder)>235) cout<<"Error!";
    int CurrentCompressionID = 0;
    int NoOfFiles = Function.NoOfFiles(Folder,".apk");
    FolderFileArray = new ProjectFilePropertiesStruct[NoOfFiles];
    GenerateFolderFileArray(Folder,".apk");
    Function.ClearConsole(0);
    MenuDesign();
    Function.HeaderDesign(1,6,15,70);
    Function.StringOut(28,9,15,65,1,"");
    for(; CurrentCompressionID<NoOfFiles; CurrentCompressionID++) {
        Function.HeaderDesign(1,6,18,70);
        Function.SetCord(9,18);
        Function.ColorOut("Current File : ",0);
        Function.ColorOut(FolderFileArray[CurrentCompressionID].FileName,1,50);
        Function.HeaderDesign(1,6,20,70);
        Function.SetCord(9,20);
        Function.ColorOut("No Of Files Done : ",0);
        cout<<CurrentCompressionID<<" out of "<<NoOfFiles;
        Function.HeaderDesign(1,6,22,70);
        Function.PercentageBar(NoOfFiles*6,CurrentCompressionID*6,8,24,69);
        CompressFile(FolderFileArray[CurrentCompressionID],CurrentCompressionID,NoOfFiles,2);
        Function.ClearFrame(24,18,Function.StringLength(FolderFileArray[CurrentCompressionID].FileName),0);
    }
    Function.SetCord(9,18);
    Function.ColorOut("Current File : ",0);
    Function.ColorOut(FolderFileArray[CurrentCompressionID-1].FileName,1,50);
    Function.HeaderDesign(1,6,20,70);
    Function.SetCord(9,20);
    Function.ColorOut("No Of Files Done : ",0);
    cout<<CurrentCompressionID<<" out of "<<NoOfFiles;
    Function.HeaderDesign(1,6,22,70);
    Function.PercentageBar(NoOfFiles*6,NoOfFiles*6,8,24,69);
    string CompressedFolder = Folder;
    CompressedFolder+="_Cassini[";
    stringstream StringStream;
    StringStream<<Function.GetCurrentDate(1)<<"-"<<Function.GetCurrentDate(2)<<"-"<<Function.GetCurrentDate(3)<<"]["<<Function.GetCurrentDate(4)<<"-"<<Function.GetCurrentDate(5)<<"-"<<Function.GetCurrentDate(6)<<"]";
    CompressedFolder+=StringStream.str();
    string Command = "mkdir \"";
    Command+=CompressedFolder;
    Command+="\"";
    Function.ClearFrame(9,22,25,0);
    Function.SetCord(9,22);
    Function.ColorOut("Copying Files...",0);
    Function.CMD(Command.c_str());
	Function.ClearFrame(8,24,69,2);
    if(Function.CopyFiles("temp",CompressedFolder.c_str(),".apk",8,24,69)){
	    Function.ClearFrame(8,24,69,2);
	    if(Function.DeleteFiles("temp",".apk",8,24,69)){  
            Function.ClearConsole(0);
            MenuDesign();
            Function.HeaderDesign(1,6,15,70);
            Function.SetCord(9,15);
            Function.ColorOut(Folder,0,69);
            Function.HeaderDesign(1,6,18,70);
            Function.SetCord(9,18);
            Function.ColorOut("Compression Ratio : ",0);
            double CompressionRatio = (Function.FolderSize(CompressedFolder.c_str(),".apk")/Function.FolderSize(Folder.c_str(),".apk"))*100;
            cout<<CompressionRatio<<" %";
            Function.HeaderDesign(2,6,20,70);
            Function.StringOut(29,9,20,65,0,CompressedFolder);
            Function.StringOut(30,9,24,65,0,"open the compressed folder and go back.");
            getch();
            string CMDOpenCompressFolder = "explorer ";
            CMDOpenCompressFolder+=CompressedFolder;
            system(CMDOpenCompressFolder.c_str());
            cin.clear();
    	    Menu(MenuID_Main);
        }else cout<<"Error!";
	}else cout<<"Error!";
}

void InitializePNGFileCount(const char* DirectoryPath){
    DIR *Directory;
    struct dirent *DirectoryStructure;
    if ((Directory=opendir(DirectoryPath))!=NULL) {
        while((DirectoryStructure=readdir(Directory))!=NULL) {
            char* FileName = DirectoryStructure->d_name;
            if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                string Path = DirectoryPath;
                Path+="\\";
                Path+=FileName;
                if(GetFileAttributes(Path.c_str())==FILE_ATTRIBUTE_DIRECTORY) {
                    InitializePNGFileCount(Path.c_str());
                } else {
                    string FileTypeGet = FileName;
                    string FileTypeGetTemp = FileName;
                    if(Function.StringLength(FileName)>4) FileTypeGet = FileTypeGet.substr(Function.StringLength(FileTypeGet)-4);
                    if(Function.StringLength(FileName)>6) FileTypeGetTemp = FileTypeGetTemp.substr(Function.StringLength(FileTypeGetTemp)-6);
                    if((!strcmp(FileTypeGet.c_str(),".png"))&&(strcmp(FileTypeGetTemp.c_str(),".9.png"))) {
                        PNGFileCount++;
                    }
                }
            }
        }
        closedir(Directory);
    }
}

void CompressPNG(const char* DirectoryPath,string Quality,int NoOfFiles, int CurrentCompressionID) {
    DIR *Directory;
    struct dirent *DirectoryStructure;
    if ((Directory=opendir(DirectoryPath))!=NULL) {
        while((DirectoryStructure=readdir(Directory))!=NULL) {
            char* FileName = DirectoryStructure->d_name;
            if(strcmp(FileName,".")&&strcmp(FileName,"..")) {
                string Path = DirectoryPath;
                Path+="\\";
                Path+=FileName;
                if(GetFileAttributes(Path.c_str())==FILE_ATTRIBUTE_DIRECTORY) {
                    CompressPNG(Path.c_str(),Quality,NoOfFiles,CurrentCompressionID);
                } else {
                    string FileTypeGet = FileName;
                    string FileTypeGetTemp = FileName;
                    if(Function.StringLength(FileName)>4) FileTypeGet = FileTypeGet.substr(Function.StringLength(FileTypeGet)-4);
                    if(Function.StringLength(FileName)>6) FileTypeGetTemp = FileTypeGetTemp.substr(Function.StringLength(FileTypeGetTemp)-6);
                    if((!strcmp(FileTypeGet.c_str(),".png"))&&(strcmp(FileTypeGetTemp.c_str(),".9.png"))) {
                        string FilePath = DirectoryPath;
                        FilePath+="\\";
                        FilePath+=FileName;
                        string Command = "call resources\\pngquant --force --ext .png --quality=";
                        Command+=Quality;
                        Command+=" ";
                        Command+=FilePath;
                        Function.CMD(Command.c_str());
                        TempPNGFileCount++;
                        Function.PercentageBar(PNGFileCount,TempPNGFileCount,37,21,40);
                    }
                }
            }
        }
        closedir(Directory);
    }
}

void CompressFile(ProjectFilePropertiesStruct File,int CurrentCompressionID,int NoOfFiles,int Type) {
    stringstream StringStream;
    StringStream<<File.CompressionLevel;
    string CompressionLevelString = StringStream.str();
    StringStream.str(string());
    StringStream.clear();
    StringStream<<File.PNGCompressionLevel;
    string PNGCompressionQualityString = StringStream.str();
    string FileName = File.FileName;
    string FileFolder = FileName.substr(0,Function.StringLength(FileName)-4);
    string FilePath;
    if(Type==1){
        FilePath = "%0\\..\\projects\\";
        FilePath+=GetCurrentProject().Title;
        FilePath+="\\";
    }else if(Type==2){
        FilePath = DataVariableSTRING[0];
        if((DataVariableSTRING[1][0]=='"')&&(DataVariableSTRING[1][Function.StringLength(DataVariableSTRING[1])-1]=='"')) {
            FilePath = FilePath.substr(1,Function.StringLength(FilePath)-1);
            FilePath = FilePath.substr(0,Function.StringLength(FilePath)-1);
        }
        FilePath+="\\";
    }
    FilePath+=FileName;
    string FileTempFolder = "%0\\..\\temp\\";
    FileTempFolder+=FileFolder;
    string FilePathTemp = "%0\\..\\temp\\";
    FilePathTemp+=FileName;
    string Command = "";
    // Pass - 1
    Function.ClearFrame(9,22,25,0);
    Function.SetCord(9,22);
    Function.ColorOut("Extracting File...",0);
    Command = "call resources\\7za x ";
    Command+=FilePath;
    Command+=" -o";
    Command+=FileTempFolder;
    Function.CMD(Command.c_str());
    Function.PercentageBar(NoOfFiles*6,(CurrentCompressionID)*6+1,8,24,69);
    PNGFileCount = 0;
    TempPNGFileCount = 0;
    // Pass - 2
    if(File.PNGCompression==1){
        InitializePNGFileCount(FileTempFolder.c_str());
        Function.ClearFrame(9,22,25,0);
        Function.SetCord(9,22);
        Function.ColorOut("Compressing PNG Files...",0);
        CompressPNG(FileTempFolder.c_str(),PNGCompressionQualityString,NoOfFiles,CurrentCompressionID);
    }
    Function.PercentageBar(NoOfFiles*6,(CurrentCompressionID)*6+2,8,24,69);
    Function.ClearFrame(7,22,70,0);
    Function.HeaderDesign(1,6,22,70);
    // Pass - 3
    Function.SetCord(9,22);
    Function.ColorOut("Compressing APK...",0);
    Command = "call resources\\7za a -tzip ";
    Command+=FilePathTemp;
    Command+=" ";
    Command+=FileTempFolder;
    Command+="\\* -mx";
    Command+=CompressionLevelString;
    Command+=" -y";
    Function.CMD(Command.c_str());
    Function.PercentageBar(NoOfFiles*6,(CurrentCompressionID)*6+3,8,24,69);
    // Pass - 4
    if(File.SignApk==1) {
    	Function.ClearFrame(9,22,25,0);
	    Function.SetCord(9,22);
	    Function.ColorOut("Signing APK...",0);
        Command = "java -jar %0\\..\\resources\\signapk.jar %0\\..\\resources\\testkey.x509.pem %0\\..\\resources\\testkey.pk8 ";
        Command+=FilePathTemp;
        Command+=" ";
        Command+=FilePathTemp;
        Command+="-Signed";
        Function.CMD(Command.c_str());
        Command = "del ";
        Command+=FilePathTemp;
        Function.CMD(Command.c_str());
        Command = "ren \"";
        Command+=FilePathTemp;
        Command+="-Signed\" \"";
        Command+=FileName;
        Command+="\"";
        Function.CMD(Command.c_str());
    }
    Function.PercentageBar(NoOfFiles*6,(CurrentCompressionID)*6+4,8,24,69);
    // Pass - 5
    if(File.ZipAlign==1) {
    	Function.ClearFrame(9,22,25,0);
	    Function.SetCord(9,22);
	    Function.ColorOut("Performing ZipAlign...",0);
        Command = "call resources\\zipalign -f 4 ";
        Command+=FilePathTemp;
        Command+=" ";
        Command+=FilePathTemp.substr(0,Function.StringLength(FilePathTemp)-4);
        Command+="-ZipAligned.apk";
        Function.CMD(Command.c_str());
        Command = "del ";
        Command+=FilePathTemp;
        Function.CMD(Command.c_str());
        Command = "ren \"";
        Command+=FilePathTemp.substr(0,Function.StringLength(FilePathTemp)-4);
        Command+="-ZipAligned.apk\" \"";
        Command+=FileName;
        Command+="\"";
        Function.CMD(Command.c_str());
    }
    Function.PercentageBar(NoOfFiles*6,(CurrentCompressionID)*6+5,8,24,69);
    // Pass - 6
    Function.ClearFrame(9,22,25,0);
    Function.SetCord(9,22);
    Function.ColorOut("Deleting Files...",0);
    Command = "rmdir /S /Q ";
    Command+=FileTempFolder;
    Function.CMD(Command.c_str());
}

void ManageExceptions() {
    Menu(MenuID_ManageExceptions);
}

void ManageFileByName() {
    Menu(MenuID_ManageFileByName);
}

void ManageFileByList() {
    Menu(MenuID_ManageFileByList);
}

void ManageProperties() {
    Menu(MenuID_ManageProperties);
}

void MaintainProjectBack() {
    CurrentProjectSelected = 0;
    MaintainProjectSelectType = 0;
    delete[] ProjectFileArray;
    InitializeVariables();
    Menu(MenuID_Main);
}

void UpdateProjectPropertiesConfirmYes() {
    Function.ClearFrame(9,28,50,0);
    Function.ClearFrame(8,31,50,3);
    Function.HeaderDesign(1,6,28,70);
    Function.StringOut(15,9,28,50,1,"");
    Function.CreateButtons(UpdateProjectFilesPropertiesConfirm,2,DefaultButton,1,2);
}

void UpdateProjectPropertiesConfirmNo() {
    Menu(MenuID_MaintainProject);
}

void UpdateProjectFilesPropertiesConfirmYes() {
    UpdateProject();
    UpdateProjectDatabase(1);
}

void UpdateProjectFilesPropertiesConfirmNo() {
    UpdateProject();
    UpdateProjectDatabase(2);
}

void GenerateProjectList() {
    int ProjectSelectID = 0;
    int Count = 0;
    ProjectList = new list[NoOfProjects()];
    for(int i=0; i<NoOfProjects(); i++) {
        ProjectList[i].status = 1;
        ProjectList[i].x = 8;
        ProjectList[i].y = 18+i*2;
        ProjectList[i].ListReturnValue = i;
        strcpy(ProjectList[i].text,ProjectArray[i].Title);
        ProjectList[i].HighlightedFlag = 0;
    }
    ProjectSelectID = Function.CreateLists(ProjectList,NoOfProjects(),0,65,2);
    DataVariableSTRING[0] = ProjectArray[ProjectSelectID].Title;
    for(int i=0; i<=Function.StringLength(DataVariableSTRING[0]); i++) {
        CurrentProject[i]=DataVariableSTRING[0][i];
    }
    CurrentProjectSelected = 1;
    GenerateProjectFileArray();
    Menu(MenuID_MaintainProject);
}

void GenerateProjectFileList() {
    int ProjectFileSelectID = 0;
    ProjectFileList = new list[GetCurrentProject().NoOfFiles];
    for(int i=0; i<GetCurrentProject().NoOfFiles; i++) {
        ProjectFileList[i].status = 1;
        ProjectFileList[i].x = 8;
        ProjectFileList[i].y = 20+i*2;
        ProjectFileList[i].ListReturnValue = i;
        string ProjectFileArrayName = ProjectFileArray[i].FileName;
        if(ProjectFileArray[i].ExceptionStatus==1) {
            ProjectFileList[i].HighlightedFlag = 1;
        } else {
            ProjectFileList[i].HighlightedFlag = 0;
        }
        strcpy(ProjectFileList[i].text,ProjectFileArrayName.c_str());
    }
    ProjectFileSelectID = Function.CreateLists(ProjectFileList,GetCurrentProject().NoOfFiles,0,65,2);
    DataVariableSTRING[1] = ProjectFileArray[ProjectFileSelectID].FileName;
    Menu(MenuID_ManageFileByNameSelected);
}

void MaintainProjectSelectType1() {
    MaintainProjectSelectType = 1;
}

void MaintainProjectSelectType2() {
    MaintainProjectSelectType = 2;
}

int main(int argc, char *argv[])
{
    InitializeVariables();
    Function.CursorToggle(0);
    Function.SetConsoleDimensions(DefaultWidth,DefaultHeight);
    GenerateProjectArray();
    Menu(MenuID_Main);
    return EXIT_SUCCESS;
}
