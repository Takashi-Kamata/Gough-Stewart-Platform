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

% Last Modified by GUIDE v2.5 29-Sep-2022 17:30:16

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
delete(hObject);
clearvars  -global initialised serialportObj leg_length global_handles progress progress_max
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
max_x= 35;           %Maximum translation in x direction
min_x= -35;          %Minimum translation in x direction

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
    set(hObject,'BackgroundColor',[.94 .94 .94]);
end
% Initial Value
max_x= 35;           %Maximum translation in x direction
min_x= -35;          %Minimum translation in x direction

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
max_y= 35;           %Maximum translation in x direction
min_y= -35;          %Minimum translation in x direction

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
    set(hObject,'BackgroundColor',[.94 .94 .94]);
end
% Initial Value
max_y= 35;           %Maximum translation in x direction
min_y= -35;          %Minimum translation in x direction

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
max_z= 35;           %Maximum translation in x direction
min_z= -35;          %Minimum translation in x direction

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
    set(hObject,'BackgroundColor',[.94 .94 .94]);
end
% Initial Value
max_z= 35;           %Maximum translation in x direction
min_z= -35;          %Minimum translation in x direction

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
max_roll= 180*pi/180;        %Maximum roll angle
min_roll= -180*pi/180;          %Minimum roll angle

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
    set(hObject,'BackgroundColor',[.94 .94 .94]);
end
% Initial Value
max_roll= 180*pi/180;        %Maximum roll angle
min_roll= -180*pi/180;          %Minimum roll angle

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
max_pitch= 180*pi/180;        %Maximum pitch angle
min_pitch= -180*pi/180;       %Minimum pitch angle

% Get Slider value
slide= get(hObject,'Value');
slide * (max_pitch - min_pitch)  + min_pitch;
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
    set(hObject,'BackgroundColor',[.94 .94 .94]);
end
% Initial Value
max_pitch= 180*pi/180;        %Maximum pitch angle
min_pitch= -180*pi/180;       %Minimum pitch angle

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
max_yaw= 180*pi/180;        %Maximum yaw angle
min_yaw= -180*pi/180;       %Minimum yaw angle

slide = get(hObject,'Value');

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
    set(hObject,'BackgroundColor',[.94 .94 .94]);
end
% Initial Value
max_yaw= 180*pi/180;        %Maximum yaw angle
min_yaw= -180*pi/180;       %Minimum yaw angle

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
global leg_length
leg_length = calculate_stewart_platform(handles.r_B,...
                                   handles.r_P,...
                                   handles.rod_length,...
                                   handles.alpha_B*pi/180,...
                                   handles.alpha_P*pi/180,...
                                   handles.trans,... 
                                   handles.orient);
lag=toc;
% Bounding
bad = zeros(1,6);
for i=1:6
    if leg_length(i) > (12*2.54 + handles.rod_length) || leg_length(i) < (handles.rod_length)
        bad(i) = 1;
    end
end


leg_length = fix((leg_length - handles.rod_length)./2.54*100);
if (~isempty(whos('global','initialised'))) && ~(max(bad)>0) 
    global serialportObj    
    writeline(serialportObj,strcat("A",string(leg_length(1))));
    writeline(serialportObj,strcat("B",string(leg_length(2))));
    writeline(serialportObj,strcat("C",string(leg_length(3))));
    writeline(serialportObj,strcat("D",string(leg_length(4))));
    writeline(serialportObj,strcat("E",string(leg_length(5))));
    writeline(serialportObj,strcat("F",string(leg_length(6))));
    writeline(serialportObj,"J");
end

set(handles.m1_length, "String", fix(leg_length(1)/12) + "%");
set(handles.m2_length, "String", fix(leg_length(2)/12) + "%");
set(handles.m3_length, "String", fix(leg_length(3)/12) + "%");
set(handles.m4_length, "String", fix(leg_length(4)/12) + "%");
set(handles.m5_length, "String", fix(leg_length(5)/12) + "%");
set(handles.m6_length, "String", fix(leg_length(6)/12) + "%");

if bad(1) == 1
    set(handles.m1_length,'BackgroundColor',[1 0 0]);
else
    set(handles.m1_length,'BackgroundColor',[.94 .94 .94]);
end

if bad(2) == 1
    set(handles.m2_length,'BackgroundColor',[1 0 0]);
else
    set(handles.m2_length,'BackgroundColor',[.94 .94 .94]);
end

if bad(3) == 1
    set(handles.m3_length,'BackgroundColor',[1 0 0]);
else
    set(handles.m3_length,'BackgroundColor',[.94 .94 .94]);
end

if bad(4) == 1
    set(handles.m4_length,'BackgroundColor',[1 0 0]);
else
    set(handles.m4_length,'BackgroundColor',[.94 .94 .94]);
end  

if bad(5) == 1
    set(handles.m5_length,'BackgroundColor',[1 0 0]);
else
    set(handles.m5_length,'BackgroundColor',[.94 .94 .94]);
end   

if bad(6) == 1
    set(handles.m6_length,'BackgroundColor',[1 0 0]);
else
    set(handles.m6_length,'BackgroundColor',[.94 .94 .94]);
end

disp('Calc duration: ')
disp(lag)


% --- Executes when user attempts to close stewart_platform_panel.
function stewart_platform_panel_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to stewart_platform_panel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hint: delete(hObject) closes the figure
clearvars  -global initialised serialportObj leg_length global_handles progress progress_max
 
delete(hObject);

function enable_interface()
% Enable Sliders
global global_handles
set(global_handles.x_slider,'Enable','on');
set(global_handles.y_slider,'Enable','on');
set(global_handles.z_slider,'Enable','on');
set(global_handles.pitch_slider,'Enable','on');
set(global_handles.yaw_slider,'Enable','on');
set(global_handles.roll_slider,'Enable','on');

% Enable Buttons
set(global_handles.pushbutton10,'Enable','on');
set(global_handles.pushbutton11,'Enable','on');
set(global_handles.pushbutton12,'Enable','on');
set(global_handles.pushbutton13,'Enable','on');
set(global_handles.pushbutton14,'Enable','on');
set(global_handles.pushbutton15,'Enable','on');

function disable_interface()
global global_handles
% Disable Sliders
set(global_handles.x_slider,'Enable','off');
set(global_handles.y_slider,'Enable','off');
set(global_handles.z_slider,'Enable','off');
set(global_handles.pitch_slider,'Enable','off');
set(global_handles.yaw_slider,'Enable','off');
set(global_handles.roll_slider,'Enable','off');

% Disable Buttons
set(global_handles.pushbutton10,'Enable','off');
set(global_handles.pushbutton11,'Enable','off');
set(global_handles.pushbutton12,'Enable','off');
set(global_handles.pushbutton13,'Enable','off');
set(global_handles.pushbutton14,'Enable','off');
set(global_handles.pushbutton15,'Enable','off');

% --- Executes on button press in pushbutton9.
function pushbutton9_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(hObject,'Enable','off');
global serialportObj
global initialised
initialised = 1;
global global_handles
global_handles = handles;
enable_interface();

set(handles.wavy,'Enable','on');
serialportObj = serialport("COM12",115200, "Timeout", 60);
configureTerminator(serialportObj,"CR/LF");
configureCallback(serialportObj,"terminator",@readSerialData)
flush(serialportObj);
writeline(serialportObj,strcat("L","1"));

function readSerialData(src,evt)
    global progress progress_max
    data = readline(src);
    if (~isempty(data))
        switch data
            case "O"
                update_progress();
            case "M"
                disable_interface();
                disp("Started Downloading")
            case "N"
                enable_interface();
                progress = progress_max;
                disp("Finished Downloading")
            otherwise
                disp(data)
        end
    end
    flush(src);

% --- Executes on button press in pushbutton10.
function pushbutton10_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
t = 0:.1:2*pi;
count = 1;

% magic
ang = atan((cos(t)));
disp = (9)*sin(t) + 16;
while count < length(t)
    handles.pitch = ang(count);
    handles.z = disp(count);
    handles.trans(3)= handles.z;
    handles.orient(2)= handles.pitch;
    guidata(hObject, handles);
    do_the_stewart(handles);
    count = count + 1;
    if ~(count < length(t))
       count = 1;
    end
end


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


% --- Executes on button press in pushbutton14.
function pushbutton14_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton14 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global serialportObj
writeline(serialportObj,strcat("K","1"));


% --- Executes on button press in pushbutton15.
function pushbutton15_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton15 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global serialportObj
writeline(serialportObj,strcat("J","1"));

function update_progress()
global progress
progress = progress + 1;



% --- Executes on button press in pushbutton17.
function pushbutton17_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton17 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.x_slider,'Value',0.5);
set(handles.y_slider,'Value',0.5);
set(handles.z_slider,'Value',0.5);
max = 35;           %Maximum translation in x direction
min = -35;          %Minimum translation in x direction
slide = 0.5;
handles.x= slide * (max - min)  + min;
x_pos= num2str(handles.x);
handles.trans(1)= handles.x;
set(handles.show_x,'String',x_pos);
handles.y= slide * (max - min)  + min;
y_pos= num2str(handles.y);
handles.trans(2)= handles.y;
set(handles.show_y,'String',y_pos);
handles.z= slide * (max - min)  + min;
z_pos= num2str(handles.z);
handles.trans(3)= handles.z;
set(handles.show_z,'String',z_pos);
guidata(hObject, handles);
do_the_stewart(handles);

% --- Executes on button press in pushbutton16.
function pushbutton16_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton16 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.roll_slider,'Value',0.5);
set(handles.pitch_slider,'Value',0.5);
set(handles.yaw_slider,'Value',0.5);
max= 180*pi/180;        %Maximum yaw angle
min= -180*pi/180;       %Minimum yaw angle
slide = 0.5;
handles.roll= slide * (max - min)  + min;
roll_pos= num2str(handles.roll*180/pi);
handles.orient(1)= handles.roll;
set(handles.show_roll,'String',roll_pos);
handles.pitch= slide * (max - min)  + min;
handles.orient(2)= handles.pitch;
pitch_pos= num2str(handles.pitch*180/pi);
set(handles.show_pitch,'String',pitch_pos);
handles.yaw= slide * (max - min)  + min;
yaw_pos= num2str(handles.yaw*180/pi);
handles.orient(3)= handles.yaw;
set(handles.show_yaw,'String',yaw_pos);
guidata(hObject, handles);
do_the_stewart(handles);


% --- Executes on button press in wavy.
function wavy_Callback(hObject, eventdata, handles)
% hObject    handle to wavy (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% writeline(serialportObj,strcat("M","1"));
% 
global progress progress_max
progress = 0;
f = waitbar(0, 'Starting');

%RYO's
custom_t = 0;
if (custom_t)
    ang_eq = @(t) (atan((cos(t))));

    t = [0];
    h = 1.0;
    tol = 0.5;
    angle = ang_eq(0);

    while t(end) < 2*pi
        t_1 = t(end) + h;

        angle_1 = ang_eq(t_1);
        error = abs(angle_1-angle);

        if error < tol
            angle = angle_1;
            t = [t, t_1];

            h = 1.5 * h;

            if (t(end) + h) > 2*pi
                h = 2*pi - t(end);
            end

        else
            h = 0.9 * h;
        end
    end
else
    t = 0:0.2:2*pi;
end

disp("Trajectory Points " + length(t))
progress_max = length(t) * 6;
ang = double(atan((cos(t)))/1.7);
dis = double((8)*sin(t) + 16);
global serialportObj
writeline(serialportObj,strcat("M","1"));
for index = 1:length(ang)
    leg_length = calculate_stewart_platform(handles.r_B,...
                                       handles.r_P,...
                                       handles.rod_length,...
                                       handles.alpha_B*pi/180,...
                                       handles.alpha_P*pi/180,...
                                       [0 0 dis(index)],... 
                                       [0.02 ang(index) 0]);
    % Bounding
    bad = zeros(1,6);
    for i=1:6
        if leg_length(i) > (12*2.54 + handles.rod_length) || leg_length(i) < (handles.rod_length)
            bad(i) = 1;
        end
    end
    if max(bad)==1
        disp("Bad math");
    end
    leg_length = fix((leg_length - handles.rod_length)./2.54*100);
    writeline(serialportObj,strcat("O",string(leg_length(1))));
    writeline(serialportObj,strcat("P",string(leg_length(2))));
    writeline(serialportObj,strcat("Q",string(leg_length(3))));
    writeline(serialportObj,strcat("R",string(leg_length(4))));
    writeline(serialportObj,strcat("S",string(leg_length(5))));
    writeline(serialportObj,strcat("T",string(leg_length(6))));
end
writeline(serialportObj,strcat("N",string(length(ang))));

while (progress/progress_max < 1)
        waitbar(progress/progress_max, f);
end
close(f)

