/*
	Author : Divya Mamgai
	License : The MIT License (MIT)
	Copyright (c) 2016 Divya Mamgai
	Version : Cassini.1.1
*/
#include<iostream>
#include<conio.h>
#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<dirent.h>
#include<iomanip>
#include<fstream>
#include<time.h>

using namespace std;

int success_flag = 0;

int _STR_LEN(string STR){
    int LEN;
    for(LEN=0; STR[LEN]!='\0'; ++LEN);
    return LEN;
}

__int64 TransverseDirectory(string path)
{
    WIN32_FIND_DATA data;
    __int64 size = 0;
    string fname = path + "\\*.*";
    HANDLE h = FindFirstFile(fname.c_str(),&data);
    if(h != INVALID_HANDLE_VALUE)
    {
        do {
            if( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                if( strcmp(data.cFileName,".") != 0 &&strcmp(data.cFileName,"..") != 0)
                {
                    fname = path + "\\" + data.cFileName;
                    size += TransverseDirectory(fname);
                }

            }
            else
            {
                LARGE_INTEGER sz;
                sz.LowPart = data.nFileSizeLow;
                sz.HighPart = data.nFileSizeHigh;
                size += sz.QuadPart;

            }
        }while( FindNextFile(h,&data) != 0);
        FindClose(h);

    }
    return size;
}

string exec(char* cmd){
	string _cmd = cmd;
	_cmd+=" > nul";
   	const char* _cmd_temp = _cmd.c_str();
   	char __cmd[strlen(_cmd_temp)];
   	strcpy(__cmd,_cmd_temp);
    FILE* pipe = _popen(__cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    _pclose(pipe);
    return result;
}

string oexec(char* cmd){
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    _pclose(pipe);
    return result;
}

string _replace(string str,string find,string replacewith)
{
    size_t position = 0;
    for(position=str.find(find);position!=string::npos;position=str.find(find,position))
    {
    	str.replace(position ,1,replacewith);
    }
    return str;
}

char HEADER_FRAME[][81]=
{
    "   ######## ######## ######## ######## ######## ##    ## ######## ",
    "   ##       ##    ## ##       ##          ##    ####  ##    ##    ",
    "   ##       ######## ######## ########    ##    ## ## ##    ##    ",
    "   ##       ##    ##       ##       ##    ##    ##  ####    ##    ",
    "   ######## ##    ## ######## ######## ######## ##   ### ######## "
};

void CURSOR_TOGGLE(int VISIBILITY_FLAG)
{
    HANDLE hOut;
    CONSOLE_CURSOR_INFO ConCurInf;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    ConCurInf.dwSize = 10;
    switch(VISIBILITY_FLAG)
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


void SET_CORD(short x,short y)
{
    COORD coord = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void SET_CONSOLE_DIMENSIONS(int WIDTH,int HEIGHT)
{
    HWND CONSOLE = GetConsoleWindow();
    const HWND DESKTOP = GetDesktopWindow();
    RECT CONSOLE_RECT,DESKTOP_RECT;
    GetWindowRect(CONSOLE, &CONSOLE_RECT);
    GetWindowRect(DESKTOP, &DESKTOP_RECT);
    int CONSOLE_POS_X,CONSOLE_POS_Y;
    int SCREEN_WIDTH = DESKTOP_RECT.right,SCREEN_HEIGHT = DESKTOP_RECT.bottom;
    CONSOLE_POS_X = (SCREEN_WIDTH-WIDTH)/2;
    CONSOLE_POS_Y = (SCREEN_HEIGHT-HEIGHT)/2;
    MoveWindow(CONSOLE, CONSOLE_POS_X, CONSOLE_POS_Y, WIDTH, HEIGHT, TRUE);
}

void CLEAR_LINE(int X,int Y,int WIDTH){
	SET_CORD(X,Y);
	for(int i=0;i<WIDTH;i++) cout<<" ";
}

int searchforpng(string directory){
	int flag = 0,count = 0;
	DIR *dir;
	struct dirent *ent;
	if ((dir=opendir(directory.c_str()))!=NULL){
	  while((ent=readdir(dir))!=NULL) {
	  	
	  	   char* file_name = ent->d_name;
	  	   
		   if(strcmp(file_name,".")&&strcmp(file_name,"..")){
	  	   		string _file_name = file_name;
	  	   		string file_ext = "NULL";
	  	   		string _file_ext = "NULL";
	  	   		if(_STR_LEN(_file_name)>4){
	  	   			file_ext = _file_name.substr(_STR_LEN(_file_name)-4);
	  	   		}
	  	   		if(_STR_LEN(_file_name)>6){
	  	   			_file_ext = _file_name.substr(_STR_LEN(_file_name)-6);
	  	   		}
				if(strcmp(file_ext.c_str(),".png")) continue;
				else if(!(strcmp(_file_ext.c_str(),".9.png"))) continue;
				else count++;
		   }
		   
	  }
	  closedir(dir);
	}
	if(count>0) flag = 1;
	return flag;
}

void processpng(string directory){
	DIR *dir;
	struct dirent *ent;
	if ((dir=opendir(directory.c_str()))!=NULL) {
	  while ((ent = readdir (dir)) != NULL) {
	  	
	  	   char* file_name = ent->d_name;
	  	   
		   if(strcmp(file_name,".")&&strcmp(file_name,"..")){
	  	   		string _file_name = file_name;
	  	   		string file_ext = "NULL";
	  	   		string _file_ext = "NULL";
	  	   		if(_STR_LEN(_file_name)>4){
	  	   			file_ext = _file_name.substr(_STR_LEN(_file_name)-4);
	  	   		}
	  	   		if(_STR_LEN(_file_name)>6){
	  	   			_file_ext = _file_name.substr(_STR_LEN(_file_name)-6);
	  	   		}
				if(strcmp(file_ext.c_str(),".png")) continue;
				else if(!(strcmp(_file_ext.c_str(),".9.png"))) continue;
				else {
					
					string cmd="call res\\pngquant --ext _C.png --force \"";
					cmd+=directory;
					cmd+=_file_name;
					cmd+="\"";
				   	const char* _cmd_temp_1 = cmd.c_str();
				   	char _cmd_1[strlen(_cmd_temp_1)];
				   	strcpy(_cmd_1,_cmd_temp_1);
					exec(_cmd_1);
					
					cmd="IF EXIST \"";
					cmd+=directory;
					cmd+=_file_name.substr(0,(_STR_LEN(_file_name)-4));
					cmd+="_C.png";
					cmd+="\" ( del /F /Q \"";
					cmd+=directory;
					cmd+=_file_name;
					cmd+="\" )";
				   	const char* _cmd_temp_2 = cmd.c_str();
				   	char _cmd_2[strlen(_cmd_temp_2)];
				   	strcpy(_cmd_2,_cmd_temp_2);
					exec(_cmd_2);
				   	
					cmd="ren \"";
					cmd+=directory;
					cmd+=_file_name.substr(0,(_STR_LEN(_file_name)-4));
					cmd+="_C.png";
					cmd+="\" \"";
					cmd+=_file_name;
					cmd+="\"";
				   	const char* _cmd_temp_3 = cmd.c_str();
				   	char _cmd_3[strlen(_cmd_temp_3)];
				   	strcpy(_cmd_3,_cmd_temp_3);	
					exec(_cmd_3);
				
				}
		   }
		   
	  }
	  closedir(dir);
	}
}

int main(){
 	main:
 	system("cls");
 	cout<<setprecision(5);
	if(!success_flag) SET_CONSOLE_DIMENSIONS(800,370);
	else SET_CONSOLE_DIMENSIONS(800,360);
 	CURSOR_TOGGLE(0);
	for(int i=0;i<=26;i++){
		for(int j=0;j<80;j++){
			cout<<"#";
		}
	}
	SET_CORD(0,0);
	for(int i=3;i<=23;i++){
		for(int j=5;j<=74;j++){
			SET_CORD(j,i);
			cout<<" ";
		}
	}
	for(int i=0;i<5;i++){
		SET_CORD(6,i+5);
		cout<<HEADER_FRAME[i];
	}
	for(int i=5;i<=74;i++){
		SET_CORD(i,12);
		cout<<"#";
	}
	SET_CORD(7,14);
	cout<<"By - Divya Mamgai                                          Ver.1.1";
	for(int i=5;i<=74;i++){
		SET_CORD(i,16);
		cout<<"#";
	}
	if(!success_flag){
		SET_CORD(7,18);
		cout<<"Please move all the system files to the folder - working";
		for(int i=5;i<=74;i++){
			SET_CORD(i,20);
			cout<<"#";
		}
		SET_CORD(7,22);
		cout<<"Press Enter once all the files are moved. [Press Escape to exit.]";
		SET_CORD(0,0);
	}else{
						
    		DIR *_dir;
			struct dirent *_ent;
			if ((_dir = opendir ("build")) != NULL) {
			  while ((_ent = readdir (_dir)) != NULL) {
			  	   char* apkname = _ent->d_name;
				   if(strcmp(apkname,".")&&strcmp(apkname,"..")){
			  	   		string _apkname = apkname;
					   	string apkext = "NULL";
					   	if(_STR_LEN(_apkname)>4) apkext = _apkname.substr(_STR_LEN(_apkname)-4);
						if(strcmp(apkext.c_str(),".apk")) continue;
						else {
							string cmd;
							cmd="ren \"%0\\..\\build\\";
							cmd+=_apkname;
							cmd+="\" \"";
							cmd+=_replace(_apkname,"$"," ");
							cmd+="\"";
						   	const char* _cmd_temp = cmd.c_str();
						   	char _cmd[strlen(_cmd_temp)];
						   	strcpy(_cmd,_cmd_temp);
							exec(_cmd);
						}
				   }
				   
			  }
			  closedir (_dir);
			} else cout<<"An ERROR!";
				
    		DIR *__dir;
			struct dirent *__ent;
			if ((__dir = opendir ("working")) != NULL) {
			  while ((__ent = readdir (__dir)) != NULL) {
			  	   char* apkname = __ent->d_name;
				   if(strcmp(apkname,".")&&strcmp(apkname,"..")){
			  	   		string _apkname = apkname;
					   	string apkext = "NULL";
					   	if(_STR_LEN(_apkname)>4) apkext = _apkname.substr(_STR_LEN(_apkname)-4);
						if(strcmp(apkext.c_str(),".apk")) continue;
						else {
							string cmd;
							cmd="ren \"%0\\..\\working\\";
							cmd+=_apkname;
							cmd+="\" \"";
							cmd+=_replace(_apkname,"$"," ");
							cmd+="\"";
						   	const char* _cmd_temp = cmd.c_str();
						   	char _cmd[strlen(_cmd_temp)];
						   	strcpy(_cmd,_cmd_temp);
							exec(_cmd);
						}
				   }
				   
			  }
			  closedir (_dir);
			} else cout<<"An ERROR!";		

		SET_CORD(7,18);
		__int64 _working=TransverseDirectory("working\\"),_build=TransverseDirectory("build\\");
		double working = static_cast<double>(_working);
		double build = static_cast<double>(_build);
		double compression_ratio = (build/working)*100;
		if(compression_ratio>100) cout<<"Failed to compress!";
		else cout<<"Success! Your system apps are successfully compressed!";
		SET_CORD(7,20);
		cout<<"The original size of all the system apps was "<<((working/1024)/1024)<<" MB.";
		SET_CORD(7,21);
		cout<<"The new size of all the system apps is "<<((build/1024)/1024)<<" MB.";
		SET_CORD(7,22);
		cout<<"The Compression ratio was "<<compression_ratio<<"%.";		
		success_flag = 0;
		getch();
		goto main;
	}
	
        int COUNT = 0;
		DWORD MODE;
        INPUT_RECORD EVENT;
        BOOL FLAG = TRUE;
        HANDLE HSTDINHANDLE = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(HSTDINHANDLE,&MODE);
        SetConsoleMode(HSTDINHANDLE,0);
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));                 // To flush the non-required previous data.
        while(TRUE)
        {
            if((WaitForSingleObject(HSTDINHANDLE,0)==WAIT_OBJECT_0)&&(COUNT!=0))
            {
                DWORD CONSOLE_INPUT_COUNT;
                ReadConsoleInput(HSTDINHANDLE,&EVENT,1,&CONSOLE_INPUT_COUNT);
                if((EVENT.EventType==KEY_EVENT)&&(EVENT.Event.KeyEvent.bKeyDown))
                {

                    		int no_of_files = 0;
							int no_of_files_done = 0;
							float percent_completed = 0.0;
							
                    switch(EVENT.Event.KeyEvent.wVirtualKeyCode)
                    {
                    	case VK_RETURN:
                    		
                    		DIR *_dir;
							struct dirent *_ent;
							if ((_dir=opendir("working"))!=NULL){
							  while ((_ent=readdir(_dir))!=NULL){
							  	   char* apkname = _ent->d_name;
								   if(strcmp(apkname,".")&&strcmp(apkname,"..")){
							  	   		string _apkname = apkname;
							  	   		string apkext = _apkname.substr(_STR_LEN(_apkname)-4);
										if(strcmp(apkext.c_str(),".apk")) continue;
										else {
											string cmd;
											cmd="ren \"%0\\..\\working\\";
											cmd+=_apkname;
											cmd+="\" \"";
											cmd+=_replace(_apkname," ","$");
											cmd+="\"";
										   	const char* _cmd_temp = cmd.c_str();
										   	char _cmd[strlen(_cmd_temp)];
										   	strcpy(_cmd,_cmd_temp);
											exec(_cmd);
											no_of_files++;
										}
								   }
								   
							  }
							  closedir(_dir);
							}
							                		
                    		DIR *__dir;
							struct dirent *__ent;
							if ((__dir=opendir("build"))!=NULL){
							  while ((__ent=readdir(__dir))!=NULL){
							  	   char* apkname = __ent->d_name;
								   if(strcmp(apkname,".")&&strcmp(apkname,"..")){
							  	   		string _apkname = apkname;
							  	   		string apkext = _apkname.substr(_STR_LEN(_apkname)-4);
										if(strcmp(apkext.c_str(),".apk")) continue;
										else {
											string cmd;
											cmd="ren \"%0\\..\\build\\";
											cmd+=_apkname;
											cmd+="\" \"";
											cmd+=_replace(_apkname," ","$");
											cmd+="\"";
										   	const char* _cmd_temp = cmd.c_str();
										   	char _cmd[strlen(_cmd_temp)];
										   	strcpy(_cmd,_cmd_temp);
											exec(_cmd);
										}
								   }
								   
							  }
							  closedir(__dir);
							}
							
							if(no_of_files>0){
								
								SET_CONSOLE_DIMENSIONS(800,420);
								for(int i=26;i<=30;i++){
									for(int j=0;j<80;j++){
										SET_CORD(j,i);
										cout<<"#";
									}
								}
								SET_CORD(0,0);

								SET_CORD(7,18);
								for(int i=5;i<=72;i++){
									cout<<" ";
								}
								SET_CORD(7,22);
								for(int i=5;i<=72;i++){
									cout<<" ";
								}
								for(int i=25;i<=27;i++){
									for(int j=5;j<=74;j++){
										SET_CORD(j,i);
										cout<<" ";
									}
								}
								SET_CORD(7,18);
								cout<<"Current File: ";
								for(int i=1;i<=3;i++){
									SET_CORD(49,16+i);
									cout<<"##";
								}
								for(int i=1;i<=3;i++){
									SET_CORD(62,16+i);
									cout<<"##";
								}
								SET_CORD(7,22);
								cout<<"Current Process: ";
								SET_CORD(39,26);
								cout<<"0 %";
								SET_CORD(53,18);
								cout<<"T: "<<no_of_files;
								SET_CORD(66,18);
								cout<<"C: "<<no_of_files_done;
								
	                    		DIR *dir;
								struct dirent *ent;
								if ((dir = opendir ("working")) != NULL) {
								  while ((ent = readdir (dir)) != NULL) {
									   char* apkname = ent->d_name;
									   if(strcmp(apkname,".")&&strcmp(apkname,"..")){
											
										   	string _apkname = apkname;
										   	string apkext = "NULL";
										   	if(_STR_LEN(_apkname)>4) apkext = _apkname.substr(_STR_LEN(_apkname)-4);
										   	
											if(strcmp(apkext.c_str(),".apk")) continue;
										   	
											string cmd = "call res\\7za x %0\\..\\working\\";
										   	cmd+=_apkname;
										   	cmd+=" -o%0\\..\\temp\\";
										   	cmd+=_apkname.substr(0,(_STR_LEN(_apkname)-4));
										   	cmd+=" -y";
										   	const char* _cmd_temp_1 = cmd.c_str();
										   	char _cmd_1[strlen(_cmd_temp_1)];
										   	strcpy(_cmd_1,_cmd_temp_1);
										   	CLEAR_LINE(21,18,25);
										   	SET_CORD(21,18);
										   	string __apkname = _replace(_apkname,"$"," ");
										   	__apkname = __apkname.substr(0,_STR_LEN(__apkname)-4);
										   	if(_STR_LEN(__apkname)>18){
										   		cout<<__apkname.substr(0,18)<<"....apk";
										   	}else{
										   		cout<<__apkname;
										   	}
										   	
										   	_apkname = apkname;									   	
										   	
										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Extracting apk";
											exec(_cmd_1);						
									
										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Processing PNG files [In the res directory]";
																						
					                    		DIR *dir;
												struct dirent *ent;
												string folder_dir = "%0\\..\\temp\\";
												folder_dir+=_apkname.substr(0,(_STR_LEN(_apkname)-4));
												folder_dir+="\\res\\";
												if ((dir = opendir (folder_dir.c_str())) != NULL) {
												  while ((ent = readdir (dir)) != NULL) {
												  	   char* folder_name = ent->d_name;
													   if(strcmp(folder_name,".")&&strcmp(folder_name,"..")){
													   		string _folder_dir = folder_dir;
													   		_folder_dir+=folder_name;
													   		_folder_dir+="\\";
															if(searchforpng(_folder_dir)){
																processpng(_folder_dir);
															}
													   }
													   
												  }
												  closedir(dir);
												}else{
										   		  CLEAR_LINE(24,22,51);
												  SET_CORD(24,22);
												  cout<<"No PNG files found, skipping";
												  Sleep(3000);
												}
									
										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Processing PNG files [In the assets directory]";
											
												folder_dir = "%0\\..\\temp\\";
												folder_dir+=_apkname.substr(0,(_STR_LEN(_apkname)-4));
												folder_dir+="\\assets\\";
												if(searchforpng(folder_dir)){
													processpng(folder_dir);
												}
									   	
										   	cmd="call res\\7za a -tzip %0\\..\\build\\";
										   	cmd+=_apkname;
										   	cmd+=" %0\\..\\temp\\";
										   	cmd+=_apkname.substr(0,(_STR_LEN(_apkname)-4));
										   	cmd+="\\* -mx9 -y";
										   	const char* _cmd_temp_2 = cmd.c_str();
										   	char _cmd_2[strlen(_cmd_temp_2)];
										   	strcpy(_cmd_2,_cmd_temp_2);
										   	
										   	_apkname = apkname;
										   	
										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Packing apk";
											exec(_cmd_2);
											
											cmd="java -jar %0\\..\\res\\signapk.jar %0\\..\\res\\testkey.x509.pem %0\\..\\res\\testkey.pk8 %0\\..\\build\\";	
											cmd+=_apkname;
											cmd+=" %0\\..\\build\\";
											cmd+=_apkname.substr(0,(_STR_LEN(_apkname)-4));
											cmd+="-Signed.apk";
										   	const char* _cmd_temp_3 = cmd.c_str();
										   	char _cmd_3[strlen(_cmd_temp_3)];
										   	strcpy(_cmd_3,_cmd_temp_3);
										   	
										   	_apkname = apkname;
										   	
										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Signing apk";
											exec(_cmd_3);

											cmd="del %0\\..\\build\\";
											cmd+=_apkname;
										   	const char* _cmd_temp_4 = cmd.c_str();
										   	char _cmd_4[strlen(_cmd_temp_4)];
										   	strcpy(_cmd_4,_cmd_temp_4);
											exec(_cmd_4);
											
											_apkname = apkname;
											
											cmd="ren \"%0\\..\\build\\";
											cmd+=_apkname.substr(0,(_STR_LEN(_apkname)-4));
											cmd+="-Signed.apk";
											cmd+="\" \"";
											cmd+=_apkname;
											cmd+="\"";
										   	const char* _cmd_temp_5 = cmd.c_str();
										   	char _cmd_5[strlen(_cmd_temp_5)];
										   	strcpy(_cmd_5,_cmd_temp_5);
										   	
										   	_apkname = apkname;
										   	
										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Processing some files";
											exec(_cmd_5);
											
											cmd="call res\\zipalign -f 4 %0\\..\\build\\";
											cmd+=_apkname;
											cmd+=" %0\\..\\build\\";
											cmd+=_apkname.substr(0,(_STR_LEN(_apkname)-4));
											cmd+="-Zipaligned.apk";
										   	const char* _cmd_temp_6 = cmd.c_str();
										   	char _cmd_6[strlen(_cmd_temp_6)];
										   	strcpy(_cmd_6,_cmd_temp_6);
										   	
										   	_apkname = apkname;

										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Zipaligning apk";
											exec(_cmd_6);
											
											cmd="del \"%0\\..\\build\\";
											cmd+=_apkname;
											cmd+="\"";
										   	const char* _cmd_temp_7 = cmd.c_str();
										   	char _cmd_7[strlen(_cmd_temp_7)];
										   	strcpy(_cmd_7,_cmd_temp_7);
											exec(_cmd_7);
											
											_apkname = apkname;
											
											cmd="ren \"%0\\..\\build\\";
											cmd+=_apkname.substr(0,(_STR_LEN(_apkname)-4));
											cmd+="-Zipaligned.apk";
											cmd+="\" \"";
											cmd+=_apkname;
											cmd+="\"";
										   	const char* _cmd_temp_8 = cmd.c_str();
										   	char _cmd_8[strlen(_cmd_temp_8)];
										   	strcpy(_cmd_8,_cmd_temp_8);
										   	
										   	_apkname = apkname;
										   	
										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Processing some files";
											exec(_cmd_8);
											
											cmd="IF EXIST \"%0\\..\\";
											cmd+=_apkname;
											cmd+="\"";
											cmd+=" del \"%0\\..\\";
											cmd+=_apkname;
											cmd+=".tmp\"";
										   	const char* _cmd_temp_9 = cmd.c_str();
										   	char _cmd_9[strlen(_cmd_temp_9)];
										   	strcpy(_cmd_9,_cmd_temp_9);
										   	
										   	_apkname = apkname;

										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Deleting temp files";
											exec(_cmd_9);
											
											cmd="rmdir /S /Q %0\\..\\temp\\";
											cmd+=_apkname.substr(0,(_STR_LEN(_apkname)-4));
										   	const char* _cmd_temp_10 = cmd.c_str();
										   	char _cmd_10[strlen(_cmd_temp_10)];
										   	strcpy(_cmd_10,_cmd_temp_10);
										   	
										   	_apkname = apkname;
										   	
										   	CLEAR_LINE(24,22,51);
											SET_CORD(24,22);
											cout<<"Deleting temp files";
											exec(_cmd_10);
											
											no_of_files_done++;
											percent_completed = (float)((float)(static_cast<float>(no_of_files_done)/static_cast<float>(no_of_files))*100.0);
											SET_CORD(69,18);
											cout<<no_of_files_done;
											
											for(int i=0;i<=(percent_completed*0.65);i++){
												if(i==65){
													if(i==31||i==32||i==33||i==34||i==35||i==36||i==37||i==38) continue;
												}else{
													if(i==31||i==32||i==33||i==34||i==35||i==36) continue;
												}
												SET_CORD(7+i,26);
												cout<<"#";
											}
																
											SET_CORD(39,26);
											cout<<(int)(percent_completed)<<" %";
											if((int)(percent_completed)==100){
												 SET_CORD(44,26);
												 cout<<" ";
											}
											
									   }
								  }
								  closedir (dir);
								  if(no_of_files_done==no_of_files){
								  	 success_flag = 1;
								  	 goto main;
								  }
								  else{
								   	  CLEAR_LINE(24,22,51);
 									  SET_CORD(24,22);
 									  cout<<"An unexpected error occurred!";
								  }
								} else cout<<"An ERROR!";
							}else{
								CLEAR_LINE(7,22,65);
								SET_CORD(7,22);
								cout<<"Sorry no file(s) found. Press Enter again to retry.";
							}							
                        break;
                    case VK_ESCAPE:
                    		SET_CORD(0,50);
							exit(0);
                    	break;
                    }
                }
            }
            COUNT++;
        }     
	return 0;
}
