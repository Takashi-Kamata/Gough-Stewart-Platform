function varargout = stewart_platform(varargin)
% STEWART_PLATFORM MATLAB code for stewart_platform.fig
%      STEWART_PLATFORM, by itself, creates a new STEWART_PLATFORM or raises the existing
%      singleton*.
%
%      H = STEWART_PLATFORM returns the handle to a new STEWART_PLATFORM or the handle to
%      the existing singleton*.
%
%      STEWART_PLATFORM('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in STEWART_PLATFORM.M with the given input arguments.
%
%      STEWART_PLATFORM('Property','Value',...) creates a new STEWART_PLATFORM or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before stewart_platform_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to stewart_platform_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help stewart_platform

% Last Modified by GUIDE v2.5 22-Sep-2022 14:09:10

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @stewart_platform_OpeningFcn, ...
                   'gui_OutputFcn',  @stewart_platform_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before stewart_platform is made visible.
function stewart_platform_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to stewart_platform (see VARARGIN)

% Choose default command line output for stewart_platform
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes stewart_platform wait for user response (see UIRESUME)
% uiwait(handles.stewart_platform_panel);
% Initialise variables
handles.trans=[0 0 0];
handles.orient=[0 0 0 ];
handles.ardu=0;
handles.arduino_status=0;
handles.servos=0;
handles.toggle=0;
do_the_stewart(handles);
guidata(hObject, handles);


% --- Outputs from this function are returned to the command line.
function varargout = stewart_platform_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in exit_pushbutton.
function exit_pushbutton_Callback(hObject, eventdata, handles)
% hObject    handle to exit_pushbutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%Exit the GUI
close all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% X-Slider %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on slider movement.
function x_slider_Callback(hObject, eventdata, handles)
% hObject    handle to x_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
max_x= 10;           %Maximum translation in x direction
min_x= -10;          %Minimum translation in x direction

slide= get(hObject,'Value');

handles.x= slide * (max_x - min_x)  + min_x;
x_pos= num2str(handles.x);
handles.trans(1)= handles.x;

set(handles.show_x,'String',x_pos);
guidata(hObject, handles);

do_the_stewart(handles);

% --- Executes during object creation, after setting all properties.
function x_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to x_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end
% Initial Value
max_x= 4;           %Maximum translation in x direction
min_x= -4;          %Minimum translation in x direction

slide= get(hObject,'Value');
handles.x= slide * (max_x - min_x)  + min_x;
handles.trans(1)= handles.x;
guidata(hObject, handles)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% Y-Slider %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on slider movement.
function y_slider_Callback(hObject, eventdata, handles)
% hObject    handle to y_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
max_y= 10;           %Maximum translation in x direction
min_y= -10;          %Minimum translation in x direction

slide= get(hObject,'Value');

handles.y= slide * (max_y - min_y)  + min_y;
y_pos= num2str(handles.y);
handles.trans(2)= handles.y;

set(handles.show_y,'String',y_pos);
guidata(hObject, handles); 

do_the_stewart(handles);
                                       
% --- Executes during object creation, after setting all properties.
function y_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to y_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end
% Initial Value
max_y= 4;           %Maximum translation in x direction
min_y= -4;          %Minimum translation in x direction

slide= get(hObject,'Value');
handles.y= slide * (max_y - min_y)  + min_y;
handles.trans(2)= handles.y;
guidata(hObject, handles); 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% Z-Slider %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on slider movement.
function z_slider_Callback(hObject, eventdata, handles)
% hObject    handle to z_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
max_z= 10;           %Maximum translation in x direction
min_z= -10;          %Minimum translation in x direction

slide= get(hObject,'Value');

handles.z= slide * (max_z - min_z)  + min_z;
z_pos= num2str(handles.z);
handles.trans(3)= handles.z;

set(handles.show_z,'String',z_pos);
guidata(hObject, handles);

do_the_stewart(handles);


% --- Executes during object creation, after setting all properties.
function z_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to z_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end
% Initial Value
max_z= 4;           %Maximum translation in x direction
min_z= -4;          %Minimum translation in x direction

slide= get(hObject,'Value');
handles.z= slide * (max_z - min_z)  + min_z;
handles.trans(3)= handles.z;
guidata(hObject, handles);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% Roll-Slider %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on slider movement.
function roll_slider_Callback(hObject, eventdata, handles)
% hObject    handle to roll_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
max_roll= 15*pi/180;        %Maximum roll angle
min_roll= -15*pi/180;          %Minimum roll angle

slide= get(hObject,'Value');

handles.roll= slide * (max_roll - min_roll)  + min_roll;
roll_pos= num2str(handles.roll*180/pi);
handles.orient(1)= handles.roll;

set(handles.show_roll,'String',roll_pos);
guidata(hObject, handles);

do_the_stewart(handles);                                       


% --- Executes during object creation, after setting all properties.
function roll_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to roll_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end
% Initial Value
max_roll= 15*pi/180;        %Maximum roll angle
min_roll= -15*pi/180;          %Minimum roll angle

slide= get(hObject,'Value');
handles.roll= slide * (max_roll - min_roll)  + min_roll;
handles.orient(1)= handles.roll;
guidata(hObject, handles);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% Pitch-Slider %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on slider movement.
function pitch_slider_Callback(hObject, eventdata, handles)
% hObject    handle to pitch_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
max_pitch= 15*pi/180;        %Maximum pitch angle
min_pitch= -15*pi/180;       %Minimum pitch angle

% Get Slider value
slide= get(hObject,'Value');
handles.pitch= slide * (max_pitch - min_pitch)  + min_pitch;
handles.orient(2)= handles.pitch;

% Show slider value
pitch_pos= num2str(handles.pitch*180/pi);
set(handles.show_pitch,'String',pitch_pos);

guidata(hObject, handles);

do_the_stewart(handles);
                                       

% --- Executes during object creation, after setting all properties.
function pitch_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pitch_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end
% Initial Value
max_pitch= 15*pi/180;        %Maximum pitch angle
min_pitch= -15*pi/180;       %Minimum pitch angle

% Get Slider value
slide= get(hObject,'Value');
handles.pitch= slide * (max_pitch - min_pitch)  + min_pitch;
handles.orient(2)= handles.pitch;
guidata(hObject, handles);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% Yaw-Slider %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on slider movement.
function yaw_slider_Callback(hObject, eventdata, handles)
% hObject    handle to yaw_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
max_yaw= 15*pi/180;        %Maximum yaw angle
min_yaw= -15*pi/180;       %Minimum yaw angle

slide= get(hObject,'Value');

handles.yaw= slide * (max_yaw - min_yaw)  + min_yaw;
yaw_pos= num2str(handles.yaw*180/pi);
handles.orient(3)= handles.yaw;

set(handles.show_yaw,'String',yaw_pos);
guidata(hObject, handles);

do_the_stewart(handles);

% --- Executes during object creation, after setting all properties.
function yaw_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to yaw_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end
% Initial Value
max_yaw= 15*pi/180;        %Maximum yaw angle
min_yaw= -15*pi/180;       %Minimum yaw angle

slide= get(hObject,'Value');

handles.yaw= slide * (max_yaw - min_yaw)  + min_yaw;
handles.orient(3)= handles.yaw;
guidata(hObject, handles);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% Show handles %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on button press in show_handle_pushbutton.
function show_handle_pushbutton_Callback(hObject, eventdata, handles)
% hObject    handle to show_handle_pushbutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
disp(handles);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% Geometry Input %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function r_B_inputbox_Callback(hObject, eventdata, handles)
% hObject    handle to r_B_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of r_B_inputbox as text
%        str2double(get(hObject,'String')) returns contents of r_B_inputbox as a double
handles.r_B= str2num(get(hObject,'String'));
guidata(hObject,handles);

do_the_stewart(handles);                                       

% --- Executes during object creation, after setting all properties.
function r_B_inputbox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to r_B_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% Initial Value
handles.r_B= str2num(get(hObject,'String'));
guidata(hObject,handles);

function r_P_inputbox_Callback(hObject, eventdata, handles)
% hObject    handle to r_P_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of r_P_inputbox as text
%        str2double(get(hObject,'String')) returns contents of r_P_inputbox as a double
handles.r_P= str2num(get(hObject,'String'));
guidata(hObject,handles);

do_the_stewart(handles); 

% --- Executes during object creation, after setting all properties.
function r_P_inputbox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to r_P_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% Initial Value
handles.r_P= str2num(get(hObject,'String'));
guidata(hObject,handles);



function rod_length_inputbox_Callback(hObject, eventdata, handles)
% hObject    handle to rod_length_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of rod_length_inputbox as text
%        str2double(get(hObject,'String')) returns contents of rod_length_inputbox as a double
handles.rod_length= str2num(get(hObject,'String'));
guidata(hObject,handles);

do_the_stewart(handles);
                                       
                                       
                                       
% --- Executes during object creation, after setting all properties.
function rod_length_inputbox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to rod_length_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% Initial Value
handles.rod_length= str2num(get(hObject,'String'));
guidata(hObject,handles);

function alpha_B_inputbox_Callback(hObject, eventdata, handles)
% hObject    handle to alpha_B_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of alpha_B_inputbox as text
%        str2double(get(hObject,'String')) returns contents of alpha_B_inputbox as a double
handles.alpha_B= str2num(get(hObject,'String'));
guidata(hObject,handles);

do_the_stewart(handles);
                                       
% --- Executes during object creation, after setting all properties.
function alpha_B_inputbox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to alpha_B_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% Initial value
handles.alpha_B= str2num(get(hObject,'String'));
guidata(hObject,handles);

function alpha_P_inputbox_Callback(hObject, eventdata, handles)
% hObject    handle to alpha_P_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of alpha_P_inputbox as text
%        str2double(get(hObject,'String')) returns contents of alpha_P_inputbox as a double
handles.alpha_P= str2num(get(hObject,'String'));
guidata(hObject,handles);

do_the_stewart(handles);



% --- Executes during object creation, after setting all properties.
function alpha_P_inputbox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to alpha_P_inputbox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% Initial value
handles.alpha_P= str2num(get(hObject,'String'));
guidata(hObject,handles);

% --- Executes on button press in roll_pitch_sweep_pushbutton.
function roll_pitch_sweep_pushbutton_Callback(hObject, eventdata, handles)
% hObject    handle to roll_pitch_sweep_pushbutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.roll_pitch_sweep_pushbutton,'BackgroundColor','green');
for i=0:0.1:2*pi
    handles.orient=[10*pi/180*cos(i) 10*pi/180*sin(i) 0];
    
    do_the_stewart(handles);
    drawnow
end
for i=2*pi:-0.1:1
    handles.orient=[10*pi/180*cos(i) 10*pi/180*sin(i) 0];
    
    do_the_stewart(handles);
    drawnow
end
set(handles.roll_pitch_sweep_pushbutton,'BackgroundColor','default');
handles.orient=[0 0 0];
guidata(hObject,handles);

% --- Executes on button press in trans_sweep_pushbutton.
function trans_sweep_pushbutton_Callback(hObject, eventdata, handles)
% hObject    handle to trans_sweep_pushbutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.trans_sweep_pushbutton,'BackgroundColor','green');
for i=0:0.1:2*pi
    handles.trans=[4*sin(i) 0 0];
    
    do_the_stewart(handles);
    drawnow
end
for i=0:0.1:2*pi
    handles.trans=[0 4*sin(i) 0];
    
    do_the_stewart(handles);
    drawnow
end
for i=0:0.1:2*pi
    handles.trans=[0 0 2*sin(i)];
    
    do_the_stewart(handles);
    drawnow
end
set(handles.trans_sweep_pushbutton,'BackgroundColor','default');
handles.orient=[0 0 0];
guidata(hObject,handles);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function do_the_stewart(handles)
%Does the stewart: - Calculates the new angle for given geometry and wanted
%                    position
%                  - Sets the servos connnected with the arduino to the
%                    calculated angles
tic;
imag_count=0;
global leg_length
leg_length = calculate_stewart_platform(handles.r_B,...
                                   handles.r_P,...
                                   handles.rod_length,...
                                   handles.alpha_B*pi/180,...
                                   handles.alpha_P*pi/180,...
                                   handles.trans,... 
                                   handles.orient);
leg_length = fix((leg_length./2.54 - 12)*100);
lag=toc;
disp('Calc duration: ')
disp(lag)


% --- Executes when user attempts to close stewart_platform_panel.
function stewart_platform_panel_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to stewart_platform_panel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hint: delete(hObject) closes the figure
global serialportObj
delete(hObject);
delete(serialportObj);
close all
clear
clc

% --- Executes on button press in pushbutton9.
function pushbutton9_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
disp("Init called");
set(hObject,'Enable','off');
global serialportObj
serialportObj = serialport("COM12",115200);
configureTerminator(serialportObj,"CR");
while 1
    readline(serialportObj)
end


% --- Executes on button press in pushbutton10.
function pushbutton10_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
disp("send")
global serialportObj
global leg_length
writeline(serialportObj,strcat("A",string(leg_length(1))));
writeline(serialportObj,strcat("B",string(leg_length(2))));
writeline(serialportObj,strcat("C",string(leg_length(3))));
writeline(serialportObj,strcat("D",string(leg_length(4))));
writeline(serialportObj,strcat("E",string(leg_length(5))));
writeline(serialportObj,strcat("F",string(leg_length(6))));

% --- Executes on button press in pushbutton11. halt
function pushbutton11_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global serialportObj
writeline(serialportObj,strcat("G","1"));

% --- Executes on button press in pushbutton12. extend all
function pushbutton12_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global serialportObj
writeline(serialportObj,strcat("H","1"));

% --- Executes on button press in pushbutton13. retract all
function pushbutton13_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global serialportObj
writeline(serialportObj,strcat("I","1"));
