clear; close all; clc;
no_drive = load("OLTF_Data.mat");

%% Q1
% Kelton McGrath

% a)
vss = no_drive.V(end);
tau = 0.08;

figure();
hold on;
plot(no_drive.t, no_drive.V, 'x');
scatter(tau, 0.632*vss, 'x');
title("Angular Velocity of Wheel over Time");
xlabel("Time [s]");
ylabel("Ω [rad/s]");
legend({'Ω', '(1.06, 0.632*Ωss)'}, 'Location','northwest');
text(0.075, 0.5, 'Kelton McGrath [u1106273]');

% b)
index = no_drive.V<0.96*vss & no_drive.V>0;
fun = @(vel) log(1-vel/vss);

figure();
plot(no_drive.t(index), fun(no_drive.V(index)), 'x');
title('Linearization of Speed over Time');
xlabel('Time [s]');
ylabel('log(1-Ω/Ωss)');
text(1.022, -2.8, 'Kelton McGrath [u1196273]');

% d)
G = tf([5*1.366], [0.0473, 1]);
new_t = linspace(1, no_drive.t(end), length(index));

figure();
hold on;
step_out = step(G, new_t);
plot(new_t, step_out);
plot(no_drive.t, no_drive.V, 'x');
title("Angular Velocity of Wheel over Time");
xlabel("Time [s]");
ylabel("Ω [rad/s]");
legend({'Theory', 'Experimental'}, 'Location','northwest');
text(0.075, 0.5, 'Kelton McGrath [u1106273]');