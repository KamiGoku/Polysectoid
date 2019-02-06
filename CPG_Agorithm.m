clear all;
close all;
theta = zeros(15,1);
%rel_theta = ones(15,7);
tau = 1;
nu = 0*ones(1,15);
% nu = .005*ones(1,15);
% nu(1:5)=.001;
% nu(11:15)=.001;
% nu(1:end)=0;

Test_const=.01;% change test const to see how weight values affect performance
weight = Test_const*ones(15,7);%weight values

Limb_const=.0003;%change limb coupling
weight(:,6:7)=Limb_const*ones(15,2);%limb weight coupling

Left_const=.004;%change left limb coupling
weight(1:5,1:5)=Left_const*ones(5);%limb weight coupling

Right_const=.004;%change right limb coupling
weight(11:15,1:5)=Right_const*ones(5);%limb weight coupling
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55
bias = zeros(15,7);
TemplateBias = [0   -1  -2  -3  -4
                1   0   -1  -2  -3
                2   1   0   -1  -2
                3   2   1   0   -1
                4   3   2   1   0];
Body_bias=.01;%change limb coupling
bias(6:10,1:5)=Body_bias*TemplateBias;%body bias coupling

Left_bias=.01;%change left limb coupling
bias(1:5,1:5)=Left_bias*TemplateBias;%left limb bias coupling

Right_bias=.01;%change right limb coupling
bias(11:15,1:5)=Right_bias*TemplateBias;%right limb bias coupling

N = 10000;
Theta_Buffer = zeros(15,N+1);

theta (1:end)=[1 .7 .8 1 .6 2 .7 .9 1 .6 1.3 .7 3 1 .6];%sample initial condition
%rel_theta = rel_theta.*theta;
%main loop
for n=1:N
    for i=1:15
        RelPhase = zeros(1,7);
        if i<=5
            RelPhase(1:5)=theta(1:5);
            RelPhase(6)=theta(i+5);
            RelPhase(7)=theta(i+10);
        elseif i<=10
            RelPhase(1:5)=theta(6:10);
            RelPhase(6)= theta(i+5);
            RelPhase(7)= theta(i-5);
        else
            RelPhase(1:5)=theta(11:15);
            RelPhase(6)= theta(i-10);
            RelPhase(7)= theta(i-5);
        end  
        theta(i) = PhaseUpdate(theta(i), RelPhase , weight(i,:), bias(i,:), nu(i), tau);
%        rel_theta(i,:)=theta(i);
        Theta_Buffer(i,n+1)=theta(i);
    end
    if n == 4000
        A0 = 0;
    end
end

figure;
for i = 1:15
    txt = ['theta' num2str(i)];
    plot((Theta_Buffer(i,:)),'DisplayName',txt);
    hold on;
end
title('Oscillator Phase Response(nu=.005)');
ylabel('Phase (radians)');
%ylabel('Output(x)');
xlabel('iterations');
legend('show');

function updated_phase = PhaseUpdate(phase, phase_array, weight, bias, freq, tau)
    WSum = sum(weight.*sin(phase_array+bias-phase));
    temp = (2*pi*freq + WSum)/tau;
    updated_phase = mod(temp+phase,2*pi);
end

