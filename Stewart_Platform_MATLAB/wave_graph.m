clc
close all
clear


% t = 0:1:3600;
amp = 1.5;
period = 10;
% y = amp*sin(t/p);
% 
% d = diff(y);
% val = subs(d,x,pi/2);
% plot(t, y);

syms x t;
hold on
f = amp*sin(2*pi*x / period);
disp(f);

d = diff(f,x);
disp(d);

g = atan(d);
disp(g);

m = atan(d);
disp(m);


%t = 0:.3:2*pi;
ang_eq = @(t) (subs(m,t));

t = [0];
h = 1.0;
tol = 0.2;
angle = subs(m,t);

while t(end) < period
    t_1 = t(end) + h;
    
    angle_1 = subs(m,t_1);
    error = abs(angle_1-angle);
    
    if error < tol
        angle = angle_1;
        t = [t, t_1];
        
        h = 2.0 * h;
        
        if (t(end) + h) > period
            h = period - t(end);
        end
        
    else
        h = 0.9 * h;
    end
end

disp(length(t))


ang = double(subs(g, t));
disp = double(subs(f, t));




grid on
% lims=[0 period];
% xlim(lims)
o_t = 0:10;
p1 = m;
p2 = f;
fplot(p1, "o", "Color", "r", 'LineWidth',1.5);
fplot(p1, "Color", "r", 'LineWidth',2);
fplot(p2, "o",  "Color", "b", 'LineWidth',1.5);
fplot(p2,  "Color", "b", 'LineWidth',2);
xlim([0 10])
xlabel("Period")
ylabel("Amplitude/Angle")
legend(["Vertical Displacement: sin(x)","", "Tilt Angle: arctan(cos(x))"],'FontSize',18);
title("Wave Motion")
figure(2);
hold on
grid on
p3 = subs(m,t);
p4 = subs(f,t);
plot(t, p3, "o", "Color", "r", 'LineWidth',1.5);
plot(t, p3,  "Color", "r", 'LineWidth',2);
plot(t, p4, "o",  "Color", "b", 'LineWidth',1.5);
plot(t, p4,  "Color", "b", 'LineWidth',2);
xlim([0 10])
xlabel("Period")
ylabel("Amplitude/Angle")
legend(["Vertical Displacement: sin(x)","", "Tilt Angle: arctan(cos(x))"],'FontSize',18);
title("Optimised Wave Motion")




