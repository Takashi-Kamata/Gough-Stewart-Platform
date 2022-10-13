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

% disp("Trajectory Points " + length(t))
% progress_max = length(t) * 6;
ang = double(atan((cos(t)))/1.7);
dis = double((8)*sin(t) + 16);
% global serialportObj
% writeline(serialportObj,strcat("M","1"));

transmit_data = zeros(length(ang), 6);
for index = 1:length(ang)
    leg_length = calculate_stewart_platform(30.85,...
                                       15.6,...
                                       57,...
                                       15*pi/180,...
                                       15*pi/180,...
                                       [0 0 dis(index)],... 
                                       [0 ang(index) 0]);
    % Bounding
    bad = zeros(1,6);
    for i=1:6
        if leg_length(i) > (12*2.54 + 57) || leg_length(i) < (57)
            bad(i) = 1;
        end
    end
    if max(bad)==1
        disp("Bad math");
    end
    leg_length = ((leg_length - 57)./2.54);
    disp(leg_length)
    transmit_data(index, :) = leg_length;
%     writeline(serialportObj,strcat("O",string(leg_length(1))));
%     writeline(serialportObj,strcat("P",string(leg_length(2))));
%     writeline(serialportObj,strcat("Q",string(leg_length(3))));
%     writeline(serialportObj,strcat("R",string(leg_length(4))));
%     writeline(serialportObj,strcat("S",string(leg_length(5))));
%     writeline(serialportObj,strcat("T",string(leg_length(6))));
end
% writeline(serialportObj,strcat("N",string(length(ang))));
disp(transmit_data)