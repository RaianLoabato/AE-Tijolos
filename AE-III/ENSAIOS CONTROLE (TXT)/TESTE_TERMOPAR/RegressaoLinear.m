% Define os intervalos de cada regressão
x = 1490:4050;
y = 4050:5200;

% Define as duas equações, cada uma no seu próprio domínio
y1 = (0.18707*x - 278.571);
y2 = (0.11288*y + 22.06);

plot(x, y1,'r','LineWidth', 2.5);  %r = red
hold on;
plot(y, y2,'b', 'LineWidth', 2.5); 
hold off;

xlabel('Eixo X');
ylabel('Eixo Y');
title('Regressão 1 - Regressão 2');
legend('y1 = Regressão 1', 'y2 = Regressão 2');
grid on;