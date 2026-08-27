% 1. Configurações Iniciais
tamanhoVetor = 18000; 
fsa = 1; % Frequência de amostragem

% Garantindo que os vetores sejam do mesmo tamanho e orientação (coluna)
t_crop = tempo_s(1:tamanhoVetor);
sig_crop = temperatura_C(1:tamanhoVetor);

% 2. Cálculo da FFT
fftECG = fft(sig_crop);
% Criando o eixo de frequências apenas até fsa/2 para melhor visualização
fz = linspace(0, fsa/2, floor(tamanhoVetor/2) + 1);

% Módulo da FFT (Normalizado e apenas a parte positiva)
figure(1)
mag_fft = abs(fftECG(1:length(fz))) / tamanhoVetor;
plot(fz, mag_fft);
title('Espectro de Frequência (Magnitude)');
xlabel('Frequência (Hz)');
ylabel('Amplitude');

figure();
pwelch(sig_crop);

% % 3. Filtragem
% % O filtro 'Hd' (gerado no fdatool) deve ser aplicado apenas ao sinal de dados
% sinalFiltrado = filter(Hd, sig_crop);
% 
% % 4. Visualização do Resultado
% figure(2)
% plot(t_crop, sinalFiltrado);
% title('Sinal de Temperatura Filtrado');
% xlabel('Tempo (s)');
% ylabel('Temperatura (°C)');
% grid on;



% % Plot 1 - Temperatura
% figure(1);
% plot(tempo_s, temperatura_C, 'r-', 'LineWidth', 1.5);
% grid on;
% xlabel('Tempo (segundos)');
% ylabel('Temperatura (°C)');
% title('Aquecimento do Forno °C');
% 
% %degrau = 0.25*ones(7598,1)
% %degrau = 0.50*ones(18636,1)
% 
% % Informações básicas
% fprintf('Pronto!\n');
% fprintf('Pontos carregados: %d\n', length(tempo));
% fprintf('Tempo total: %.1f minutos\n', max(tempo)/60);
% fprintf('Temperatura final: %.2f °C\n', temperatura(end));