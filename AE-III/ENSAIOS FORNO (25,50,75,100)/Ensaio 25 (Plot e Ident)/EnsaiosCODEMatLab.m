% Plot 1 - Temperatura
figure(1);
plot(tempo_s, temperatura_C, 'r-', 'LineWidth', 1.5);
grid on;
xlabel('Tempo (segundos)');
ylabel('Temperatura (°C)');
title('Aquecimento do Forno °C');

%degrau = 0.25*ones(7598,1)

% Informações básicas
fprintf('Pronto!\n');
fprintf('Pontos carregados: %d\n', length(tempo));
fprintf('Tempo total: %.1f minutos\n', max(tempo)/60);
fprintf('Temperatura final: %.2f °C\n', temperatura(end));